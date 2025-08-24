@echo off
rem ----------------------------------------------------------------------
rem
rem	サンプル自動実行のための makefile
rem
rem ----------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
set SAMPDIR=\psx\sample
set CHGDIR=%SAMPDIR%\autopad\chgdir.bat
set ECHOMAKE=%SAMPDIR%\autopad\echomake.bat
cd %SAMPDIR%\autopad

rem ------------------------------------------------
rem	引数のチェック
rem ------------------------------------------------
if "%1"=="autopad"  goto AUTOPAD
rem ------------------------------------------------
rem	make するサンプルの種類
rem ------------------------------------------------
if "%2"=="" goto USAGE
set APDKIND=""
if "%2"=="save" set APDKIND=apd_save
if "%2"=="load" set APDKIND=apd_load
if "%APDKIND%"=="" goto USAGE

rem ------------------------------------------------
rem	make するサンプルのディレクトリ
rem ------------------------------------------------
if "%1"=="" goto USAGE
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
rem ------------------------------------------------
rem	start
rem ------------------------------------------------
:START

rem ------------------------------------------------
rem	autopad.obj
rem ------------------------------------------------
:AUTOPAD
	psymake
	if not "%1"=="all" goto END

rem ------------------------------------------------
rem	cd
rem ------------------------------------------------
:CD
	call %SAMPDIR%\autopad\makecd.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	cmplr
rem ------------------------------------------------
:CMPLR
	call %SAMPDIR%\autopad\makecmp.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	ds
rem ------------------------------------------------
:DS
	call %SAMPDIR%\autopad\makeds.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	etc
rem ------------------------------------------------
:ETC
	call %SAMPDIR%\autopad\makeetc.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	graphics
rem ------------------------------------------------
:GRAPHICS
	call %SAMPDIR%\autopad\makegra.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	math
rem ------------------------------------------------
:MATH
	call %SAMPDIR%\autopad\makemath.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	moduleh
rem ------------------------------------------------
:MODULE
	call %SAMPDIR%\autopad\makemod.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	press
rem ------------------------------------------------
:PRESS
	call %SAMPDIR%\autopad\makepres.bat
	if not "%1"=="all" goto END
rem ------------------------------------------------
rem	sound
rem ------------------------------------------------
:SOUND
	call %SAMPDIR%\autopad\makesnd.bat
	if not "%1"=="all" goto END

goto END
rem ------------------------------------------------
rem	USAGE
rem ------------------------------------------------
:USAGE
echo Usage: apdmake.bat [サンプルの種類] [save or load]
echo        サンプルの種類: （一つだけ選択して指定する）
echo               all ... 全てを make
echo               autopad ... autopad.obj を make
echo               cd,cmplr,ds,etc,graphics,
echo               math,press,sound      ... それぞれのディレクトリのみ make
echo        save or load: save ... パッドファイルセーブ用サンプルの make
echo                      load ... 自動実行用サンプルの make
echo ＊注意＊ 引数は全て小文字で指定してください
rem ------------------------------------------------
rem	END
rem ------------------------------------------------
:END

