#include "method.h"
#include "class.h"
#include "utils.h"

struct code_attribute_info* get_code_attribute_info(struct method_info* pmethod_info)
{
	int i;
	struct code_attribute_info* pcode_attribute_info = NULL;
	for (i = 0; i < pmethod_info->attributes_count; i++)
	{
		if (pmethod_info->pattributes[i].attribute_type == CODE_ATTRIBUTE_INT_TYPE)
		{
			pcode_attribute_info = (struct code_attribute_info* )pmethod_info->pattributes[i].pinfo;
			break;
		}
	}

	return pcode_attribute_info;
}

u2 get_parameters_size(struct method_info* pmethod_info)
{
	u2 size = 0;
	struct constant_utf8_info* pdescriptor = pmethod_info->pdescriptor_constant_utf8_info;
	u1* pchar = pdescriptor->pbytes;

	// the first byte is '('
	pchar ++;
	while(*pchar != ')')
	{
		switch (*pchar)
		{
		case DOUBLE:
			size += 8;
			break;
		case LONG:
			size += 8;

		default:
			size += 4;
			break;
		}
		pchar ++;
	}
	
	return size;
}