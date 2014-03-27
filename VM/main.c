#include <stdio.h>

#include "classloader.h"
#include "def.h"
#include "memory.h"
#include "classloader.h"
#include "class.h"
#include "conf.h"

#include "utils.h"
#include "def.h"
#include "classloader.h"
#include "opcode.h"

#include "method.h"
#include "stack.h"
#include "interpreter.h"

#define DEBUG 1


int main()
{
	struct Class *pclass;
	struct method_info* pmethod_main;
	struct stack* pmain_stack;
	init_code_area(MAX_CODE_AREA_SIZE);
	init_class_loader();
	pclass =  load_class("./TestA_static.class");
	load_pending_classes();
	pmethod_main = find_method("com/test/TestA", "main");
	

	pmain_stack = init_stack(1024);
	push_frame(pmain_stack, pclass, pmethod_main);

	set_current_stack(pmain_stack);
	//interpreter();

	return 0;
}

