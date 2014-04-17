#include "class.h"
#include "def.h"
#include "conf.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

u2 get_total_public_protected_fields_size(struct Class* pclass, u1 fileds_type);

void get_class_name_internal(struct Class* pclass, struct class_name_entry** pclass_name_entry)
{
	u2 this_class_index;
	//u2 this_class_name_index;
	//u2 name_len;
	//u1* pname;
	struct constant_class_info* this_class_info;
	//struct constant_utf8_info* this_class_name_info;
	// check the class format for the class name
	if (pclass == NULL || pclass_name_entry == NULL || *(pclass_name_entry) == NULL)
	{
		printf("null pointer\n");
		return;
	}

	//TODO need more check, for example, whether this_class is valid or not?
	this_class_index = pclass->this_class;
	this_class_info = (struct constant_class_info* )pclass->pcp_info[this_class_index].pinfo;

	constant_class_info2class_name_entry(pclass, this_class_info, pclass_name_entry);

	/*
	this_class_name_index = this_class_info->name_index;
	this_class_name_info = (struct constant_utf8_info* )pclass->pcp_info[this_class_name_index].pinfo;

	name_len = this_class_name_info->length;
	pname = this_class_name_info->pbytes;

	(*pclass_name_entry)->name_len = name_len;
	(*pclass_name_entry)->pname = pname;
	*/
}

void constant_class_info2class_name_entry(struct Class* pclass, struct constant_class_info* pconstant_class_info, struct class_name_entry** pclass_name_entry)
{
	u1* pname;
	u2 name_len;
	u2 this_class_name_index;
	struct constant_utf8_info* this_class_name_info;


	this_class_name_index = pconstant_class_info->name_index;
	this_class_name_info = (struct constant_utf8_info* )pclass->pcp_info[this_class_name_index].pinfo;

	name_len = this_class_name_info->length;
	pname = this_class_name_info->pbytes;

	(*pclass_name_entry)->name_len = name_len;
	(*pclass_name_entry)->pname = pname;

}

u2 get_class_name_start_index(struct class_name_entry* pclass_name_entry)
{
	char* p = pclass_name_entry->pname;
	int offset = pclass_name_entry->name_len;

	while(--offset >= 0)
	{
		if (*(p + offset) == '/')
		{
			break;
		}
	}

	return ++offset;
}

void print_class_name(struct class_name_entry* pclass_name_entry)
{
	int i;
	for (i = 0; i < pclass_name_entry->name_len; i++)
	{
		printf("%c", *(pclass_name_entry->pname + i));
	}
}

struct method_info* find_method(u1* class_name, u2 class_name_len, u1* method_name, u2 method_name_len,
	u1* method_descriptor, u2 method_descriptor_len)
{
	extern struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);
	struct class_entry* pclass_entry;
	struct Class* pclass;
	struct method_info* pmethod = NULL;

	u2 class_name_hash = hash(class_name, class_name_len, CLASS_TABLE_SIZE);

	pclass_entry = loaded_class_table[class_name_hash];
	if (pclass_entry != NULL)
	{
		while(pclass_entry != NULL)
		{
			//TODO use pthis_class
			pclass = pclass_entry->pclass;

			if (compare(class_name, strlen(class_name), pclass_entry->pclass_name_entry->pname, pclass_entry->pclass_name_entry->name_len))
			{
				pmethod = find_class_method(pclass, method_name, method_name_len, method_descriptor, method_descriptor_len);
				break;
			}
			
			else
			{
				pclass_entry = pclass_entry->next;
			}

		}
	}

	return pmethod;

}

