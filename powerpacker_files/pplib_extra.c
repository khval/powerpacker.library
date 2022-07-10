
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/emulation.h>

#include <libraries/powerpacker.h>
#include <libraries/ppbase.h>

#ifndef PPLIB_H
#include <pplib.h>
#endif

const char PX20[] = "PX20";
const char PP20[] = "PP20";
const char PPMM[] = "PPMM";

void __get_eff_param(ULONG efficiency, unsigned char *eff_param1, unsigned char *eff_param2, unsigned char *eff_param3 );
CrunchInfo* ppAllocCrunchInfo( ULONG efficiency, ULONG speedup, BOOL (*func)(ULONG,ULONG,APTR), APTR userdata);
void ppFreeCrunchInfo( CrunchInfo* info );
static unsigned char* updateSpeedupLarge(unsigned char* curr, unsigned char* next, int count, CrunchInfo* info);
static void prepareDict(int repeats, CrunchInfo* info);

const char *ppErrorMessage (ULONG errorcode)
{
	const char *error_str = "N/A";

	switch (errorcode)
	{
		case PP_CRUNCHABORTED:
			error_str = "PP_CRUNCHABORTED";
			break;

		case PP_OPENERR:
			error_str = "PP_OPENERR"; 
			break;

		case PP_READERR:
			error_str = "PP_READERR"; 
			break;

		case PP_NOMEMORY:
			error_str = "PP_NOMEMORY"; 
			break;

		case PP_CRYPTED:
			error_str = "PP_CRYPTED"; 
			break;

		case PP_PASSERR:
			error_str = "PP_PASSERR"; 
			break;

		case PP_UNKNOWNPP:
			error_str = "PP_UNKNOWNPP"; 
			break;

		case PP_EMPTYFILE:
			error_str = "PP_EMPTYFILE"; 
			break;
	}

	printf("DB(%s)\n",error_str);

	return error_str;
}

BOOL ppEnterPassword( struct Screen * screen, UBYTE * buffer)
{
#warning "ppEnterPassword: is not implemented"

	return FALSE;
}

BOOL ppGetPassword( struct Screen * screen, ULONG * buffer, ULONG maxchars, ULONG checksum)
{
#warning "ppGetPassword: is not implemented"
	return FALSE;
}

CrunchInfo* ppAllocCrunchInfo( ULONG efficiency, ULONG speedup, BOOL (*func)(ULONG,ULONG,APTR), APTR userdata)
{
	int old_version = 0;
	unsigned char eff_param1,eff_param2,eff_param3;

	CrunchInfo* info = (CrunchInfo*)malloc(sizeof(CrunchInfo));

	if (info == NULL) {
		return NULL;
	}

	__get_eff_param( efficiency, &eff_param1, &eff_param2, &eff_param3 );

	info -> progress_fn = func;

	info->w00[0] = info->b2C[0] = 9;
	info->w00[1] = info->b2C[1] = eff_param3;
	info->w00[2] = info->b2C[2] = eff_param1;
	info->w00[3] = info->b2C[3] = eff_param2;

	info->w08[0] = (1 << 9);
	info->w08[1] = (1 << eff_param3);
	info->w08[2] = (1 << eff_param1);
	info->w08[3] = (1 << eff_param2);

	info->w10[0] = 0;
	info->w10[1] = 1;
	info->w10[2] = 2;
	info->w10[3] = 3;

	info->addrs_count = 0x10000;

	int multiply = old_version ? 2 : 1;

	info->wnd_max = (unsigned int)(1 << eff_param2) * (int)sizeof(unsigned short) * multiply;

	info->wnd1 = (unsigned short*)malloc(info->wnd_max * sizeof(unsigned short) * 2 * multiply);
	info->wnd2 = NULL;

	info->addrs = NULL;
	if (info->wnd1)
	{
		info->wnd2 = &info->wnd1[info->wnd_max];

		memset(info->wnd1, 0, info->wnd_max * sizeof(unsigned short) * 2 * multiply);
		info->addrs = (unsigned char**)malloc(info->addrs_count * sizeof(unsigned char*));

		if (info->addrs)
		{
			memset(info->addrs, 0, info->addrs_count * sizeof(unsigned char*));
			return info;
		}
	}

	ppFreeCrunchInfo(info);
	return NULL;
}

