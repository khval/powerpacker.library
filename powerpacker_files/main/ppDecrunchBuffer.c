/* :ts=4
 *  $VER: ppDecrunchBuffer.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppDecrunchBuffer ******************************************
*
*   NAME
*      ppDecrunchBuffer -- Description
*
*   SYNOPSIS
*      void ppDecrunchBuffer(UBYTE * endcrun, UBYTE * decrbuff, 
*          ULONG * effptr, ULONG col);
*
*   FUNCTION
*
*   INPUTS
*       endcrun - 
*       decrbuff - 
*       effptr - 
*       col - 
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

void ppDecrunchBuffer(
       UBYTE * endcrun,
       UBYTE * decrbuff,
       ULONG * effptr,
       ULONG col);

void _powerpacker_ppDecrunchBuffer(struct PowerPackerIFace *Self,
       UBYTE * endcrun,
       UBYTE * decrbuff,
       ULONG * effptr,
       ULONG col)
{
  ppDecrunchBuffer(endcrun, decrbuff, effptr, col);
}

