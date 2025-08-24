@echo off
rem ------------------------------------------------
rem
rem	makefile (graphics)
rem
rem ------------------------------------------------

set CDIR=%SAMPDIR%\graphics
rem ------------------------------------------------
rem	2d
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\2d
rem ------------------------------------------------
rem	balls
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\balls
rem ------------------------------------------------
rem	bg
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\bg\bgsample
	call %ECHOMAKE% %CDIR%\bg\fix32
rem ------------------------------------------------
rem	clutfog
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\clutfog
rem ------------------------------------------------
rem	diffuse
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\diffuse
rem ------------------------------------------------
rem	divide
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\divide\active
	call %ECHOMAKE% %CDIR%\divide\clip
rem ------------------------------------------------
rem	dmpsx
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\dmpsx
rem ------------------------------------------------
rem	fballs
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\fballs
rem ------------------------------------------------
rem	gsgpu
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\gsgpu
rem ------------------------------------------------
rem	hmd
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\hmd\anim
	call %ECHOMAKE% %CDIR%\hmd\basic
	call %ECHOMAKE% %CDIR%\hmd\mime
rem ------------------------------------------------
rem	cman
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\jimen
rem ------------------------------------------------
rem	mat
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\mat
rem ------------------------------------------------
rem	mesh
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\mesh\qmesh
	call %ECHOMAKE% %CDIR%\mesh\rmesh
	call %ECHOMAKE% %CDIR%\mesh\smesh
rem ------------------------------------------------
rem	mime
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\mime\joint
	call %ECHOMAKE% %CDIR%\mime\vertex
	call %ECHOMAKE% %CDIR%\mime\vjmime
rem ------------------------------------------------
rem	mipmap
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\mipmap
rem ------------------------------------------------
rem	misc
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\misc\60frame
	call %ECHOMAKE% %CDIR%\misc\getode
rem ------------------------------------------------
rem	oden
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\oden
rem ------------------------------------------------
rem	phong
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\phong
rem ------------------------------------------------
rem	pmd
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\pmd
rem ------------------------------------------------
rem	ppm
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\ppm
rem ------------------------------------------------
rem	rgb24
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\rgb24
rem ------------------------------------------------
rem	rotate
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\rotate\arot
	call %ECHOMAKE% %CDIR%\rotate\intrpol
	call %ECHOMAKE% %CDIR%\rotate\mat2rot
rem ------------------------------------------------
rem	shadow
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\shadow
rem ------------------------------------------------
rem	tmd
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\tmd
rem ------------------------------------------------
rem	tmdview
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\tmdview\lowlevel
	call %ECHOMAKE% %CDIR%\tmdview\rcube
	call %ECHOMAKE% %CDIR%\tmdview\shuttle
	call %ECHOMAKE% %CDIR%\tmdview\tmdview3
	call %ECHOMAKE% %CDIR%\tmdview\tmdview4
	call %ECHOMAKE% %CDIR%\tmdview\tmdview5
rem ------------------------------------------------
rem	tod
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\tod\janken
	call %ECHOMAKE% %CDIR%\tod\todview
rem ------------------------------------------------
rem	trr
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\trr
rem ------------------------------------------------
rem	tuto
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\tuto
rem ------------------------------------------------
rem	walk
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\walk
rem ------------------------------------------------
rem	zimen
rem ------------------------------------------------
	call %ECHOMAKE% %CDIR%\zimen
rem ------------------------------------------------
rem	end
rem ------------------------------------------------
:END
	cd %SAMPDIR%\autopad
