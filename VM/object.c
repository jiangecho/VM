#include "object.h"
#include "heap.h"
#include "class.h"

#include <stdio.h>

Object *(object_table[OBJECT_TABLE_SIZE]) = {NULL};

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
		pobject->pvalues = (u1* )pobject + sizeof(Object);

		add_object_to_object_table(pobject);
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

// do not include the fields inherited from the super classes
Class_instance* create_class_instance(struct Class* pclass)
{
	u4 size;
	Class_instance* pclass_instance = NULL;

	//size = sizeof(Class_instance) + get_class_total_fields_size(pclass);
	size = sizeof(Class_instance) + pclass->class_fields_size;
	pclass_instance = (Class_instance* )malloc_heap(size);

	if (pclass_instance)
	{
		pclass_instance->fields_size = pclass->class_fields_size;
		pclass_instance->pclass = pclass;
		pclass_instance->pvalues = (u1* )pclass_instance + sizeof(Class_instance);

		pclass->pclass_instance = pclass_instance;
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


// TODO optimize this function
void add_object_to_object_table(Object* pobject)
{
	int i;
	for (i = 0; i < OBJECT_TABLE_SIZE; i ++)
	{
		if (object_table[i] == NULL)
		{
			break;
		}
	}

	if (i < OBJECT_TABLE_SIZE)
	{
		object_table[i] = pobject;
	}
	else
	{
		printf("error: the object table is full\n");
	}

}