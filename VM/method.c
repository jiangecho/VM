#include "method.h"
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