static void writeBits(int count, unsigned int value, write_res_t* dst, CrunchInfo* info)
{
	int i;

	for (i = 0; i < count; ++i)
	{
		int bit = value & 1;
		value >>= 1;

		unsigned int c = dst->token & 0x80000000;
		dst->token = (dst->token << 1) | bit;

		if (c != 0)
		{
			//printf("%08X\n", dst->token);
			dst->ptr[0] = dst->token;
			dst->ptr++;
			dst->token = 1;

			if (dst->ptr >= dst->ptr_max)
			{
				memcpy(info->dst, info->tmp, 0x200);
				info->dst = &info->dst[0x200];
				dst->ptr = info->tmp;
			}
		}
	}
}

static void writeMoreBits(int count, write_res_t* dst, CrunchInfo* info)
{
	if (count < 4)
	{
		writeBits(2, count - 1, dst, info);
	}
	else
	{
		int i;
		writeBits(2, (count - 4) % 3, dst, info);

		for ( i = 0; i < (count - 4) / 3 + 1; ++i)
		{
			writeBits(2, 3, dst, info);
		}
	}

	writeBits(1, 0, dst, info);
}


static int ppCrunchBuffer_sub(CrunchInfo* info)
{
	int i;

	for (i = 0; i < info->wnd_max; ++i)
	{
		info->wnd1[i] = 0;
		info->wnd2[i] = 0;
	}

	for (i = 0; i < (USHRT_MAX + 1); ++i) {
		info->addrs[i] = 0;
	}

	info->dst = info->start;

	info->wnd_off = 0;
	info->wnd_left = info->wnd_max;
	unsigned int max_size = info->wnd_left;

	if (info->wnd_left >= info->fsize) {
		max_size = info->fsize;
	}

	updateSpeedupLarge(&info->start[-info->wnd_max], info->start, max_size, info);

	info->print_pos = info->start;
	unsigned char* src_curr = info->start;

	write_res_t res;
	res.ptr = info->tmp;
	res.ptr_max = &info->tmp[0x80];
	res.token = 1;

	int bits = 0;

	while (src_curr < info->src_end) { // check_end
		int progress = (int)(src_curr - info->print_pos);

		if (progress >= 0x200) {
			info->print_pos += progress;

			if (info -> progress_fn != NULL) 
			{
//				info -> progress_fn((unsigned int)(info->print_pos - info->start), (unsigned int)(info->dst - info->start + (res.ptr - info->tmp) * sizeof(unsigned int)), info->fsize);
			}
		}

		unsigned char* src_max = &src_curr[0x7FFF];

		if (src_max >= info->src_end) {
			src_max = info->src_end;
		}

		int repeats = 1;
		unsigned char* next_src = &src_curr[1];
		unsigned char* cmp_src = NULL;
		unsigned short wnd_off_ = info->wnd_off;
		int bits_ = bits;
		unsigned int token_ = res.token;

		while (1) { // main_loop
			next_src = &next_src[repeats - 1];
			cmp_src = &cmp_src[repeats - 1];

			int skip = 0;

			while (info->wnd1[info->wnd_off]) {
				int off = info->wnd1[info->wnd_off];

				next_src = &next_src[off];
				info->wnd_off = (info->wnd_off + off);

				if (next_src < src_max && *next_src == src_curr[repeats] && next_src >= cmp_src) {
					next_src = &next_src[1 - repeats];

					cmp_src = &src_curr[2];
					unsigned char* cmp_from = &next_src[1];

					while (cmp_src < src_max && *cmp_src++ == *cmp_from++);
					cmp_from--;

					if (src_max < cmp_from) {
						cmp_src = cmp_src - cmp_from + src_max;
						cmp_from = src_max;
					}

					int curr_repeats = (int)(cmp_src - src_curr - 1);

					if (repeats < curr_repeats) {
						int shift = (int)(cmp_from - src_curr - curr_repeats);
						unsigned short curr_bits = 3;

						if (curr_repeats < 5) {
							curr_bits = curr_repeats - 2;
						}

						if (info->w08[curr_bits] >= shift) {
							repeats = curr_repeats;
							res.token = (res.token & 0xFFFF0000) | (shift & 0xFFFF);
							bits = curr_bits;
						}
					}

					skip = 1;
					break; // main_loop
				}
			}

			if (skip) {
				continue; // main_loop
			}

			// set_token
			info->value = res.token & 0xFFFF;
			info->bits = bits;
			info->wnd_off = wnd_off_;
			bits = bits_;
			res.token = token_;

			if (repeats == 1) {
				writeBits(8, src_curr[0], &res, info);
				bits++;

				prepareDict(1, info);
				src_curr = updateSpeedupLarge(src_curr, &src_curr[1], 1, info);
			}
			else {
				if (repeats < info->wnd_max) {
					prepareDict(repeats, info);
					src_curr = updateSpeedupLarge(src_curr, &src_curr[repeats], repeats, info);
				}
				else
				{
					int i;
					src_curr = &src_curr[repeats];

					info->wnd_off = 0;
					info->wnd_left = info->wnd_max;

					for (i = 0; i < info->wnd_max; ++i) {
						info->wnd1[info->wnd_off] = 0;
						info->wnd2[info->wnd_off++] = 0;
					}

					info->wnd_off = 0;
					info->wnd_left = info->wnd_max;

					src_curr = updateSpeedupLarge(&src_curr[-info->wnd_left], src_curr, info->wnd_left, info);
				}

				if (bits == 0)
				{
					writeBits(1, 1, &res, info);
				}
				else
				{
					writeMoreBits(bits, &res, info);
					bits = 0;
				}

				if (repeats >= 5)
				{
					if (repeats < 12) {
						writeBits(3, repeats - 5, &res, info);
					}
					else
					{
						int i;
						writeBits(3, (repeats - 12) % 7, &res, info);

						for ( i = 0; i < ((repeats - 12) / 7) + 1; ++i)
						{
							writeBits(3, 7, &res, info);
						}
					}
				}

				unsigned short bits_count = info->bits;
				unsigned short value = info->value - 1;
				int count = 1;

				if (repeats >= 5) {
					if (value >= 0x80) {
						writeBits(info->w00[bits_count], value, &res, info);
						value = 1;
					}
					else {
						writeBits(7, value, &res, info);
						value = 0;
					}
				}
				else {
					count = info->w00[bits_count];
				}

				writeBits(count, value, &res, info);
				writeBits(2, info->w10[bits_count], &res, info);
			}

			break; // check_end
		}
	}

	writeMoreBits(bits, &res, info);

	bits = 0;

	if (res.token != 1)
	{
		int bit = 0;

		while (!bit)
		{
			bits++;
			bit = res.token & 0x80000000;
			res.token <<= 1;
		}

		// here the original cruncher uses a pointer to the source data end
		// I decided to use hardcoded dword

		unsigned int address = 0x00000000;
		unsigned int last_token = 0;
		int i;

		for (i = 0; i < bits; ++i)
		{
			bit = address & 1;
			address >>= 1;
			last_token = (last_token << 1) | bit;
		}

		last_token |= res.token;
		res.ptr[0] = last_token;
		res.ptr++;
	}

	int last_size = (int)(res.ptr - info->tmp);

	memcpy(info->dst, info->tmp, last_size * sizeof(unsigned int));
	((unsigned int*)info->dst)[last_size] = (info->fsize << 8) | (bits & 0xFF);

	return (int)(info->dst - info->start + last_size * sizeof(unsigned int) + sizeof(unsigned int));
}

