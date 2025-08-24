all: main
	
main:
	ccpsx -O -Xo$80040000 main.c -omain.cpe,main.sym,main.map

apd_save:
	ccpsx -O -Xo$80040000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-ointrpols.cpe

apd_load:
	ccpsx -O -Xo$80040000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-ointrpoll.cpe

load:
	pqbload ../../data/tim/earth8.tim 80100000


for_menu: main_m
	
main_m:
	ccpsx -O -Xo$80040000 main.c c:\psx\lib\none3.obj -omain_m.cpe,main_m.sym,main_m.map
	cpe2x main_m.cpe
	del main_m.cpe
