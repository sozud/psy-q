all:
	ccpsx -O -Xo$80010000 main.c effect.c mesh.c glasstex.c \
	-o2d.cpe,2d.sym

test:
	ccpsx -O -Xo$80010000 main.c effect.c mesh.c glasstex.c \
	\psx\src\etc\pad.c -o2d.cpe,2d.sym

apd_save:
	ccpsx -O -Xo$80010000 main.c effect.c mesh.c glasstex.c \
	-DAPD_SAVE \psx\lib\autopad.obj -o2ds.cpe

apd_load:
	ccpsx -O -Xo$80010000 main.c effect.c mesh.c glasstex.c \
	-DAPD_LOAD \psx\lib\autopad.obj -o2dl.cpe

for_menu:
	ccpsx -O -Xo$80010000 main.c effect.c mesh.c glasstex.c \
	\psx\lib\none3.obj -o2d.cpe
	cpe2x 2d.cpe
	del 2d.cpe
