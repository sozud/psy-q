all:
	ccpsx -O -Xo$80080000 oden.c -ooden.cpe,oden.sym

apd_save:
	ccpsx -O -Xo$80080000 -DAPD_SAVE oden.c \psx\lib\autopad.obj \
	-oodens.cpe

apd_load:
	ccpsx -O -Xo$80080000 -DAPD_LOAD oden.c \psx\lib\autopad.obj \
	-oodenl.cpe

for_menu:
	ccpsx -O -Xo$80080000 \psx\lib\none3.obj oden.c -ooden.cpe
	cpe2x oden.cpe
	del   oden.cpe

load:
        pqbload oden.tim 801c0000
	pqbload oden.tmd 80040000

