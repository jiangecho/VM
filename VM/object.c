#include "object.h"
#include "heap.h"
#include "class.h"

#include <stdio.h>

// when the return value is NULL, trigger GC and have another try
Object* create_object(struct Class* pclass)
{
	u4 size;
	Object* pobject = NULL;
	size = sizeof(Object) + get_instance_total_fields_size(pclass);

	pobject = (Object* )malloc_heap(size);
	if (pobject)
	{
		pobject->fields_size = size - sizeof(Object);
		pobject->pclass = pclass;
		pobject->ref_count = 0;
	}
	else
	{
		// TODO trigger GC
	}

	return pobject;

}

// when GC is triggered, it will call this function to remove the object
void remove_object(Object* pobject)
{
	free_heap(pobject);
}

Class_instance* create_class_instance(struct Class* pclass)
{
	u4 size;
	Class_instance* pclass_instance = NULL;

	size = sizeof(Class_instance) + get_class_total_fields_size(pclass);
	pclass_instance = (Class_instance* )malloc_heap(size);

	if (pclass_instance)
	{
		pclass_instance->fields_size = size - sizeof(Class_instance);
		pclass_instance->pclass = pclass;
	}
	else
	{
		// TODO trigger GC
	}

	return pclass_instance;
}



// but under normal condition, we do not remove any class instance
void remove_class_instance(Class_instance* pclass_instance)
{
	free_heap(pclass_instance);
}

