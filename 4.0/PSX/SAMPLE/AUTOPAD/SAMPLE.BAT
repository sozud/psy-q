@echo off
rem ----------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル
rem
rem	setval.bat を編集して、環境変数の設定を行ってください。
rem
rem
rem ----------------------------------------------------------------------
set SAMPDIR=\psx\sample

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
call %SAMPDIR%\autopad\setval.bat

rem ------------------------------------------------
rem	引数のチェック
rem ------------------------------------------------
if "%1"=="" goto USAGE
if "%2"=="" goto KIND
if not "%2"=="/d" goto USAGE

rem ------------------------------------------------
rem	結果出力ファイルが既にある場合は消去する
rem ------------------------------------------------
if exist %LOGFILE% del %LOGFILE%

rem ------------------------------------------------
rem	実行するサンプルの種類
rem ------------------------------------------------
:KIND
if "%1"=="all"      goto START
if "%1"=="cd"       goto CD
if "%1"=="cmplr"    goto CMPLR
if "%1"=="ds"       goto DS
if "%1"=="etc"      goto ETC
if "%1"=="graphics" goto GRAPHICS
if "%1"=="math"     goto MATH
if "%1"=="module"   goto MODULE
if "%1"=="press"    goto PRESS
if "%1"=="sound"    goto SOUND
goto USAGE

:START
rem ------------------------------------------------
rem	cd
rem ------------------------------------------------
:CD
	call %SAMPDIR%\autopad\loadcd.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	cmplr
rem ------------------------------------------------
:CMPLR
	call %SAMPDIR%\autopad\loadcmp.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	ds
rem ------------------------------------------------
:DS
	call %SAMPDIR%\autopad\loadds.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	etc
rem ------------------------------------------------
:ETC
	call %SAMPDIR%\autopad\loadetc.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	graphics
rem ------------------------------------------------
:GRAPHICS
	call %SAMPDIR%\autopad\loadgra.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	math
rem ------------------------------------------------
:MATH
	call %SAMPDIR%\autopad\loadmath.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	module
rem ------------------------------------------------
:MODULE
	call %SAMPDIR%\autopad\loadmod.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	press
rem ------------------------------------------------
:PRESS
	call %SAMPDIR%\autopad\loadpres.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	sound
rem ------------------------------------------------
:SOUND
	call %SAMPDIR%\autopad\loadsnd.bat
	if not "%1"=="all" goto END

goto END
rem ------------------------------------------------
rem	USAGE
rem ------------------------------------------------
:USAGE
echo Usage: sample.bat [サンプルの種類] [/d]
echo        サンプルの種類: （一つだけ選択して指定する）
echo               all ... 全てを実行
echo               cd,cmplr,ds,etc,graphics,
echo               math,press,sound      ... それぞれのディレクトリのみ実行
echo        /d: 既存のログファイルを消去する
echo ＊注意＊ 引数は全て小文字で指定してください
rem ------------------------------------------------
rem	END
rem ------------------------------------------------
:END

