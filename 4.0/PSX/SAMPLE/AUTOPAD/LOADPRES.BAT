@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（press）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\press

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	tuto
rem ------------------------------------------------
:TUTO
:TUTO1
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto1l.cpe
	if "%1"=="tuto1" goto EXIT
:W_TUTO1
	if exist %TMPFILE% goto TUTO2
		delay %WAIT%
		goto W_TUTO1
rem ------------------------------------------------
:TUTO2
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto2l.cpe
	if "%1"=="tuto2" goto EXIT
:W_TUTO2
	if exist %TMPFILE% goto TUTO3
		delay %WAIT%
		goto W_TUTO2
rem ------------------------------------------------
:TUTO3
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto3l.cpe
	if "%1"=="tuto3" goto EXIT
:W_TUTO3
	if exist %TMPFILE% goto TUTO4
		delay %WAIT%
		goto W_TUTO3
rem ------------------------------------------------
:TUTO4
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto4l.cpe
	if "%1"=="tuto4" goto EXIT
:W_TUTO4
	if exist %TMPFILE% goto TUTO5
		delay %WAIT%
		goto W_TUTO4
rem ------------------------------------------------
:TUTO5
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto5l.cpe
	if "%1"=="tuto5" goto EXIT
:W_TUTO5
	if exist %TMPFILE% goto TUTO6
		delay %WAIT%
		goto W_TUTO5
rem ------------------------------------------------
:TUTO6
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto6l.cpe
	if "%1"=="tuto6" goto EXIT
:W_TUTO6
	if exist %TMPFILE% goto TUTO7
		delay %WAIT%
		goto W_TUTO6
rem ------------------------------------------------
:TUTO7
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto7l.cpe
	if not "%1"=="" goto EXIT
:W_TUTO7
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_TUTO7
rem ------------------------------------------------
rem	end
rem ------------------------------------------------
:END
	resetps
	cd %SAMPDIR%\autopad
