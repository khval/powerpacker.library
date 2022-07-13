/* :ts=4
 *  $VER: ppLoadData2.c $Revision$ (13-Jul-2022)
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

/****** powerpacker/main/ppLoadData2 ******************************************
*
*   NAME
*      ppLoadData2 -- Description
*
*   SYNOPSIS
*      ULONG ppLoadData2(char * filename, UBYTE ** output_buffer, 
*          APTR (*alloc_fn)(ULONG), BOOL (*password_fn)(UBYTE *, ULONG));
*
*   FUNCTION
*
*   INPUTS
*       filename - 
*       output_buffer - 
*       alloc_fn - 
*       password_fn - 
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

ULONG _powerpacker_ppLoadData2(struct PowerPackerIFace *Self,
	char * filename,
	UBYTE **output_buffer,
	ULONG *output_buffer_len,
	APTR (*alloc_fn)(ULONG),
	void  (*free_fn)(ULONG),
	BOOL (*password_fn)(UBYTE *, ULONG))
{
  return ppLoadData2(filename, output_buffer, output_buffer_len, alloc_fn, free_fn, password_fn);
}

