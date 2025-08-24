@echo off
rem ------------------------------------------------
rem
rem	makefile (module)
rem
rem ------------------------------------------------

set CDIR=%SAMPDIR%\module
rem ------------------------------------------------
rem	execmenu
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\execmenu
	call %ECHOMAKE% %CDIR%\execmenu\anim
	call %ECHOMAKE% %CDIR%\execmenu\balls
	call %ECHOMAKE% %CDIR%\execmenu\rcube

cd %SAMPDIR%\autopad
