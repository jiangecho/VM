#include "classloader.h"
#include "memory.h"
#include "class.h"
#include "def.h"
#include "conf.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC 0xbebafeca

// the first element is the head, does not contain any entry
static struct pending_load_class* ppending_load_class = NULL;

//global
//struct class_entry* ploaded_classes = NULL;
struct class_entry *(loaded_class_table[CLASS_TABLE_SIZE]);

void init_class_loader();
struct Class* load_class(char* pclass_path);
void prepare_class(struct Class** ppclass);

/*
* yes, 1
* no, 0
*/
u1 is_class_loaded(struct class_name_entry* pclass_name_entry);
void add_class_to_pending_list(struct class_name_entry* pclass_name_entry);
void add_class_to_loaded_class_table(struct Class* pclass);
u1 is_class_in_pending_list(struct class_name_entry* pclass_name_entry);


u1 is_class_loaded(struct class_name_entry* pclass_name_entry)
{
	u1 is_loaded = 0;
	struct class_name_entry* ptmp_class_name_entry = (struct class_name_entry* )malloc(sizeof(struct class_name_entry));
	u4 name_hash = hash(pclass_name_entry->pname, pclass_name_entry->name_len, CLASS_TABLE_SIZE);
	struct class_entry* pclass_entry = loaded_class_table[name_hash]; 

	while(pclass_entry != NULL)
	{
		// TODO
		get_class_name(pclass_entry->pclass, &ptmp_class_name_entry);
		
		// 1, equal
		if (compare(ptmp_class_name_entry->pname, ptmp_class_name_entry->name_len, pclass_name_entry->pname, pclass_name_entry->name_len))
		{
			is_loaded = 1;
			break;
		}
		pclass_entry = pclass_entry->next;
	}

	free(ptmp_class_name_entry);
	return is_loaded;
}

//1, has been added to the pending list
//0, does not 
u1 is_class_in_pending_list(struct class_name_entry* pclass_name_entry)
{
	u1 ret = 0;
	// the head is emputy
	struct pending_load_class* pcur = ppending_load_class->next;
	while(pcur != NULL)
	{
		if (pclass_name_entry->name_len == pcur->pclass_name_entry->name_len
			&& compare(pclass_name_entry->pname, pclass_name_entry->name_len, 
						pcur->pclass_name_entry->pname, pcur->pclass_name_entry->name_len))
		{
			ret = 1;
			break;
		}
		pcur = pcur->next;
	}

	return ret;
}

void add_class_to_pending_list(struct class_name_entry* pclass_name_entry)
{
	struct pending_load_class* pcur = ppending_load_class;
	struct pending_load_class* pnew_entry;


	pnew_entry = (struct pending_load_class* )malloc(sizeof(struct pending_load_class));
	pnew_entry->pclass_name_entry = pclass_name_entry;
	pnew_entry->next = NULL;


	while(pcur->next != NULL)
	{
		pcur = pcur->next;
	}
	pcur->next = pnew_entry;

}

void add_class_to_loaded_class_table(struct Class* pclass)
{
	u2 name_hash;
	struct class_entry** pcur_class_entry;
	struct class_entry* pnew_class_entry;
	struct class_name_entry* pclass_name_entry;

	if (pclass == NULL)
	{
		printf("%d  %d, null pointer\n", __FILE__, __LINE__);
		return;
	}

	pclass_name_entry = (struct class_name_entry* )malloc(sizeof(struct class_name_entry));
	get_class_name(pclass, &pclass_name_entry);

	name_hash = hash(pclass_name_entry->pname, pclass_name_entry->name_len, CLASS_TABLE_SIZE);
	pcur_class_entry = &(loaded_class_table[name_hash]);

	free(pclass_name_entry);

	pnew_class_entry = (struct class_entry* )malloc(sizeof(struct class_entry));
	pnew_class_entry->pclass = pclass;
	pnew_class_entry->next = NULL;

	if (*pcur_class_entry != NULL)
	{
		while((*pcur_class_entry)->next != NULL)
		{
			*pcur_class_entry = (*pcur_class_entry)->next;
		}

		(*pcur_class_entry)->next = pnew_class_entry;
	}
	else
	{
		*pcur_class_entry = pnew_class_entry;
		(*pcur_class_entry)->next = NULL;
	}


}


void init_class_loader()
{
	//TODO remember to free the memory
	ppending_load_class = (struct pending_load_class* )malloc(sizeof(struct pending_load_class));
	ppending_load_class->next = NULL;

	memset(loaded_class_table, 0, sizeof(loaded_class_table));

}

