all:
	ccpsx -O -Xo$80100000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80100000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-obasics.cpe

apd_load:
	ccpsx -O -Xo$80100000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-obasicl.cpe

for_menu:
        ccpsx -O -Xo$80100000 main.c \psx\lib\none3.obj -obasic.cpe
        cpe2x basic.cpe
        del basic.cpe

load:
	pqbload ..\simple\sample.vh 80030000
	pqbload ..\simple\sample.vb 80032a20
	pqbload sample.sep 80010000
	pqbload sinfonie.seq 80015000

clean:
	del *.cpe
	del *.sym
	del *.map
	del *.?~
	del *.??~
