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

 // attention: do not include the fields inherited from the super classes
 * the class fields structure
 *  
 *      current class's class fields
 *      [removed]super interfaces' fields // all interface fields are static and final
 *      [remvoed]super classes' class fields
*/

void prepare(struct Class* pclass)
{
	int i, j, index;
	u2 offset = 0;
	struct field_info field_info;
	struct attribute_info attribute_info;
	struct constant_integer_info* pconstant_integer_info;
	struct Class* ptmp_class;


	Class_instance* pclass_instance = NULL;
	struct Class* psuper_class = pclass->psuper_class;

	if ((pclass->status != CLASS_LINKED)
		|| pclass->status > CLASS_PREPARED) 
	{ 
		return ;
	}


	//size = sizeof(Class_instance) + pclass->class_total_fields_size;

	// the current class's class fields has been set to the default vales in the malloc_heap function
	pclass_instance = create_class_instance(pclass);
	if (!pclass_instance)
	{
		return;
	}

	// set the final static fields to constant values
	// so in the initialization procedure, we do not need to set the values any more
	for (i = 0; i < pclass->fields_count; i ++)
	{
		field_info = pclass->pfields[i];
		if (mask(field_info.access_flags, ACC_STATIC) && mask(field_info.access_flags, ACC_FINAL))
		{

			for (j = 0; j < field_info.attributes_count; j++)
			{
				attribute_info = field_info.pattributes[j];
				
				//TODO more attribute type
				if (attribute_info.attribute_type == CONSTANT_VALUE_ATTRIBUTE_INT_TYPE)
				{
					index = ((struct constant_value_attribute_info* )(attribute_info.pinfo))->constant_value_index;


					//TODO now only support int type
					if (pclass->pcp_info[index].tag == CONSTANT_Integer)
					{
						pconstant_integer_info = (struct constant_integer_info* )pclass->pcp_info[index].pinfo;

						// attention can not use field_info.offset as the offset here
						// because the field_info.offset include the private field

						//TODO now only support int value
						// TODO fix bug?: big endian
						*((int* )((u1* )pclass_instance->pvalues + offset)) = pconstant_integer_info->value;
					}
					else
					{
						printf("now only support int\n");
					}
				}

			}

			offset += get_field_size(*(u1* )(field_info.pdescriptor_constant_utf8_info)->pbytes);

		}
	}
	pclass->status = CLASS_PREPARED;

	// set the super interfaces' fields to the default values
	for (i = 0; i < pclass->interfaces_count; i++)
	{
		ptmp_class = ((struct constant_class_info* )(pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]].pinfo))->pclass;
		prepare(ptmp_class);
	}

	// set the super classes' fields to the default values
	ptmp_class = pclass->psuper_class;
	while (ptmp_class != NULL)
	{
		prepare(ptmp_class);
		ptmp_class = ptmp_class->psuper_class;
	}

}
