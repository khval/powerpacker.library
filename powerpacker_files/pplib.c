/* pplib 1.2: a simple PowerPacker decompression and decryption library
 * placed in the Public Domain on 25-Nov-2010 by Stuart Caie.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/emulation.h>

#include <libraries/ppbase.h>

#ifndef PPLIB_H
#include <pplib.h>
#endif

#ifdef USE_PPLOADDATA
/* also demonstrates how to use the other functions of pplib by yourself */

int64 file_len(BPTR fh)
{
	ChangeFilePosition(fh, 0, OFFSET_END);
	int64 len = (int64) GetFilePosition(fh);
	ChangeFilePosition(fh, 0, OFFSET_BEGINNING);

	return len;
}


BOOL get_password(BOOL (*password_func)(UBYTE *, ULONG), char *password, ULONG checksum )
{
	BOOL password_pass = FALSE;

	password[0]=0;

	if (password_func)
	{
		if (IsNative(password_func))
		{
			password_pass = password_func(password,checksum);
		}
		else
		{
			password_pass = EmulateTags( password_func,
						ET_RegisterA0, password,
						ET_RegisterD0, checksum,
						TAG_END);
		}
		return TRUE;
	}
	return FALSE;
}

ULONG ppLoadData(char * filename,  ULONG col,  ULONG memtype, UBYTE ** bufptr, ULONG * buflenptr, BOOL (*password_func)(UBYTE *, ULONG)) 
{
	unsigned char *src = NULL, *dest;
	unsigned int srclen, destlen;
	int err = PPERR_OK, eff;
	BPTR fh;
	*bufptr = NULL;

	// open file, find out srclen, allocate src and read file 

	if ((fh = FOpen( filename, MODE_OLDFILE, 0)))
	{
		if ((srclen = file_len(fh)))
		{
			if ((src = malloc(srclen)))
			{
				if (FRead(fh,src, 1, srclen) != srclen)
				{
					free(src); err = PPERR_READ;
				}
			}
			else err = PPERR_NOMEMORY;
		}
		else err = PPERR_SEEK;
		FClose(fh);
	}
	else err = PPERR_OPEN;

	if (err) return err;

	switch ((src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3]) 
	{
		case 0x50503230: eff = 4; break; /* PP20 */
		case 0x50504C53: eff = 8; break; /* PPLS */
		case 0x50583230: eff = 6;        /* PX20 */
				{
					char password[16];
					ULONG checksum = ((src[4]<<8)|src[5]);

					if (get_password( password_func, password, checksum ))
					{
						if (ppCalcChecksum(password) != checksum )
							err = PPERR_PASSWORD;
						else
							ppDecrypt(&src[10], srclen-14, ppCalcPasskey(password));
					}
					else err = PPERR_PASSWORD;
				}
				break;
		default:
			err = PPERR_DATAFORMAT;
	}

	if (err)
	{
		free(src);
		return err;
	}

	/* allocate memory for decrunch buffer, then decrunch */

	destlen = (src[srclen-4] << 16) | (src[srclen-3] << 8) | src[srclen-2];

	if ((dest = (unsigned char*) AllocMem(destlen, memtype)))
	{
		printf("ppDecrunchBuffer(%08x, %08x, %08x, %d, %d)\n",
				&src[eff], &src[eff+4], dest, srclen-(eff+8), destlen	);

		if (!ppDecrunchBuffer_m(&src[eff], &src[eff+4], dest, srclen-(eff+8), destlen))
			err = PPERR_DECRUNCH;
	}
  
	if (src) free(src);

	if (err)
	{
		printf("err: %d\n", err);

		if (dest) FreeMem(dest,destlen);
		*bufptr = NULL;
		*buflenptr = 0;
	}
	else
	{
		*bufptr = dest;
		*buflenptr = destlen;
	}
	return err;
}
#endif

#define PP_READ_BITS(nbits, var) do {                            \
    bit_cnt = (nbits); (var) = 0;				 \
    while (bits_left < bit_cnt) {				 \
	if (buf < src) return 0; /* out of source bits */	 \
	bit_buffer |= *--buf << bits_left;			 \
	bits_left += 8;						 \
    }								 \
    bits_left -= bit_cnt;					 \
    while (bit_cnt--) {						 \
	(var) = ((var) << 1) | (bit_buffer & 1);		 \
	bit_buffer >>= 1;					 \
    }								 \
} while (0)

