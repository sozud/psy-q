main:
	ccpsx -O -Xo$80040000 main.c sim.c graph.c db.c -omain.cpe,main.sym

for_menu:
	ccpsx -O -Xo$80040000 \psx\lib\none2.obj main.c sim.c graph.c db.c -oearth.cpe
	cpe2x earth.cpe
	del   earth.cpe
load:
	pqbload earth.tmd 80210000