static unsigned char* updateSpeedupLarge(unsigned char* curr, unsigned char* next, int count, CrunchInfo* info)
{
	unsigned char* src = &curr[info->wnd_max];
	int i;

	for (i = 0; i < count; ++i)
	{
		if (&src[i] >= (info->src_end - 1)) continue;

		unsigned short val = (src[i + 0] << 8) | src[i + 1];
		unsigned char* back = info->addrs[val];

		if (back != NULL)
		{
			int new_val = (int)(&src[i] - back);
			int diff = (int)(back - next);

			if (diff >= 0 && new_val < info->wnd_max / sizeof(unsigned short))
			{
				if (diff >= info->wnd_left) diff -= info->wnd_max;

				info->wnd1[info->wnd_off + diff] = new_val;
				info->wnd2[info->wnd_off + diff] = new_val;
			}
		}

		info->addrs[val] = &src[i];
	}

	return &src[count] - info->wnd_max;
}


#define sfree(fn,ptr) if(ptr) fn(ptr); ptr = NULL

void ppFreeCrunchInfo( CrunchInfo* info )
{
	if (info)
	{
		sfree(free,info -> wnd1);
		sfree(free,info -> addrs);
		free(info);
	}
}

ULONG ppCrunchBuffer( CrunchInfo* info,unsigned char* buf, unsigned int len) 
{
	info->start = buf;
	info->fsize = len;
	info->src_end = &buf[len];

	return ppCrunchBuffer_sub( info );
}

