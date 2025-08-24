@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（etc）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\etc

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	message
rem ------------------------------------------------
:MES1
	call %CHGDIR% %CDIR%\card
	call %RESET%
	call %ECHORUN% apdmsg.cpe
:W_MSG1
	if exist %TMPFILE% goto CARD
		delay %WAIT%
		goto W_MSG1
rem ------------------------------------------------
rem	card
rem ------------------------------------------------
:CARD
	call %CHGDIR% %CDIR%\card
	call %RESET%
	call %ECHORUN% cardl.cpe
	if not "%1"=="" goto EXIT
:W_CARD
	if exist %TMPFILE% goto MCRD
		delay %WAIT%
		goto W_CARD
rem ------------------------------------------------
rem	mcrd
rem ------------------------------------------------
:MCRD
	call %CHGDIR% %CDIR%\mcrd
:MCRD1
	call %CHGDIR% %CDIR%\mcrd\tuto1
	call %RESET%
	call %ECHORUN% mcrd1l.cpe
	if "%1"=="mcrd1" goto EXIT
:W_MCRD1
	if exist %TMPFILE% goto MCRD2
		delay %WAIT%
		goto W_MCRD1
rem ------------------------------------------------
:MCRD2
	call %CHGDIR% %CDIR%\mcrd\tuto2
	call %RESET%
	call %ECHORUN% mcrd2l.cpe
	if not "%1"=="" goto EXIT
:W_MCRD2
	if exist %TMPFILE% goto MOUSE
		delay %WAIT%
		goto W_MCRD2
rem ------------------------------------------------
rem	mouse
rem ------------------------------------------------
:MOUSE
	goto THREAD
	call %CHGDIR% %CDIR%\mouse
	call %RESET%
	call %ECHORUN% mouse.cpe
	if not "%1"=="" goto EXIT
:W_MOUSE
	if exist %TMPFILE% goto THREAD
		delay %WAIT%
		goto W_MOUSE
rem ------------------------------------------------
rem	thread
rem ------------------------------------------------
:THREAD
	call %CHGDIR% %CDIR%\thread
	call %RESET%
	call %ECHORUN% threadl.cpe
	if not "%1"=="" goto EXIT
:W_THREAD
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_THREAD
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
