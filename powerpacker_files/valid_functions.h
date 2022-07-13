
#include <proto/intuition.h>

void ppDecrypt( unsigned char * buffer, unsigned int len, unsigned int key);
BOOL ppWriteDataHeader( BPTR lock, ULONG eff, BOOL crypt, ULONG checksum);
CrunchInfo* ppAllocCrunchInfo( ULONG eff, ULONG speedup, BOOL (*func)(ULONG,ULONG,APTR), APTR userdata);
unsigned short ppCalcChecksum( const char * passwd );
unsigned int ppCalcPasskey( const char * passwd);
ULONG ppCrunchBuffer( CrunchInfo* crunchinfo, unsigned char * buffer, unsigned int length);
ULONG ppCrunchBufferDest( unsigned char * crunchinfo, unsigned char * buffer, unsigned char * dest, ULONG len);
void ppFreeCrunchInfo( CrunchInfo* crunchinfo );
BOOL ppGetPassword( struct Screen * screen, ULONG * buffer, ULONG maxchars, ULONG checksum);
void ppDecrunchBuffer( UBYTE * endcrun, UBYTE * decrbuff, ULONG * effptr, ULONG col);
const char *ppErrorMessage(LONG error);
BOOL ppEnterPassword( struct Screen * screen, UBYTE * buffer);

ULONG ppLoadData__legacy__(char * filename,  ULONG col,  ULONG memtype, UBYTE ** bufferptr, ULONG * buflenptr, BOOL (*password_func)(UBYTE *, ULONG));
ULONG ppLoadData2(char * filename, UBYTE ** bufferptr, ULONG * lenptr, APTR (*alloc_func) (ULONG) , void (*free_func) (ULONG), BOOL (*password_func)(UBYTE *, ULONG));

