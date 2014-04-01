#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static u1* pcode_area = NULL;
static u4 code_area_offset = 0;
static u4 code_area_max_size = 0; 

u1* init_code_area(u4 max_size)
{
	u1* p = (u1*)malloc(max_size);
	if(p != NULL)
	{
		pcode_area = p;
		code_area_max_size = max_size;
		code_area_offset = 0;
	}

	// set the code area to 0
	memset(p, 0, max_size);
	printf("init code area ok\n");
	return p;
}

void uninit_code_area()
{
	code_area_max_size = 0;
	code_area_offset = 0;
	pcode_area = NULL;
	free(pcode_area);
	//printf("uninit code area ok\n");
}

u1* malloc_code_area(u4 size)
{
	u1* p = NULL;
	if ((pcode_area != NULL)
		&&(size > 0)
		&& (code_area_offset < code_area_max_size)
		&& (code_area_offset + size <= code_area_max_size))
	{
		p = pcode_area + code_area_offset;
		code_area_offset += size;
	}
	else
	{
		printf("there is no memory in the code area\n");
	}
	//printf("mallac code area ok\n");
	return p;
}

void free_code_area(u1* pcode_area)
{

}
