
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define __USE_INLINE__

#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/emulation.h>

#include <libraries/powerpacker.h>

#include "common.h"

const char PX20[] = "PX20";
const char PP20[] = "PP20";
const char PPMM[] = "PPMM";

static inline unsigned int rol(unsigned int n, int c)
{
	const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);  // assumes width is a power of 2.
	c &= mask;
	return (n << c) | (n >> ((-c) & mask));
}

static inline unsigned short ror(unsigned short n, int c)
{
	const unsigned int mask = (CHAR_BIT * sizeof(n) - 1);
	c &= mask;
	return (n >> c) | (n << ((-c) & mask));
}


unsigned int ppCalcPasskey(const char* passwd) {
	unsigned int result = 0;

	while (*passwd) {
		result = rol(result, 1);
		result += passwd[0];

#ifdef __little_endian__
		result = ((result & 0xFFFF0000) >> 16) | ((result & 0x0000FFFF) << 16);
#endif
		
		passwd++;
	}

	return result;
}
		
unsigned short ppCalcChecksum(const char* passwd) {
	unsigned short result = 0;

	while (*passwd)
		{
		result = ror(result, passwd[0]);
		result += passwd[0];
		passwd++;
	}

	return result;
}

static void encrypt(unsigned int* tmp, int size, unsigned int passwd)
{
		int i;
		for (i = 0; i < size; ++i)   *tmp++ ^= passwd;
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

typedef void (*progress_cb)(unsigned int src_off, unsigned int dst_off, unsigned int fsize);

static int ppCrunchBuffer_sub(progress_cb cb, CrunchInfo* info)
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

			if (cb != NULL) {
				cb((unsigned int)(info->print_pos - info->start), (unsigned int)(info->dst - info->start + (res.ptr - info->tmp) * sizeof(unsigned int)), info->fsize);
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

static void print_progress(unsigned int src_off, unsigned int dst_off, unsigned int fsize)
{
	if (src_off == 0) {
		return;
	}

	unsigned int crunched = (src_off * 100) / fsize;
	unsigned int gain = (dst_off * 100) / src_off;

	printf("\r%u%% crunched. (%u%% gain)   ", crunched, 100 - gain);
}

int ppCrunchBuffer( CrunchInfo* info,unsigned char* buf,	unsigned int len) 
{
	info->start = buf;
	info->fsize = len;
	info->src_end = &buf[len];

	return ppCrunchBuffer_sub(print_progress, info);
}

ULONG ppCrunchBufferDest( UBYTE * crunchinfo, UBYTE * buffer, UBYTE * dest, ULONG len)
{
	return 0;
}

void ppFreeCrunchInfo(CrunchInfo* info)
{
	if (info == NULL) return;
	if (info->addrs != NULL) free(info->addrs);
	free(info->wnd1);
	free(info);
}

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

CrunchInfo* ppAllocCrunchInfo( ULONG efficiency, ULONG speedup, BOOL (*func)(ULONG,ULONG,APTR), APTR userdata)
{
	int old_version = 0;
	unsigned char eff_param1,eff_param2,eff_param3;

	CrunchInfo* info = (CrunchInfo*)malloc(sizeof(CrunchInfo));

	if (info == NULL) {
		return NULL;
	}

	__get_eff_param( efficiency, &eff_param1, &eff_param2, &eff_param3 );

	info -> func = func;

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
	if (info->wnd1) {
		info->wnd2 = &info->wnd1[info->wnd_max];

		memset(info->wnd1, 0, info->wnd_max * sizeof(unsigned short) * 2 * multiply);

		info->addrs = (unsigned char**)malloc(info->addrs_count * sizeof(unsigned char*));

		if (info->addrs) {
			memset(info->addrs, 0, info->addrs_count * sizeof(unsigned char*));

			return info;
		}
	}

	ppFreeCrunchInfo(info);
	return NULL;
}


void ppDecrypt(unsigned char* buffer, int size, unsigned int key)
{
	int i;
	for ( i = 0; i < size; i += 4)
	{
		buffer[i + 0] ^= (key >> 24) & 0xFF;
		buffer[i + 1] ^= (key >> 16) & 0xFF;
		buffer[i + 2] ^= (key >> 8) & 0xFF;
		buffer[i + 3] ^= (key >> 0) & 0xFF;
	}
}

/* the decoder presented here is taken from pplib by Stuart Caie. The
 * following statement comes from the original source.
 *
 * pplib 1.0: a simple PowerPacker decompression and decryption library
 * placed in the Public Domain on 2003-09-18 by Stuart Caie.
 */

#define PP_READ_BITS(nbits, var) do {								\
		bit_cnt = (nbits); (var) = 0;								\
		while (bits_left < bit_cnt) {								\
				if (buf < src) return -1;						\
				bit_buffer |= *--buf << bits_left;				\
				bits_left += 8;										\
		}														\
		bits_left -= bit_cnt;										\
		while (bit_cnt--) {										\
				(var) = ((var) << 1) | (bit_buffer & 1);		\
				bit_buffer >>= 1;								\
		}														\
} while (0)

#define PP_BYTE_OUT(byte) do {										\
		if (out <= dest) return -1;								\
		*--out = (byte); written++;								\
} while (0)

