all: tuto0 tuto1 tuto2 tuto3
	
tuto0:
	ccpsx -O3 -Xo$80100000 tuto0.c -otuto0.cpe,tuto0.sym

tuto1:
	ccpsx -O3 -Xo$80100000 tuto1.c -otuto1.cpe,tuto1.sym

tuto2:
	ccpsx -O3 -Xo$80100000 tuto2.c -otuto2.cpe,tuto2.sym

tuto3:
	ccpsx -O3 -Xo$80100000 tuto3.c -otuto3.cpe,tuto3.sym

apd_save:
	ccpsx -O3 -Xo$80100000 -Xn512 -DAPD_SAVE tuto0.c \psx\lib\autopad.obj \
	-ohmdan0s.cpe
	ccpsx -O3 -Xo$80100000 -Xn512 -DAPD_SAVE tuto1.c \psx\lib\autopad.obj \
	-ohmdan1s.cpe
	ccpsx -O3 -Xo$80100000 -Xn512 -DAPD_SAVE tuto2.c \psx\lib\autopad.obj \
	-ohmdan2s.cpe
	ccpsx -O3 -Xo$80100000 -Xn512 -DAPD_SAVE tuto3.c \psx\lib\autopad.obj \
	-ohmdan3s.cpe

apd_load:
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto0.c \psx\lib\autopad.obj \
	-ohmdan0l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto1.c \psx\lib\autopad.obj \
	-ohmdan1l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto2.c \psx\lib\autopad.obj \
	-ohmdan2l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto3.c \psx\lib\autopad.obj \
	-ohmdan3l.cpe

for_menu:
	ccpsx -O3 -Xo$80100000 tuto0.c \psx\lib\none3.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe

	ccpsx -O3 -Xo$80100000 tuto1.c \psx\lib\none3.obj -otuto1.cpe,tuto1.sym
	cpe2x tuto1.cpe
	del tuto1.cpe

	ccpsx -O3 -Xo$80100000 tuto2.c \psx\lib\none3.obj -otuto2.cpe,tuto2.sym
	cpe2x tuto2.cpe
	del tuto2.cpe

	ccpsx -O3 -Xo$80100000 tuto3.c \psx\lib\none3.obj -otuto3.cpe,tuto3.sym
	cpe2x tuto3.cpe
	del tuto3.cpe


load0:
	pqbload ../data/tri3/tri.hmd 80010000

load1:
	pqbload ../data/tri3/tri.hmd 80010000

load2:
	pqbload ../data/cube0\cube0.hmd 80010000

load3:
	pqbload ../data/tri4/tri.hmd 80010000