ULONG ppCrunchBufferDest( UBYTE * crunchinfo, UBYTE * buffer, UBYTE * dest, ULONG len)
{
	printf("%s:%d (error: missing / not supported)\n",__FUNCTION__,__LINE__);
	return 0;
}

BOOL ppWriteDataHeader(  BPTR lock, ULONG efficiency, BOOL crypt,  ULONG checksum)
{
	int success = FALSE;
	unsigned char eff_param1,eff_param2,eff_param3;
	char b2C[4];

	if (crypt) 
	{
		if (FWrite(lock, PX20, 1, sizeof(PX20) - 1) != sizeof(PX20) - 1 ) return FALSE;
		if (write_word( lock, checksum) != sizeof(checksum)) return FALSE;
	}
	else
	{
		if (FWrite(lock, PP20, 1, sizeof(PP20) - 1) != sizeof(PP20) - 1) return FALSE;
	}

	__get_eff_param( efficiency, &eff_param1, &eff_param2, &eff_param3 );

	b2C[0] = 9;
	b2C[1] = eff_param3;
	b2C[2] = eff_param1;
	b2C[3] = eff_param2;

	if ( FWrite( lock, b2C, 4, 1) != 1) return FALSE;

	return TRUE;
}

// ---------- support functions etc....

void __get_eff_param(ULONG efficiency, unsigned char *eff_param1, unsigned char *eff_param2, unsigned char *eff_param3 )
{
	*eff_param3 = 10;
	*eff_param2 = 11;
	*eff_param1 = 11;

	switch (efficiency) {
	case 1:
		*eff_param2 = 9;
		*eff_param1 = 9;
		*eff_param3 = 9;
		break;
	case 2:
		*eff_param2 = 10;
		*eff_param1 = 10;
	case 3:
		break;
	case 4:
		*eff_param2 = 12;
		*eff_param1 = 12;
		break;
	case 5:
		*eff_param1 = 12;
		*eff_param2 = 13;
		break;
	}
}

static void prepareDict(int repeats, CrunchInfo* info)
{
	int i;
	for (i = 0; i < repeats; ++i)
	{
		info->wnd1[info->wnd_off] = 0;
		info->wnd2[info->wnd_off++] = 0;

		info->wnd_left--;
		if (info->wnd_left == 0)
		{
			info->wnd_left = info->wnd_max;
			info->wnd_off = 0;
		}
	}
}

