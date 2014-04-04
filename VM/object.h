#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "def.h"

typedef struct object_header
{
	struct Class* pclass;
	u2 ref_count; // for GC base on reference count
	u2 fields_size;
}Object;


typedef struct class_instance_header
{
	struct Class* pclass;
	u2 fields_size;
}Class_instance;
 
Object* create_object(struct Class* pclass);
void remove_object(Object* pobject);

Class_instance* create_class_instance(struct Class* pclass);
void remove_class_instance(Class_instance* pclass_instance);

#endif