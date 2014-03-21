#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdio.h>
#include "def.h"


u1* init_code_area(u4 max_size);
void uninit_code_area();
u1* malloc_code_area(u4 size);

// TODO we do not need to free the code area, the code area can only be write once, and then it will
// be read only.
void free_code_area(u1* pcode_area);

u1* init_heap(u4 max_size);
void uninit_heap();

u1* malloc_heap(u4 size);
void free_heap(u1* pheap);

u1* init_stack(u4 max_size);
void uninit_stack();


#endif