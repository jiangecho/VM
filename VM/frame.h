#ifndef _FRAME_H_
#define _FRAME_H_

#include "def.h"

struct frame
{
	u2 max_locals;
	u2 max_stack;

	// pointer to the previous frame
	struct frame* fp;

	// the offset of the next opcode
	u2 pc;

	u2 method_index_in_constant_pool;
	struct method_info* pmethod_info;
};


#endif