ABSTRACT: PSYQ DIRECTORY			07/31/96


This directory contains the PSYQ development tools such as
the compiler, debugger and many other useful utilities for
PlayStation development.

For installation instructions please refer to the technical
note on installation, [cdrom]:\docs\technote\install.(doc/pdf)

The programs/utilities marked "NEW!" are new releases since the
last release.  

BIN
        COMPILER & LINKER
  NEW! -Assembler                          2.14 asmpsx.exe
  NEW! -Assembler for C compiler output    2.34 aspsx.exe
  NEW! -Performs the C++ Compiler main      -   cc1plpsx.exe     
        pass.                               -   cc1psx.exe      
  NEW! -PsyQ Compiler                      1.18 ccpsx.exe                     
       -C++ Compiler                       1.12 cpluspsx.exe
  NEW! -                                    -   cpppsx.exe
       -PsyQ Librarian utility             2.04 psylib.exe
  NEW! -PsyQ Linker                        2.38 psylink.exe   
       -PsyQ Make utility                  1.12 psymake.exe
  NEW! -PsyQ initializaton file.            -   psyq.ini
  
        DEBUGGER
  NEW! -PlayStation Development debugger   4.9  dbugpsx.exe       
       -Message handler TSR (for debugger  1.02 mess1.com 
        output)
       -Message handler TSR (for RS232     3.01 mess3.com
        output)
       -PC message test program             -   testmess.com

        DEVELOPMENT BOARD UTILITIES
       -Program for delay in batch files.   -   delay.com
       -PsyQ bios extensions for the       1.23 dexbios.com
        DTL-H2000 board.      
       -Utility to load a COFF file to      -   pqaload.com
        target RAM.        
       -Utility to load a raw bin file to   -   pqbload.com
        target RAM.
       -Utility to execute from address     -   pqgopsx.com
       -Utility to reset DTL-H2000 board    -   resetps.exe
  NEW! -PsyQ executable for downloading    2.33 run.exe
        program/runner.
  NEW! -Batch file for resetps and patch    -   rp.bat

        PlayStation executables needed to run emulation
        sessions and also to switch between the emulation
        session and the CD-R session.
       -Utility to start the program from   -   cdexec.cpe
        the CD-ROM/Emulator.
       -Command switch to select use of     -   selcd.cpe
        CD-R (DTL-2010)       
       -Command switch to select use of     -   selemu.cpe
        CD emulator drive.  
  NEW! -Batch file for cdexec               -   runcdex.bat
  NEW! -Batch file for selcd                -   runselcd.bat
  NEW! -Batch file for selemu               -   runselemu.bat                              

        SN SYSTEMS FILES
       -SN Original patch ROM utility       -   snpatch.cpe

        UTILITIES
       -Utility to view more lines in a     -   bv.exe
        ms-dos window.
  NEW! -Utility to dump contents of a      2.0  dumpcpe.exe
        *.cpe file.  
  NEW! -Utility to dump contents of a      1.0  dumpexe.exe
        *.exe file.
  NEW! -Utility to dump contents of a      2.0  dumpobj.exe
        *.obj file.
       -Utility to dump contents of a       -   dumpsym.exe 
        *.sym file.
       -Kanji Utility                       -   Kanji.com
       -Utility to view the version         -   version.com
        number of a *.exe file.
  NEW! -DOS extender                        -   dos4gw.exe
  NEW! -Symbol file compaction utility     1.3  symmunge.exe


DOC	
	This directory contains sub-directories representing
        PSYQ programs with additional documentation. 

        Compiler documentation              -   Compiler
        Debugger documentation              -   Debugger
        Miscellaneous SN documentation      -   Sn
        Symmunge documentation              -   Symmunge

SAMPLE    
        This directory contains samples released by SN for 
        use with particular tools.

        Samples for debugger (overlay)      -   Debugger

SRC
        This directory contains source for SN's library,
        libsn and utility, symmunnge.

        Source code for libsn.lib           -   Sn
        Sounce code for utility symmunge    -   symmunge.


We have provided some of the documents in both plain text
and Adobe Acrobat PDF format.  All the documents have been
incorporated into the CD search index. Please use an Acrobat 
reader to view the PDF documents. 


=======================================================================

Copyright (C) 1994 - 1996 Sony Computer Entertainment Inc.
			              All Rights Reserved.

PlayStation and PlayStation logos are trademarks of Sony Computer
Entertainment Inc. All other trademarks are property of their 
respective owners and/or their licensors.


SONY COMPUTER ENTERTAINMENT AMERICA
919 East Hillsdale Blvd, 2nd Floor
Foster City CA 94404
415-655-8000    
E-mail: DevTech_Support@interactive.sony.com
Developer Support BBS:     415-655-8119
Developer Support Hotline: 415-655-8181    

SONY COMPUTER ENTERTAINMENT EUROPE
Waverley House
7-12 Noel Street
London W1V 4HH
E-mail: dev_support@interactive.sony.com
WWW: http://www.scee.sony.co.uk
                      FAX:  +44 (0) 171 390 4324
Developer Support Hotline:  +44 (0) 171 390 1680

========================================================================