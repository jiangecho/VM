#ifndef _STACK_H_
#define _STACK_H_

#include "conf.h"
#include "frame.h"

// the stack structure is just like the following
/************************************************************************/
/* 
the top is high address

		[operand stacks ]
		[frame structure]
		[local variables]




*/
/************************************************************************/

struct stack
{
	u2 size;
	u1* pbottom;
	u1* ptop;
	struct frame* pcurrent_frame;
};


struct stack* init_stack(u4 size);
//TODO
void uninit_stack(struct stack* pstack);
u1 push_frame(struct stack* pstack, struct method_info* pmethod_info);
u1 pop_frame(struct stack* pstack);

#endif