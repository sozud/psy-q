all: tuto0 tuto1 tuto2 tuto3 tuto4 tuto5 tuto6 tuto7
	
tuto0:
	ccpsx -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym

tuto1:
	ccpsx -Xo$80080000 tuto1.c -otuto1.cpe,tuto1.sym

tuto2:
	ccpsx -Xo$80080000 tuto2.c -otuto2.cpe,tuto2.sym

tuto3:
	ccpsx -Xo$80080000 tuto3.c -otuto3.cpe,tuto3.sym

tuto4:
	ccpsx -Xo$80080000 tuto4.c -otuto4.cpe,tuto4.sym

tuto5:
	ccpsx -Xo$80080000 tuto5.c -otuto5.cpe,tuto5.sym

tuto6:
	ccpsx -Xo$80080000 tuto6.c -otuto6.cpe,tuto6.sym

tuto7:
	ccpsx -Xo$80080000 tuto7.c -otuto7.cpe,tuto7.sym

apd_save:
	ccpsx -Xo$80080000 -DAPD_SAVE tuto0.c \psx\lib\autopad.obj \
	-otmdv40s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto1.c \psx\lib\autopad.obj \
	-otmdv41s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto2.c \psx\lib\autopad.obj \
	-otmdv42s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto3.c \psx\lib\autopad.obj \
	-otmdv43s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto4.c \psx\lib\autopad.obj \
	-otmdv44s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto5.c \psx\lib\autopad.obj \
	-otmdv45s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto6.c \psx\lib\autopad.obj \
	-otmdv46s.cpe
	ccpsx -Xo$80080000 -DAPD_SAVE tuto7.c \psx\lib\autopad.obj \
	-otmdv47s.cpe
apd_load:
	ccpsx -Xo$80080000 -DAPD_LOAD tuto0.c \psx\lib\autopad.obj \
	-otmdv40l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto1.c \psx\lib\autopad.obj \
	-otmdv41l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto2.c \psx\lib\autopad.obj \
	-otmdv42l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto3.c \psx\lib\autopad.obj \
	-otmdv43l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto4.c \psx\lib\autopad.obj \
	-otmdv44l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto5.c \psx\lib\autopad.obj \
	-otmdv45l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto6.c \psx\lib\autopad.obj \
	-otmdv46l.cpe
	ccpsx -Xo$80080000 -DAPD_LOAD tuto7.c \psx\lib\autopad.obj \
	-otmdv47l.cpe

for_menu:
	ccpsx -Xo$80080000 tuto0.c \psx\lib\none3.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe

	ccpsx -Xo$80080000 tuto1.c \psx\lib\none3.obj -otuto1.cpe,tuto1.sym
	cpe2x tuto1.cpe
	del tuto1.cpe

	ccpsx -Xo$80080000 tuto2.c \psx\lib\none3.obj -otuto2.cpe,tuto2.sym
	cpe2x tuto2.cpe
	del tuto2.cpe

	ccpsx -Xo$80080000 tuto3.c \psx\lib\none3.obj -otuto3.cpe,tuto3.sym
	cpe2x tuto3.cpe
	del tuto3.cpe

	ccpsx -Xo$80080000 tuto4.c \psx\lib\none3.obj -otuto4.cpe,tuto4.sym
	cpe2x tuto4.cpe
	del tuto4.cpe

	ccpsx -Xo$80080000 tuto5.c \psx\lib\none3.obj -otuto5.cpe,tuto5.sym
	cpe2x tuto5.cpe
	del tuto5.cpe

	ccpsx -Xo$80080000 tuto6.c \psx\lib\none3.obj -otuto6.cpe,tuto6.sym
	cpe2x tuto6.cpe
	del tuto6.cpe

	ccpsx -Xo$80080000 tuto7.c \psx\lib\none3.obj -otuto7.cpe,tuto7.sym
	cpe2x tuto7.cpe
	del tuto7.cpe

load:
        pqbload ..\data\pat16.tim 80010000
	pqbload ..\data\pat8.tim  80020000
	pqbload ..\data\pat4.tim  80030000
	pqbload ..\data\cube3g.tmd 80040000

load1:
        pqbload ..\data\pat16.tim 80010000
	pqbload ..\data\pat8.tim  80020000
	pqbload ..\data\pat4.tim  80030000
	pqbload ..\data\cube3.tmd 80040000

load2:
	pqbload ..\data\venus3g.tmd 80040000