int ppDecrunchBuffer(unsigned char* src, unsigned int src_len, unsigned char* dest, unsigned int dest_len) {
	unsigned char* buf, *out, *dest_end, *off_lens, bits_left = 0, bit_cnt;
	unsigned int bit_buffer = 0, x, todo, offbits, offset, written = 0;

	if (!src || !dest) return -1;

	/* set up input and output pointers */
	off_lens = src; src = &src[4];
	buf = &src[src_len];

	out = dest_end = &dest[dest_len];

	/* skip the first few bits */
	PP_READ_BITS(src[src_len + 3], x);

	/* while there are input bits left */
	while (written < dest_len) {
		PP_READ_BITS(1, x);
		if (x == 0) {
			/* bit==0: literal, then match. bit==1: just match */
			todo = 1; do { PP_READ_BITS(2, x); todo += x; } while (x == 3);
			while (todo--) { PP_READ_BITS(8, x); PP_BYTE_OUT(x); }

			/* should we end decoding on a literal, break out of the main loop */
			if (written == dest_len) break;
		}

		/* match: read 2 bits for initial offset bitlength / match length */
		PP_READ_BITS(2, x);
		offbits = off_lens[x];
		todo = x + 2;
		if (x == 3) {
			PP_READ_BITS(1, x);
			if (x == 0) offbits = 7;
			PP_READ_BITS(offbits, offset);
			do { PP_READ_BITS(3, x); todo += x; } while (x == 7);
		}
		else {
			PP_READ_BITS(offbits, offset);
		}
		if (&out[offset] >= dest_end) return -1; /* match_overflow */
		while (todo--) { x = out[offset]; PP_BYTE_OUT(x); }
	}

	/* all output bytes written without error */
	return 0;
}

BOOL ppWriteDataHeader(
	   BPTR lock,
	   ULONG efficiency,
	   BOOL crypt,
	   ULONG checksum)
{
	int success = false;
	unsigned char eff_param1,eff_param2,eff_param3;
	char b2C[4];

	if (crypt) 
	{
		if (FWrite(lock, PX20, 1, sizeof(PX20) - 1) != sizeof(PX20) - 1 ) return false;
		if (write_word( lock, checksum) != sizeof(checksum)) return false;
	}
	else
	{
		if (FWrite(lock, PP20, 1, sizeof(PP20) - 1) != sizeof(PP20) - 1) return false;
	}

	__get_eff_param( efficiency, &eff_param1, &eff_param2, &eff_param3 );

	b2C[0] = 9;
	b2C[1] = eff_param3;
	b2C[2] = eff_param1;
	b2C[3] = eff_param2;

	if ( FWrite( lock, b2C, 4, 1) != 1) return false;

	return true;
}

