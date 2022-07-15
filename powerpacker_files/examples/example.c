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

UBYTE *filestart = NULL;
ULONG filelen;

char *file;

APTR pp_alloc_fn(ULONG size)
{
	return malloc(size);
}

void pp_free_fn(APTR ptr)
{
	free(ptr);
}

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

	err = ppLoadData2 (file, &filestart, &filelen, pp_alloc_fn, pp_free_fn, NULL);

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

	if (filestart) pp_free_fn (filestart);

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

