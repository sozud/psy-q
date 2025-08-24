@echo off
rem -------------------------------------------------------------------------
rem
rem	サンプル自動実行バッチファイル（graphics）
rem
rem -------------------------------------------------------------------------

rem ------------------------------------------------
rem	環境変数の設定
rem ------------------------------------------------
rem if "%SAMPDIR%"=="" call \psx\sample\autopad\setval.bat
call \psx\sample\autopad\setval.bat
set CDIR=%SAMPDIR%\graphics

if not "%1"=="" goto %1
rem ------------------------------------------------
rem	2d\2d
rem ------------------------------------------------
:2D
	call %CHGDIR% %CDIR%\2d
	call %RESET%
	call %ECHORUN% 2dl.cpe
	if not "%1"=="" goto EXIT
:W_2D
	if exist %TMPFILE% goto BALLS
		delay %WAIT%
		goto W_2D
rem ------------------------------------------------
rem	balls\balls
rem ------------------------------------------------
:BALLS
	call %CHGDIR% %CDIR%\balls
	call %RESET%
	call %ECHORUN% ballsl.cpe
	if not "%1"=="" goto EXIT
:W_BALLS
	if exist %TMPFILE% goto BGSAMPLE
		delay %WAIT%
		goto W_BALLS
rem ------------------------------------------------
rem	bg
rem ------------------------------------------------
:BGSAMPLE
	call %CHGDIR% %CDIR%\bg\bgsample
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% bgsampll.cpe
	if not "%1"=="" goto EXIT
:W_BGSAMPLE
	if exist %TMPFILE% goto FIX32
		delay %WAIT%
		goto W_BGSAMPLE
rem ------------------------------------------------
:FIX32
	call %CHGDIR% %CDIR%\bg\fix32
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% fix32l.cpe
	if not "%1"=="" goto EXIT
:W_FIX32
	if exist %TMPFILE% goto CLUTFOG
		delay %WAIT%
		goto W_FIX32
rem ------------------------------------------------
rem	clutfog
rem ------------------------------------------------
:CLUTFOG
rem ------------------------------------------------
:CLUT0
	call %CHGDIR% %CDIR%\clutfog
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% clut0l.cpe
	if "%1"=="clut0" goto EXIT
:W_CLUT0
	if exist %TMPFILE% goto CLUT1
		delay %WAIT%
		goto W_CLUT0
rem ------------------------------------------------
:CLUT1
	call %CHGDIR% %CDIR%\clutfog
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% clut1l.cpe
	if "%1"=="clut1" goto EXIT
:W_CLUT1
	if exist %TMPFILE% goto CLUT2
		delay %WAIT%
		goto W_CLUT1
rem ------------------------------------------------
:CLUT2
	call %CHGDIR% %CDIR%\clutfog
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% clut2l.cpe
	if "%1"=="clut2" goto EXIT
:W_CLUT2
	if exist %TMPFILE% goto CLUT3
		delay %WAIT%
		goto W_CLUT2
rem ------------------------------------------------
:CLUT3
	call %CHGDIR% %CDIR%\clutfog
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% clut3l.cpe
	if not "%1"=="" goto EXIT
:W_CLUT3
	if exist %TMPFILE% goto DIFFUSE
		delay %WAIT%
		goto W_CLUT3
rem ------------------------------------------------
rem	diffuse
rem ------------------------------------------------
:DIFFUSE
	call %CHGDIR% %CDIR%\diffuse
	call %RESET%
	call %ECHORUN% diffusel.cpe
	if not "%1"=="" goto EXIT
:W_DIFFUSE
	if exist %TMPFILE% goto DIVIDE
		delay %WAIT%
		goto W_DIFFUSE
rem ------------------------------------------------
rem	divide
rem ------------------------------------------------
:DIVIDE
:ACTIVE
	call %CHGDIR% %CDIR%\divide\active
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% activel.cpe
	if "%1"=="active" goto EXIT
