/* :ts=4
 *  $VER: ppFreeCrunchInfo.c $Revision$ (03-Jul-2022)
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

/****** powerpacker/main/ppFreeCrunchInfo ******************************************
*
*   NAME
*      ppFreeCrunchInfo -- Description
*
*   SYNOPSIS
*      void ppFreeCrunchInfo(APTR crunchinfo);
*
*   FUNCTION
*
*   INPUTS
*       crunchinfo - 
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

void _powerpacker_ppFreeCrunchInfo(struct PowerPackerIFace *Self,
       APTR crunchinfo)
{
  ppFreeCrunchInfo(crunchinfo);
}

