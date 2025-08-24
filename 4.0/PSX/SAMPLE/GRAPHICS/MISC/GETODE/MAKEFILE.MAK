all:
	ccpsx -O4 -Xo$80010000 main.c -oballs.cpe,balls.sym

apd_save:
        ccpsx -O4 -Xo$80010000 main.c \
        -DAPD_SAVE \psx\lib\autopad.obj -ogetodes.cpe

apd_load:
        ccpsx -O4 -Xo$80010000 main.c \
        -DAPD_LOAD \psx\lib\autopad.obj -ogetodel.cpe,getodel.sym

for_menu:
	ccpsx -O4 -Xo$80010000 main.c \psx\lib\none3.obj -ogetode.cpe
	cpe2x getode.cpe
	del getode.cpe
