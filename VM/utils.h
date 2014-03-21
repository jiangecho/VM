#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include "def.h"

//rotatingHash
u4 hash(u1* pchar, u2 len, u4 prime);
// C does not support override;
//u4 hash(u1* pchar, u4 prime);

//TODO 
// 0, not equal
// 1, equal
int compare(u1* pchar1, u4 len1, u1* pchar2, u4 len2);

u4 fread_u4(FILE *fp);
u2 fread_u2(FILE *fp);
u1 fread_u1(FILE *fp);

int fread_int(FILE *fp);
short fread_short(FILE *fp);
long fread_long(FILE *fp);

#endif