all:
#	ccpsx -G0 -O3 -Xo$80010000  main8.c rc9.c ../../../lib/2mbyte.obj -om.cpe,m.sym
	ccpsx -G0 -O3 -Xo$80010000  main8.c rc9.c -om.cpe,m.sym
