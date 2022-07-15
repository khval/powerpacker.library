
#ifndef header_powerpacker_h
#define header_powerpacker_h

#ifdef __GNUC__
   #ifdef __PPC__
    #pragma pack(2)
   #endif
#elif defined(__VBCC__)
   #pragma amiga-align
#endif

#define PPERR_OK           (0) /* no error                  */
#define PPERR_OPEN         (-1L) /* error opening file        */
#define PPERR_READ         (-2L) /* error reading from file   */
#define PPERR_NOMEMORY     (-3) /* out of memory             */
#define PPERR_PASSWORD     (-5L) /* bad or missing password   */
#define PPERR_UNKNOWN     (-6L) /* error unknown    */
#define PPERR_SEEK         (PPERR_UNKNOWN) /* error seeking in file     */
#define PPERR_DATAFORMAT   (PPERR_UNKNOWN) /* error in data format      */
#define PPERR_DECRUNCH     (PPERR_UNKNOWN) /* error decrunching data    */
#define PPERR_ARGS         (PPERR_UNKNOWN) /* bad arguments to function */

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

