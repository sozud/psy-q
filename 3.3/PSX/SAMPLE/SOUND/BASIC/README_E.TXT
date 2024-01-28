$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$	    
             +------------------------------------------+
	     |///    Sound sample program for PC    /// |
	     +------------------------------------------+

o SAMPLE:

	main.c : play the SEQ data file and SEP data file.

o controller:
	START:			play SEQ data.
	Triangle button:	play SEP data Track 1.
				SEQ decrescendo.
	O button:		play SEP data Track 2.
				SEP data Track 2 decrescendo.
	X button:		stop SEQ, SEP data Track 2.
	SELECT:			Quit program.

o Launch the program:
	
	> psymake
	> psymake load
	> dbugpsx main /r50 /e

		   Copyright (C) 1994 by Sony Computer Entertainment Inc.
