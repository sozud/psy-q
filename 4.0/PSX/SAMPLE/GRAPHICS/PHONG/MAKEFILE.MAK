all:
	ccpsx -O -Xo$80010000 test.c scn.c  -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE test.c scn.c \
	\psx\lib\autopad.obj -ophongs.cpe

apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD test.c scn.c \
	\psx\lib\autopad.obj -ophongl.cpe

for_menu:
        ccpsx -O -Xo$80010000 test.c scn.c \psx\lib\none3.obj -ophong.cpe
        cpe2x phong.cpe
        del phong.cpe