//TODO do not consider the package name
// load class from class file
struct Class* load_class(char* pclass_path)
{
	u1 tag;
	u2 *pinterfaces_index_in_constant_pool = NULL;

	//ATTENTION: please open the file in binary mode!!!
	//TODO open the file successfully?
	FILE *pfile = fopen(pclass_path, "rb");

	struct Class *pclass = (struct Class *)malloc_code_area(sizeof(struct Class));

	struct cp_info *pcp_info = NULL;
	struct field_info *pfield_info = NULL;
	struct attribute_info *pattribute_info = NULL;
	struct method_info *pmethod_info = NULL;

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

	struct class_name_entry* pclass_name_entry;

	struct code_attribute_info* pcode_attribute_info;
	struct exception_table_entry* pexception_table_entry;

	u4 i, j, k, m, n;
	if (pclass == NULL) 
	{
		printf("do not have enough code area\n");
		return NULL;
	}
	if (pfile == NULL)
	{
		printf("can not open file: %s\n", pclass_path);
		return NULL;
	}
	pclass->status = CLASS_LOADING;
	pclass->magic = fread_u4(pfile);
	pclass->minor_version = fread_u2(pfile);
	pclass->major_version = fread_u2(pfile);
	pclass->constant_pool_count = fread_u2(pfile); // == plus one

	pcp_info = (struct cp_info *)malloc_code_area(sizeof(struct cp_info) * pclass->constant_pool_count);
	pclass->pcp_info = pcp_info;

	//The constant_pool table is indexed from 1 to constant_pool_count-1
	pcp_info ++;
	for (i = 1; i < pclass->constant_pool_count; i++)
	{
		tag = fread_u1(pfile);
		pcp_info->tag = tag;

		switch(tag)
		{
		case CONSTANT_Class:
			pconstant_class_info = (struct constant_class_info *)malloc_code_area(sizeof(struct constant_class_info));
			pconstant_class_info->name_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_class_info;

			break;
		case CONSTANT_Fieldref:
			pconstant_fieldref_info = (struct constant_fieldref_info*)malloc_code_area(sizeof(struct constant_fieldref_info));
			pconstant_fieldref_info->class_index = fread_u2(pfile);
			pconstant_fieldref_info->name_and_type_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_fieldref_info;
			break;
		case CONSTANT_Methodref:
			pconstant_methodref_info = (struct constant_methodref_info *)malloc_code_area(sizeof(struct constant_methodref_info));
			pconstant_methodref_info->class_index = fread_u2(pfile);
			pconstant_methodref_info->name_and_type_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_methodref_info;
			break;
		case CONSTANT_InterfaceMethodref:
			pconstant_interfacemethodref_info = (struct constant_interfacemethodref_info*)malloc_code_area(sizeof(struct constant_interfacemethodref_info));
			pconstant_interfacemethodref_info->class_index = fread_u2(pfile);
			pconstant_interfacemethodref_info->name_and_type_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_interfacemethodref_info;
			break;
		case CONSTANT_String:
			pconstant_string_info = (struct constant_string_info*)malloc_code_area(sizeof(struct constant_string_info));
			pconstant_string_info->string_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_string_info;
			break;
		case CONSTANT_Integer:
			pconstant_integer_info = (struct constant_integer_info*)malloc_code_area(sizeof(struct constant_integer_info));
			pconstant_integer_info->value = fread_int(pfile);
			pcp_info->pinfo = pconstant_integer_info;
			break;
		case CONSTANT_Float:
			//TODO now we do not support float
			printf("warning: now we do not support float\n");
			fread_u4(pfile);
			break;

		// ATTENTION: long and double take two entries in the constant pool
		// please refer to 4.4.5
		case CONSTANT_Long:
			//TODO now we do not support long
			printf("warning: now we do not support long\n");
			fread_u4(pfile);
			fread_u4(pfile);

			i ++;
			pcp_info ++;

			break;
		case CONSTANT_Double:
			//TODO now we do not support double 
			printf("warning: now we do not support double\n");
			fread_u4(pfile);
			fread_u4(pfile);

			i ++;
			pcp_info ++;

			break;

		case CONSTANT_NameAndType:
			pconstant_name_and_type_info = (struct constant_name_and_type_info *)malloc_code_area(sizeof(struct constant_name_and_type_info));
			pconstant_name_and_type_info->name_index = fread_u2(pfile);
			pconstant_name_and_type_info->descriptor_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_name_and_type_info;
			break;
		case CONSTANT_Utf8:
			pconstant_utf8_info = (struct constant_utf8_info *)malloc_code_area(sizeof(struct constant_utf8_info));
			pconstant_utf8_info->length = fread_u2(pfile);
			pconstant_utf8_info->pbytes = (u1*)malloc_code_area(pconstant_utf8_info->length);

			if (pconstant_utf8_info->length > 0)
			{
				for (j = 0; j < pconstant_utf8_info->length; j++)
				{
					*(pconstant_utf8_info->pbytes + j) = fread_u1(pfile);
				}
			}

			pcp_info->pinfo = pconstant_utf8_info;
			break;
		case CONSTANT_MethodHandle:
			pconstant_method_handle_info = (struct constant_method_handle_info*)malloc_code_area(sizeof(struct constant_method_handle_info));
			pconstant_method_handle_info->reference_kind = fread_u1(pfile);
			pconstant_method_handle_info->reference_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_method_handle_info;
			break;
		case CONSTANT_MethodType:
			pconstant_method_type_info = (struct constant_method_type_info*)malloc_code_area(sizeof(struct constant_method_type_info));
			pconstant_method_type_info->descriptor_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_method_type_info;
			break;
		case CONSTANT_InvokeDynamic:
			pconstant_invokedynamic_info = (struct constant_invokedynamic_info*)malloc_code_area(sizeof(struct constant_invokedynamic_info));
			pconstant_invokedynamic_info->bootstrap_method_attr_index = fread_u2(pfile);
			pconstant_invokedynamic_info->name_and_type_index = fread_u2(pfile);
			pcp_info->pinfo = pconstant_invokedynamic_info;
			break;
		default:
			printf("default: tag error!\n");
			break;

		}

		pcp_info ++;
	}
	pclass->access_flags = fread_u2(pfile);
	pclass->this_class = fread_u2(pfile);
	pclass->super_class = fread_u2(pfile);

