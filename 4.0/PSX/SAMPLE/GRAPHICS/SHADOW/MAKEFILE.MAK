all:
	ccpsx -O3 -Xo$80010000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80010000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-oshadows.cpe

apd_load:
	ccpsx -O3 -Xo$80010000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-oshadowl.cpe

for_menu:
        ccpsx -O3 -Xo$80010000 main.c \psx\lib\none3.obj -oshadow.cpe
        cpe2x shadow.cpe
        del shadow.cpe
