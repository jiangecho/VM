#include "class.h"
#include "def.h"
#include "conf.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>

static u2 get_total_public_protected_fields_size(struct Class* pclass, u1 fileds_type);

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

struct method_info* find_method(char* class_name, u2 class_name_len, char* method_name, u2 method_name_len)
{
	extern struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);
	struct class_entry* pclass_entry;
	struct Class* pclass;
	struct cp_info* pcp_info;
	struct constant_utf8_info* pconstant_utf8_info;
	struct constant_class_info* pconstant_class_info;
	struct method_info* pmethods;
	struct method_info* pmethod = NULL;

	u2 class_name_hash = hash(class_name, class_name_len, CLASS_TABLE_SIZE);
	u2 class_index;
	u2 class_name_index;

	int i;

	pclass_entry = loaded_class_table[class_name_hash];
	if (pclass_entry != NULL)
	{
		while(pclass_entry != NULL)
		{
			//TODO use pthis_class
			pclass = pclass_entry->pclass;
			class_index= pclass->this_class;
			pcp_info = pclass->pcp_info;
			pmethods = pclass->pmethods;

			pconstant_class_info = (struct constant_class_info* )pcp_info[class_index].pinfo;
			class_name_index = pconstant_class_info->name_index;

			pconstant_utf8_info = (struct constant_utf8_info* )pcp_info[class_name_index].pinfo;

			if (compare(class_name, strlen(class_name), pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
			{
				for (i = 0; i < pclass->method_count; i ++)
				{
					pconstant_utf8_info = (struct constant_utf8_info* )pcp_info[pmethods[i].name_index].pinfo;
					if (compare(method_name, method_name_len, pconstant_utf8_info->pbytes, pconstant_utf8_info->length))
					{
						pmethod = &(pmethods[i]);
						
						goto found;
					}
				}
			}
			else
			{
				pclass_entry = pclass_entry->next;
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
	return pclass->class_fields_size - pclass->public_protected_class_fields_size + get_total_public_protected_fields_size(pclass, 0);
}


u2 get_instance_total_fields_size(struct Class* pclass)
{
	return pclass->instance_fileds_size - pclass->public_protected_instance_fields_size + get_total_public_protected_fields_size(pclass, 1);
}


static u2 get_total_public_protected_fields_size(struct Class* pclass, u1 fileds_type)
{
	int i;
	u2 size = 0;
	struct Class* ptmp_class;

	if (pclass->psuper_class != NULL)
	{
		if (fileds_type == 0)
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
		if (fileds_type == 0)
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