@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（cmplr）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\cmplr

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	scratch
rem ------------------------------------------------
:SCRATCH
	call %CHGDIR% %CDIR%\scratch
	call %RESET%
	call %ECHORUN% scratchl.cpe
:W_SCRATCH
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_SCRATCH
rem ------------------------------------------------
rem	end
rem ------------------------------------------------
:END
	resetps
	cd %SAMPDIR%\autopad
