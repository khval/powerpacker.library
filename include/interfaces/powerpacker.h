#ifndef POWERPACKER_INTERFACE_DEF_H
#define POWERPACKER_INTERFACE_DEF_H

/*
** This file was machine generated by idltool 53.24.
** Do not edit
*/ 

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

#ifndef LIBRARIES_POWERPACKER_H
#include <libraries/powerpacker.h>
#endif

#ifdef __cplusplus
#ifdef __USE_AMIGAOS_NAMESPACE__
namespace AmigaOS {
#endif
extern "C" {
#endif

struct PowerPackerIFace
{
	struct InterfaceData Data;

	ULONG APICALL (*Obtain)(struct PowerPackerIFace *Self);
	ULONG APICALL (*Release)(struct PowerPackerIFace *Self);
	APTR Expunge_UNIMPLEMENTED;
	APTR Clone_UNIMPLEMENTED;
	ULONG APICALL (*ppLoadData)(struct PowerPackerIFace *Self, char * filename, ULONG col, ULONG memtype, UBYTE ** bufferptr, ULONG * lenptr, BOOL (*funcptr)());
	void APICALL (*ppDecrunchBuffer)(struct PowerPackerIFace *Self, UBYTE * endcrun, UBYTE * decrbuff, ULONG * effptr, ULONG col);
	ULONG APICALL (*ppCalcChecksum)(struct PowerPackerIFace *Self, char * string);
	ULONG APICALL (*ppCalcPasskey)(struct PowerPackerIFace *Self, char * string);
	void APICALL (*ppDecrypt)(struct PowerPackerIFace *Self, UBYTE * buffer, ULONG * len, ULONG key);
	BOOL APICALL (*ppGetPassword)(struct PowerPackerIFace *Self, struct Screen * screen, ULONG * buffer, ULONG maxchars, ULONG checksum);
	APTR APICALL (*ppAllocCrunchInfo)(struct PowerPackerIFace *Self, ULONG eff, ULONG speedup, BOOL (*func)(), APTR userdata);
	void APICALL (*ppFreeCrunchInfo)(struct PowerPackerIFace *Self, APTR crunchinfo);
	ULONG APICALL (*ppCrunchBuffer)(struct PowerPackerIFace *Self, APTR crunchinfo, UBYTE * buffer, ULONG length);
	BOOL APICALL (*ppWriteDataHeader)(struct PowerPackerIFace *Self, BPTR lock, ULONG eff, BOOL crypt, ULONG checksum);
	BOOL APICALL (*ppEnterPassword)(struct PowerPackerIFace *Self, struct Screen * screen, UBYTE * buffer);
	char * APICALL (*ppErrorMessage)(struct PowerPackerIFace *Self, ULONG error);
	ULONG APICALL (*ppCrunchBufferDest)(struct PowerPackerIFace *Self, UBYTE * crunchinfo, UBYTE * buffer, UBYTE * dest, ULONG len);
};

#ifdef __cplusplus
}
#ifdef __USE_AMIGAOS_NAMESPACE__
}
#endif
#endif

#endif /* POWERPACKER_INTERFACE_DEF_H */