:W_ACTIVE
	if exist %TMPFILE% goto CLIP
		delay %WAIT%
		goto W_ACTIVE
rem ------------------------------------------------
:CLIP
	call %CHGDIR% %CDIR%\divide\clip
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% clipl.cpe
	if not "%1"=="" goto EXIT
:W_CLIP
	if exist %TMPFILE% goto DMPSX
		delay %WAIT%
		goto W_CLIP
rem ------------------------------------------------
rem	dmpsx
rem ------------------------------------------------
:DMPSX
	call %CHGDIR% %CDIR%\dmpsx
	call %RESET%
	call %ECHORUN% dmpsxl.cpe
	if not "%1"=="" goto EXIT
:W_DMPSX
	if exist %TMPFILE% goto DMPSXD
		delay %WAIT%
		goto W_DMPSX
rem ------------------------------------------------
:DMPSXD
	call %CHGDIR% %CDIR%\dmpsx
	call %RESET%
	call %ECHORUN% dmpsxdl.cpe
	if not "%1"=="" goto EXIT
:W_DMPSXD
	if exist %TMPFILE% goto FBALLS
		delay %WAIT%
		goto W_DMPSXD
rem ------------------------------------------------
rem	fballs
rem ------------------------------------------------
:FBALLS
	call %CHGDIR% %CDIR%\fballs
	call %RESET%
	call %ECHORUN% fballsl.cpe
	if not "%1"=="" goto EXIT
:W_FBALLS
	if exist %TMPFILE% goto GSGPU
		delay %WAIT%
		goto W_FBALLS
rem ------------------------------------------------
rem	gsgpu
rem ------------------------------------------------
:GSGPU
:GSGPU0
	call %CHGDIR% %CDIR%\gsgpu
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% gsgpu0l.cpe
	if "%1"=="gsgpu0" goto EXIT
:W_GSGPU0
	if exist %TMPFILE% goto GSGPU1
		delay %WAIT%
		goto W_GSGPU0
rem ------------------------------------------------
:GSGPU1
	call %CHGDIR% %CDIR%\gsgpu
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% gsgpu1l.cpe
	if not "%1"=="" goto EXIT
:W_GSGPU1
	if exist %TMPFILE% goto HMD
		delay %WAIT%
		goto W_GSGPU1
rem ------------------------------------------------
rem	hmd
rem ------------------------------------------------
:HMD
	call %CHGDIR% %CDIR%\hmd
:HMDAN0
	call %CHGDIR% %CDIR%\hmd\anim
	call %RESET%
	psymake load0
		delay %DELAY4%
	call %ECHORUN% hmdan0l.cpe
	if "%1"=="hmdan0" goto EXIT
:W_HMDAN0
	if exist %TMPFILE% goto HMDAN1
		delay %WAIT%
		goto W_HMDAN0
rem ------------------------------------------------
:HMDAN1
	call %CHGDIR% %CDIR%\hmd\anim
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% hmdan1l.cpe
	if "%1"=="hmdan1" goto EXIT
:W_HMDAN1
	if exist %TMPFILE% goto HMDAN2
		delay %WAIT%
		goto W_HMDAN1
rem ------------------------------------------------
:HMDAN2
	call %CHGDIR% %CDIR%\hmd\anim
	call %RESET%
	psymake load2
		delay %DELAY4%
	call %ECHORUN% hmdan2l.cpe
	if "%1"=="hmdan2" goto EXIT
:W_HMDAN2
	if exist %TMPFILE% goto HMDAN3
		delay %WAIT%
		goto W_HMDAN2
rem ------------------------------------------------
:HMDAN3
	call %CHGDIR% %CDIR%\hmd\anim
	call %RESET%
	psymake load3
		delay %DELAY4%
	call %ECHORUN% hmdan3l.cpe
	if "%1"=="hmdan3" goto EXIT
	if "%1"=="hmdan"  goto EXIT
