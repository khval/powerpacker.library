/* :ts=4
 *  $VER: ppDecrypt.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppDecrypt ******************************************
*
*   NAME
*      ppDecrypt -- Description
*
*   SYNOPSIS
*      void ppDecrypt(UBYTE * buffer, ULONG * len, ULONG key);
*
*   FUNCTION
*
*   INPUTS
*       buffer - 
*       len - 
*       key - 
*
*   RESULT
*       This function does not return a result
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

void _powerpacker_ppDecrypt(struct PowerPackerIFace *Self,
       UBYTE * buffer,
       ULONG * len,
       ULONG key)
{
  ppDecrypt(buffer, len, key);
}

