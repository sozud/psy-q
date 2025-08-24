all:
	ccpsx -O -Xo$80100000 -Xm main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80100000 -Xm -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-omutuals.cpe

apd_load:
	ccpsx -O -Xo$80100000 -Xm -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-omutuall.cpe

for_menu:
	ccpsx -O -Xo$80100000 -Xm main.c c:\psx\lib\none3.obj -omutual.cpe,mutual.sym
	cpe2x mutual.cpe

load:
	pqbload mutual.vh 80020000
	pqbload mutual.vb 80025000
	pqbload mutual.vh 80060000
	pqbload mutual.vb 80065000
	pqbload mutual1.seq 80010000
	pqbload mutual2.seq 80018000

clean:
	del *.exe
	del *.sym
	del *.map
	del *.cpe
	del *.?~
	del *.??~
