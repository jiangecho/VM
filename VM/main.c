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
#include "heap.h"
#include "interpreter.h"
#include "prepare.h"

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
	pclass =  load_class("./com/test/TestA.class");
	//pclass =  load_class("./java_lib/java/lang/Exception.class");
	//pclass =  load_class("./java_lib/java/lang/Throwable.class");
	load_pending_classes();
	pmethod_main = find_method("com/test/TestA", strlen("com/test/TestA"), "main", strlen("main"),
		"([Ljava/lang/String;)V", strlen("([Ljava/lang/String;)V"));
	//link_class(pclass);
	link();
	pmethod_main = find_method("com/test/TestA", strlen("com/test/TestA"), "main", strlen("main"),
		"([Ljava/lang/String;)V", strlen("([Ljava/lang/String;)V"));


	pmain_stack = init_stack(MAX_STACK_SIZE);
	init_heap(1024 * 1024);
	//prepare(pclass);
	push_frame(pmain_stack, pclass, pmethod_main, NULL);

	set_current_stack(pmain_stack);
	interpreter();


	return 0;
}

