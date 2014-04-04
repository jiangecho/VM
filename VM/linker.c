#include "linker.h"
#include "def.h"
#include "class.h"
#include "method.h"
#include "attribute.h"
#include "conf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define CLASS_LINKED 1
#define UN_LINKED 0

static void link_constant_class_info(struct Class* pclass, u2 class_index)
{
	struct constant_class_info *pconstant_class_info;
	struct constant_utf8_info *pconstant_utf8_info;
	pconstant_class_info = (struct constant_class_info * )pclass->pcp_info[class_index].pinfo;

	pconstant_utf8_info = (struct constant_utf8_info * )pclass->pcp_info[pconstant_class_info->name_index].pinfo;

	((struct constant_class_info* )(pclass->pcp_info[class_index].pinfo))->pclass = find_class(pconstant_utf8_info->pbytes, pconstant_utf8_info->length);
}

static void link_constant_name_and_type_info(struct Class* pclass, u2 name_and_type_descriptor_index)
{
	//return (struct constant_name_and_type_info* )pcp_info[name_and_type_descriptor_index].pinfo;
	struct constant_name_and_type_info* pconstant_name_and_type_info = (struct constant_name_and_type_info* )(pclass->pcp_info[name_and_type_descriptor_index].pinfo);
	u2 name_index = pconstant_name_and_type_info->name_index;
	u2 descriptor_index = pconstant_name_and_type_info->descriptor_index;
	pconstant_name_and_type_info->pname_constant_utf8_info = (struct constant_utf8_info* )pclass->pcp_info[name_index].pinfo;
	pconstant_name_and_type_info->pdescriptor_constant_utf8_info = (struct constant_utf8_info* )pclass->pcp_info[descriptor_index].pinfo;

}

static void link_constant_method_type_info(struct Class* pclass, u2 method_index)
{
	struct constant_method_type_info* pconstant_method_type_info = (struct constant_method_type_info* )pclass->pcp_info[method_index].pinfo;
	pconstant_method_type_info->pconstant_utf8_info = (struct constant_utf8_info* )pclass->pcp_info[pconstant_method_type_info->descriptor_index].pinfo;
}


static void link_constant_string_info(struct Class* pclass, u2 string_index)
{
	u2 utf8_index = ((struct constant_string_info* )(pclass->pcp_info[string_index].pinfo))->string_index;
	struct constant_string_info* pconstant_string_info = (struct constant_string_info* )pclass->pcp_info[utf8_index].pinfo; 
	pconstant_string_info->pconstant_utf8_info = (struct constant_utf8_info* )pclass->pcp_info[pconstant_string_info->string_index].pinfo;
}

