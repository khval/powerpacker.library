/* :ts=4
 *  $VER: ppErrorMessage.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppErrorMessage ******************************************
*
*   NAME
*      ppErrorMessage -- Description
*
*   SYNOPSIS
*      char * ppErrorMessage(ULONG error);
*
*   FUNCTION
*
*   INPUTS
*       error - 
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

char * _powerpacker_ppErrorMessage(struct PowerPackerIFace *Self,
       ULONG error)
{
  return ppErrorMessage(error);
}

