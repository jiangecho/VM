#ifndef _FRAME_H_
#define _FRAME_H_

#include "def.h"

struct frame
{
	u2 max_locals;
	u2 max_stack;

	// pointer to the previous frame
	struct frame* fp;

	// the offset of the current opcode
	u2 pc;

	// the offset(related to the start address of this frame) of the current frame stack
	u2 sp;

	//u2 method_index_in_constant_pool;

	//TODO optimize 
	struct method_info* pmethod_info;
	struct Class* pclass;
};


#endif
