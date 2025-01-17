#ifndef PROTO_POWERPACKER_H
#define PROTO_POWERPACKER_H

/*
**	$Id$
**
**	Prototype/inline/pragma header file combo
**
**	Copyright (c) 2010 Hyperion Entertainment CVBA.
**	All Rights Reserved.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef LIBRARIES_POWERPACKER_H
#include <libraries/powerpacker.h>
#endif

/****************************************************************************/

#ifndef __NOLIBBASE__
 #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
  extern struct AmigaOS::Library * PowerPackerBase;
 #else
  extern struct Library * PowerPackerBase;
 #endif
#endif /* __NOLIBBASE__ */

/****************************************************************************/

#ifdef __amigaos4__
 #include <interfaces/powerpacker.h>
 #ifdef __USE_INLINE__
  #include <inline4/powerpacker.h>
 #endif /* __USE_INLINE__ */
 #ifndef CLIB_POWERPACKER_PROTOS_H
  #define CLIB_POWERPACKER_PROTOS_H 1
 #endif /* CLIB_POWERPACKER_PROTOS_H */
 #ifndef __NOGLOBALIFACE__
  #if defined(__cplusplus) && defined(__USE_AMIGAOS_NAMESPACE__)
   extern struct AmigaOS::PowerPackerIFace * IPowerPacker;
  #else
   extern struct PowerPackerIFace * IPowerPacker;
  #endif
 #endif /* __NOGLOBALIFACE__ */
#else /* __amigaos4__ */
 #ifndef CLIB_POWERPACKER_PROTOS_H
  #include <clib/powerpacker_protos.h>
 #endif /* CLIB_POWERPACKER_PROTOS_H */
 #if defined(__GNUC__)
  #ifndef __PPC__
   #include <inline/powerpacker.h>
  #else /* __PPC__ */
   #include <ppcinline/powerpacker.h>
  #endif /* __PPC__ */
 #elif defined(__VBCC__)
  #ifndef __PPC__
   #include <inline/powerpacker_protos.h>
  #endif /* __PPC__ */
 #else /* __GNUC__ */
  #include <pragmas/powerpacker_pragmas.h>
 #endif /* __GNUC__ */
#endif /* __amigaos4__ */

/****************************************************************************/

#endif /* PROTO_POWERPACKER_H */
