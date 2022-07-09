/* :ts=4
 *  $VER: ppGetPassword.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppGetPassword ******************************************
*
*   NAME
*      ppGetPassword -- Description
*
*   SYNOPSIS
*      BOOL ppGetPassword(struct Screen * screen, ULONG * buffer, 
*          ULONG maxchars, ULONG checksum);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       buffer - 
*       maxchars - 
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

BOOL _powerpacker_ppGetPassword(struct PowerPackerIFace *Self,
       struct Screen * screen,
       ULONG * buffer,
       ULONG maxchars,
       ULONG checksum)
{
  return ppGetPassword(screen, buffer, maxchars, checksum);
}

