all: tuto0 tuto1
	
tuto0:
	ccpsx -O -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym
tuto1:
	ccpsx -O -Xo$80080000 tuto1.c -otuto1.cpe,tuto1.sym

apd_save:
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto0.c \psx\lib\autopad.obj \
	-ogsgpu0s.cpe
	ccpsx -O -Xo$80080000 -DAPD_SAVE tuto1.c \psx\lib\autopad.obj \
	-ogsgpu1s.cpe

apd_load:
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto0.c \psx\lib\autopad.obj \
	-ogsgpu0l.cpe
	ccpsx -O -Xo$80080000 -DAPD_LOAD tuto1.c \psx\lib\autopad.obj \
	-ogsgpu1l.cpe

for_menu: tuto0.exe tuto1.exe
	
tuto0.exe:
	ccpsx -O -Xo$80080000 tuto0.c \psx\lib\none3.obj -otuto0.cpe
	cpe2x tuto0.cpe
	del tuto0.cpe

tuto1.exe:
	ccpsx -O -Xo$80080000 tuto1.c \psx\lib\none3.obj -otuto1.cpe
	cpe2x tuto1.cpe
	del tuto1.cpe

load:
	pqbload mado.tim 80180000
	pqbload mado.tmd 80040000
