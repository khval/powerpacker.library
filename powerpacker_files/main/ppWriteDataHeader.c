/* :ts=4
 *  $VER: ppWriteDataHeader.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppWriteDataHeader ******************************************
*
*   NAME
*      ppWriteDataHeader -- Description
*
*   SYNOPSIS
*      BOOL ppWriteDataHeader(BPTR lock, ULONG eff, BOOL crypt, 
*          ULONG checksum);
*
*   FUNCTION
*
*   INPUTS
*       lock - 
*       eff - 
*       crypt - 
*       checksum - 
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

BOOL _powerpacker_ppWriteDataHeader(struct PowerPackerIFace *Self,
       BPTR lock,
       ULONG eff,
       BOOL crypt,
       ULONG checksum)
{
  return ppWriteDataHeader(lock, eff, crypt, checksum);
}