void link_constant_pool(struct Class* pclass)
{
	u2 i;

	struct constant_class_info *pconstant_class_info = NULL;
	struct constant_utf8_info *pconstant_utf8_info = NULL;
	struct constant_methodref_info *pconstant_methodref_info = NULL;
	struct constant_name_and_type_info *pconstant_name_and_type_info = NULL;
	struct constant_fieldref_info* pconstant_fieldref_info = NULL;
	struct constant_interfacemethodref_info* pconstant_interfacemethodref_info = NULL;
	struct constant_string_info* pconstant_string_info = NULL; 
	struct constant_integer_info* pconstant_integer_info = NULL;
	struct constant_method_handle_info* pconstant_method_handle_info = NULL;
	struct constant_method_type_info* pconstant_method_type_info = NULL;
	struct constant_invokedynamic_info* pconstant_invokedynamic_info = NULL;

	struct cp_info* pcp_info = pclass->pcp_info;

	u1* pconstant_pool_status = (u1* )malloc(sizeof(u1) * pclass->constant_pool_count);
	memset(pconstant_pool_status, UN_LINKED, sizeof(u1) * pclass->constant_pool_count);

	// the constant pool start from index 1
	for (i = 1; i < pclass->constant_pool_count; i++)
	{
		if (*(pconstant_pool_status + i) == CLASS_LINKED)
		{
			continue;
		}
		switch(pcp_info[i].tag)
		{
		case CONSTANT_Class:

			link_constant_class_info(pclass, i);

			break;
		case CONSTANT_Fieldref:
			pconstant_fieldref_info = (struct constant_fieldref_info* )pcp_info[i].pinfo;

			// the constant_class_info at class_index has not been linked
			if (*(pconstant_pool_status + pconstant_fieldref_info->class_index) == UN_LINKED)
			{
				link_constant_class_info(pclass, pconstant_fieldref_info->class_index);
				*(pconstant_pool_status + pconstant_fieldref_info->class_index) = CLASS_LINKED;
				
			}

			pconstant_fieldref_info->pclass = ((struct constant_class_info* )pcp_info[pconstant_fieldref_info->class_index].pinfo)->pclass;

			if (*(pconstant_pool_status + pconstant_fieldref_info->name_and_type_index)  == UN_LINKED)
			{
				//pconstant_name_and_type_info->pname_constant_utf8_info
				link_constant_name_and_type_info(pclass, pconstant_fieldref_info->name_and_type_index);
				*(pconstant_pool_status + pconstant_fieldref_info->name_and_type_index) = CLASS_LINKED;
			}
			pconstant_fieldref_info->pconstant_name_and_type_info = (struct constant_name_and_type_info* )pcp_info[pconstant_fieldref_info->name_and_type_index].pinfo;

			break;
		case CONSTANT_Methodref:
			pconstant_methodref_info = (struct constant_methodref_info * )pcp_info[i].pinfo;

			if (*(pconstant_pool_status + pconstant_methodref_info->class_index) == UN_LINKED)
			{
				link_constant_class_info(pclass, pconstant_methodref_info->class_index);
				*(pconstant_pool_status + pconstant_methodref_info->class_index) = CLASS_LINKED;
			}

			pconstant_methodref_info->pclass = ((struct constant_class_info* )(pcp_info[pconstant_methodref_info->class_index].pinfo))->pclass;

			if (*(pconstant_pool_status + pconstant_methodref_info->name_and_type_index) == UN_LINKED)
			{
				link_constant_name_and_type_info(pclass, pconstant_methodref_info->name_and_type_index);
				*(pconstant_pool_status + pconstant_methodref_info->name_and_type_index) = CLASS_LINKED;
			}

			pconstant_methodref_info->pconstant_name_and_type_info = (struct constant_name_and_type_info* )pcp_info[pconstant_methodref_info->name_and_type_index].pinfo;

			break;
		case CONSTANT_InterfaceMethodref:
			pconstant_interfacemethodref_info = (struct constant_interfacemethodref_info* )pcp_info[i].pinfo;

			if (*(pconstant_pool_status + pconstant_interfacemethodref_info->class_index) == UN_LINKED)
			{
				link_constant_class_info(pclass, pconstant_interfacemethodref_info->class_index);
				*(pconstant_pool_status + pconstant_interfacemethodref_info->class_index) = CLASS_LINKED;
			}

			pconstant_interfacemethodref_info->pclass = ((struct constant_class_info* )(pcp_info[pconstant_interfacemethodref_info->class_index].pinfo))->pclass;

			if (*(pconstant_pool_status + pconstant_interfacemethodref_info->name_and_type_index) == UN_LINKED)
			{
				link_constant_name_and_type_info(pclass, pconstant_interfacemethodref_info->name_and_type_index);
				*(pconstant_pool_status + pconstant_interfacemethodref_info->name_and_type_index) = CLASS_LINKED;
			}

			pconstant_interfacemethodref_info->pconstant_name_and_type_info = (struct constant_name_and_type_info* )pcp_info[pconstant_interfacemethodref_info->name_and_type_index].pinfo;
			
			break;
		case CONSTANT_String:
			pconstant_string_info = (struct constant_string_info* )pcp_info[i].pinfo;
			link_constant_string_info(pclass, i);

			break;
		case CONSTANT_Integer:
			break;
		case CONSTANT_Float:
			//TODO now we do not support float
			printf("warning: now we do not support float\n");
			break;

		// ATTENTION: long and double take two entries in the constant pool
		// please refer to 4.4.5
		case CONSTANT_Long:
			//TODO now we do not support long
			printf("warning: now we do not support long\n");
			// long and double occupy two entry
			i ++;
			break;
		case CONSTANT_Double:
			//TODO now we do not support double 
			printf("warning: now we do not support double\n");
			i ++;
			break;

		case CONSTANT_NameAndType:
			pconstant_name_and_type_info = (struct constant_name_and_type_info * )pcp_info[i].pinfo;
			link_constant_name_and_type_info(pclass, i);
			break;
		case CONSTANT_Utf8:
			//we do need to link utf8
			break;
		case CONSTANT_MethodHandle:
			printf("-------------do not support method handle");
			break;
		case CONSTANT_MethodType:
			link_constant_method_type_info(pclass, i);
			break;
		case CONSTANT_InvokeDynamic:
			break;
		default:
			printf("default: tag error!\n");
			break;
		}
		*(pconstant_pool_status + i) = CLASS_LINKED;
	}

	free(pconstant_pool_status);
}

