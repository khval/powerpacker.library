
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
ULONG ppLoadData(char * filename,  ULONG col,  ULONG memtype, UBYTE ** bufferptr, ULONG * lenptr, BOOL (*password_func)(UBYTE *, ULONG));



