all:
	ccpsx -O3 -Xo$80010000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80010000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-otrrs.cpe

apd_load:
	ccpsx -O3 -Xo$80010000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-otrrl.cpe

for_menu:
        ccpsx -O3 -Xo$80010000 main.c \psx\lib\none3.obj -otrr.cpe
        cpe2x trr.cpe
        del trr.cpe

