#include "utils.h"

/*
// C does not support override;
//TODO now i just want to support a maximum of 2^8 classes, so ...
u4 hash(u1* pchar, u4 prime){
	int n = 0;
	while(*(pchar + n) != '\0')
	{
		n ++;
	}

	return hash(pchar, (u2)n, prime);
}
*/

//TODO now i just want to support a maximum of 2^8 classes, so ...
u4 hash(u1* pchar, u2 len, u4 prime){
	int hash;
	u1 i ;
	hash = len;

	for(i = 0; i < len; i++)
	{
		hash = (hash << 4)^(hash >> 28)^*(pchar + i);
	}
	hash %= prime;

	return (u4)hash;
}

//TODO 
// 0, not equal
// 1, equal
int compare(u1* pchar1, u4 len1, u1* pchar2, u4 len2)
{
	int ret = 0;
	u4 i = 0;
	if (len1 == len2)
	{
		for(i = 0; i < len1; i++)
		{
			if (*(pchar1 + i) != *(pchar2 + i))
			{
				break;
			}
		}
		if (i == len1)
		{
			ret = 1;
		}
	}

	return ret;
}

u4 fread_u4(FILE *fp)
{
	u1 byte1, byte2, byte3, byte4;
	byte1 = fgetc(fp);
	byte2 = fgetc(fp);
	byte3 = fgetc(fp);
	byte4 = fgetc(fp);

	return ((((u4)byte1) << 24) & 0xFF000000) 
		| ((((u4)byte2) << 16) & 0x00FF0000) 
		| ((((u4)byte3) << 8) & 0x0000FF00) 
		| ((u4)byte4 & 0x000000FF);

}

u2 fread_u2(FILE *fp)
{
	u1 byte1, byte2;
	byte1 = fgetc(fp);
	byte2 = fgetc(fp);
	if (byte1 == EOF)
	{
		printf("the end of file\n");
	}

	return ((((u2)byte1) << 8) & 0xFF00)
		| (((u2)byte2) & 0x00FF);

}

u1 fread_u1(FILE *fp)
{
	//EOF end of file
	int test = fgetc(fp);
	//u1 byte1 = fgetc(fp);
	u1 byte1 = test;
	/*
	if (byte1 == EOF)
	{
		printf("the end of file\n");
	}
	*/
	if (feof(fp))
	{
		printf("the end of file\n");
	}
	return  byte1;
}

int fread_int(FILE *fp)
{
	u1 byte1, byte2, byte3, byte4;
	byte1 = fgetc(fp);
	byte2 = fgetc(fp);
	byte3 = fgetc(fp);
	byte4 = fgetc(fp);

	return ((((int)byte4) << 24) & 0xFF000000) 
		| ((((int)byte3) << 16) & 0x00FF0000) 
		| ((((int)byte2) << 8) & 0x0000FF00) 
		| ((int)byte1 & 0x000000FF);

}

short fread_short(FILE *fp)
{
	u1 byte1, byte2;
	byte1 = fgetc(fp);
	byte2 = fgetc(fp);

	return ((((short)byte2) << 8) & 0xFF00)
		| (((short)byte1) & 0x00FF);

}

u1 mask(u2 value, u2 mask)
{
	return ((value & mask) == mask) ? 1 : 0;
}