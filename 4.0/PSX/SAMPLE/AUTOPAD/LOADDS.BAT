@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（DS）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\ds

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	tuto
rem ------------------------------------------------
:TUTO
:TUTO0
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto0l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO0
	if exist %TMPFILE% goto TUTO1
		delay %WAIT%
		goto W_TUTO0
rem ------------------------------------------------
:TUTO1
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto1l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO1
	if exist %TMPFILE% goto TUTO2
		delay %WAIT%
		goto W_TUTO1
rem ------------------------------------------------
:TUTO2
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto2l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO2
	if exist %TMPFILE% goto TUTO3
		delay %WAIT%
		goto W_TUTO2
rem ------------------------------------------------
:TUTO3
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto3l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO3
	if exist %TMPFILE% goto TUTO4
		delay %WAIT%
		goto W_TUTO3
rem ------------------------------------------------
:TUTO4
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto4l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO4
	if exist %TMPFILE% goto TUTO5
		delay %WAIT%
		goto W_TUTO4
rem ------------------------------------------------
:TUTO5
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto5l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO5
	if exist %TMPFILE% goto TUTO6
		delay %WAIT%
		goto W_TUTO5
rem ------------------------------------------------
:TUTO6
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto6l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO6
	if exist %TMPFILE% goto TUTO7
		delay %WAIT%
		goto W_TUTO6
rem ------------------------------------------------
:TUTO7
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto7l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO7
	if exist %TMPFILE% goto TUTO8
		delay %WAIT%
		goto W_TUTO7
rem ------------------------------------------------
:TUTO8
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto8l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO8
	if exist %TMPFILE% goto TUTO9
		delay %WAIT%
		goto W_TUTO8
rem ------------------------------------------------
:TUTO9
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto9l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO9
	if exist %TMPFILE% goto TUTO10
		delay %WAIT%
		goto W_TUTO9
rem ------------------------------------------------
:TUTO10
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto10l.cpe
	if "%1"=="tuto0" goto EXIT
:W_TUTO10
	if exist %TMPFILE% goto TUTO11
		delay %WAIT%
		goto W_TUTO10
rem ------------------------------------------------
:TUTO11
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto11l.cpe
	if not "%1"=="" goto EXIT
:W_TUTO11
	if exist %TMPFILE% goto TUTO12
		delay %WAIT%
		goto W_TUTO11
rem ------------------------------------------------
:TUTO12
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto12l.cpe
	if not "%1"=="" goto EXIT
:W_TUTO12
	if exist %TMPFILE% goto TUTO13
		delay %WAIT%
		goto W_TUTO12
rem ------------------------------------------------
:TUTO13
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto13l.cpe
	if not "%1"=="" goto EXIT
:W_TUTO13
	if exist %TMPFILE% goto TUTO14
		delay %WAIT%
		goto W_TUTO13
rem ------------------------------------------------
:TUTO14
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto14l.cpe
	if not "%1"=="" goto EXIT
:W_TUTO14
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_TUTO14
rem ------------------------------------------------
rem	end
rem ------------------------------------------------
:EXIT
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto EXIT
:END
	resetps
	cd %SAMPDIR%\autopad
