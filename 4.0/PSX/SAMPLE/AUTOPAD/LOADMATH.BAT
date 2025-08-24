@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（math）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\math

rem ------------------------------------------------
rem	tree
rem ------------------------------------------------
:TREE
	call %CHGDIR% %CDIR%\tree
	call %RESET%
	call %ECHORUN% treel.cpe
:W_TREE
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_TREE
rem ------------------------------------------------
rem	end
rem ------------------------------------------------
:END
	resetps
	cd %SAMPDIR%\autopad
