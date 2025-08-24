@echo off
rem ------------------------------------------------
rem
rem	makefile (etc)
rem
rem ------------------------------------------------

set CDIR=%SAMPDIR%\etc
rem ------------------------------------------------
rem	card
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\card
rem ------------------------------------------------
rem	mcrd
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\mcrd\tuto1
	call %ECHOMAKE% %CDIR%\mcrd\tuto2
rem ------------------------------------------------
rem	thread
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\thread

cd %SAMPDIR%\autopad
