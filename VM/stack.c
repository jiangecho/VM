#include "stack.h"
#include "class.h"
#include "attribute.h"

#include <stdlib.h>
#include <stdio.h>


struct stack* init_stack(u4 size)
{
	struct stack* pstack = (struct stack* )malloc(sizeof(struct stack));
	if (pstack == NULL)
	{
		return NULL;
	}

	pstack->pbottom = (u1* )malloc(size);
	if (pstack->pbottom == NULL)
	{
		free(pstack);
		return NULL;
	}

	pstack->ptop = pstack->pbottom;
	pstack->size = size;
	pstack->pcurrent_frame = NULL;
	return pstack;
}

void uninit_stack(struct stack* pstack)
{
	free(pstack);
}

// TODO the next method's local variables overlap current method's operand stack
u1 push_frame(struct stack* pstack, struct Class* pclas, struct method_info* pmethod_info)
{
	//TODO parse the method_info to find out the size of the frame
	int i;
	int ret = FAIL;
	u2 current_frame_max_size, parameters_size;
	struct code_attribute_info* pcode_attribute_info;
	struct frame* pframe;

	parameters_size = get_parameters_size(pmethod_info);

	for (i = 0; i < pmethod_info->attributes_count; i ++)
	{
		if (pmethod_info->pattributes[i].attribute_type == CODE_ATTRIBUTE_INT_TYPE)
		{

			pcode_attribute_info = (struct code_attribute_info* )pmethod_info->pattributes[i].pinfo;

			current_frame_max_size = pcode_attribute_info->max_locals * sizeof(u4) + pcode_attribute_info->max_stack * sizeof(u4) 
				+ sizeof(struct frame);
			if ((pstack->ptop - parameters_size + current_frame_max_size - pstack->pbottom) > pstack->size)
			{
				printf("stack over follow\n");
				ret = FAIL;
			}
			else
			{
				// the frames is overlap

				if (pstack->ptop != pstack->pbottom)
				{
					pframe = (struct frame* )(((u1* )pstack->pcurrent_frame->sp) - parameters_size + pcode_attribute_info->max_locals * sizeof(u4));
					pframe->plocals_start_addr = (u4* )(((u1* )pstack->pcurrent_frame->sp) - parameters_size);
				}
				else
				{
					pframe = (struct frame* )(pstack->ptop + pcode_attribute_info->max_locals * sizeof(u4));
					pframe->plocals_start_addr = (u4* )pstack->ptop;
				}

				//pframe = (struct frame* )(pframe->plocals_start_addr + pcode_attribute_info->max_locals * sizeof(u4));

				pframe->pstack_start_addr = (u4* )((u1* )pframe + sizeof(struct frame));

				pframe->max_locals = pcode_attribute_info->max_locals;
				pframe->max_stack = pcode_attribute_info->max_stack;
				pframe->fp = pstack->pcurrent_frame;
				pframe->pmethod_info = pmethod_info;
				pframe->pclass = pclas;
				pframe->pc = pcode_attribute_info->pcode;
				pframe->sp = pframe->pstack_start_addr;
				if (pstack->pcurrent_frame != NULL)
				{
					//pstack->pcurrent_frame->sp -= parameters_size;
					pstack->pcurrent_frame->sp = (u4* )(((u1* )(pstack->pcurrent_frame->sp)) - parameters_size);
				}

				pstack->pcurrent_frame = pframe;
				//pstack->ptop = pstack->ptop - parameters_size + current_frame_max_size;
				pstack->ptop = (u1* )pframe->plocals_start_addr + current_frame_max_size;

				ret = OK;
			}
			
			break;
		}
	}
	return ret;
}

u1 pop_frame(struct stack* pstack)
{
	struct frame* pcurrent_frame = pstack->pcurrent_frame;
	pstack->pcurrent_frame = pstack->pcurrent_frame->fp;

	if (pstack->pcurrent_frame != NULL)
	{
		pstack->ptop = (u1* )pcurrent_frame->plocals_start_addr + get_parameters_size(pcurrent_frame->pmethod_info);
	}
	else
	{
		pstack->ptop = pstack->pbottom;
	}


	// while pstack->ptop == pstack->pbottom, it means that it is the end of current thread(there is not
	// any more bytecode to interpreter).
	if (pstack->ptop > pstack->pbottom)
	{
		return OK;
	}
	else
	{
		return FAIL;
	}

} 

// attention: copy value from the p pointing address to the stack
void push_4(struct stack* pstack, void* p)
{
	int i;
	struct frame* pcurrent_frame = pstack->pcurrent_frame; 
	u1* psp = (u1* )pcurrent_frame->sp;
	
	for(i = 0; i < 4; i ++)
	{
		*(psp + i) = *((u1* )p + i);
	}

	pcurrent_frame->sp ++;
}
void push_8(struct stack* pstack, void* p)
{
	int i;
	struct frame* pcurrent_frame = pstack->pcurrent_frame; 
	u1* psp = (u1* )pcurrent_frame->sp;
	
	for(i = 0; i < 8; i ++)
	{
		*(psp + i) = *((u1* )p + i);
	}

	pcurrent_frame->sp ++;
	pcurrent_frame->sp ++;
}

// attention: please use the return address to copy the popped value
// can not use the value directly
void* pop_4(struct stack* pstack)
{
	struct frame* pframe = pstack->pcurrent_frame; 
	pframe->sp -= 4;
	return pframe->sp;
}
void* pop_8(struct stack* pstack)
{
	struct frame* pframe = pstack->pcurrent_frame; 
	pframe->sp -= 8;
	return pframe->sp;
}