	pclass->interfaces_count = fread_u2(pfile);
	if (pclass->interfaces_count > 0)
	{
		pinterfaces_index_in_constant_pool = (u2 *)malloc_code_area(
			sizeof(u2) * pclass->interfaces_count);

		pclass->pinterfaces_index_in_constant_pool = pinterfaces_index_in_constant_pool;
		for (i = 0; i < pclass->interfaces_count; i++)
		{
			*(pinterfaces_index_in_constant_pool + i) = fread_u2(pfile);
		}
	}
	else
	{
		pclass->pinterfaces_index_in_constant_pool = NULL;
	}

	pclass->fields_count = fread_u2(pfile);
	if (pclass->fields_count > 0)
	{
		pfield_info = (struct field_info *)malloc_code_area(sizeof(struct field_info) * pclass->fields_count);
		pclass->pfields = pfield_info;
		for (i = 0; i < pclass->fields_count; i ++)
		{
			pfield_info->access_flags = fread_u2(pfile);
			pfield_info->name_index = fread_u2(pfile);
			pfield_info->descriptor_index = fread_u2(pfile);
			pfield_info->attributes_count = fread_u2(pfile);

			if (pfield_info->attributes_count > 0)
			{
				pattribute_info = (struct attribute_info *)malloc_code_area(sizeof(struct attribute_info) * pfield_info->attributes_count);
				pfield_info->pattributes = pattribute_info;

				for (j = 0; j < pfield_info->attributes_count; j ++)
				{
					pattribute_info->attribute_name_index = fread_u2(pfile);
					pattribute_info->attribute_length = fread_u4(pfile);

					if (pattribute_info->attribute_length > 0)
					{
						pattribute_info->pinfo = (u1* )malloc_code_area(pattribute_info->attribute_length);
						for (k = 0; k < pattribute_info->attribute_length; k++)
						{
							//TODO
							//*(pattribute_info->pinfo + k) = fread_u1(pfile);
							fread_u1(pfile);
						}
						
					}
					else
					{
						pattribute_info->pinfo = NULL;
					}

					pattribute_info ++;

				}

			}
			else
			{
				pfield_info->pattributes = NULL;
			}


			pfield_info ++;

		}

	}
	else
	{
		pclass->pfields = NULL;

	}

