

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <exec/libraries.h>
#include <exec/emulation.h>

#define __USE_INLINE__

#include <stdio.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <utility/tagitem.h>

#include <libraries/powerpacker.h>
#include "valid_functions.h"

#undef Close
#undef Open

STATIC APTR stub_Open_ppc(ULONG *regarray);
STATIC APTR stub_Close_ppc(ULONG *regarray);
STATIC APTR stub_Expunge_ppc(ULONG *regarray);
STATIC ULONG stub_Reserved_ppc(ULONG *regarray);

STATIC ULONG stub_ppLoadData_ppc(ULONG *regarray);
STATIC void stub_ppDecrunchBuffer_ppc(ULONG *regarray);
STATIC ULONG stub_ppCalcChecksum_ppc(ULONG *regarray);
STATIC ULONG stub_ppCalcPasskey_ppc(ULONG *regarray);
STATIC void stub_ppDecrypt_ppc(ULONG *regarray);
STATIC ULONG stub_ppGetPassword_ppc(ULONG *regarray);
STATIC ULONG stub_ppOBSOLETE_ppc(ULONG *regarray);
STATIC ULONG stub_ppOverlayDecrHdr_ppc(ULONG *regarray);
STATIC ULONG stub_ppAllocCrunchInfo_ppc(ULONG *regarray);
STATIC void stub_ppFreeCrunchInfo_ppc(ULONG *regarray);
STATIC ULONG stub_ppCrunchBuffer_ppc(ULONG *regarray);
STATIC ULONG stub_ppWriteDataHeader_ppc(ULONG *regarray);
STATIC ULONG stub_ppEnterPassword_ppc(ULONG *regarray);
STATIC ULONG stub_ppErrorMessage_ppc(ULONG *regarray);

const struct EmuTrap stub_Open = { TRAPINST, TRAPTYPE, (void *) stub_Open_ppc };
const struct EmuTrap stub_Close = { TRAPINST, TRAPTYPE, (void *) stub_Close_ppc };
const struct EmuTrap stub_Expunge = { TRAPINST, TRAPTYPE, (void *) stub_Expunge_ppc };
const struct EmuTrap stub_Reserved = { TRAPINST, TRAPTYPE, (void *) stub_Reserved_ppc };
const struct EmuTrap stub_ppLoadData = { TRAPINST, TRAPTYPE, (void *) stub_ppLoadData_ppc };
const struct EmuTrap stub_ppDecrunchBuffer = { TRAPINST, TRAPTYPE, (void *) stub_ppDecrunchBuffer_ppc };
const struct EmuTrap stub_ppCalcChecksum = { TRAPINST, TRAPTYPE, (void *) stub_ppCalcChecksum_ppc };
const struct EmuTrap stub_ppCalcPasskey = { TRAPINST, TRAPTYPE, (void *) stub_ppCalcPasskey_ppc };
const struct EmuTrap stub_ppDecrypt = { TRAPINST, TRAPTYPE, (void *) stub_ppDecrypt_ppc };
const struct EmuTrap stub_ppGetPassword = { TRAPINST, TRAPTYPE, (void *) stub_ppGetPassword_ppc };
const struct EmuTrap stub_ppOBSOLETE = { TRAPINST, TRAPTYPE, (void *) stub_ppOBSOLETE_ppc };
const struct EmuTrap stub_ppOverlayDecrHdr = { TRAPINST, TRAPTYPE, (void *) stub_ppOverlayDecrHdr_ppc };
const struct EmuTrap stub_ppAllocCrunchInfo = { TRAPINST, TRAPTYPE, (void *) stub_ppAllocCrunchInfo_ppc };
const struct EmuTrap stub_ppFreeCrunchInfo = { TRAPINST, TRAPTYPE, (void *) stub_ppFreeCrunchInfo_ppc };
const struct EmuTrap stub_ppCrunchBuffer = { TRAPINST, TRAPTYPE, (void *) stub_ppCrunchBuffer_ppc };
const struct EmuTrap stub_ppWriteDataHeader = { TRAPINST, TRAPTYPE, (void *) stub_ppWriteDataHeader_ppc };
const struct EmuTrap stub_ppEnterPassword = { TRAPINST, TRAPTYPE, (void *) stub_ppEnterPassword_ppc };
const struct EmuTrap stub_ppErrorMessage = { TRAPINST, TRAPTYPE, (void *) stub_ppErrorMessage_ppc };

const ULONG VecTable68K[] = {
	(ULONG) &stub_Open,
	(ULONG) &stub_Close,
	(ULONG) &stub_Expunge,
	(ULONG) &stub_Reserved,
	(ULONG) &stub_ppLoadData,
	(ULONG) &stub_ppDecrunchBuffer,
	(ULONG) &stub_ppCalcChecksum,
	(ULONG) &stub_ppCalcPasskey,
	(ULONG) &stub_ppDecrypt,
	(ULONG) &stub_ppGetPassword,
	(ULONG) &stub_ppOBSOLETE,
	(ULONG) &stub_ppOBSOLETE,
	(ULONG) &stub_ppOBSOLETE,
	(ULONG) &stub_ppOBSOLETE,
	(ULONG) &stub_ppOverlayDecrHdr,
	(ULONG) &stub_ppAllocCrunchInfo,
	(ULONG) &stub_ppFreeCrunchInfo,
	(ULONG) &stub_ppCrunchBuffer,
	(ULONG) &stub_ppWriteDataHeader,
	(ULONG) &stub_ppEnterPassword,
	(ULONG) &stub_ppOBSOLETE,
	(ULONG) &stub_ppErrorMessage,
	(ULONG)-1
};

