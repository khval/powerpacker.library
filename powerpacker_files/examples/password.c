
#include <stdio.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <libraries/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>

#include <proto/powerpacker.h>
#include <libraries/ppbase.h>

int _main(int args, char **arg)
{
	char buffer[50];
	ULONG chksum = 33;

	BOOL opt = ppGetPassword ((struct Screen *) NULL, (UBYTE *) buffer, (ULONG) sizeof(buffer), (ULONG) chksum );

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