:W_HMDAN3
	if exist %TMPFILE% goto BASIC
		delay %WAIT%
		goto W_HMDAN3
rem ------------------------------------------------
:BASIC
:BASIC0
	call %CHGDIR% %CDIR%\hmd\basic
	call %RESET%
	psymake load0
		delay %DELAY4%
	call %ECHORUN% basic0l.cpe
	if "%1"=="basic0" goto EXIT
:W_BASIC0
	if exist %TMPFILE% goto BASIC1
		delay %WAIT%
		goto W_BASIC0
rem ------------------------------------------------
:BASIC1
	call %CHGDIR% %CDIR%\hmd\basic
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% basic1l.cpe
	if "%1"=="basic1" goto EXIT
:W_BASIC1
	if exist %TMPFILE% goto BASIC2
		delay %WAIT%
		goto W_BASIC1
rem ------------------------------------------------
:BASIC2
	call %CHGDIR% %CDIR%\hmd\basic
	call %RESET%
	psymake load2
		delay %DELAY4%
	call %ECHORUN% basic2l.cpe
	if "%1"=="basic2" goto EXIT
:W_BASIC2
	if exist %TMPFILE% goto BASIC3
		delay %WAIT%
		goto W_BASIC2
rem ------------------------------------------------
:BASIC3
	call %CHGDIR% %CDIR%\hmd\basic
	call %RESET%
	psymake load3
		delay %DELAY4%
	call %ECHORUN% basic3l.cpe
	if "%1"=="basic3" goto EXIT
	if "%1"=="basic"  goto EXIT
:W_BASIC3
	if exist %TMPFILE% goto HMIME
		delay %WAIT%
		goto W_BASIC3
rem ------------------------------------------------
:HMIME
	call %CHGDIR% %CDIR%\hmd\mime
	call %RESET%
	psymake load0
		delay %DELAY4%
	call %ECHORUN% hmimel.cpe
	if not "%1"=="" goto EXIT
:W_HMIME
	if exist %TMPFILE% goto JIMEN
		delay %WAIT%
		goto W_HMIME
rem ------------------------------------------------
rem	jimen
rem ------------------------------------------------
:JIMEN
	call %CHGDIR% %CDIR%\jimen
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% jimenl.cpe
	if not "%1"=="" goto EXIT
:W_JIMEN
	if exist %TMPFILE% goto MAT
		delay %WAIT%
		goto W_JIMEN
rem ------------------------------------------------
rem	mat
rem ------------------------------------------------
:MAT
	call %CHGDIR% %CDIR%\mat
	call %RESET%
	call %ECHORUN% matl.cpe
	if not "%1"=="" goto EXIT
:W_MAT
	if exist %TMPFILE% goto MESH
		delay %WAIT%
		goto W_MAT
rem ------------------------------------------------
rem	mesh
rem ------------------------------------------------
:MESH
	call %CHGDIR% %CDIR%\mesh
:QMESH0
	call %CHGDIR% %CDIR%\mesh\qmesh
	call %RESET%
	call %ECHORUN% qmesh0l.cpe
	if not "%1"=="" goto EXIT
:W_QMESH0
	if exist %TMPFILE% goto QMESH1
		delay %WAIT%
		goto W_QMESH0
rem ------------------------------------------------
:QMESH1
	call %CHGDIR% %CDIR%\mesh\qmesh
	call %RESET%
	call %ECHORUN% qmesh1l.cpe
	if not "%1"=="" goto EXIT
:W_QMESH1
	if exist %TMPFILE% goto RMESH
		delay %WAIT%
		goto W_QMESH1
rem ------------------------------------------------
:RMESH
	call %CHGDIR% %CDIR%\mesh\rmesh
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% rmeshl.cpe
	if not "%1"=="" goto EXIT
