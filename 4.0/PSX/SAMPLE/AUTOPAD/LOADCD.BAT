@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（ＣＤ）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\cd

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	earth
rem ------------------------------------------------
:EARTH
	call %CHGDIR% %CDIR%\earth
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% earthl.cpe
	if not "%1"=="" goto EXIT
:W_EARTH
	if exist %TMPFILE% goto MOVIE
		delay %WAIT%
		goto W_EARTH
rem ------------------------------------------------
rem	movie
rem ------------------------------------------------
:MOVIE
:MOVIE0
	call %CHGDIR% %CDIR%\movie
	call %RESET%
	call %ECHORUN% movie0l.cpe
	if "%1"=="movie0" goto EXIT
:W_MOVIE0
	if exist %TMPFILE% goto MOVIE1
		delay %WAIT%
		goto W_MOVIE0
rem ------------------------------------------------
:MOVIE1
	call %CHGDIR% %CDIR%\movie
	call %RESET%
	call %ECHORUN% movie1l.cpe
	if "%1"=="movie1" goto EXIT
:W_MOVIE1
	if exist %TMPFILE% goto MOVIE2
		delay %WAIT%
		goto W_MOVIE1
rem ------------------------------------------------
:MOVIE2
	call %CHGDIR% %CDIR%\movie
	call %RESET%
	call %ECHORUN% movie2l.cpe
	if "%1"=="movie2" goto EXIT
:W_MOVIE2
	if exist %TMPFILE% goto MOVIE3
		delay %WAIT%
		goto W_MOVIE2
rem ------------------------------------------------
:MOVIE3
	call %CHGDIR% %CDIR%\movie
	call %RESET%
	call %ECHORUN% movie3l.cpe
	if not "%1"=="" goto EXIT
:W_MOVIE3
	if exist %TMPFILE% goto STR3D
		delay %WAIT%
		goto W_MOVIE3
rem ------------------------------------------------
rem	str3d
rem ------------------------------------------------
:STR3D
	call %CHGDIR% %CDIR%\str3d
	call %RESET%
	psymake load
		delay %DELAAY4%
	call %ECHORUN% str3dl.cpe
	if not "%1"=="" goto EXIT
:W_STR3D
	if exist %TMPFILE% goto TUTO
		delay %WAIT%
		goto W_STR3D
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
	if "%1"=="tuto7" goto EXIT
:W_TUTO7
	if exist %TMPFILE% goto TUTO8
		delay %WAIT%
		goto W_TUTO7
rem ------------------------------------------------
:TUTO8
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto8l.cpe
	if "%1"=="tuto8" goto EXIT
:W_TUTO8
	if exist %TMPFILE% goto TUTO9
		delay %WAIT%
		goto W_TUTO8
rem ------------------------------------------------
:TUTO9
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto9l.cpe
	if "%1"=="tuto9" goto EXIT
:W_TUTO9
	if exist %TMPFILE% goto TUTO10
		delay %WAIT%
		goto W_TUTO9
rem ------------------------------------------------
:TUTO10
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto10l.cpe
	if "%1"=="tuto10" goto EXIT
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
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_TUTO11
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
