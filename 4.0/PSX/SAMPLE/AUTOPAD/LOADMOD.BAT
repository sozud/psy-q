@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（MODULE)
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\module

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	execmenu
rem ------------------------------------------------
:EXECMEN
	call %CHGDIR% %CDIR%\execmenu
	call %RESET%
	call %ECHORUN% execmenl.cpe
	if not "%1"=="" goto EXIT
:W_EXECMEN
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_EXECMEN
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
