
#ifndef header_powerpacker_h
#define header_powerpacker_h

#ifdef __GNUC__
   #ifdef __PPC__
    #pragma pack(2)
   #endif
#elif defined(__VBCC__)
   #pragma amiga-align
#endif

typedef struct {
	unsigned int tag;
	unsigned char* src;
	unsigned int src_len;
	unsigned char* dst;
	unsigned int dst_len;
} decrunch_t;

typedef struct {
	unsigned int token;
	unsigned int* ptr;
	unsigned int* ptr_max;
} write_res_t;

#ifdef __GNUC__
   #ifdef __PPC__
    #pragma pack()
   #endif
#elif defined(__VBCC__)
   #pragma default-align
#endif

typedef struct {
	unsigned short w00[4];
	unsigned short w08[4];
	unsigned short w10[4];
	unsigned char b2C[4];
	unsigned char* start;
	unsigned int fsize;
	unsigned char* src_end;

	unsigned char** addrs;
	unsigned int addrs_count;
	unsigned char* dst;
	unsigned int tmp[0x80];
	unsigned char* print_pos;

	unsigned short value;
	unsigned short bits;

	unsigned short* wnd1;
	unsigned short* wnd2;
	unsigned short wnd_max;
	unsigned short wnd_off;
	unsigned short wnd_left;

	BOOL (*progress_fn)(ULONG,ULONG,APTR);
} CrunchInfo;


#endif

