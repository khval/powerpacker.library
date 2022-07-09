/* :ts=4
 *  $VER: ppLoadData.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppLoadData ******************************************
*
*   NAME
*      ppLoadData -- Description
*
*   SYNOPSIS
*      ULONG ppLoadData(char * filename, ULONG col, ULONG memtype, 
*          UBYTE ** bufferptr, ULONG * lenptr, BOOL (*funcptr)());
*
*   FUNCTION
*
*   INPUTS
*       filename - 
*       col - 
*       memtype - 
*       bufferptr - 
*       lenptr - 
*       funcptr - 
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

ULONG _powerpacker_ppLoadData(struct PowerPackerIFace *Self,
       char * filename,
       ULONG col,
       ULONG memtype,
       UBYTE ** bufferptr,
       ULONG * lenptr,
       BOOL (*funcptr)())
{
  return ppLoadData(filename, col, memtype, bufferptr, lenptr, funcptr);
}

