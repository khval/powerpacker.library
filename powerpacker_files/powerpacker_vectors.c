
/* This file was generated by idltool 53.24. Do not edit */

#include "powerpacker_vectors.h"

STATIC uint32 VARARGS68K _powerpacker_UNIMPLEMENTED(struct PowerPackerIFace *iface UNUSED)
{
  return 0;
}
STATIC CONST APTR main_v1_vectors[] =
{
    _powerpacker_Obtain,
    _powerpacker_Release,
    _powerpacker_UNIMPLEMENTED, /* Expunge */
    _powerpacker_UNIMPLEMENTED, /* Clone */
    _powerpacker_ppLoadData__legacy__,
    _powerpacker_ppDecrunchBuffer,
    _powerpacker_ppCalcChecksum,
    _powerpacker_ppCalcPasskey,
    _powerpacker_ppDecrypt,
    _powerpacker_ppGetPassword,
    _powerpacker_ppAllocCrunchInfo,
    _powerpacker_ppFreeCrunchInfo,
    _powerpacker_ppCrunchBuffer,
    _powerpacker_ppWriteDataHeader,
    _powerpacker_ppEnterPassword,
    _powerpacker_ppErrorMessage,
    _powerpacker_ppCrunchBufferDest,
    _powerpacker_ppLoadData2,
    (APTR)-1
};