struct method_info* find_class_method(struct Class* pclass, u1* method_name, u2 method_name_len,
	u1* method_descriptor, u2 method_descriptor_len)
{
	int i;
	struct constant_utf8_info* pconstant_utf8_info;
	struct cp_info* pcp_info;
	struct method_info* pmethods;
	struct method_info* pmethod = NULL;

	if (pclass->status >= CLASS_LINKED)
	{
		for (i = 0; i < pclass->method_count; i ++)
		{
			pconstant_utf8_info = pclass->pmethods[i].pname_constant_utf8_info;
			if (compare(method_name, method_name_len, pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
			{
				pconstant_utf8_info = pclass->pmethods[i].pdescriptor_constant_utf8_info;
				if (compare(method_descriptor, method_descriptor_len, pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
				{
					pmethod = &(pclass->pmethods[i]);
					
					goto found;
				}
			}
		}
	}
	else
	{
		pcp_info = pclass->pcp_info;
		pmethods = pclass->pmethods;

		for (i = 0; i < pclass->method_count; i ++)
		{
			pconstant_utf8_info = (struct constant_utf8_info* )pcp_info[pmethods[i].name_index].pinfo;
			if (compare(method_name, method_name_len, pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
			{
				pconstant_utf8_info = (struct constant_utf8_info*)pcp_info[pmethods[i].descriptor_index].pinfo;
				if (compare(method_descriptor, method_descriptor_len, pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
				{
					pmethod = &(pmethods[i]);

					goto found;
				}
			}
		}
	}

found:

	return pmethod;
}

struct Class* find_class(char* pclass_name, u2 class_name_len)
{
	extern struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);
	u2 class_name_hash = hash(pclass_name, class_name_len, CLASS_TABLE_SIZE);

	struct class_entry* pclass_entry = loaded_class_table[class_name_hash];
	struct Class* pclass = NULL;

	while(pclass_entry != NULL)
	{
		if (compare(pclass_name, class_name_len, pclass_entry->pclass_name_entry->pname, pclass_entry->pclass_name_entry->name_len))
		{
			pclass = pclass_entry->pclass;	
			break;
		}
		pclass_entry = pclass_entry->next;
	}

	return pclass;

}

// include the fields inherited from the super classes
u2 get_class_total_fields_size(struct Class* pclass)
{
	// add the private fields size;
	if (pclass->class_total_fields_size == 0xFF)
	{
		if (pclass->public_protected_class_total_fields_size == 0xFF)
		{
			pclass->public_protected_class_total_fields_size = get_total_public_protected_fields_size(pclass, CLASS_FIELD);
		}
		pclass->class_total_fields_size = pclass->class_fields_size - pclass->public_protected_class_fields_size + pclass->public_protected_class_total_fields_size; 
	}
	return pclass->class_total_fields_size;
}


u2 get_instance_total_fields_size(struct Class* pclass)
{
	if (pclass->instance_total_fileds_size == 0xFF)
	{
		if (pclass->public_protected_instance_total_fields_size == 0xFF)
		{
			pclass->public_protected_instance_total_fields_size = get_total_public_protected_fields_size(pclass, INSTANCE_FIELD);
		}
		pclass->instance_total_fileds_size = pclass->instance_fileds_size - pclass->public_protected_instance_fields_size + pclass->public_protected_instance_total_fields_size;
	}
	return pclass->instance_total_fileds_size;
}


u2 get_total_public_protected_fields_size(struct Class* pclass, u1 fileds_type)
{
	int i;
	u2 size = 0;
	struct Class* ptmp_class;

	if (pclass->psuper_class != NULL)
	{
		if (fileds_type == CLASS_FIELD)
		{
			size += pclass->public_protected_class_fields_size + get_total_public_protected_fields_size(pclass->psuper_class, fileds_type);
		}
		else
		{
			size += pclass->public_protected_instance_fields_size + get_total_public_protected_fields_size(pclass->psuper_class, fileds_type);
		}

		for (i = 0; i < pclass->interfaces_count; i ++)
		{
			ptmp_class = ((struct constant_class_info* )((pclass->pcp_info[pclass->pinterfaces_index_in_constant_pool[i]]).pinfo))->pclass;
			size += get_total_public_protected_fields_size(ptmp_class, fileds_type);
		}

		return size;
	}
	else
	{
		if (fileds_type == CLASS_FIELD)
		{
			size = pclass->public_protected_class_fields_size;
		}
		else
		{
			size = pclass->public_protected_instance_fields_size;
		}

		return size;
		
	}

}

u1 is_subclass_of(struct Class* pbase_class, struct Class* pclass)
{
	u1 ret = FAIL;
	struct Class* ptmp_class = pclass->psuper_class;
	while(!ptmp_class)
	{
		if (pbase_class == ptmp_class)
		{
			ret = OK;
			break;
		}
		ptmp_class = ptmp_class->psuper_class;

	}

	return ret;

}

u1 is_superclass_of(struct Class* pbase_class, struct Class* pclass)
{
	return is_subclass_of(pclass, pbase_class);
}