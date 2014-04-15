#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "def.h"

typedef struct object_header
{
	struct Class* pclass;
	u2 ref_count; // for GC base on reference count
	u2 fields_size;
	void* pvalues;
}Object;


typedef struct class_instance_header
{
	struct Class* pclass;
	u2 fields_size;
	void* pvalues;
}Class_instance;
 
Object* create_object(struct Class* pclass);
void remove_object(Object* pobject);

Class_instance* create_class_instance(struct Class* pclass);
void remove_class_instance(Class_instance* pclass_instance);

void add_object_to_object_table(Object* pobject);

#endif