:W_RMESH
	if exist %TMPFILE% goto SMESH0
		delay %WAIT%
		goto W_RMESH
rem ------------------------------------------------
:SMESH0
	call %CHGDIR% %CDIR%\mesh\smesh
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% smesh0l.cpe
	if not "%1"=="" goto EXIT
:W_SMESH0
	if exist %TMPFILE% goto SMESH1
		delay %WAIT%
		goto W_SMESH0
rem ------------------------------------------------
:SMESH1
	call %CHGDIR% %CDIR%\mesh\smesh
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% smesh1l.cpe
	if not "%1"=="" goto EXIT
:W_SMESH1
	if exist %TMPFILE% goto SMESH2
		delay %WAIT%
		goto W_SMESH1
rem ------------------------------------------------
:SMESH2
	call %CHGDIR% %CDIR%\mesh\smesh
	call %RESET%
	psymake load2
		delay %DELAY4%
	call %ECHORUN% smesh2l.cpe
	if not "%1"=="" goto EXIT
:W_SMESH2
	if exist %TMPFILE% goto MIME
		delay %WAIT%
		goto W_SMESH2
rem ------------------------------------------------
rem	mime
rem ------------------------------------------------
:MIME
:JMIME
	call %CHGDIR% %CDIR%\mime\joint
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% jmimel.cpe
	if not "%1"=="" goto EXIT
:W_JMIME
	if exist %TMPFILE% goto VJMIM0
		delay %WAIT%
		goto W_JMIME
rem ------------------------------------------------
:VJMIM0
	call %CHGDIR% %CDIR%\mime\vjmime
	call %RESET%
	psymake load0
		delay %DELAY4%
	call %ECHORUN% vjmime0l.cpe
	if not "%1"=="" goto EXIT
:W_VJMIM0
	if exist %TMPFILE% goto VJMIM1
		delay %WAIT%
		goto W_VJMIM0
rem ------------------------------------------------
:VJMIM1
	call %CHGDIR% %CDIR%\mime\vjmime
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% vjmime1l.cpe
	if not "%1"=="" goto EXIT
:W_VJMIM1
	if exist %TMPFILE% goto VMIME
		delay %WAIT%
		goto W_VJMIM1
rem ------------------------------------------------
:VMIME
	call %CHGDIR% %CDIR%\mime\vertex
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% vmimel.cpe
	if not "%1"=="" goto EXIT
:W_VMIME
	if exist %TMPFILE% goto MIPMAP
		delay %WAIT%
		goto W_VMIME
rem ------------------------------------------------
rem	mipmap
rem ------------------------------------------------
:MIPMAP
	call %CHGDIR% %CDIR%\mipmap
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% mipmapl.cpe
	if not "%1"=="" goto EXIT
:W_MIPMAP
	if exist %TMPFILE% goto MISC
		delay %WAIT%
		goto W_MIPMAP
rem ------------------------------------------------
rem	misc
rem ------------------------------------------------
:MISC
:60FRAME
	call %CHGDIR% %CDIR%\misc\60frame
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% 60framel.cpe
	if not "%1"=="" goto EXIT
:W_60FRAME
	if exist %TMPFILE% goto GETODE
		delay %WAIT%
		goto W_60FRAME
rem ------------------------------------------------
:GETODE
	call %CHGDIR% %CDIR%\misc\getode
	call %RESET%
	call %ECHORUN% getodel.cpe
	if not "%1"=="" goto EXIT
:W_GETODE
	if exist %TMPFILE% goto ODEN
		delay %WAIT%
		goto W_GETODE
rem ------------------------------------------------
rem	oden
rem ------------------------------------------------
:ODEN
	call %CHGDIR% %CDIR%\oden
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% odenl.cpe
	if not "%1"=="" goto EXIT
:W_ODEN
	if exist %TMPFILE% goto PHONG
		delay %WAIT%
		goto W_ODEN