STATIC APTR stub_Open_ppc(ULONG *regarray)
{
	struct Library *Base = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *ExtLib = (struct ExtendedLibrary *) ((ULONG)Base + Base->lib_PosSize);
	struct LibraryManagerInterface *Self = (struct LibraryManagerInterface *) ExtLib->ILibrary;

	regarray[REG68K_D0/4] = (ULONG) Self -> Open( 0);

	return (APTR) regarray[REG68K_D0/4];
}

STATIC APTR stub_Close_ppc(ULONG *regarray)
{
	struct Library *Base = (struct Library *) regarray[REG68K_A6/4];
	struct ExtendedLibrary *ExtLib = (struct ExtendedLibrary *) ((ULONG)Base + Base->lib_PosSize);
	struct LibraryManagerInterface *Self = (struct LibraryManagerInterface *) ExtLib->ILibrary;

	regarray[REG68K_D0/4] = (ULONG) Self -> Close();

	return (APTR) regarray[REG68K_D0/4];
}

STATIC APTR stub_Expunge_ppc(ULONG *regarray)
{
	return (APTR) regarray[REG68K_D0/4];
}

STATIC ULONG stub_Reserved_ppc(ULONG *regarray)
{
	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppLoadData_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppLoadData__legacy__( 
		( char *) regarray[REG68K_A0/4],
		(ULONG) regarray[REG68K_D0/4],
		(ULONG) regarray[REG68K_D1/4],
		(UBYTE **) regarray[REG68K_A1/4],
		(ULONG *) regarray[REG68K_A2/4],
		(BOOL (*)()) regarray[REG68K_A3/4] );

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC void stub_ppDecrunchBuffer_ppc(ULONG *regarray)
{
	ppDecrunchBuffer(
		( UBYTE *) regarray[REG68K_A0/4],
		( UBYTE *) regarray[REG68K_A1/4],
		( ULONG *) regarray[REG68K_A2/4],
		( ULONG) regarray[REG68K_D0/4] );
}

STATIC ULONG stub_ppCalcChecksum_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppCalcChecksum( (const char *) regarray[REG68K_A0/4] );
	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppCalcPasskey_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppCalcPasskey( (const char *) regarray[REG68K_A0/4] );
	return (ULONG) regarray[REG68K_D0/4];
}

STATIC void stub_ppDecrypt_ppc(ULONG *regarray)
{
	ppDecrypt( 
		(unsigned char *) regarray[REG68K_A0/4],
		(ULONG) regarray[REG68K_D0/4],
		(ULONG) regarray[REG68K_D1/4] );
}

STATIC ULONG stub_ppGetPassword_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = (ULONG) ppGetPassword( 
			(struct Screen *) regarray[REG68K_A0/4],
			(char *) regarray[REG68K_A1/4],
			(ULONG) regarray[REG68K_D0/4],
			(ULONG) regarray[REG68K_D1/4] );

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppOBSOLETE_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppOBSOLETE( (const char *) regarray[REG68K_A0/4] );
	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppOverlayDecrHdr_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppOverlayDecrHdr( (const char *) regarray[REG68K_A0/4] );
	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppAllocCrunchInfo_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = (ULONG) ppAllocCrunchInfo( 
		(ULONG) regarray[REG68K_D0/4], 
		(ULONG) regarray[REG68K_D1/4], 
		(BOOL (*)(ULONG,ULONG,APTR)) regarray[REG68K_A0/4], 
		(APTR) regarray[REG68K_A1/4]);

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC void stub_ppFreeCrunchInfo_ppc(ULONG *regarray)
{
	ppFreeCrunchInfo( (const char *) regarray[REG68K_A0/4] );
}

STATIC ULONG stub_ppCrunchBuffer_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppCrunchBuffer( 
		(CrunchInfo*) regarray[REG68K_A0/4],
		(unsigned char *) regarray[REG68K_A1/4], 
		(unsigned int) regarray[REG68K_D0/4]);

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppWriteDataHeader_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppWriteDataHeader(
		 (BPTR) regarray[REG68K_D0/4], 
		 (ULONG) regarray[REG68K_D1/4],
		 (BOOL) regarray[REG68K_D2/4],
		 (ULONG) regarray[REG68K_D3/4]);

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppEnterPassword_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppEnterPassword( 
			(struct Screen *) regarray[REG68K_A0/4],
			(UBYTE *) regarray[REG68K_A1/4]);

	return (ULONG) regarray[REG68K_D0/4];
}

STATIC ULONG stub_ppErrorMessage_ppc(ULONG *regarray)
{
	regarray[REG68K_D0/4] = ppErrorMessage( (LONG) regarray[REG68K_A0/4] );
	return (ULONG) regarray[REG68K_D0/4];
}



