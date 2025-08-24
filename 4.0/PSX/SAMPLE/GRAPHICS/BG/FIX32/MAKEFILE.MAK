all:
	ccpsx -g -Xo$80080000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-ofix32s.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-ofix32l.cpe

for_menu:
	ccpsx -g -Xo$80080000 \psx\lib\none3.obj main.c -ofix32.cpe
	cpe2x fix32.cpe
	del   fix32.cpe

load:
	pqbload sample.tim 80180000
	pqbload sample.cel 80140000
	pqbload sample0.bgd 80100000
	pqbload sample1.bgd 80101000
	pqbload sample2.bgd 80102000
	pqbload sample3.bgd 80103000