rem ------------------------------------------------
rem	phong
rem ------------------------------------------------
:PHONG
	call %CHGDIR% %CDIR%\phong
	call %RESET%
	call %ECHORUN% phongl.cpe
	if not "%1"=="" goto EXIT
:W_PHONG
	if exist %TMPFILE% goto PMD
		delay %WAIT%
		goto W_PHONG
rem ------------------------------------------------
rem	pmd
rem ------------------------------------------------
:PMD
:PMD0
	call %CHGDIR% %CDIR%\pmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% pmd0l.cpe
	if "%1"=="pmd0" goto EXIT
:W_PMD0
	if exist %TMPFILE% goto PMD1
		delay %WAIT%
		goto W_PMD0
rem ------------------------------------------------
:PMD1
	call %CHGDIR% %CDIR%\pmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% pmd1l.cpe
	if "%1"=="pmd1" goto EXIT
:W_PMD1
	if exist %TMPFILE% goto PMD2
		delay %WAIT%
		goto W_PMD1
rem ------------------------------------------------
:PMD2
	call %CHGDIR% %CDIR%\pmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% pmd2l.cpe
	if "%1"=="pmd2" goto EXIT
:W_PMD2
	if exist %TMPFILE% goto PMD3
		delay %WAIT%
		goto W_PMD2
rem ------------------------------------------------
:PMD3
	call %CHGDIR% %CDIR%\pmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% pmd3l.cpe
	if "%1"=="pmd3" goto EXIT
:W_PMD3
	if exist %TMPFILE% goto PMD5
		delay %WAIT%
		goto W_PMD3
rem ------------------------------------------------
:PMD5
	call %CHGDIR% %CDIR%\pmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% pmd5l.cpe
	if not "%1"=="" goto EXIT
:W_PMD5
	if exist %TMPFILE% goto PPM
		delay %WAIT%
		goto W_PMD5
rem ------------------------------------------------
rem	ppm
rem ------------------------------------------------
:PPM
	call %CHGDIR% %CDIR%\ppm
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% ppml.cpe
	if not "%1"=="" goto EXIT
:W_PPM
	if exist %TMPFILE% goto RGB24
		delay %WAIT%
		goto W_PPM
rem ------------------------------------------------
rem	rgb24
rem ------------------------------------------------
:RGB24
	call %CHGDIR% %CDIR%\rgb24
	call %RESET%
	call %ECHORUN% rgb24l.cpe
	if not "%1"=="" goto EXIT
:W_RGB24
	if exist %TMPFILE% goto ROTATE
		delay %WAIT%
		goto W_RGB24
rem ------------------------------------------------
rem	rotate
rem ------------------------------------------------
:ROTATE
:AROT
	call %CHGDIR% %CDIR%\rotate\arot
	call %RESET%
	call %ECHORUN% arotl.cpe
	if "%1"=="arot" goto EXIT
:W_AROT
	if exist %TMPFILE% goto INTR
		delay %WAIT%
		goto W_AROT
rem ------------------------------------------------
:INTR
	call %CHGDIR% %CDIR%\rotate\intrpol
	call %RESET%
	call %ECHORUN% intrpoll.cpe
	if "%1"=="intr" goto EXIT
:W_INTR
	if exist %TMPFILE% goto MAT2ROT
		delay %WAIT%
		goto W_INTR
rem ------------------------------------------------
:MAT2ROT
	call %CHGDIR% %CDIR%\rotate\mat2rot
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% mat2rotl.cpe
	if not "%1"=="" goto EXIT
:W_MAT2ROT
	if exist %TMPFILE% goto SHADOW
		delay %WAIT%
		goto W_MAT2ROT
rem ------------------------------------------------
rem	shadow
rem ------------------------------------------------
:SHADOW
	call %CHGDIR% %CDIR%\shadow
	call %RESET%
	call %ECHORUN% shadowl.cpe
	if not "%1"=="" goto EXIT
