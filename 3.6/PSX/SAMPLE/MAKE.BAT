@echo off

set root=\psx\sample
set file=\psx\sample\make.log
set file2=\psx\sample\make2.log
set ftmp=\psx\sample\make.tmp

type nul > %file%
type nul > %file2%

call make_sub .\CD\EARTH
call make_sub .\CD\MOVIE
call make_sub .\CD\STR3D
call make_sub .\CD\TUTO
call make_sub .\CMPLR\SCRATCH
call make_sub .\ETC\CARD\MAKECARD
call make_sub .\ETC\CARD\MAX
call make_sub .\ETC\CMAN
call make_sub .\ETC\COMB
call make_sub .\ETC\MOUSE
call make_sub .\ETC\THREAD
call make_sub .\GRAPHICS\2D
call make_sub .\GRAPHICS\BALLS
call make_sub .\GRAPHICS\BG\BGSAMPLE
call make_sub .\GRAPHICS\BG\FIX32
call make_sub .\GRAPHICS\CLUTFOG
call make_sub .\GRAPHICS\DIFFUSE
call make_sub .\GRAPHICS\DIVIDE\ACTIVE
call make_sub .\GRAPHICS\DIVIDE\CLIP
call make_sub .\GRAPHICS\DMPSX
call make_sub .\GRAPHICS\FBALLS
call make_sub .\GRAPHICS\GSGPU
call make_sub .\GRAPHICS\JIMEN
call make_sub .\GRAPHICS\MAT
call make_sub .\GRAPHICS\MESH\QMESH
call make_sub .\GRAPHICS\MESH\RMESH
call make_sub .\GRAPHICS\MESH\SMESH
call make_sub .\GRAPHICS\MISC\60FRAME
call make_sub .\GRAPHICS\MIPMAP
call make_sub .\GRAPHICS\ODEN
call make_sub .\GRAPHICS\PHONG
call make_sub .\GRAPHICS\PMD
call make_sub .\GRAPHICS\PPM
call make_sub .\GRAPHICS\RGB24
call make_sub .\GRAPHICS\ROTATE\AROT
call make_sub .\GRAPHICS\ROTATE\INTRPOL
call make_sub .\GRAPHICS\ROTATE\MAT2ROT
call make_sub .\GRAPHICS\ROTATE\AXESMIME
call make_sub .\GRAPHICS\SHADOW
call make_sub .\GRAPHICS\TMD
call make_sub .\GRAPHICS\TMDVIEW\LOWLEVEL
call make_sub .\GRAPHICS\TMDVIEW\MIME
call make_sub .\GRAPHICS\TMDVIEW\RCUBE
call make_sub .\GRAPHICS\TMDVIEW\SHUTTLE
call make_sub .\GRAPHICS\TMDVIEW\TMDVIEW3
call make_sub .\GRAPHICS\TMDVIEW\TMDVIEW4
call make_sub .\GRAPHICS\TMDVIEW\TMDVIEW5
call make_sub .\GRAPHICS\TOD\JANKEN
call make_sub .\GRAPHICS\TOD\TODVIEW
call make_sub .\GRAPHICS\TRR
call make_sub .\GRAPHICS\TUTO
call make_sub .\GRAPHICS\WALK
call make_sub .\GRAPHICS\ZIMEN
call make_sub .\MATH\TREE
call make_sub .\PRESS\TUTO
call make_sub .\SOUND\BALLS
call make_sub .\SOUND\BASIC
call make_sub .\SOUND\CDVOL
call make_sub .\SOUND\MUTUAL
call make_sub .\SOUND\SIMPLE
call make_sub .\SOUND\STREAM
call make_sub .\SOUND\TUTO
call make_sub .\SOUND\XSE

del %ftmp%

set root=
set file=
set file2=
set ftmp=
