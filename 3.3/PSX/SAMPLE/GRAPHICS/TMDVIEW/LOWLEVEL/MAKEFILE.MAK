all: tuto0
	
tuto0:
	ccpsx -G 0 -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym

for_menu:
	ccpsx -G 0 -Xo$80080000 tuto0.c \psx\lib\none2.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe
load:
        pqbload ..\data\pat16.tim 80120000
	pqbload ..\data\pat8.tim  80140000
	pqbload ..\data\pat4.tim  80160000
	pqbload ..\data\cube3.tmd  80100000