	pclass->method_count = fread_u2(pfile);
	if (pclass->method_count > 0)
	{
		pmethod_info = (struct method_info *)malloc_code_area(sizeof(struct method_info) * pclass->method_count);
		pclass->pmethods = pmethod_info;
		for (i = 0; i < pclass->method_count; i ++)
		{
			pmethod_info->access_flags = fread_u2(pfile);
			pmethod_info->name_index = fread_u2(pfile);
			pmethod_info->descriptor_index = fread_u2(pfile);
			pmethod_info->attributes_count = fread_u2(pfile);

			if (pmethod_info->attributes_count > 0)
			{
				pattribute_info = (struct attribute_info *)malloc_code_area(sizeof(struct attribute_info) * pmethod_info->attributes_count);
				pmethod_info->pattributes = pattribute_info;

				for (j = 0; j < pmethod_info->attributes_count; j ++)
				{
					pattribute_info->attribute_name_index = fread_u2(pfile);
					pattribute_info->attribute_length = fread_u4(pfile);

					// TODO now we only support code attribute in the method's attributes
					switch(convert_attribute_name_2_int_value(pclass, pattribute_info))
					{
					case CODE_ATTRIBUTE_INT_TYPE:
						pcode_attribute_info = (struct code_attribute_info* )malloc_code_area(sizeof(struct code_attribute_info));
						pcode_attribute_info->max_stack = fread_u2(pfile);
						pcode_attribute_info->max_locals = fread_u2(pfile);
						pcode_attribute_info->code_length = fread_u4(pfile);

						if (pcode_attribute_info->code_length > 0)
						{
							pcode_attribute_info->pcode = (u1* )malloc_code_area(sizeof(u1) * pcode_attribute_info->code_length);
							for (k = 0; k < pcode_attribute_info->code_length; k ++)
							{
								*(pcode_attribute_info->pcode + k) = fread_u1(pfile);
							}
						}

						pcode_attribute_info->exception_table_length = fread_u2(pfile);
						if (pcode_attribute_info->exception_table_length > 0)
						{
							pexception_table_entry = (struct exception_table_entry* )malloc_code_area(sizeof(struct exception_table_entry) * pcode_attribute_info->exception_table_length);
							pcode_attribute_info->pexception_table_entry = pexception_table_entry;
							for (k = 0; k < pcode_attribute_info->exception_table_length; k ++)
							{
								pexception_table_entry->start_pc = fread_u2(pfile);
								pexception_table_entry->end_pc = fread_u2(pfile);
								pexception_table_entry->handler_pc = fread_u2(pfile);
								pexception_table_entry->catch_type = fread_u2(pfile);

								pexception_table_entry ++;
							}
						}
						{
							pcode_attribute_info->pexception_table_entry = NULL;
						}

						//TODO now we do not support additional attributes in the code attribute
						pcode_attribute_info->attributes_count = fread_u2(pfile);
						if (pcode_attribute_info->attributes_count > 0)
						{
							for (m = 0; m < pcode_attribute_info->attributes_count; m++)
							{
								// attribute_name_index;
								fread_u2(pfile);
								n = fread_u4(pfile);
								for (k = 0; k < n; k++)
								{
									fread_u1(pfile);
								}
							}
						}

						pattribute_info->pinfo = pcode_attribute_info;
						break;

					default:
						for (k = 0; k < pattribute_info->attribute_length; k++)
						{
							//TODO
							//*(pattribute_info->pinfo + k) = fread_u1(pfile);
							fread_u1(pfile);
							pattribute_info->pinfo = NULL;
						}
						break;
					}

					/*
					if (pattribute_info->attribute_length > 0)
					{
						pattribute_info->pinfo = (u1* )malloc_code_area(pattribute_info->attribute_length);
						for (k = 0; k < pattribute_info->attribute_length; k++)
						{
							//TODO
							//*(pattribute_info->pinfo + k) = fread_u1(pfile);
							fread_u1(pfile);
						}
					}
					else
					{
						pattribute_info->pinfo = NULL;
					}
					*/

					pattribute_info ++;
				}
			} 
			else
			{
				pmethod_info->pattributes = NULL;
			}

			pmethod_info++;
		}


	} 
	else
	{
		pclass->pmethods = NULL;
	}