const char *error_messages[]=
	{"N/A"};

const char *ppErrorMessage (ULONG errorcode)
{
#warning "ppErrorMessage: its possible error_messages array is too small..."

	return error_messages[errorcode];
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

int ppLoadData(char * filename,  ULONG col,  ULONG memtype, UBYTE ** bufferptr, ULONG * lenptr, BOOL (*password_func)(UBYTE *, ULONG)) 
{
	char *passwd = NULL;
	*bufferptr = NULL;

	BPTR f = FOpen(filename, MODE_OLDFILE, 0);

	if (f == (BPTR) NULL) {
		return -1;
	}

	ChangeFilePosition(f, 0, OFFSET_END);
	int64 buf_len = (int64) GetFilePosition(f);
	ChangeFilePosition(f, 0, OFFSET_BEGINNING);

	if (buf_len == 0)
	{
		FClose(f);
		return -1;
	}

	char tag[4];

	if (FRead(f, tag, 1, sizeof(tag)) != sizeof(tag))
	{
		FClose(f);
		return -1;
	}
	
	unsigned int dest_len = 0;
	unsigned int read_len = 0;
	int offset = 4;
	unsigned short checksum = 0;

	if (!memcmp(PP20, tag, sizeof(tag))) {
		ChangeFilePosition(f, -4, OFFSET_END);

		dest_len = read_dword(f);

		read_len = buf_len - offset + sizeof(decrunch_t);
		dest_len = (dest_len >> 8);
	}
	else if (!memcmp(PX20, tag, sizeof(tag))) {
		checksum = read_word(f);
		offset += 2;

		ChangeFilePosition(f, -4, OFFSET_END);

		dest_len = read_dword(f);

		read_len = buf_len - offset + sizeof(decrunch_t);
		dest_len = (dest_len >> 8);
	}
	else {
		FClose(f);
		return -1;
	}

	unsigned char* buffer = (unsigned char*)malloc(read_len);
	memset(buffer, 0, read_len);

	if (buffer == NULL) {
		FClose(f);
		return -1;
	}

	ChangeFilePosition(f, offset, OFFSET_BEGINNING);

	if (FRead(f,&buffer[sizeof(decrunch_t)], 1, read_len - sizeof(decrunch_t)) != read_len - sizeof(decrunch_t))
	{
		FClose(f);
		free(buffer);
		return -1;
	}

	FClose(f);

	decrunch_t* info = (decrunch_t*)buffer;
	*bufferptr = info;
	info->src = &buffer[sizeof(decrunch_t)];

	if (offset == 6)
	{
		bool password_pass = false;

		if ((passwd == NULL) || (strlen(passwd) > 16))
		{
			FClose(f);
			free(buffer);
			return -1;
		}

		if (password_func)
		{
			if (IsNative(password_func))
			{
				password_pass = password_func(passwd,checksum);
			}
			else
			{
				password_pass = EmulateTags( password_func,
							ET_RegisterA0, passwd,
							ET_RegisterD0, checksum,
							TAG_END);
			}
		}
		else password_pass = (ppCalcChecksum(passwd) == checksum);

		if (password_pass == false)
		{
			FClose(f);
			free(buffer);
			return -1;
		}

		unsigned int key = ppCalcPasskey(passwd);

		ppDecrypt(&info->src[4], read_len - sizeof(decrunch_t) - 8, key);
	}

	memcpy(&info->tag, PPMM, sizeof(PPMM));
	info->src_len = read_len - sizeof(decrunch_t);
	info->dst_len = dest_len;

	info->dst = (unsigned char*) AllocMem(dest_len, memtype);

	if (info->dst == NULL) return -1;

	return ppDecrunchBuffer(info->src, read_len - sizeof(decrunch_t) - 8, info->dst, dest_len);
}

