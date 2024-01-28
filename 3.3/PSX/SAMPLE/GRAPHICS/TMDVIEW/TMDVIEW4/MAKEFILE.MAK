tuto0:
	ccpsx -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym

tuto1:
	ccpsx -Xo$80080000 tuto1.c -otuto1.cpe,tuto1.sym

tuto2:
	ccpsx -Xo$80080000 tuto2.c -otuto2.cpe,tuto2.sym

for_menu:
	ccpsx -Xo$80080000 tuto0.c \psx\lib\none2.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe

	ccpsx -Xo$80080000 tuto1.c \psx\lib\none2.obj -otuto1.cpe,tuto1.sym
	cpe2x tuto1.cpe
	del tuto1.cpe

	ccpsx -Xo$80080000 tuto2.c \psx\lib\none2.obj -otuto2.cpe,tuto2.sym
	cpe2x tuto2.cpe
	del tuto2.cpe

load:
        pqbload ..\data\pat16.tim 80010000
	pqbload ..\data\pat8.tim  80020000
	pqbload ..\data\pat4.tim  80030000
	pqbload ..\data\cube3g.tmd 80040000

load1:
        pqbload ..\data\pat16.tim 80010000
	pqbload ..\data\pat8.tim  80020000
	pqbload ..\data\pat4.tim  80030000
	pqbload ..\data\cube3.tmd 80040000

