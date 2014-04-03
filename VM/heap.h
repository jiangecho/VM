#ifndef _HEAP_H_
#define _HEAP_H_

#include "def.h"
#include "conf.h"

u1 init_heap(u4 heap_max_size);
void* malloc_heap(u4 size);

// TODO now only support Object*
void free_heap(void* p);

u4 get_heap_total_free_size();
u4 get_heap_memory_entry_count();


void print_heap_info();


#endif