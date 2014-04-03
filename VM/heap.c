#include "heap.h"
#include "object.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void add_memory_entry_to_free_memory_list(struct memory_entry* pmemory_entry);

struct memory_entry
{
	void* start_addr;
	u4 len;
	struct memory_entry* next;
	struct memory_entry* prev;
};

struct heap_header
{
	u4 free_size;
	u4 memory_entry_count;
	struct memory_entry* free_memory_list;
};

//static struct memory_entry* free_memory_list;
static struct heap_header* pheap_header;

u1 init_heap(u4 heap_max_size)
{
	struct memory_entry* memory_entry;
	pheap_header = (struct heap_header* )malloc(sizeof(struct heap_header));
	
	if (pheap_header == NULL)
	{
		return FAIL;
	}

	memory_entry = (struct memory_entry* )malloc(sizeof(struct memory_entry));
	if (memory_entry)
	{
		if (memory_entry->start_addr = malloc(heap_max_size))
		{
			memory_entry->len = heap_max_size;
			memory_entry->next = NULL;
			memory_entry->prev = NULL;

			pheap_header->free_memory_list = memory_entry;
			pheap_header->free_size = heap_max_size;
			pheap_header->memory_entry_count = 1;
			return OK;
		}
		else
		{
			pheap_header = NULL;
			free(pheap_header);
			free(memory_entry);
			return FAIL;
		}
	}
	else
	{
		free(pheap_header);
		return FAIL;
	}
}


// the fastest matching method
// if the return is NULL, maybe there are too many fragments, triger GC,
// and have another try
void* malloc_heap(u4 size)
{
	// the first sizeof(u2) bytes are used to store the size
	u2 real_size = size + sizeof(u2);
	void* p = NULL;
	struct memory_entry* pmemory_entry = pheap_header->free_memory_list;

	if (real_size > pheap_header->free_size)
	{
		printf("do not have enough heap memory\n");
		return NULL;
	}

	while(pmemory_entry != NULL)
	{
		if (pmemory_entry->len > real_size)
		{
			p = (u1* )pmemory_entry->start_addr;
			*(u2* )p = real_size;
			p = (u1* )p + sizeof(u2);
			pmemory_entry->start_addr = (u1* )(pmemory_entry->start_addr) + real_size;
			pmemory_entry->len = pmemory_entry->len - real_size;
			break;
		}
		else if (pmemory_entry->len == real_size)
		{
			p = (u1* )pmemory_entry->start_addr;
			*(u2* )p = real_size;
			p = (u1* )p + sizeof(u2);

			if (pmemory_entry->prev)
			{
				// when the current memory_entry is the last one, pmemory_entry->next will be NULL
				pmemory_entry->prev->next = pmemory_entry->next;
			}
			else
			{
				// the first entry has been malloced
				pheap_header->free_memory_list = pmemory_entry->next;
			}

			if (pmemory_entry->next)
			{
				pmemory_entry->next->prev = pmemory_entry->prev;
			}

			pheap_header->memory_entry_count --;
			free(pmemory_entry);
			break;
		}
		else
		{
			pmemory_entry = pmemory_entry->next;
		}

	}

	if (p)
	{
		memset(p, 0, size);
	}
	return p;
}

// TODO now only support Object*
void free_heap(void* p)
{
	struct memory_entry* pmemory_entry = (struct memory_entry* )malloc(sizeof(struct memory_entry));
	if (! pmemory_entry)
	{
		printf("malloc fail\n");
		return;
	}

	pmemory_entry->start_addr = (u1* )p - sizeof(u2);
	pmemory_entry->len = *(u2 *)(pmemory_entry->start_addr);
	pmemory_entry->prev = NULL;
	pmemory_entry->next = NULL;

	add_memory_entry_to_free_memory_list(pmemory_entry);

}


// the memory list would aways be sorted by the memory_entry's start address
static void add_memory_entry_to_free_memory_list(struct memory_entry* pmemory_entry)
{
	struct memory_entry* ptmp;
	struct memory_entry* ptmp1;
	if (pmemory_entry == NULL)
	{
		return;
	}

	ptmp = pheap_header->free_memory_list;

	if (ptmp == NULL)
	{
		pheap_header->free_memory_list = pmemory_entry;
		pheap_header->memory_entry_count ++;
		return;
	}

	while(ptmp != NULL)
	{
		if (pmemory_entry->start_addr < ptmp->start_addr)
		{
			// can combine with the previous entry and the next entry at the same time
			if (((u1* )pmemory_entry->start_addr) + pmemory_entry->len == ptmp->start_addr
				&& (ptmp->prev != NULL)
				&& ((u1* )(ptmp->prev->start_addr) + ptmp->prev->len == pmemory_entry->start_addr))
			{
				// TODO 
				ptmp1 = ptmp->prev;
				ptmp1->len += ptmp->len + pmemory_entry->len;

				ptmp1->next = ptmp->next;

				if (ptmp->next)
				{
					ptmp->next->prev = ptmp1;
				}
				free(pmemory_entry);
				free(ptmp);
			}
			// can combine with the next entry
			else if(((u1* )pmemory_entry->start_addr) + pmemory_entry->len == ptmp->start_addr)
			{
				ptmp->start_addr = (u1* )(ptmp->start_addr) - pmemory_entry->len;
				ptmp->len += pmemory_entry->len;
				free(pmemory_entry);
			}
			// can combine with the previous entry
			else if ((ptmp->prev != NULL)
				&& ((u1* )(ptmp->prev->start_addr) + ptmp->prev->len == pmemory_entry->start_addr))
			{
				// TODO 
				ptmp->prev->len += pmemory_entry->len;
				free(pmemory_entry);
			}
			//can not combine neither the previous entry nor the next entry
			else
			{
				pmemory_entry->next = ptmp;
				ptmp->prev = pmemory_entry;
				pheap_header->free_memory_list = pmemory_entry;

				pheap_header->memory_entry_count ++;
			}

			break;
		}
		else
		{
			// now ptmp is the last entry
			if (ptmp->next == NULL)
			{
				if ((u1* )(ptmp->start_addr) + ptmp->len == pmemory_entry->start_addr)
				{
					ptmp->len += pmemory_entry->len;
					free(pmemory_entry);
				}
				else
				{
					ptmp->next = pmemory_entry;
					pmemory_entry->prev = ptmp;

					pheap_header->memory_entry_count ++;
				}
				break;
			}
			else
			{
				ptmp = ptmp->next;
			}

		}

	}

}


u4 get_heap_total_free_size()
{
	return pheap_header->free_size;
}

u4 get_heap_memory_entry_count()
{
	return pheap_header->memory_entry_count;
}

//TODO print the heap info
void print_heap_info()
{

}


