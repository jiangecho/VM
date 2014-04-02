#include "resolution.h"
#include "utils.h"
#include "class.h"

#include <stdio.h>

static u1 resolution_field(struct Class* pclass, struct constant_fieldref_info* pconstant_fieldref_info);
static u1 resolution_field(struct Class* pclass, struct constant_fieldref_info* pconstant_fieldref_info);

// due to have resolutioned the constant_classref_info,
// so now we only need to resolution constant_fieldref_info, constant_methodref_info and so on
void resolution(struct Class* pclass, u2 index_in_constant_pool)
{
	struct constant_fieldref_info* pconstant_fieldref_info;
	struct constant_methodref_info* pconstant_methodref_info;
	struct constant_interfacemethodref_info* pconstant_interfacemethodref_ino;

	struct cp_info pcp_info = pclass->pcp_info[index_in_constant_pool];
	switch(pcp_info.tag)
	{
	case CONSTANT_Fieldref:
		pconstant_fieldref_info = (struct constant_fieldref_info* )pcp_info.pinfo;
		resolution_field(pclass, pconstant_fieldref_info);

		break;
	case CONSTANT_Methodref:
		pconstant_methodref_info = (struct constant_methodref_info* )pcp_info.pinfo;

		break;
	case CONSTANT_InterfaceMethodref:
		pconstant_interfacemethodref_ino = (struct constant_interfacemethodref_info* )pcp_info.pinfo;

		break;

	default:
		printf("do not support resolution this tag: %d\n", pcp_info.tag);
		break;
	}
}


/*
 * the instance fields structure
 *  
 *      current class's instance fields
 *      super classes' public and protected fields

 * the class fields structure
 *  
 *      current class's class fields
 *      super interfaces' fields // all interface fields are static and final
 *      super classes' class fields
*/

u1 resolution_field(struct Class* pclass, struct constant_fieldref_info* pconstant_fieldref_info)
{
	u2 offset;
	int i, j;
	struct Class* ptmp_class;

	struct constant_utf8_info* pname_constant_utf8_info = pconstant_fieldref_info->pconstant_name_and_type_info->pname_constant_utf8_info;
	struct constant_utf8_info* pdescriptor_constant_utf8_info  = pconstant_fieldref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info;
	
	offset = 0;

	// resolution current class
	for (i = 0; i < pclass->fields_count; i ++)
	{
		if ((pname_constant_utf8_info == pclass->pfields[i].pname_constant_utf8_info)
			&& (pdescriptor_constant_utf8_info == pclass->pfields[i].pdescriptor_constant_utf8_info))
		{
			// found
			if (mask(pclass->pfields[i].access_flags, ACC_STATIC))
			{
				for (j = 0; j < i; j ++)
				{
					if (mask(pclass->pfields[j].access_flags, ACC_STATIC))
					{
						offset +=  get_field_size(*(pclass->pfields[j].pdescriptor_constant_utf8_info->pbytes));
					}
				}

				if (pclass->psuper_class != NULL)
				{
					offset += get_total_public_protected_fields_size(pclass->psuper_class, CLASS_FIELD);
				}

				
			}
			else
			{
				for (j = 0; j < i; j ++)
				{
					if (!mask(pclass->pfields[j].access_flags, ACC_STATIC))
					{
						offset +=  get_field_size(*(pclass->pfields[j].pdescriptor_constant_utf8_info->pbytes));
					}
				}

				if (pclass->psuper_class != NULL)
				{
					offset += get_total_public_protected_fields_size(pclass->psuper_class, INSTANCE_FIELD);
				}
			}

			pconstant_fieldref_info->offset = offset;

			return OK;
		}
		else
		{
			continue;
		}
	}


	// resolution super interfaces
	offset = 0;
	for (i = 0; i < pclass->interfaces_count; i ++)
	{
		ptmp_class = ((struct constant_class_info* )(pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]].pinfo))->pclass;
		if (resolution_field(ptmp_class, pconstant_fieldref_info))
		{
			return OK;
		}
		else
		{
			continue;
		}
	}


	// resolution super classes
	ptmp_class = pclass->psuper_class;
	while(ptmp_class != NULL)
	{
		if (resolution_field(ptmp_class, pconstant_fieldref_info))
		{
			return OK;
		}
		else
		{
			ptmp_class = ptmp_class->psuper_class;
		}

	}

	return FAIL;
}

