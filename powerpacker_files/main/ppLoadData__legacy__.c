/* :ts=4
 *  $VER: ppLoadData__legacy__.c $Revision$ (13-Jul-2022)
 *
 *  This file is part of powerpacker.
 *
 *  This file is generated by IDLTool.
 *  It's modifyed by Kjetil Hvalstrand.
 *  this file is distributed as Public Domain.
 *  it's based on developer documentation for powerpacker.library release 1.6
 *  Copyright (c) 1991-1994 Nico Francois, 
 *  See. LICENSE.md for more info.
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

/****** powerpacker/main/ppLoadData__legacy__ ******************************************
*
*   NAME
*      ppLoadData__legacy__ -- Description
*
*   SYNOPSIS
*      ULONG ppLoadData__legacy__(char * filename, ULONG col, ULONG memtype, 
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

ULONG _powerpacker_ppLoadData__legacy__(struct PowerPackerIFace *Self,
       char * filename,
       ULONG col,
       ULONG memtype,
       UBYTE ** bufferptr,
       ULONG * lenptr,
       BOOL (*funcptr)())
{
  return ppLoadData__legacy__(filename, col, memtype, bufferptr, lenptr, funcptr);
}
