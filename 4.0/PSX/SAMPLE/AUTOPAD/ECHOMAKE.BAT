@echo off
	call %CHGDIR% %1%
	echo ----------- %NOWDIR% ---------------- > %SAMPDIR%\autopad\tmp.log
	psymake %APDKIND% >> %SAMPDIR%\autopad\tmp.log
	type %SAMPDIR%\autopad\tmp.log
	type %SAMPDIR%\autopad\tmp.log >> %SAMPDIR%\autopad\make.log
