
#define __USE_INLINE__

#include <proto/dos.h>
#include <proto/exec,h>
#include <proto/powerpacker.h>

extern size_t write_word(FILE* f, unsigned short v);
extern size_t write_dwords(FILE* f, unsigned int* buf, int count);
extern unsigned short read_word(FILE* f);
extern unsigned int read_dword(FILE* f);

static long get_file_size(const char* path) {
    FILE* f = fopen(path, "rb");

    if (f == NULL) {
        return 0;
    }

    fseek(f, 0, SEEK_END);

    long size = ftell(f);

    fclose(f);

    return size;
}

static int compress(const char* src_path, const char* dst_path, unsigned int fsize, CrunchInfo* info, const char* passwd, int eff) {
    if (fsize == 0) {
        return -1;
    }

    FILE* src_h = fopen(src_path, "rb");

    if (src_h == NULL) {
        printf("Can't open '%s' file!\n", src_path);
        return -1;
    }

    info->start = (unsigned char*)malloc(fsize);

    if (info->start == NULL) {
        printf("No memory to crunch '%s'!\n", src_path);
        fclose(src_h);
        return -1;
    }

    if (fread(info->start, 1, fsize, src_h) < fsize) {
        printf("Error reading '%s'!\n", src_path);
        fclose(src_h);
        free(info->start);
        return -1;
    }

    fclose(src_h);

    unsigned int passkey = 0;
    unsigned short checksum = 0;

    if (passwd) {
        passkey = ppCalcPasskey(passwd);
        checksum = ppCalcChecksum(passwd);
    }

    printf("Crunching '%s'...\n", src_path);

    int crunched_len = ppCrunchBuffer(fsize, info->start, info);

    if (crunched_len == -1) {
        free(info->start);
        remove(dst_path);
        printf("Buffer overflow!\n");
        return -1;
    }

    printf("\n");

    if (passwd) {
        printf("Encrypting...\n");
        encrypt((unsigned int*)info->start, (crunched_len / 4) - 1, passkey);
    }

    printf("\n");
    printf("  Normal length   : %d bytes.\n", fsize);
    printf("  Crunched length : %d bytes. (Gained %d%%)\n", crunched_len, (100 - (crunched_len * 100) / fsize));

    BPTR dst_h = FOpen(dst_path, o_newfile );

    if (dst_h == NULL) {
        printf("Can't open '%s' file!\n", dst_path);
        free(info->start);
        return -1;
    }

    int error = ppWriteDataHeader(dst_h, eff, passwd != NULL, checksum, info->b2C );

    if (error) {
        printf("Error writing to '%s'!\n", dst_path);
        free(dst_h);
        free(info->start);
        return -1;
    }

    if (!error && write_dwords(dst_h, (unsigned int*)info->start, crunched_len) != crunched_len) {
        error = 1;
    }

    FClose(dst_h);
    free(info->start);

    if (error) {
        printf("Error writing to '%s'!\n", dst_path);
        return -1;
    }

    return 0;
}

int ppLoadData(const char* filename, decrunch_t** bufferptr, const char* passwd) {
    *bufferptr = NULL;

    FILE* f = fopen(filename, "rb");

    if (f == NULL) {
        return -1;
    }

    fseek(f, 0, SEEK_END);
    unsigned int buf_len = (unsigned int)ftell(f);
    fseek(f, 0, SEEK_SET);

    if (buf_len == 0) {
        fclose(f);
        return -1;
    }

    char tag[4];

    if (fread(tag, 1, sizeof(tag), f) != sizeof(tag)) {
        fclose(f);
        return -1;
    }

    unsigned int dest_len = 0;
    unsigned int read_len = 0;
    int offset = 4;
    unsigned short checksum = 0;

    if (!memcmp(PP20, tag, sizeof(tag))) {
        fseek(f, -4, SEEK_END);

        dest_len = read_dword(f);

        read_len = buf_len - offset + sizeof(decrunch_t);
        dest_len = (dest_len >> 8);
    }
    else if (!memcmp(PX20, tag, sizeof(tag))) {
        checksum = read_word(f);
        offset += 2;

        fseek(f, -4, SEEK_END);

        dest_len = read_dword(f);

        read_len = buf_len - offset + sizeof(decrunch_t);
        dest_len = (dest_len >> 8);
    }
    else {
        fclose(f);
        return -1;
    }

    unsigned char* buffer = (unsigned char*)malloc(read_len);
    memset(buffer, 0, read_len);

    if (buffer == NULL) {
        fclose(f);
        return -1;
    }

    fseek(f, offset, SEEK_SET);

    if (fread(&buffer[sizeof(decrunch_t)], 1, read_len - sizeof(decrunch_t), f) != read_len - sizeof(decrunch_t)) {
        fclose(f);
        free(buffer);
        return -1;
    }

    fclose(f);

    decrunch_t* info = (decrunch_t*)buffer;
    *bufferptr = info;
    info->src = &buffer[sizeof(decrunch_t)];

    if (offset == 6) {
        if ((passwd == NULL) || (strlen(passwd) > 16)) {
            fclose(f);
            free(buffer);
            return -1;
        }

        if (ppCalcChecksum(passwd) != checksum) {
            fclose(f);
            free(buffer);
            return -1;
        }

        unsigned int key = ppCalcPasskey(passwd);
        ppDecrypt(&info->src[4], read_len - sizeof(decrunch_t) - 8, key);
    }

    memcpy(&info->tag, PPMM, sizeof(PPMM));
    info->src_len = read_len - sizeof(decrunch_t);
    info->dst_len = dest_len;

    info->dst = (unsigned char*)malloc(dest_len);

    if (info->dst == NULL) {
        return -1;
    }

    return ppDecrunchBuffer(info->src, read_len - sizeof(decrunch_t) - 8, info->dst, dest_len);
}