:W_SHADOW
	if exist %TMPFILE% goto TMD
		delay %WAIT%
		goto W_SHADOW
rem ------------------------------------------------
rem	tmd
rem ------------------------------------------------
:TMD
:TMD0
	call %CHGDIR% %CDIR%\tmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmd0l.cpe
	if "%1"=="tmp0" goto EXIT
:W_TMD0
	if exist %TMPFILE% goto TMD1
		delay %WAIT%
		goto W_TMD0
rem ------------------------------------------------
:TMD1
	call %CHGDIR% %CDIR%\tmd
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmd1l.cpe
	if not "%1"=="" goto EXIT
:W_TMD1
	if exist %TMPFILE% goto TMDVIEW
		delay %WAIT%
		goto W_TMD1
rem ------------------------------------------------
rem	tmdview/lowlevel
rem ------------------------------------------------
:TMDVIEW
:LOWLEVEL
:LOW0
	call %CHGDIR% %CDIR%\tmdview\lowlevel
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% low0l.cpe
	if "%1"=="low0" goto EXIT
:W_LOW0
	if exist %TMPFILE% goto LOW1
		delay %WAIT%
		goto W_LOW0
rem ------------------------------------------------
:LOW1
	call %CHGDIR% %CDIR%\tmdview\lowlevel
	call %RESET%
	psymake load2
		delay %DELAY4%
	call %ECHORUN% low1l.cpe
	if "%1"=="low1" goto EXIT
:W_LOW1
	if exist %TMPFILE% goto LOW2
		delay %WAIT%
		goto W_LOW1
rem ------------------------------------------------
:LOW2
	call %CHGDIR% %CDIR%\tmdview\lowlevel
	call %RESET%
	psymake load2
		delay %DELAY4%
	call %ECHORUN% low2l.cpe
	if "%1"=="low2" goto EXIT
:W_LOW2
	if exist %TMPFILE% goto LOW3
		delay %WAIT%
		goto W_LOW2
rem ------------------------------------------------
:LOW3
	call %CHGDIR% %CDIR%\tmdview\lowlevel
	call %RESET%
	psymake load3
		delay %DELAY4%
	call %ECHORUN% low3l.cpe
	if not "%1"=="" goto EXIT
:W_LOW3
	if exist %TMPFILE% goto RCUBE
		delay %WAIT%
		goto W_LOW3
rem ------------------------------------------------
rem	tmdview/rcube
rem ------------------------------------------------
:RCUBE
	call %CHGDIR% %CDIR%\tmdview\rcube
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% rcubel.cpe
	if not "%1"=="" goto EXIT
:W_RCUBE
	if exist %TMPFILE% goto SHUTTLE
		delay %WAIT%
		goto W_RCUBE
rem ------------------------------------------------
rem	tmdview/shuttle
rem ------------------------------------------------
:SHUTTLE
	call %CHGDIR% %CDIR%\tmdview\shuttle
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% shuttlel.cpe
	if not "%1"=="" goto EXIT
:W_SHUTTLE
	if exist %TMPFILE% goto TMDV3
		delay %WAIT%
		goto W_SHUTTLE
rem ------------------------------------------------
rem	tmdview/tmdview3
rem ------------------------------------------------
:TMDV3
	call %CHGDIR% %CDIR%\tmdview\tmdview3
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv3l.cpe
	if not "%1"=="" goto EXIT
:W_TMDV3
	if exist %TMPFILE% goto TMDV4
		delay %WAIT%
		goto W_TMDV3
rem ------------------------------------------------
rem	tmdview/tmdview3
rem ------------------------------------------------
:TMDV4
:TMDV40
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv40l.cpe
	if "%1"=="tmdv40" goto EXIT
:W_TMDV40
	if exist %TMPFILE% goto TMDV41
		delay %WAIT%
		goto W_TMDV40
