all:
	ccpsx -O -Xo$80100000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80100000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-oxses.cpe

apd_load:
	ccpsx -O -Xo$80100000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-oxsel.cpe

for_menu:
        ccpsx -O -Xo$80100000 main.c \psx\lib\none3.obj -oxse.cpe
        cpe2x xse.cpe
        del xse.cpe

load:
	pqbload simple.vh 80010000
	pqbload simple.vb 80015000

clean:
	del *.?~
	del *.??~
	del *.cpe
	del *.sym
	del *.map
