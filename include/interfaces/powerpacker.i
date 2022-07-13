#ifndef POWERPACKER_INTERFACE_DEF_H
#define POWERPACKER_INTERFACE_DEF_H
/*
** This file is machine generated from idltool
** Do not edit
*/ 

#include <exec/types.i>
#include <exec/exec.i>
#include <exec/interfaces.i>

STRUCTURE PowerPackerIFace, InterfaceData_SIZE
	    FPTR IPowerPacker_Obtain
	    FPTR IPowerPacker_Release
	    FPTR PowerPackerIFace_Expunge_UNIMPLEMENTED
	    FPTR PowerPackerIFace_Clone_UNIMPLEMENTED
	    FPTR IPowerPacker_ppLoadData__legacy__
	    FPTR IPowerPacker_ppDecrunchBuffer
	    FPTR IPowerPacker_ppCalcChecksum
	    FPTR IPowerPacker_ppCalcPasskey
	    FPTR IPowerPacker_ppDecrypt
	    FPTR IPowerPacker_ppGetPassword
	    FPTR IPowerPacker_ppAllocCrunchInfo
	    FPTR IPowerPacker_ppFreeCrunchInfo
	    FPTR IPowerPacker_ppCrunchBuffer
	    FPTR IPowerPacker_ppWriteDataHeader
	    FPTR IPowerPacker_ppEnterPassword
	    FPTR IPowerPacker_ppErrorMessage
	    FPTR IPowerPacker_ppCrunchBufferDest
	    FPTR IPowerPacker_ppLoadData2
	LABEL PowerPackerIFace_SIZE

#endif
