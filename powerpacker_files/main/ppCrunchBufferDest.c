/* :ts=4
 *  $VER: ppCrunchBufferDest.c $Revision$ (03-Jul-2022)
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
#include "../valid_functions.h"

/****** powerpacker/main/ppCrunchBufferDest ******************************************
*
*   NAME
*      ppCrunchBufferDest -- Description
*
*   SYNOPSIS
*      ULONG ppCrunchBufferDest(UBYTE * crunchinfo, UBYTE * buffer, 
*          UBYTE * dest, ULONG len);
*
*   FUNCTION
*
*   INPUTS
*       crunchinfo - 
*       buffer - 
*       dest - 
*       len - 
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

ULONG _powerpacker_ppCrunchBufferDest(struct PowerPackerIFace *Self,
       UBYTE * crunchinfo,
       UBYTE * buffer,
       UBYTE * dest,
       ULONG len)
{
  return ppCrunchBufferDest(crunchinfo, buffer, dest, len);
}

