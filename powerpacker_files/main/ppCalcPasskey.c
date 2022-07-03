/* :ts=4
 *  $VER: ppCalcPasskey.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppCalcPasskey ******************************************
*
*   NAME
*      ppCalcPasskey -- Description
*
*   SYNOPSIS
*      ULONG ppCalcPasskey(char * string);
*
*   FUNCTION
*
*   INPUTS
*       string - 
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

ULONG _powerpacker_ppCalcPasskey(struct PowerPackerIFace *Self,
       char * string)
{
  return ppCalcPasskey(string);
}
