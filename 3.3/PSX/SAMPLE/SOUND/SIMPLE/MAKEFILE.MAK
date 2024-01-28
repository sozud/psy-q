all:
	ccpsx -O -Xo$80100000 main.c -omain.cpe,main.sym

for_menu:
        ccpsx -O -Xo$80100000 main.c \psx\lib\none2.obj -osimple.cpe
        cpe2x simple.cpe
        del simple.cpe

load:
	pqbload sample.vh 80015000
	pqbload sample.vb 80020000
	pqbload ..\basic\fuga.seq 80010000
clean:
	del *.?~
	del *.??~
	del *.cpe
	del *.sym
	del *.map
