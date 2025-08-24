all: tuto0 tuto1 tuto2 tuto3 tuto5
	
tuto0:
	ccpsx -O -Xo$80080000 tuto0.c db.c -otuto0.cpe,tuto0.sym
tuto1:
	ccpsx -O -Xo$80080000 tuto1.c db.c -otuto1.cpe,tuto1.sym
tuto2:
	ccpsx -O -Xo$80080000 tuto2.c db.c -otuto2.cpe,tuto2.sym
tuto3:
	ccpsx -O -Xo$80080000 tuto3.c db.c -otuto3.cpe,tuto3.sym
tuto5:
	ccpsx -c -O tuto5.c -otuto5.obj
	dmpsx tuto5.obj
	ccpsx -O -Xo$80080000 tuto5.obj db.c -otuto5.cpe,tuto5.sym

apd_save:
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto0.c db.c \
	\psx\lib\autopad.obj -opmd0s.cpe
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto1.c db.c \
	\psx\lib\autopad.obj -opmd1s.cpe
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto2.c db.c \
	\psx\lib\autopad.obj -opmd2s.cpe
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto3.c db.c \
	\psx\lib\autopad.obj -opmd3s.cpe
	ccpsx -c -O -DAPD_SAVE tuto5.c -otuto5.obj
	dmpsx tuto5.obj
	ccpsx -O -Xo$80080000 tuto5.obj db.c \psx\lib\autopad.obj \
	-opmd5s.cpe
	del tuto5.obj

apd_load:
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto0.c db.c \
	\psx\lib\autopad.obj -opmd0l.cpe
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto1.c db.c \
	\psx\lib\autopad.obj -opmd1l.cpe
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto2.c db.c \
	\psx\lib\autopad.obj -opmd2l.cpe
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto3.c db.c \
	\psx\lib\autopad.obj -opmd3l.cpe
	ccpsx -c -O -DAPD_LOAD tuto5.c -otuto5.obj
	dmpsx tuto5.obj
	ccpsx -O -Xo$80080000 tuto5.obj db.c \psx\lib\autopad.obj \
	-opmd5l.cpe
	del tuto5.obj

for_menu: tuto0.exe tuto1.exe tuto2.exe tuto3.exe tuto5.exe
	
tuto0.exe:
	ccpsx -O -Xo$80080000 tuto0.c db.c \psx\lib\none3.obj -otuto0.cpe
	cpe2x tuto0.cpe
tuto1.exe:
	ccpsx -O -Xo$80080000 tuto1.c db.c \psx\lib\none3.obj -otuto1.cpe
	cpe2x tuto1.cpe
tuto2.exe:
	ccpsx -O -Xo$80080000 tuto2.c db.c \psx\lib\none3.obj -otuto2.cpe
	cpe2x tuto2.cpe
tuto3.exe:
	ccpsx -O -Xo$80080000 tuto3.c db.c \psx\lib\none3.obj -otuto3.cpe
	cpe2x tuto3.cpe
tuto5.exe:
	ccpsx -c -O tuto5.c -otuto5.obj
	dmpsx tuto5.obj
	ccpsx -O -Xo$80080000 tuto5.obj db.c \psx\lib\none3.obj -otuto5.cpe
	cpe2x tuto5.cpe

load:
	pqbload dino.tmd    80100000
	pqbload earth.tmd   80120000
	pqbload earth4.tim  80140000
	pqbload share.pmd   80180000
	pqbload noshare.pmd 801a0000
	pqbload pat.tim     801c0000

