all:
	ccpsx -O3 -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE tuto0.c \psx\lib\autopad.obj \
	-omipmaps.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD tuto0.c \psx\lib\autopad.obj \
	-omipmapl.cpe

for_menu:
	ccpsx -O3 -Xo$80080000 tuto0.c \psx\lib\none3.obj -omipmap.cpe
	cpe2x mipmap.cpe
	del mipmap.cpe

load:
	pqbload block.tmd 80100000
	pqbload block.tim 80120000
