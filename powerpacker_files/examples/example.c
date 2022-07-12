/*********************************
*										  *
*	powerpacker.library  V35	  *
*										  *
*	Release 1.3						*
*										  *
*	(c) 1991 Nico François		 *
*										  *
*	example.c						  *
*										  *
*	This source is public domain *
*	in all respects.				 *
*										  *
*********************************/

#include <stdio.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <libraries/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>

#include <proto/powerpacker.h>
#include <libraries/ppbase.h>

UBYTE *filestart = NULL;
ULONG filelen;

char *file;

int _main(int args, char **arg)
{
	int err;

	if (args!=2)
	{
		printf("exsample {filename}\n");
		return 0;
	}
		
	file = arg[1];

	puts ("Loading file...");

	err = ppLoadData (file, DECR_POINTER, 0L, &filestart, &filelen, NULL);

	if (err)
		printf ("error: %s!\n", ppErrorMessage (err));
	else
	{
		puts ("file in memory, using it...");

		/* file is loaded at 'filestart' and can now be used */

		/* ... */
		puts ("done, freeing file...");
	}

	/* free all resources */

	if (filestart) FreeMem (filestart, filelen);

	puts ("exiting.");
	return 0;
}

int main(int args, char **arg)
{
	int ret = 0;
	if (open_libs())
	{
		ret = _main( args, arg );
	}
	else
	{
		puts ("You need powerpacker.library V33+ !");
	}
	close_libs();
	return ret;
}

