all: tuto0 tuto1 tuto2
	
tuto0:
	ccpsx -O -Xo$80040000 tuto0.c -otuto0.cpe,tuto0.sym,tuto0.map

tuto1:
	ccpsx -O -Xo$80040000 tuto1.c -otuto1.cpe,tuto1.sym,tuto1.map

tuto2:
	ccpsx -O -Xo$80040000 tuto2.c -otuto2.cpe,tuto2.sym,tuto2.map

load:
	pqbload ../data/tim/earth8.tim 80100000


for_menu: tuto0_m tuto1_m tuto2_m
	
tuto0_m:
	ccpsx -O -Xo$80040000 tuto0.c c:\psx\lib\none2.obj -otuto0_m.cpe,tuto0_m.sym,tuto0_m.map
	cpe2x tuto0_m.cpe
	del tuto0_m.cpe

tuto1_m:
	ccpsx -O -Xo$80040000 tuto1.c c:\psx\lib\none2.obj -otuto1_m.cpe,tuto1_m.sym,tuto1_m.map
	cpe2x tuto1_m.cpe
	del tuto1_m.cpe

tuto2_m:
	ccpsx -O -Xo$80040000 tuto2.c c:\psx\lib\none2.obj -otuto2_m.cpe,tuto2_m.sym,tuto2_m.map
	cpe2x tuto2_m.cpe
	del tuto2_m.cpe
