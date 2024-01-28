/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef LOADMENU_INCLUDED
#define LOADMENU_INCLUDED

#ifdef CDEXEC
#define MENUFILE "\\CDMENU.LST;1"
#elif  CDEXEC2
#define MENUFILE "CDMENU.LST"
#else
#define MENUFILE "PCMENU.LST"
#endif

#endif
