/*
**	$Id$
**	Generated by IDLTool 53.24
**	Do not edit
**
**	AutoInit stub for powerpacker
**
**	Copyright (c) 2010 Hyperion Entertainment CVBA.
**	All Rights Reserved.
*/

#include <exec/types.h>
#include <libraries/powerpacker.h>

#include <interfaces/powerpacker.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <stdlib.h>

/****************************************************************************/

struct Library * PowerPackerBase = NULL;
static struct Library * __PowerPackerBase = NULL;

/****************************************************************************/

void powerpacker_base_constructor(void)
{
    if (PowerPackerBase != NULL)
    {
        return; /* Someone was quicker, e.g. an interface constructor */
    }
    __PowerPackerBase = PowerPackerBase = IExec->OpenLibrary("powerpacker.library", 0);
    if (PowerPackerBase == NULL)
    {
        IDOS->PutErrStr("ERROR: Failed to open 'powerpacker.library'.\n");
        abort();
    }
}
__attribute__((section(".ctors.zzzz"))) static void
(*powerpacker_base_constructor_ptr)(void) USED = powerpacker_base_constructor;

/****************************************************************************/

void powerpacker_base_destructor(void)
{
    if (__PowerPackerBase != NULL)
    {
        IExec->CloseLibrary(__PowerPackerBase);
    }
}
__attribute__((section(".dtors.zzzz"))) static void
(*powerpacker_base_destructor_ptr)(void) USED = powerpacker_base_destructor;

/****************************************************************************/