rem ------------------------------------------------
:TMDV41
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% tmdv41l.cpe
	if "%1"=="tmdv41" goto EXIT
:W_TMDV41
	if exist %TMPFILE% goto TMDV42
		delay %WAIT%
		goto W_TMDV41
rem ------------------------------------------------
:TMDV42
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load1
		delay %DELAY4%
	call %ECHORUN% tmdv42l.cpe
	if "%1"=="tmdv42" goto EXIT
:W_TMDV42
	if exist %TMPFILE% goto TMDV43
		delay %WAIT%
		goto W_TMDV42
rem ------------------------------------------------
:TMDV43
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv43l.cpe
	if "%1"=="tmdv43" goto EXIT
:W_TMDV43
	if exist %TMPFILE% goto TMDV44
		delay %WAIT%
		goto W_TMDV43
rem ------------------------------------------------
:TMDV44
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv44l.cpe
	if "%1"=="tmdv44" goto EXIT
:W_TMDV44
	if exist %TMPFILE% goto TMDV45
		delay %WAIT%
		goto W_TMDV44
rem ------------------------------------------------
:TMDV45
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv45l.cpe
	if "%1"=="tmdv45" goto EXIT
:W_TMDV45
	if exist %TMPFILE% goto TMDV46
		delay %WAIT%
		goto W_TMDV45
rem ------------------------------------------------
:TMDV46
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
	psymake load2
		delay %DELAY4%
	call %ECHORUN% tmdv46l.cpe
	if "%1"=="tmdv46" goto EXIT
:W_TMDV46
	if exist %TMPFILE% goto TMDV47
		delay %WAIT%
		goto W_TMDV46
rem ------------------------------------------------
:TMDV47
	call %CHGDIR% %CDIR%\tmdview\tmdview4
	call %RESET%
	psymake load
	psymake load2
		delay %DELAY4%
	call %ECHORUN% tmdv47l.cpe
	if not "%1"=="" goto EXIT
:W_TMDV47
	if exist %TMPFILE% goto TMDVIEW5
		delay %WAIT%
		goto W_TMDV47
rem ------------------------------------------------
rem	tmdview/tmdview5
rem ------------------------------------------------
:TMDVIEW5
:TMDV50
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv50l.cpe
	if "%1"=="tmdv50" goto EXIT
:W_TMDV50
	if exist %TMPFILE% goto TMDV51
		delay %WAIT%
		goto W_TMDV50
rem ------------------------------------------------
:TMDV51
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv51l.cpe
	if "%1"=="tmdv51" goto EXIT
:W_TMDV51
	if exist %TMPFILE% goto TMDV52
		delay %WAIT%
		goto W_TMDV51
rem ------------------------------------------------
:TMDV52
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv52l.cpe
	if "%1"=="tmdv52" goto EXIT
:W_TMDV52
	if exist %TMPFILE% goto TMDV53
		delay %WAIT%
		goto W_TMDV52
rem ------------------------------------------------
:TMDV53
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv53l.cpe
	if "%1"=="tmdv53" goto EXIT
:W_TMDV53
	if exist %TMPFILE% goto TMDV54
		delay %WAIT%
		goto W_TMDV53
rem ------------------------------------------------
:TMDV54
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv54l.cpe
	if "%1"=="tmdv54" goto EXIT
:W_TMDV54
	if exist %TMPFILE% goto TMDV55
		delay %WAIT%
		goto W_TMDV54
rem ------------------------------------------------
:TMDV55
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load5
		delay %DELAY4%
	call %ECHORUN% tmdv55l.cpe
	if "%1"=="tmdv55" goto EXIT
:W_TMDV55
	if exist %TMPFILE% goto TMDV56
		delay %WAIT%
		goto W_TMDV55
rem ------------------------------------------------
:TMDV56
	call %CHGDIR% %CDIR%\tmdview\tmdview5
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% tmdv56l.cpe
	if not "%1"=="" goto EXIT
