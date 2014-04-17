#ifndef _FRAME_H_
#define _FRAME_H_

#include "def.h"

struct frame
{
	u2 max_locals;
	u2 max_stack;

	u4* pstack_start_addr;
	u4* plocals_start_addr;

	// pointer to the previous frame
	struct frame* fp;

	// point to the current opcode to execute
	u1* pc;

	// point to the current method's first opcode, only for ATHROW
	u1* base_pc;

	// point to the top of the current frame
	u4* sp;

	//TODO optimize 
	struct method_info* pmethod_info;
	struct Class* pclass;
};


#endif
