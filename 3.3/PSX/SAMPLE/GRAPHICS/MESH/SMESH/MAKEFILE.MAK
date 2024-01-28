all: tuto0 tuto1
	
tuto0:	
	ccpsx -O3 -Xo$80010000 tuto0.c -otuto0.cpe,tuto0.sym
tuto1:	
	ccpsx -O3 -Xo$80010000 tuto1.c -otuto1.cpe,tuto1.sym

for_menu: smesh0 smesh1
	
smesh0:
        ccpsx -O3 -Xo$80010000 tuto0.c c:\psx\lib\none2.obj -osmesh0.cpe,smesh0.sym,smesh0.map
        cpe2x smesh0.cpe
        del smesh0.cpe

smesh1:
        ccpsx -O3 -Xo$80010000 tuto1.c c:\psx\lib\none2.obj -osmesh1.cpe,smesh1.sym,smesh1.map
        cpe2x smesh1.cpe
        del smesh1.cpe

load:
	pqbload rcube.tim 80070000
load1:
	pqbload earth4.tim 80070000

