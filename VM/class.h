#ifndef _CLASS_H_
#define _CLASS_H_

#include "def.h"
#include "attribute.h"
#include "method.h"

#define ACC_PUBLIC          0x0001
#define ACC_FINAL           0x0010
#define ACC_SUPER           0x0020
#define ACC_INTERFACE       0x0200
#define ACC_ABSTRACT        0x0400
#define ACC_SYNTHETIC       0x1000
#define ACC_ANNOTATION      0x2000
#define ACC_ENUM            0x4000

#define ACC_PRIVATE         0x0002
#define ACC_PROTECTED       0x0004
#define ACC_STATIC          0x0008
#define ACC_VOLATILE        0x0040
#define ACC_TRANSIENT       0x0080

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
#define BYTE	'B'
#define CHAR    'C'
#define DOUBLE	'D'
#define FLOAT   'F'
#define INT     'I'
#define LONG    'J'
#define SHORT   'S'
#define BOOLEAN 'Z'

#define OBJECT  'L'
#define ARRAY   '['

#define CLASS_LOADING     0
#define CLASS_LOADED      1
#define CLASS_LINKED      2
#define CLASS_PREPARATION 3
#define CLASS_RESOLUTION  4
#define CLASS_INITIALIZATION 5

struct cp_info{
	u1 tag;
	void* pinfo;
};

struct constant_class_info{
	union{
		u2 name_index;
		struct Class* pclass;
	};
};

struct constant_fieldref_info{
	union{
		u2 class_index;
		struct Class* pclass;
	};

	union{
		u2 name_and_type_index;
		struct constant_name_and_type_info* pconstant_name_and_type_info;
	};

};

struct constant_methodref_info{
	union{
		u2 class_index;
		struct Class* pclass;
	};

	union{
		u2 name_and_type_index;
		struct constant_name_and_type_info* pconstant_name_and_type_info;
	};
};

struct constant_interfacemethodref_info{
	union{
		u2 class_index;
		struct Class* pclass;
	};

	union{
		u2 name_and_type_index;
		struct constant_name_and_type_info* pconstant_name_and_type_info;
	};
};

struct constant_string_info{
	union{
		u2 string_index;
		struct constant_utf8_info* pconstant_utf8_info;
	};
};

struct constant_integer_info{
	int value;
};

struct constant_float_info{
	u4 bytes;
};

struct constant_name_and_type_info{
	union{
		u2 name_index;
		struct constant_utf8_info* pname_constant_utf8_info;
	};
	union{
		u2 descriptor_index;
		struct constant_utf8_info* pdescriptor_constant_utf8_info;
	};
};

struct constant_utf8_info{
	u2 length;
	u1* pbytes;
};

struct interfaces_index_in_constant_pool
{
	u2* index;// the index of the constant pool
};

// TODO do not support yet
struct constant_method_handle_info
{
	u1 reference_kind;
	u2 reference_index;
};

struct constant_method_type_info
{
	union{
		u2 descriptor_index;
		struct constant_utf8_info* pconstant_utf8_info;
	};
};

//TODO do not support yet
struct constant_invokedynamic_info
{
	u2 bootstrap_method_attr_index;
	u2 name_and_type_index;
};

struct field_info{
	u2 access_flags;
	union{
		u2 name_index;
		struct constant_utf8_info* pname_constant_utf8_info;
	};

	union{
		u2 descriptor_index;
		struct constant_utf8_info* pdescriptor_constant_utf8_info;
	};

	u2 attributes_count;
	struct attribute_info *pattributes;

	//all fields are 4 bytes, except long & double
	// the offset of this field in the class fields or the instance fields
	u4 offset;
};

struct method_info{
	u2 access_flags;
	union{
		u2 name_index;
		struct constant_utf8_info* pname_constant_utf8_info;
	};

	union{
		u2 descriptor_index;
		struct constant_utf8_info* pdescriptor_constant_utf8_info;
	};
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
	union{
		u2 this_class;
		struct Class* pthis_class;
	};
	union{
		u2 super_class;
		struct Class* psuper_class;
	};
	u2 interfaces_count;
	u2* pinterfaces_index_in_constant_pool;
	//struct interfaces_index_in_constant_pool *pinterfaces_index_in_constant_pool;
	u2 fields_count;
	struct field_info *pfields;
	u2 method_count;
	struct method_info* pmethods;

	u2 attributes_count;
	struct attribute_info* pattributes;
/*
	// all fields will be 4 bytes

	// include the instance fields inherited from the super class
	// use to calculate the size of the object of this class
	u2 instance_field_count;

	// include the class fields inherited from the super class
	// use to calculate the size of memory needed to store this class's class fields
	u2 class_field_count;

	//use to speed up the calculating of subclass's class_field_count
	u2 public_protected_class_field_count;
*/
	// all fields are 4 bytes, except long & double
	// use to construct the class and instance, these values will be set when resolution
	// attention: do not include the fields inherited from the super class
	u4 class_fields_size;
	u4 instance_fileds_size;
	u4 public_protected_class_fields_size;
	u4 public_protected_instance_fields_size;

};

struct class_entry
{
	struct class_name_entry* pclass_name_entry;
	struct Class* pclass;
	struct class_entry* next;
};

struct class_name_entry
{
	u1* pname;
	u2 name_len;
};

/*
struct pending_load_class
{
	struct class_name_entry* pclass_name_entry;
	struct pending_load_class* next;
};
*/

void get_class_name_internal(struct Class* pclass, struct class_name_entry** pclass_name_entry);
void print_class_name(struct class_name_entry* pclass_name_entry);
void constant_class_info2class_name_entry(struct Class* pclass, struct constant_class_info* pconstant_class_info, struct class_name_entry** pclass_name_entry);

// return the start index of the class name(do no include the package name and the last '/')
u2 get_class_name_start_index(struct class_name_entry* pclass_name_entry);

struct method_info* find_method(char* class_name, u2 class_name_len, char* method_name, u2 method_name_len);

struct Class* find_class(char* pclass_name, u2 class_name_len);

#endif
