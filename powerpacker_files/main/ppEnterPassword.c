/* :ts=4
 *  $VER: ppEnterPassword.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppEnterPassword ******************************************
*
*   NAME
*      ppEnterPassword -- Description
*
*   SYNOPSIS
*      BOOL ppEnterPassword(struct Screen * screen, UBYTE * buffer);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       buffer - 
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

BOOL _powerpacker_ppEnterPassword(struct PowerPackerIFace *Self,
       struct Screen * screen,
       UBYTE * buffer)
{
  return ppEnterPassword(screen, buffer);
}