	pclass->attributes_count = fread_u2(pfile);
	if (pclass->attributes_count > 0)
	{
		pattribute_info = (struct attribute_info *)malloc_code_area(sizeof(struct attribute_info) * pclass->attributes_count);
		pclass->pattributes = pattribute_info;
		for (j = 0; j < pclass->attributes_count; j ++)
		{
			pattribute_info->attribute_name_index = fread_u2(pfile);
			pattribute_info->attribute_length = fread_u4(pfile);

			if (pattribute_info->attribute_length > 0)
			{
				pattribute_info->pinfo = (u1* )malloc_code_area(pattribute_info->attribute_length);
				for (k = 0; k < pattribute_info->attribute_length; k++)
				{
					//TODO
					//*(pattribute_info->pinfo + k) = fread_u1(pfile);
					fread_u1(pfile);
				}
			}
			else
			{
				pattribute_info->pinfo = NULL;
			}

			pattribute_info ++;
		}
	} 
	else
	{
		pclass->pattributes = NULL;
	}

	/*
#if PRIINT_CLASS_STRUCTURE
	printf("constant_pool_count: %d\n", pclass->constant_pool_count);

	for (i = 1; i < pclass->constant_pool_count; i ++)
	{
		printf("constant_pool[%d].tag: %d\n", i, pclass->pcp_info[i].tag);
	}

#endif
	*/

	//TODO add the needed classes into the pending list and add this class to the loaded table
	add_class_to_loaded_class_table(pclass);
	printf("loaded: %s\n", pclass_path);

	for (i = 1; i < pclass->constant_pool_count; i ++)
	{
		if (pclass->pcp_info[i].tag == CONSTANT_Class)
		{
			//add_class_to_pending_list();
			pclass_name_entry = (struct class_name_entry* )malloc(sizeof(struct class_name_entry));
			constant_class_info2class_name_entry(pclass, (struct constant_class_info* )(pclass->pcp_info[i].pinfo), &pclass_name_entry);
			if (*(pclass_name_entry->pname) == '[')
			{
				printf("do not need to load array type\n");
				continue;
			}
			if (is_class_loaded(pclass_name_entry))
			{
				printf("class: ");
				print_class_name(pclass_name_entry);
				printf(" has been loaded\n");
				continue;
			}

			if (is_class_in_pending_list(pclass_name_entry))
			{
				printf("class: ");
				print_class_name(pclass_name_entry);
				printf(" has been added to pending list\n");
				continue;
			}

			printf("add class: ");
			print_class_name(pclass_name_entry);
			printf(" to pending list\n");
			add_class_to_pending_list(pclass_name_entry);
		}
	}

	pclass->status = CLASS_LOADED;
	fclose(pfile);
	return pclass;
}

void load_pending_classes()
{
	struct pending_load_class* pcur;
	struct pending_load_class* ptmp;
	char* pfile_path;
	//include the package name
	u2 name_len;
	u2 path_len;


	pcur = ppending_load_class->next;
	while(pcur != NULL)
	{

		ppending_load_class->next = pcur->next;

		name_len = pcur->pclass_name_entry->name_len;
		path_len = strlen(JRE_LIB_PATH) + name_len + 6;

		/*
		class_name_offset = get_class_name_start_index(pcur->pclass_name_entry);
		pfile_path = (char* )malloc(sizeof(char) * (name_len - class_name_offset + 7));
		memset(pfile_path, 0, name_len - class_name_offset + 1);
		memcpy(pfile_path, pcur->pclass_name_entry->pname + class_name_offset, name_len - class_name_offset);
		memcpy(pfile_path + name_len - class_name_offset, ".class", 7);
		load_class(pfile_path);
		*/

		pfile_path = (char* )malloc(path_len + 1);
		memset(pfile_path, 0, path_len + 1);
		memcpy(pfile_path, JRE_LIB_PATH, sizeof(JRE_LIB_PATH));
		memcpy(pfile_path + strlen(JRE_LIB_PATH), pcur->pclass_name_entry->pname, name_len);
		memcpy(pfile_path + strlen(JRE_LIB_PATH) + name_len, ".class", 6);


		//TODO some arraytype [C do not need to load
		load_class(pfile_path);
		
		//free the memory in the pending list
		ptmp = pcur;
		pcur = ppending_load_class->next;
		free(ptmp);
		free(pfile_path);
	}

}

void prepare_class(struct Class** ppclass){
	struct Class* pclass = *ppclass;
	u2 field_offset_in_instance = 0;
	u2 i;

	for (i = 0; i < pclass->constant_pool_count; i++)
	{
		if (pclass->pcp_info->tag == CONSTANT_Fieldref)
		{
			//TODO 
		}
	}

}