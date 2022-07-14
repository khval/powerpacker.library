
/*
* 
* This file comes from powerpack tool for windows.
* the file is distibuted as Public domain.
*
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define __USE_INLINE__

#include <proto/dos.h>

#include "common.h"

size_t write_word(BPTR f, unsigned short v)
{
	return FWrite(f, &v, 1, sizeof(v));
}

size_t write_dwords(BPTR f, unsigned int* buf, int count)
{
	int i;
	size_t result = 0;
	
	for (i = 0; i < count / 4; ++i)
	{
		result += write_word(f, (buf[i] >> 16) & 0xFFFF);
		result += write_word(f, (buf[i] >> 0) & 0xFFFF);
	}

	return result;
}

unsigned short read_word(BPTR f)
{
	unsigned short w;
	FRead(f,&w, 1, sizeof(w));
	return w;
}

unsigned int read_dword(BPTR f)
{
	unsigned int dw;
	FRead(f,&dw, 1, sizeof(dw));
	return dw;
}
