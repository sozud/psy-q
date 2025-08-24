all: tuto0
	
tuto0:
	ccpsx -O3 -Xo$80080000 tuto0.c cdadd.c -otuto0.cpe,tuto0.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE tuto0.c cdadd.c \
	\psx\lib\autopad.obj -ostr3ds.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD tuto0.c cdadd.c \
	\psx\lib\autopad.obj -ostr3dl.cpe

for_menu:
	ccpsx -O3 -Xo$80080000 tuto0.c cdadd.c \psx\lib\none3.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe

load:
	pqbload pat16.tim 80010000
	pqbload pat8.tim  80020000
	pqbload pat4.tim  80030000
	pqbload cube3.tmd 80040000
