#include "resolution.h"
#include "utils.h"
#include "class.h"

#include <stdio.h>

static u1 resolution_field_internal(struct Class* pclass, u1 flag, struct constant_fieldref_info* pconstant_fieldref_info, 
	u2 class_field_start_offset, u2 instance_field_start_offset);
static u1 resolution_field(struct Class* pclass,  struct constant_fieldref_info* pconstant_fieldref_info);
u1 resolution_method(struct Class* pclass, struct constant_methodref_info* pconstant_methodref_info);

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
		if (!resolution_field(pclass,  pconstant_fieldref_info))
		{
			//TODO handle NoSuchFieldError, now just print it
			printf("NoSuchFieldError\n");
		}

		break;
	case CONSTANT_Methodref:
		pconstant_methodref_info = (struct constant_methodref_info* )pcp_info.pinfo;
		resolution_method(pclass, pconstant_methodref_info);

		break;
	case CONSTANT_InterfaceMethodref:
		pconstant_interfacemethodref_ino = (struct constant_interfacemethodref_info* )pcp_info.pinfo;
		//TODO

		break;

	default:
		printf("do not support resolution this tag: %d\n", pcp_info.tag);
		break;
	}
}


u1 resolution_field(struct Class* pclass,  struct constant_fieldref_info* pconstant_fieldref_info)
{
	// has been resolutioned
	if(pconstant_fieldref_info->offset != 0xFFFF)
	{
		return OK;
	}
	return resolution_field_internal(pclass, 1, pconstant_fieldref_info, 0, 0);
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

/*
 * flag: 1, call from outside the function; 0, call from inner the function
 * xxx_start_offset: this class's fields' start offset in the class instance or object instance
*/

u1 resolution_field_internal(struct Class* pclass, u1 flag, struct constant_fieldref_info* pconstant_fieldref_info, 
	u2 class_field_start_offset, u2 instance_field_start_offset)
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
		//if ((pname_constant_utf8_info == pclass->pfields[i].pname_constant_utf8_info)
		//	&& (pdescriptor_constant_utf8_info == pclass->pfields[i].pdescriptor_constant_utf8_info))
		if (compare(pname_constant_utf8_info->pbytes, pname_constant_utf8_info->length, 
			pclass->pfields[i].pname_constant_utf8_info->pbytes, pclass->pfields[i].pname_constant_utf8_info->length)
			&& compare(pdescriptor_constant_utf8_info->pbytes, pdescriptor_constant_utf8_info->length,
			pclass->pfields[i].pdescriptor_constant_utf8_info->pbytes, pclass->pfields[i].pdescriptor_constant_utf8_info->length))
		{
			// TODO  we need to check the access flag firstly


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

				/*
				if (pclass->psuper_class != NULL)
				{
					offset += get_total_public_protected_fields_size(pclass->psuper_class, CLASS_FIELD);
				}
				*/

				pconstant_fieldref_info->offset = class_field_start_offset + offset;
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
				/*
				if (pclass->psuper_class != NULL)
				{
					offset += get_total_public_protected_fields_size(pclass->psuper_class, INSTANCE_FIELD);
				}
				*/

				pconstant_fieldref_info->offset = instance_field_start_offset + offset;
			}

			//pconstant_fieldref_info->offset = offset;

			return OK;
		}
		else
		{
			continue;
		}
	}


	// resolution super interfaces

	if (flag == 1)
	{
		class_field_start_offset += pclass->class_fields_size;
		instance_field_start_offset += pclass->instance_fileds_size;
	}else
	{
		class_field_start_offset += pclass->public_protected_class_fields_size;
		instance_field_start_offset += pclass->public_protected_instance_fields_size;
	}



	for (i = 0; i < pclass->interfaces_count; i ++)
	{
		ptmp_class = ((struct constant_class_info* )(pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]].pinfo))->pclass;
		if (resolution_field_internal(ptmp_class,0, pconstant_fieldref_info, class_field_start_offset, instance_field_start_offset))
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
		if (resolution_field_internal(ptmp_class, 0, pconstant_fieldref_info, class_field_start_offset, instance_field_start_offset))
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


//TODO now do not handle signature polymorphic method
u1 resolution_method(struct Class* pclass, struct constant_methodref_info* pconstant_methodref_info)
{
	int i = 0;
	struct Class* ptmp_class;

	struct constant_utf8_info* pname_constant_utf8_info = pconstant_methodref_info->pconstant_name_and_type_info->pname_constant_utf8_info;
	struct constant_utf8_info* pdescriptor_constant_utf8_info = pconstant_methodref_info->pconstant_name_and_type_info->pdescriptor_constant_utf8_info;  

	// have been resolutioned
	if(pconstant_methodref_info->pmethod_info != NULL)
	{
		return OK;
	}

	//TODO need to check whether this class is a class or not? maybe an interface
	
	//TODO now do not handle signature polymorphic method

	// resolution current class
	for (i = 0; i < pclass->method_count; i ++)
	{
		//if ((pclass->pmethods[i].pname_constant_utf8_info == pname_constant_utf8_info)
		//	&& (pclass->pmethods[i].pdescriptor_constant_utf8_info == pdescriptor_constant_utf8_info))
		if (compare(pclass->pmethods[i].pname_constant_utf8_info->pbytes, pclass->pmethods[i].pname_constant_utf8_info->length, 
				pname_constant_utf8_info->pbytes, pname_constant_utf8_info->length)
			&& compare(pclass->pmethods[i].pdescriptor_constant_utf8_info->pbytes, pclass->pmethods[i].pdescriptor_constant_utf8_info->length,
				pdescriptor_constant_utf8_info->pbytes, pdescriptor_constant_utf8_info->length))
		{
			pconstant_methodref_info->pmethod_info = &(pclass->pmethods[i]);
			return OK;
		}
		else
		{
			continue;
		}
	}

	//resolution super classes
	ptmp_class = pclass->psuper_class;
	while(ptmp_class != NULL)
	{
		if (resolution_method(ptmp_class, pconstant_methodref_info))
		{
			return OK;
		}
		else
		{
			ptmp_class = ptmp_class->psuper_class;
			continue;
		}
	}

	//TODO resolution the super interfaces
	for(i = 0; i < pclass->interfaces_count; i++)
	{
		ptmp_class = ((struct constant_class_info* )(pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]].pinfo))->pclass;
		if(resolution_method(ptmp_class, pconstant_methodref_info))
		{
			return OK;
		}
		else
		{
			continue;
		}
	}

	return FAIL;

}