:W_TMDV56
	if exist %TMPFILE% goto TOD
		delay %WAIT%
		goto W_TMDV56
rem ------------------------------------------------
rem	tod
rem ------------------------------------------------
:TOD
:JANKEN
	call %CHGDIR% %CDIR%\tod\janken
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% jankenl.cpe
	if "%1"=="janken" goto EXIT
:W_JANKEN
	if exist %TMPFILE% goto TODVIEW
		delay %WAIT%
		goto W_JANKEN
rem ------------------------------------------------
:TODVIEW
	call %CHGDIR% %CDIR%\tod\todview
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% todviewl.cpe
	if not "%1"=="" goto EXIT
:W_TODVIEW
	if exist %TMPFILE% goto TRR
		delay %WAIT%
		goto W_TODVIEW
rem ------------------------------------------------
rem	trr
rem ------------------------------------------------
:TRR
	call %CHGDIR% %CDIR%\trr
	call %RESET%
	call %ECHORUN% trrl.cpe
	if not "%1"=="" goto EXIT
:W_TRR
	if exist %TMPFILE% goto TUTO
		delay %WAIT%
		goto W_TRR
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
	if "%1"=="tuto11" goto EXIT
:W_TUTO11
	if exist %TMPFILE% goto TUTO12
		delay %WAIT%
		goto W_TUTO11
rem ------------------------------------------------
:TUTO12
	call %CHGDIR% %CDIR%\tuto
	call %RESET%
	call %ECHORUN% tuto12l.cpe
	if "%1"=="tuto12" goto EXIT
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
	if exist %TMPFILE% goto WALK
		delay %WAIT%
		goto W_TUTO13
rem ------------------------------------------------
rem	walk
rem ------------------------------------------------
:WALK
	call %CHGDIR% %CDIR%\walk
	call %RESET%
	psymake load
		delay %DELAY4%
	call %ECHORUN% walkl.cpe
	if not "%1"=="" goto EXIT
:W_WALK
	if exist %TMPFILE% goto ZIMEN
		delay %WAIT%
		goto W_WALK
rem ------------------------------------------------
rem	zimen
rem ------------------------------------------------
:ZIMEN
:ZIMEN0
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen0l.cpe
	if "%1"=="zimen0" goto EXIT
:W_ZIMEN0
	if exist %TMPFILE% goto ZIMEN1
		delay %WAIT%
		goto W_ZIMEN0
rem ------------------------------------------------
:ZIMEN1
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen1l.cpe
	if "%1"=="zimen1" goto EXIT
:W_ZIMEN1
	if exist %TMPFILE% goto ZIMEN2
		delay %WAIT%
		goto W_ZIMEN1
rem ------------------------------------------------
:ZIMEN2
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen2l.cpe
	if "%1"=="zimen2" goto EXIT
:W_ZIMEN2
	if exist %TMPFILE% goto ZIMEN3
		delay %WAIT%
		goto W_ZIMEN2
rem ------------------------------------------------
:ZIMEN3
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen3l.cpe
	if "%1"=="zimen3" goto EXIT
:W_ZIMEN3
	if exist %TMPFILE% goto ZIMEN4
		delay %WAIT%
		goto W_ZIMEN3
rem ------------------------------------------------
:ZIMEN4
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen4l.cpe
	if "%1"=="zimen4" goto EXIT
:W_ZIMEN4
	if exist %TMPFILE% goto ZIMEN5
		delay %WAIT%
		goto W_ZIMEN4
rem ------------------------------------------------
:ZIMEN5
	call %CHGDIR% %CDIR%\zimen
	call %RESET%
	call %ECHORUN% zimen5l.cpe
	if not "%1"=="" goto EXIT
:W_ZIMEN5
	if exist %TMPFILE% goto END
		delay %WAIT%
		goto W_ZIMEN5
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
