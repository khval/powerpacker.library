/* :ts=4
 *  $VER: ppAllocCrunchInfo.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppAllocCrunchInfo ******************************************
*
*   NAME
*      ppAllocCrunchInfo -- Description
*
*   SYNOPSIS
*      APTR ppAllocCrunchInfo(ULONG eff, ULONG speedup, BOOL (*func)(), 
*          APTR userdata);
*
*   FUNCTION
*
*   INPUTS
*       eff - 
*       speedup - 
*       func - 
*       userdata - 
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

APTR _powerpacker_ppAllocCrunchInfo(struct PowerPackerIFace *Self,
       ULONG eff,
       ULONG speedup,
       BOOL (*func)(),
       APTR userdata)
{
	return ppAllocCrunchInfo(eff, speedup, func, userdata);
}

