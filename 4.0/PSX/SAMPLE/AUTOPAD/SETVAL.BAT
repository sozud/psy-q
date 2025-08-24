@echo off
rem ----------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル --環境変数の設定--
rem
rem	-- 各変数の説明 --
rem
rem	SAMPDIR	sample の path            (例: C:\psx\sample)
rem	WAIT	サンプルが終了したかどうかチェックに行く間隔。
rem		delay に渡す引数          (例: 180)
rem	LOGFILE	結果出力用ログファイル名 （例: C:\psx\sample\report.log）
rem	PATCHX	patchx.cpe の path       （例: C:\psx\bin\patchx.cpe）
rem	NOPRINT	noprint.cpe の path      （例: c:\psx\bin\noprint.cpe）
rem	TMPFILE	サンプル終了チェックのためのテンポラリファイル名
rem	                                 （例: C:\tmp\apdtmp.txt）
rem	DELAY1	resetps の後の wait time （例: 9）
rem	DELAY2	patchx  の後の wait time （例: 9）
rem	DELAY3	noprint の後の wait time （例: 9）
rem	DELAY4	psymake load の後の wait time （例: 9）
rem
rem
rem	-- ディレクトリ構成 --
rem
rem	%HOME%/autopad/sample.bat（このファイル）
rem	              /loadcd.bat
rem	              /loadgra.bat
rem	                    :
rem	      /cd
rem	      /graphics
rem	 	   :
rem
rem
rem ----------------------------------------------------------------------
set SAMPDIR=c:\psx\sample
set LOGFILE=c:\psx\sample\report.log
set PATCHX=c:\psx\bin\patchw.cpe
set NOPRINT=c:\psx\bin\noprint.cpe
set TMPFILE=c:\tmp\apdtmp.txt
set WAIT=180
set DELAY1=300
set DELAY2=10
set DELAY3=10
set DELAY4=10
set RESET=%SAMPDIR%\autopad\reset.bat
set ECHORUN=%SAMPDIR%\autopad\echorun.bat
set CHGDIR=%SAMPDIR%\autopad\chgdir.bat

