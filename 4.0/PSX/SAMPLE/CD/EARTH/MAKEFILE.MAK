tuto0:
	ccpsx -O -Xo$80040000 tuto0.c cdadd.c db.c -oearth.cpe,earth.sym

apd_save:
	ccpsx -O -Xo$80040000 -DAPD_SAVE tuto0.c cdadd.c db.c \
	\psx\lib\autopad.obj -oearths.cpe

apd_load:
	ccpsx -O -Xo$80040000 -DAPD_LOAD tuto0.c cdadd.c db.c \
	\psx\lib\autopad.obj -oearthl.cpe

for_menu:
	ccpsx -O -Xo$80040000 \psx\lib\none3.obj tuto0.c cdadd.c db.c -oearth.cpe
	cpe2x earth.cpe
	del   earth.cpe
load:
	pqbload earth.tmd   80120000



