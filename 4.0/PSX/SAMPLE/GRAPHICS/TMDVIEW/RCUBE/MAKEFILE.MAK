all:
	ccpsx -g -Xo$80080000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -g -Xo$80080000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-orcubes.cpe

apd_load:
	ccpsx -g -Xo$80080000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-orcubel.cpe

for_menu:
	ccpsx -g -Xo$80080000 main.c \psx\lib\none3.obj -omain.cpe,main.sym
	cpe2x main.cpe
	del main.cpe

load:
	pqbload rcube.tmd 80010000
	pqbload rcube.tim 80020000
