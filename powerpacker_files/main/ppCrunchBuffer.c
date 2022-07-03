/* :ts=4
 *  $VER: ppCrunchBuffer.c $Revision$ (03-Jul-2022)
 *
 *  This file is part of powerpacker.
 *
 *  Copyright (c) 1991-1994 Nico Francois.
 *  Free distribution, see LICENSE for more info.
 *
 * $Id$
 *
 * $Log$
 *
 *
 */


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/powerpacker.h>
#include <proto/powerpacker.h>
#include <stdarg.h>

/****** powerpacker/main/ppCrunchBuffer ******************************************
*
*   NAME
*      ppCrunchBuffer -- Description
*
*   SYNOPSIS
*      ULONG ppCrunchBuffer(APTR crunchinfo, UBYTE * buffer, ULONG length);
*
*   FUNCTION
*
*   INPUTS
*       crunchinfo - 
*       buffer - 
*       length - 
*
*   RESULT
*       The result ...
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/

ULONG _powerpacker_ppCrunchBuffer(struct PowerPackerIFace *Self,
       APTR crunchinfo,
       UBYTE * buffer,
       ULONG length)
{
  return ppCrunchBuffer(crunchinfo, buffer, length);
}

