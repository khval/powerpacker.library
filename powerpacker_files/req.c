
#define __USE_INLINE__

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/requester.h>

#define ALL_REACTION_CLASSES
#include <reaction/reaction.h>
#include <reaction/reaction_macros.h>

#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/icclass.h>
#include <intuition/gadgetclass.h>

char *req_input(struct Screen *screen, const char *title, const char *body, const char *buttons, ULONG image, int maxlen )
{  
	char *buffer; 
	Object *req = 0;		// the requester itself
	int button;			// the button that was clicked by the user

	buffer = alloca( maxlen );
	buffer[0] = 0;

	// fill in the requester structure
	req = NewObject(REQUESTER_GetClass(), NULL, 
		REQ_Type,       REQTYPE_STRING,
		REQ_TitleText,  (ULONG)title,
		REQ_BodyText,   (ULONG)body,
		REQ_GadgetText, (ULONG) buttons ,
		REQ_Image,      image,

		REQS_Buffer, buffer,
		REQS_MaxChars, maxlen-1,

		TAG_DONE);
        
	if (req) 
	{ 
		struct orRequest reqmsg;

		reqmsg.MethodID  = RM_OPENREQ;
		reqmsg.or_Attrs  = NULL;
		reqmsg.or_Window = NULL;
		reqmsg.or_Screen = screen;

		button = IDoMethodA(req, (Msg) &reqmsg);     
		DisposeObject(req);

		return (button == 1) ? strdup(buffer) : NULL;

	}  else printf("[request] Failed to allocate requester\n");

	return( NULL ); // give the button number back to the caller
}

