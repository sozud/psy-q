@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（sound）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\sound

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	balls
rem ------------------------------------------------
:BALLS
	call %CHGDIR% %CDIR%\balls
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% ballsl.cpe
	if not "%1"=="" goto EXIT
:W_BALLS
	if exist %TMPFILE% goto BASIC
		delay %WAIT%
		goto W_BALLS
rem ------------------------------------------------
rem	basic
rem ------------------------------------------------
:BASIC
	call %CHGDIR% %CDIR%\basic
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% basicl.cpe
	if not "%1"=="" goto EXIT
:W_BASIC
	if exist %TMPFILE% goto CDVOL
		delay %WAIT%
		goto W_BASIC
rem ------------------------------------------------
rem	cdvol
rem ------------------------------------------------
:CDVOL
	call %CHGDIR% %CDIR%\cdvol
	call %RESET%
	call %ECHORUN% cdvoll.cpe
	if not "%1"=="" goto EXIT
:W_CDVOL
	if exist %TMPFILE% goto MUTUAL
		delay %WAIT%
		goto W_CDVOL
rem ------------------------------------------------
rem	mutual
rem ------------------------------------------------
:MUTUAL
	call %CHGDIR% %CDIR%\mutual
	call %RESET%
	call %ECHORUN% mutuall.cpe
	if not "%1"=="" goto EXIT
:W_MUTUAL
	if exist %TMPFILE% goto SIMPLE
		delay %WAIT%
		goto W_MUTUAL
rem ------------------------------------------------
rem	simple
rem ------------------------------------------------
:SIMPLE
	call %CHGDIR% %CDIR%\simple
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% simplel.cpe
	if not "%1"=="" goto EXIT
:W_SIMPLE
	if exist %TMPFILE% goto JUMP
		delay %WAIT%
		goto W_SIMPLE
rem ------------------------------------------------
:JUMP
	call %CHGDIR% %CDIR%\simple
	call %RESET%
	psymake load
		delay %DELAAY4%
	call %ECHORUN% jumpl.cpe
	if not "%1"=="" goto EXIT
:W_JUMP
	if exist %TMPFILE% goto STREAM
		delay %WAIT%
		goto W_JUMP
rem ------------------------------------------------
rem	stream
rem ------------------------------------------------
:STREAM
:STREAM1
	call %CHGDIR% %CDIR%\stream
	call %RESET%
	psymake load8
		delay %DELAY4%
	call %ECHORUN% stream1l.cpe
	if "%1"=="stream1" goto EXIT
:W_STRM1
	if exist %TMPFILE% goto STREAM2
		delay %WAIT%
		goto W_STRM1
rem ------------------------------------------------
:STREAM2
	call %CHGDIR% %CDIR%\stream
	call %RESET%
	psymake loadapd
		delay %DELAY4%
	call %ECHORUN% stream2l.cpe
	if not "%1"=="" goto EXIT
:W_STRM2
	if exist %TMPFILE% goto TUTO
		delay %WAIT%
		goto W_STRM2
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
	if not "%1"=="" goto EXIT
:W_TUTO6
	if exist %TMPFILE% goto XSE
		delay %WAIT%
		goto W_TUTO6
rem ------------------------------------------------
rem	xse
rem ------------------------------------------------
:XSE
	call %CHGDIR% %CDIR%\xse
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% xsel.cpe
	if not "%1"=="" goto EXIT
:W_XSE
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_XSE
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
