all:
	ccpsx -g -Xo$80080000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-obgsampls.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-obgsampll.cpe

for_menu:
	ccpsx -g -Xo$80080000 \psx\lib\none3.obj main.c -obgsample.cpe
	cpe2x bgsample.cpe
	del   bgsample.cpe

load:
	pqbload sample.tim 80180000
	pqbload sample.bgd 80100000
	pqbload sample.cel 80140000
