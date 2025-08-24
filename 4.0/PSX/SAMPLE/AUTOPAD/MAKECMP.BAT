@echo off
rem ------------------------------------------------
rem
rem	makefile (cmplr)
rem
rem ------------------------------------------------

set CDIR=%SAMPDIR%\cmplr
rem ------------------------------------------------
rem	scratch
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\scratch

cd %SAMPDIR%\autopad
