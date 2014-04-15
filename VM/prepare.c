#include "prepare.h"
#include "heap.h"
#include "class.h"
#include "utils.h"
#include "object.h"

#include <stdio.h>

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
static void prepare_internal(struct Class* pclass, Class_instance* pclass_instance, u2 current_class_field_start_offset, u1 flag);
u1 prepare(struct Class* pclass)
{
	u2 size;
	Class_instance* pclass_instance = NULL;
	struct Class* psuper_class = pclass->psuper_class;

	if ((pclass->status != CLASS_LINKED)
		|| pclass->status > CLASS_PREPARED) 
	{ 
		return FAIL;
	}


	//size = sizeof(Class_instance) + pclass->class_total_fields_size;

	// the current class's class fields has been set to the default vales in the malloc_heap function
	pclass_instance = create_class_instance(pclass);

	if (pclass_instance)
	{
		prepare_internal(pclass, pclass_instance, 0, 1);
	}

	pclass->status = CLASS_PREPARED;

	while(psuper_class)
	{
		prepare(psuper_class);
		psuper_class = psuper_class->psuper_class;
	}

	return OK;
}

static void prepare_internal(struct Class* pclass, Class_instance* pclass_instance, u2 current_class_field_start_offset, u1 flag)
{
	int i, j, index;
	u2 offset = 0;
	struct field_info field_info;
	struct attribute_info attribute_info;
	struct constant_integer_info* pconstant_integer_info;
	struct Class* ptmp_class;




	// set the final static fields to constant values
	// so in the initialization procedure, we do not need to set the values any more
	for (i = 0; i < pclass->fields_count; i ++)
	{
		field_info = pclass->pfields[i];
		if (mask(field_info.access_flags, ACC_STATIC) && mask(field_info.access_flags, ACC_FINAL))
		{
			/*
			if (flag == 1)
			{
				field_size += get_field_size(*(u1* )(field_info.pdescriptor_constant_utf8_info)->pbytes);
			}
			else
			{
				if (mask(field_info.access_flags, ACC_PUBLIC) || mask(field_info.access_flags, ACC_PROTECTED))
				{
					field_size += get_field_size(*(u1* )(field_info.pdescriptor_constant_utf8_info)->pbytes);
				}
				else
				{
					continue;
				}
			}
			*/

			if((flag == 1) || (mask(field_info.access_flags, ACC_PUBLIC)) || mask(field_info.access_flags, ACC_PROTECTED))
			{
				for (j = 0; j < field_info.attributes_count; j++)
				{
					attribute_info = field_info.pattributes[j];
					
					//TODO more attribute type
					if (attribute_info.attribute_type == CONSTANT_VALUE_ATTRIBUTE_INT_TYPE)
					{
						index = ((struct constant_value_attribute_info* )(attribute_info.pinfo))->constant_value_index;
						pconstant_integer_info = (struct constant_integer_info* )pclass->pcp_info[index].pinfo;

						// attention can not use field_info.offset as the offset here
						// because the field_info.offset include the private field

						//TODO now only support int value
						// TODO fix bug?: big endian
						*((int* )((u1* )pclass_instance->pvalues + current_class_field_start_offset + offset)) = pconstant_integer_info->value;
					}

				}

				offset += get_field_size(*(u1* )(field_info.pdescriptor_constant_utf8_info)->pbytes);
			}

		}
	}

	if (flag == 1)
	{
		current_class_field_start_offset += pclass->class_fields_size;
	}
	else
	{
		current_class_field_start_offset += pclass->public_protected_class_fields_size;
	}


	// set the super interfaces' fields to the default values
	for (i = 0; i < pclass->interfaces_count; i++)
	{
		ptmp_class = ((struct constant_class_info* )(pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]].pinfo))->pclass;
		prepare_internal(ptmp_class, pclass_instance, current_class_field_start_offset, 0);
	}

	// set the super classes' fields to the default values
	ptmp_class = pclass->psuper_class;
	while (ptmp_class != NULL)
	{
		prepare_internal(ptmp_class, pclass_instance, current_class_field_start_offset, 0);
		ptmp_class = ptmp_class->psuper_class;
	}

}
