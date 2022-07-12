

#define __USE_INLINE__

#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <proto/dos.h>

struct NewlibIFace * INewlib = NULL;
struct DOSIFace *IDOS = NULL;
struct IntuitionIFace *IIntuition = NULL;

struct Library *NewLibBase = NULL;
struct Library *DOSBase = NULL;
struct Library *IntuitionBase = NULL;

///////////////////////////////////////////////////////

struct Library	*WindowBase = NULL;
struct WindowIFace *IWindow = NULL;

struct Library	*ListBrowserBase = NULL;
struct ListBrowserIFace *IListBrowser = NULL;

struct Library	*LayoutBase = NULL;
struct LayoutIFace *ILayout = NULL;

struct Library	*StringBase = NULL;
struct StringIFace *IString = NULL;

struct Library	*ButtonBase = NULL;
struct ButtonIFace *IButton = NULL;

struct Library	*ChooserBase = NULL;
struct ChooserIFace *IChooser = NULL;

struct Library	*RequesterBase = NULL;
struct RequesterIFace *IRequester = NULL;

/////////////////////////////////////////////////////

void close_lib(struct Library **base,struct Interface **i)
{
	struct ExecIFace *IExec = (struct ExecIFace *)(*(struct ExecBase **)4)->MainInterface;

	if (*i) DropInterface((struct Interface*) *i); *i = 0; 
	if (*base) CloseLibrary(*base); *base = 0; 
}

BOOL open_lib( const char *name, int ver , const char *iname, int iver, struct Library **base, struct Interface **interface)
{

	*interface = NULL;
	*base = OpenLibrary( name , ver);

	if (*base)
	{
		 *interface = GetInterface( *base,  iname , iver, TAG_END );
		if (!*interface) if (IDOS) Printf("Unable to getInterface %s for %s %d!\n",iname,name,ver);
	}
	else
	{
	   	if (IDOS) Printf("Unable to open the %s %ld!\n",name,ver);
	}

	return (*interface) ? TRUE : FALSE;
}

BOOL open_libs()
{
	if ( ! (open_lib( "dos.library", 53L , "main", 1, &DOSBase, (struct Interface **) &IDOS  )) ) return FALSE;
	if ( ! (open_lib( "newlib.library", 53L , "main", 1, &NewLibBase, (struct Interface **) &INewlib  )) ) return FALSE;
	if ( ! (open_lib( "intuition.library", 53L , "main", 1, &IntuitionBase, (struct Interface **) &IIntuition  )) ) return FALSE;

	if ( ! (open_lib( "window.class", 54L , "main", 1, &WindowBase, (struct Interface **) &IWindow  )) ) return FALSE;
	if ( ! (open_lib( "requester.class", 53, "main", 1, &RequesterBase, (struct Interface **) &IRequester )) ) return FALSE;
	if ( ! (open_lib( "listbrowser.gadget", 53 , "main", 1, &ListBrowserBase, (struct Interface **) &IListBrowser )) ) return FALSE;
	if ( ! (open_lib( "layout.gadget", 53, "main", 1, &LayoutBase, (struct Interface **) &ILayout)) ) return FALSE;
	if ( ! (open_lib( "string.gadget", 53, "main", 1, &StringBase, (struct Interface **) &IString )) ) return FALSE;
	if ( ! (open_lib( "chooser.gadget", 53, "main", 1, &ChooserBase, (struct Interface **) &IChooser)) ) return FALSE;
	if ( ! (open_lib( "button.gadget", 53L , "main", 1, &ButtonBase, (struct Interface **) &IButton  )) ) return FALSE;

	return TRUE;
}

void close_libs()
{

// Classes

	close_lib(&WindowBase,(struct Interface **) &IWindow);
	close_lib(&ListBrowserBase, (struct Interface **) &IListBrowser);
	close_lib(&LayoutBase,(struct Interface **) &ILayout);
	close_lib(&StringBase,(struct Interface **) &IString);
	close_lib(&ChooserBase,(struct Interface **) &IChooser);
	close_lib(&RequesterBase,(struct Interface **) &IRequester);

// libs

	close_lib( &DOSBase , (struct Interface **) &IDOS );
	close_lib( &NewLibBase, (struct Interface **) &INewlib );
	close_lib( &IntuitionBase, (struct Interface **) &IIntuition );

}

