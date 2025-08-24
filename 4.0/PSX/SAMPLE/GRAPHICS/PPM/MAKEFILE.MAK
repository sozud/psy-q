all:
	ccpsx -O3 -Xo$80080000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-oppms.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-oppml.cpe

for_menu:
        ccpsx -O3 -Xo$80080000 main.c \psx\lib\none3.obj -oppm.cpe
        cpe2x ppm.cpe
        del ppm.cpe

load:
	pqbload earth4.tim 80180000
