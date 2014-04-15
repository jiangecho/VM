#ifndef _STACK_H_
#define _STACK_H_

#include "conf.h"
#include "frame.h"

// the frame structure is just like the following
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
u1 push_frame(struct stack* pstack, struct Class* pclass, struct method_info* pmethod_info, void (*pcallback)());

// when the return value is FAIL, the current interpreter should be ended
u1 pop_frame(struct stack* pstack, void (*pcallback)());

// attention: copy value from the p pointing address to the stack
void push_4(struct stack* pstack, void* p);
void push_8(struct stack* pstack, void* p);

// attention: please use the return address to copy the popped value
// can not use the value directly
void* pop_4(struct stack* pstack);
void* pop_8(struct stack* pstack);

#endif
