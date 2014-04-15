#include "initialize.h"
#include "stack.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

// only execute the class and its super classes' <clinit> method
// do not include the super interfaces
u1 initialize(struct Class* pclass, struct stack* pstack)
{
	struct method_info* pmethod_info;
	struct Class* pcur_class;
	u2 super_class_count = 0;

	if (pclass->status >= CLASS_INITIALIZING)
	{
		return OK;
	}
	// when it is a class, we need to recursively initialize its super class
	if (!mask(pclass->access_flags, ACC_INTERFACE))
	{
		
		pcur_class = pclass->psuper_class;
		while(pcur_class)
		{
			//TODO now we do not support Object's clinit
			// because some methods in clinit, we still do not support, so...
			if (compare(pcur_class->pclass_name_entry->pname, pcur_class->pclass_name_entry->name_len,
				"java/lang/Object", strlen("java/lang/Object")))
			{
				break;
			}


			if (pcur_class->status < CLASS_INITIALIZING)
			{
				pmethod_info = find_class_method(pcur_class, "<clinit>", strlen("<clinit>"), "()V", strlen("()V"));
				if (pmethod_info)
				{
					pcur_class->status = CLASS_INITIALIZING;
					push_frame(pstack, pcur_class, pmethod_info, update_current_frame);
				}
				else
				{
					pcur_class->status = CLASS_INITIALIZED;
				}
			}
			else
			{
				break;
			}

			pcur_class = pcur_class->psuper_class;
		}

	}
	else
	{
		pmethod_info = find_class_method(pclass, "<clinit>", strlen("<clinit>"), "()V", strlen("()V"));
		if (pmethod_info)
		{
			push_frame(pstack, pcur_class, pmethod_info, update_current_frame);
		}
	}
	
	return OK;
}