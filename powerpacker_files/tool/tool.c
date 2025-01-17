#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/powerpacker.h>

extern size_t write_word(BPTR f, unsigned short v);
extern size_t write_dwords(BPTR f, unsigned int* buf, int count);
extern unsigned short read_word(BPTR f);
extern unsigned int read_dword(BPTR f);

static void print_progress(unsigned int src_off, unsigned int dst_off, unsigned int fsize);

static long get_file_size(const char* path)
{
	BPTR f = FOpen(path, MODE_OLDFILE,0);
	if (f == 0) return 0;

	ChangeFilePosition(f, 0, OFFSET_END);
	long size = GetFilePosition(f);
	FClose(f);

	return size;
}

static void encrypt(unsigned int* tmp, int size, unsigned int passwd)
{
	int i;
	for (i = 0; i < size; ++i)   *tmp++ ^= passwd;
}

static int compress(const char* src_path, const char* dst_path, unsigned int fsize, CrunchInfo* info, const char* passwd, int eff)
{
	if (fsize == 0) return -1;

	BPTR src_h = FOpen(src_path, MODE_OLDFILE, 0);

	if (src_h == 0)
	{
		printf("Can't open '%s' file!\n", src_path);
		return -1;
	}

	info->start = (unsigned char*) malloc(fsize);

	if (info->start == NULL)
	{
		printf("No memory to crunch '%s'!\n", src_path);
		FClose(src_h);
		return -1;
	}

	if (FRead(src_h,info->start, 1, fsize) < fsize)
	{
		printf("Error reading '%s'!\n", src_path);
		FClose(src_h);
		free(info->start);
		return -1;
	}

	FClose(src_h);

	unsigned int passkey = 0;
	unsigned short checksum = 0;

	if (passwd)
	{
		passkey = ppCalcPasskey(passwd);
		checksum = ppCalcChecksum(passwd);
	}

	printf("Crunching '%s'...\n", src_path);

	int crunched_len = ppCrunchBuffer(info, info->start, fsize);

	if (crunched_len == -1)
	{
		free(info->start);
		remove(dst_path);
		printf("Buffer overflow!\n");
		return -1;
	}

	printf("\n");

	if (passwd)
	{
		printf("Encrypting...\n");
		encrypt((unsigned int*)info->start, (crunched_len / 4) - 1, passkey);
	}

	printf("\n");
	printf("  Normal length   : %d bytes.\n", fsize);
	printf("  Crunched length : %d bytes. (Gained %d%%)\n", crunched_len, (100 - (crunched_len * 100) / fsize));

	BPTR dst_h = FOpen(dst_path, MODE_NEWFILE, 0 );

	if (dst_h == NULL)
	{
		printf("Can't open '%s' file!\n", dst_path);
		free(info->start);
		return -1;
	}

	if ( ! ppWriteDataHeader(dst_h, eff, passwd != NULL, checksum ) )
	{
		printf("*1* Error writing to '%s'!\n", dst_path);
		if (dst_h) FClose(dst_h);
		free(info->start);
		return -1;
	}

	if ( write_dwords(dst_h, (unsigned int*)info->start, crunched_len) != crunched_len)
	{
		printf("*2* Error writing to '%s'!\n", dst_path);
	}

	FClose(dst_h);
	free(info->start);
	return 0;
}

static void print_help()
{
	printf(
		"  Crunch: powerpack <source> <destination> <-c> [-e=EFFICIENCY] [-p=PASSWORD] [-o] [-h]\n"
		"Decrunch: powerpack <source> <destination> <-d> [-p=PASSWORD] [-h]\n"
		"With:\n"
		"		  -c: Crunch (compress)\n"
		"		  -d: Decrunch (decompress)\n"
		"  EFFICIENCY: 1 = Fast, 2 = Mediocre, 3 = Good (def), 4 = Very Good, 5 = Best\n"
		"	PASSWORD: Encrypt/decrypt file. Max 16 characters\n"
		"		  -o: Use it to compress with the old PP alorithm.\n"
		"			  The difference in the size of a window:\n"
		"			  - Old version: 0x4000\n"
		"			  - New version: 0x8000\n"
		"		  -h: Show this help\n\n");
}


APTR ppAlloc_fn( ULONG size )
{
	return AllocVecTags( size, AVT_Type, MEMF_PRIVATE, TAG_END);
}

void ppFree_fn( APTR address )
{
	FreeVec( address );
}

int _main(int argc, char* argv[])
{
	printf("POWER-PACKER 36.10 (28.9.93) Data Cruncher.\n");
	printf("  Written by Nico Franuois (POWER PEAK)\n");
	printf("  POWERPACK created by Dr. MefistO in 2020, based on pplib 1.0 (2003)\n");
	printf("  Version: v1.0\n\n");

	if (argc < 2)
	{
		print_help();
		return -1;
	}

	char passwd[17];
	memset(passwd, 0, sizeof(passwd));

	int old_version = 0;
	int eff = 3;
	int i = 3;
	int mode = -1;

	while (i < argc)
	{
		if (((argv[i][0] == '-') || (argv[i][0] == '/')))
		{
			switch (argv[i][1])
			{
				case 'c': 
					mode = 0;
					break;

				case 'd':
					mode = 1;
					break;

				case 'h':
					print_help();
					return 0;

				case 'e': 
					if (sscanf(&argv[i][3], "%d", &eff) != 1) return -1;
					break;

				case 'p': 
					if (sscanf(&argv[i][3], "%16s", passwd) != 1) return -1;
					break;

				case 'o': 
					old_version = 1;
					break;
			}
		}
		i++;
	}

	if (mode == -1)
	{
		printf("Incorrect mode. Please, use '-c' to crunch or '-d' to decrunch\n\n");
		print_help();
		return -1;
	}

	int result = -1;

	if (mode == 0)
	{
		CrunchInfo* info = ppAllocCrunchInfo(eff, old_version, NULL, NULL);

		unsigned int fsize = get_file_size(argv[1]);

		result = compress(argv[1], argv[2], fsize, info, passwd[0] != 0 ? passwd : NULL, eff);
		ppFreeCrunchInfo(info);
		printf("\nDone.\n");
	}
	else
	{
		unsigned char *buf;
		ULONG buflen;

		result = ppLoadData2(argv[1], &buf, &buflen, ppAlloc_fn, ppFree_fn, NULL );

		if (buf == NULL)
		{
			printf("Cannot decrunch '%s'!\n", argv[1]);
			return 224;
		}

		BPTR dst_h = FOpen(argv[2], MODE_NEWFILE ,0);
		if (dst_h == 0)
		{
			printf("Cannot open '%s' for write!\n", argv[2]);
			ppFree_fn(buf);
			return 223;
		}
		else
		{
			if (FWrite(dst_h, buf, 1, buflen) != buflen)
			{
				printf("Cannot write to '%s'!\n", argv[2]);
				result = 223;
			}
		}

		if (buf) ppFree_fn(buf);
		FClose(dst_h);
	}

	return result;
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


extern BOOL open_libs();
extern void close_libs();

int main(int argc, char* argv[])
{
	int ret = 122;
	if (open_libs())
	{
		ret = _main( argc, argv );
	}
	close_libs();
	return ret;
}