static void link_field(struct Class* pclass)
{
	int i;
	struct field_info* pfield_info = pclass->pfields;
	
	for (i = 0; i < pclass->fields_count; i ++)
	{
		pfield_info[i].pname_constant_utf8_info = ((struct constant_utf8_info*)(pclass->pcp_info[pfield_info[i].name_index].pinfo));
		pfield_info[i].pdescriptor_constant_utf8_info = ((struct constant_utf8_info* )(pclass->pcp_info[pfield_info[i].descriptor_index].pinfo));
	}

}

static void link_method(struct Class* pclass)
{
	int i;
	struct method_info* pmethod_info = pclass->pmethods;

	for (i = 0; i < pclass->method_count;i ++)
	{
		pmethod_info[i].pname_constant_utf8_info = (struct constant_utf8_info* )(pclass->pcp_info[pmethod_info[i].name_index].pinfo);
		pmethod_info[i].pdescriptor_constant_utf8_info = (struct constant_utf8_info* )(pclass->pcp_info[pmethod_info[i].descriptor_index].pinfo);
	}

}

// do not need to link the attributes
static void link_attribute(struct Class* pclass)
{

}

// attention: this method must not be called before link_constant_pool
static void link_this_and_super(struct Class* pclass)
{
	struct constant_class_info* pconstant_class_info = (struct constant_class_info* )pclass->pcp_info[pclass->this_class].pinfo;
	pclass->pthis_class = pconstant_class_info->pclass;

	// Object does not have super class
	if (pclass->super_class != 0)
	{
		pconstant_class_info = (struct constant_class_info* )pclass->pcp_info[pclass->super_class].pinfo;
		pclass->psuper_class = pconstant_class_info->pclass;
	}
}

//TODO link the fields, methods and attribute
void link()
{
	extern struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);

	int i;
	struct class_entry* pclass_entry;
	for (i = 0; i < CLASS_TABLE_SIZE; i ++)
	{
		pclass_entry = loaded_class_table[i];

		while(pclass_entry != NULL)
		{
			/*
			link_constant_pool(pclass_entry->pclass);
			link_field(pclass_entry->pclass);
			link_method(pclass_entry->pclass);
			link_this_and_super(pclass_entry->pclass);
			*/

			link_class(pclass_entry->pclass);
			pclass_entry = pclass_entry->next;
		}
	}
}

void link_class(struct Class* pclass)
{
	link_constant_pool(pclass);
	link_field(pclass);
	link_method(pclass);
	link_this_and_super(pclass);

	pclass->status = CLASS_LINKED;
}