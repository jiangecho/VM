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

	pstack->size = size;
	pstack->pcurrent_frame = NULL;
	return pstack;
}

void uninit_stack(struct stack* pstack)
{
	free(pstack);
}

u1 push_frame(struct stack* pstack, struct method_info* pmethod_info)
{
	//TODO parse the method_info to find out the size of the frame
	int i;
	int ret = FAIL;
	u2 frame_size;
	struct code_attribute_info* pcode_attribute_info;
	struct frame* pframe;

	for (i = 0; i < pmethod_info->attributes_count; i ++)
	{
		if (pmethod_info->pattributes[i].attribute_type == CODE_ATTRIBUTE_INT_TYPE)
		{

			pcode_attribute_info = (struct code_attribute_info* )pmethod_info->pattributes[i].pinfo;

			frame_size = pcode_attribute_info->max_locals * sizeof(u4) + pcode_attribute_info->max_stack * sizeof(u4) 
				+ sizeof(struct frame);
			if ((pstack->ptop + frame_size - pstack->pbottom) > pstack->size)
			{
				printf("stack over follow\n");
				ret = FAIL;
			}
			else
			{
				pframe = (struct frame* )(pstack->ptop + pcode_attribute_info->max_locals * sizeof(u4));

				pframe->max_locals = pcode_attribute_info->max_locals;
				pframe->max_stack = pcode_attribute_info->max_stack;
				pframe->fp = pstack->pcurrent_frame;
				pframe->pmethod_info = pmethod_info;
				pframe->pc = 0;

				pstack->pcurrent_frame = pframe;
				pstack->ptop += frame_size;

				ret = OK;
			}
			
			break;
		}
	}
	return ret;
}

u1 pop_frame(struct stack* pstack)
{
	int size;
	struct frame* pframe = pstack->pcurrent_frame;
	size = pframe->max_locals * sizeof(u4) + pframe->max_stack * sizeof(u4) + sizeof(*pframe);

	pstack->pcurrent_frame = pframe->fp;
	pstack->ptop -= size;

	if (pstack->ptop >= pstack->pbottom)
	{
		return OK;
	}
	else
	{
		return FAIL;
	}

}
