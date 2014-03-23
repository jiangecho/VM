#include "attribute.h"
#include "utils.h"
#include "def.h"
#include "class.h"

#include <string.h>

//TODO add more attributes support
u1 convert_attribute_name_2_int_value(struct Class* pclass, struct attribute_info* pattribute_info)
{
	u1 value = 0xFF;
	u2  name_index = pattribute_info->attribute_name_index;
	struct constant_utf8_info* pcontant_utf8_info = (struct constant_utf8_info* )(pclass->pcp_info[name_index].pinfo);

	if (compare(pcontant_utf8_info->pbytes, pcontant_utf8_info->length, CONSTANT_VALUE_ATTRIBUTE, strlen(CONSTANT_VALUE_ATTRIBUTE)))
	{
		value = CONSTANT_VALUE_ATTRIBUTE_INT_TYPE;
	}
	else if (compare(pcontant_utf8_info->pbytes, pcontant_utf8_info->length, CODE_ATTRIBUTE, strlen(CODE_ATTRIBUTE)))
	{
		value = CODE_ATTRIBUTE_INT_TYPE;
	}
	else if (compare(pcontant_utf8_info->pbytes, pcontant_utf8_info->length, EXCEPTIONS_ATTRIBUTE, strlen(EXCEPTIONS_ATTRIBUTE)))
	{
		value = EXCEPTIONS_ATTRIBUTE_INT_TYPE;
	}
	

	return value;
}