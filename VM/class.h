#ifndef _CLASS_H_
#define _CLASS_H_

#include "def.h"
#include "attribute.h"

#define ACC_PUBLIC          0x0001
#define ACC_FINAL           0x0010
#define ACC_SUPER           0x0020
#define ACC_INTERFACE       0x0200
#define ACC_ABSTRACT        0x0400
#define ACC_SYNTHETIC       0x1000
#define ACC_ANNOTATION      0x2000
#define ACC_ENUM            0x4000

#define CONSTANT_Class              7
#define CONSTANT_Fieldref           9
#define CONSTANT_Methodref          10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String             8
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_NameAndType        12
#define CONSTANT_Utf8               1
#define CONSTANT_MethodHandle       15
#define CONSTANT_MethodType         16
#define CONSTANT_InvokeDynamic      18

//field type
#define BYTE	B
#define CHAR    C
#define DOUBLE	D
#define FLOAT   F
#define INT     I
#define LONG    J
#define SHORT   S
#define BOOLEAN Z

#define OBJECT  L
#define ARRAY   [


struct cp_info{
	u1 tag;
	void* pinfo;
};

struct constant_class_info{
	u2 name_index;
};

struct constant_fieldref_info{
	u2 class_index;
	u2 name_and_type_index;

	//TODO all fields are 4 bytes, except long & double
	u2 offset_in_instance;
};

struct constant_methodref_info{
	u2 class_index;
	u2 name_and_type_index;
};

struct constant_interfacemethodref_info{
	u2 class_index;
	u2 name_and_type_index;
};

struct constant_string_info{
	u2 string_index;
};

struct constant_integer_info{
	int value;
};

struct constant_float_info{
	u4 bytes;
};

struct constant_name_and_type_info{
	u2 name_index;
	u2 descriptor_index;
};

struct constant_utf8_info{
	u2 length;
	u1* pbytes;
};

struct interfaces_index_in_constant_pool
{
	u2* index;// the index of the constant pool
};

struct constant_method_handle_info
{
	u1 reference_kind;
	u2 reference_index;
};

struct constant_method_type_info
{
	u2 descriptor_index;
};

struct constant_invokedynamic_info
{
	u2 bootstrap_method_attr_index;
	u2 name_and_type_index;
};

struct field_info{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	struct attribute_info *pattributes;
};

struct method_info{
	u2 access_flags;
	u2 name_index;
	u2 descriptor_index;
	u2 attributes_count;
	struct attribute_info *pattributes;
};

struct Class{
	// the following will be initialized while loading
	u1 status;
	u4 magic;
	u2 minor_version;
	u2 major_version;
	u2 constant_pool_count;
	struct cp_info *pcp_info;
	u2 access_flags;
	u2 this_class;
	u2 super_class;
	u2 interfaces_count;
	u2* pinterfaces_index_in_constant_pool;
	//struct interfaces_index_in_constant_pool *pinterfaces_index_in_constant_pool;
	u2 fields_count;
	struct field_info *pfields;
	u2 method_count;
	struct method_info* pmethods;
	u2 attributes_count;
	struct attribute_info* pattributes;

	// the following will be initialized while resolution
	struct Class* pthis_class;
	struct Class* psuper_class;

	// all fields will be 4 bytes

	// include the instance fields inherited from the super class
	// use to calculate the size of the object of this class
	u2 instance_field_count;

	// include the class fields inherited from the super class
	// use to calculate the size of memory needed to store this class's class fields
	u2 class_field_count;

	//use to speed up the calculating of subclass's class_field_count
	u2 public_protected_class_field_count;


};

struct class_entry
{
	struct Class* pclass;
	struct class_entry* next;
};

struct class_name_entry
{
	u1* pname;
	u2 name_len;
};

struct pending_load_class
{
	struct class_name_entry* pclass_name_entry;
	struct pending_load_class* next;
};

void get_class_name(struct Class* pclass, struct class_name_entry** pclass_name_entry);
void print_class_name(struct class_name_entry* pclass_name_entry);
void constant_class_info2class_name_entry(struct Class* pclass, struct constant_class_info* pconstant_class_info, struct class_name_entry** pclass_name_entry);

// return the start index of the class name(do no include '/')
u2 get_class_name_start_index(struct class_name_entry* pclass_name_entry);

#endif