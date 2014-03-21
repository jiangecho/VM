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

#define DEBUG 1


int main()
{

	struct Class *pclass;
	init_code_area(MAX_CODE_AREA_SIZE);
	init_class_loader();
	pclass =  load_class("./test.class");
	load_pending_classes();
	

	return 0;

	
}