static void print_help() {
    printf(
        "  Crunch: powerpack <source> <destination> <-c> [-e=EFFICIENCY] [-p=PASSWORD] [-o] [-h]\n"
        "Decrunch: powerpack <source> <destination> <-d> [-p=PASSWORD] [-h]\n"
        "With:\n"
        "          -c: Crunch (compress)\n"
        "          -d: Decrunch (decompress)\n"
        "  EFFICIENCY: 1 = Fast, 2 = Mediocre, 3 = Good (def), 4 = Very Good, 5 = Best\n"
        "    PASSWORD: Encrypt/decrypt file. Max 16 characters\n"
        "          -o: Use it to compress with the old PP alorithm.\n"
        "              The difference in the size of a window:\n"
        "              - Old version: 0x4000\n"
        "              - New version: 0x8000\n"
        "          -h: Show this help\n\n"
    );
}


nt main(int argc, char* argv[]) {
    printf("POWER-PACKER 36.10 (28.9.93) Data Cruncher.\n");
    printf(u8"  Written by Nico François (POWER PEAK)\n");
    printf("  Decompiled by Dr. MefistO in 2020\n");
    printf("  Version: v1.0\n\n");

    if (argc < 2) {
        print_help();
        return -1;
    }

    char passwd[17];
    memset(passwd, 0, sizeof(passwd));

    int old_version = 0;
    int eff = 3;
    int i = 3;
    int mode = -1;

    while (i < argc) {
        if (((argv[i][0] == '-') || (argv[i][0] == '/'))) {
            switch (argv[i][1]) {
            case 'c': {
                mode = 0;
            } break;
            case 'd': {
                mode = 1;
            } break;
            case 'h':
                print_help();
                return 0;
            case 'e': {
                if (sscanf(&argv[i][3], "%d", &eff) != 1) {
                    return -1;
                }
            } break;
            case 'p': {
                if (sscanf(&argv[i][3], "%16s", passwd) != 1) {
                    return -1;
                }
            } break;
            case 'o': {
                old_version = 1;
                break;
            }
            }
        }

        i++;
    }

    if (mode == -1) {
        printf("Incorrect mode. Please, use '-c' to crunch or '-d' to decrunch\n\n");
        print_help();
        return -1;
    }

    int result = -1;

    if (mode == 0) {
        CrunchInfo* info = ppAllocCrunchInfo(eff, old_version);

        unsigned int fsize = get_file_size(argv[1]);

        result = compress(argv[1], argv[2], fsize, info, passwd[0] != 0 ? passwd : NULL, eff);
        ppFreeCrunchInfo(info);
        printf("\nDone.\n");
    }
    else {
        decrunch_t* info;

        result = ppLoadData(argv[1], &info, passwd[0] != 0 ? passwd : NULL);

        if (info == NULL) {
            printf("Cannot decrunch '%s'!\n", argv[1]);
            return -1;
        }

        FILE* dst_h = fopen(argv[2], "wb");

        if (dst_h == NULL) {
            printf("Cannot open '%s' for write!\n", argv[2]);
            free(info);
            return -1;
        }

        if (fwrite(info->dst, 1, info->dst_len, dst_h) != info->dst_len) {
            printf("Cannot write to '%s'!\n", argv[2]);
            result = -1;
        }

        printf("Successfully decrunched '%s' into '%s'\n", argv[1], argv[2]);
        printf("Result: %d -> %d bytes\n", info->src_len, info->dst_len);

        free(info);
        fclose(dst_h);
    }

    return result;
}

int ppWriteDataHeader(int eff, int crypt, unsigned short checksum, unsigned char* table, FILE* dst_h) {
	int error = 0;

	if (crypt) {
		if (fwrite(PX20, 1, sizeof(PX20) - 1, dst_h) != sizeof(PX20) - 1) {
			error = 1;
		}

		if (!error && write_word(dst_h, checksum) != sizeof(checksum)) {
			error = 1;
		}
	}
	else {
		if (!error && fwrite(PP20, 1, sizeof(PP20) - 1, dst_h) != sizeof(PP20) - 1) {
			error = 1;
		}
	}

	if (!error && fwrite(table, 1, 4, dst_h) != 4) {
		error = 1;
	}

	return error;
}

