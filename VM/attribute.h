#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include "def.h"

#define CONSTANT_VALUE_ATTRIBUTE "ConstantValue"
#define CODE_ATTRIBUTE "Code"
#define EXCEPTIONS_ATTRIBUTE "Exceptions"

#define CONSTANT_VALUE_ATTRIBUTE_INT_TYPE   0
#define CODE_ATTRIBUTE_INT_TYPE             1
#define EXCEPTIONS_ATTRIBUTE_INT_TYPE       2 
#define UNKNOWN_ATTRIBUTE_INT_TYPE			0xFF
//TODO minimum implement, only support part of the attributes

struct attribute_info{
	u2 attribute_name_index;
	u4 attribute_length;

	//TODO 
	u1 attribute_type;

	void* pinfo;
};

struct constant_value_attribute_info
{
	u2 constant_value_index;
};

struct exception_table_entry
{
	u2 start_pc;
	u2 end_pc;
	u2 handler_pc;
	u2 catch_type;
};

struct code_attribute_info
{
	u2 max_stack;
	u2 max_locals;
	u4 code_length;
	u1* pcode;
	u2 exception_table_length;
	struct exception_table_entry* pexception_table_entry;
	u2 attributes_count;
	struct attribute_info* pattributes;
};

struct exceptions_attribute_info
{
	u2 number_of_exceptions;
	u2* pexception_index_table;
};


u1 convert_attribute_name_2_int_value(struct Class* pclass, struct attribute_info* pattribute_info);

#endif