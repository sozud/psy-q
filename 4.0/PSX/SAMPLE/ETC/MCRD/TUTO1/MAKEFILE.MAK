all:
	ccpsx -O2 -G0 -Xo$80040000 main.c balls.c -omain.cpe,main.sym

apd_save:
	ccpsx -O2 -G0 -Xo$80040000 -DAPD_SAVE main.c balls.c \
	\psx\lib\autopad.obj -omcrd1s.cpe

apd_load:
	ccpsx -O2 -G0 -Xo$80040000 -DAPD_LOAD main.c balls.c \
	\psx\lib\autopad.obj -omcrd1l.cpe

for_menu:
	ccpsx -O2 -G0 -Xo$80040000 main.c balls.c \psx\lib\none3.obj -omain.cpe,main.sym
	cpe2x main.cpe
	del main.cpe
