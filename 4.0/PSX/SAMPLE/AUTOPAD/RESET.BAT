@echo off

if exist %TMPFILE% del %TMPFILE%
resetps
	delay %DELAY1%
run %PATCHX%
	delay %DELAY2%
run %NOPRINT%
	delay %DELAY3%