#define PP_BYTE_OUT(byte) do {                                   \
	if (out <= dest) return 0; /* output overflow */	 \
	*--out = (byte); written++;				 \
} while (0)

static int ppDecrunchBuffer_main(const unsigned char *eff,
				 const unsigned char *src, unsigned char *dest,
				 unsigned int src_len, unsigned int dest_len,
				 const unsigned int litbit)
{
    const unsigned char *buf = &src[src_len];
    unsigned char *out = &dest[dest_len], *dest_end = out;
    unsigned int bit_buffer = 0, x, todo, offbits, offset=0, written = 0;
    unsigned char bits_left = 0, bit_cnt;

	if (src == NULL || dest == NULL)
	{
		printf("src or dest is null\n");
		 return 0;
	}

	/* skip the first few bits */
	PP_READ_BITS(src[src_len + 3], x);

	/* while we still have output to unpack */
	while (written < dest_len)
	{
		PP_READ_BITS(1, x);
		if (x == litbit)
		{
			todo = 1; do { PP_READ_BITS(2, x); todo += x; } while (x == 3);
			while (todo--) { PP_READ_BITS(8, x); printf("%c",x); PP_BYTE_OUT(x); }

			/* should we end decoding on a literal, break out */
			if (written == dest_len) break;
		}

	/* match */
	PP_READ_BITS(2, x);
	offbits = eff[x];
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


	printf("out[offset: %d]: %08x >= %08x\n",offset, &out[offset],dest_end);

	if (&out[offset] >= dest_end)
	{
		printf("match overflow\n");

		 return 0; /* match_overflow */
	}

	while (todo--) { x = out[offset]; printf("%c",x); PP_BYTE_OUT(x); }
    }

    /* all output bytes written without error */
    return 1;
}

int ppDecrunchBuffer(const unsigned char *eff, const unsigned char *src, unsigned char *dest, unsigned int src_len, unsigned int dest_len)
{
    return ppDecrunchBuffer_main(eff, src, dest, src_len, dest_len, 0);
}

int ppDecrunchBuffer_m(const unsigned char *eff, const unsigned char *src, unsigned char *dest, unsigned int src_len, unsigned int dest_len)
{
    return ppDecrunchBuffer_main(eff, src, dest, src_len, dest_len, 1);
}

unsigned int ppCalcChecksum(const unsigned char *password)
{
    unsigned int cksum = 0;
    unsigned char c, shift;

    /* for each byte in the password */
    while ((c = *password++)) {
	/* barrel-shift the 16 bit checksum right by [c] bits */
	shift = c & 0x0F;
	if (shift) cksum = (cksum >> shift) | (cksum << (16-shift));

	/* add c to the cksum, with 16 bit wrap */
	cksum = (cksum + c) & 0xFFFF;
    }

    return cksum;
}

unsigned int ppCalcPasskey(const unsigned char *password)
{
    unsigned int key = 0;
    unsigned char c;

    /* for each byte in the password */
    while ((c = *password++)) {
	/* rotate 32 bit key left by one bit */
	key = (key << 1) | (key >> (32-1));
	key &= 0xFFFFFFFF;

	/* add c to the key, with 32 bit wrap */
	key = (key + c) & 0xFFFFFFFF;
    
	/* swap lower and upper 16 bits */
	key = (key << 16) | (key >> 16);
	key &= 0xFFFFFFFF;
    }
  
    return key;
}

void ppDecrypt(unsigned char *data, unsigned int len, unsigned int key)
{
    unsigned char k0 = (key >> 24) & 0xFF;
    unsigned char k1 = (key >> 16) & 0xFF;
    unsigned char k2 = (key >>  8) & 0xFF;
    unsigned char k3 = (key      ) & 0xFF;

    len = ((len + 3) >> 2) - 1;

    /* to replicate unofficial powerpacker.library v37.3 bug, uncomment line */
    /*len &= 0xFFFF;*/

    /* XOR data with key */
    do {
	*data++ ^= k0;
	*data++ ^= k1;
	*data++ ^= k2;
	*data++ ^= k3;
    } while (len--);
}




