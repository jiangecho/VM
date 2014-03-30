#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

#include "linker.h"

#define DEBUG 1

struct constant_class_info_test{
	union {
		u2 name_index;
		struct Class* pclass;
	};
	union {
		u2 name_index1;
		struct Class* pclass2;
	};

};

int main()
{
	struct Class *pclass;
	struct method_info* pmethod_main;
	struct stack* pmain_stack;
	extern struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);
	
	init_code_area(MAX_CODE_AREA_SIZE);
	init_class_loader();
	pclass =  load_class("./SuperInterface.class");
	load_pending_classes();
	link();
	pclass =  load_class("./SubInterface.class");
	link_class(pclass);

	/*
	pmethod_main = find_method("com/test/TestA", strlen("com/test/TestA"), "main", strlen("main"));

	pclass = NULL;
	pclass = find_class("com/test/TestA", strlen("com/test/TestA"));

	

	pmain_stack = init_stack(1024);
	push_frame(pmain_stack, pclass, pmethod_main);

	set_current_stack(pmain_stack);
	//interpreter();
	*/
	//link_class(loaded_class_table[3]);


	return 0;
}

