all:
	ccpsx -O3 -Xo$80010000 main.c -oballs.cpe,balls.sym

apd_save:
	ccpsx -O3 -Xo$80010000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-othreads.cpe
apd_load:
	ccpsx -O3 -Xo$80010000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-othreadl.cpe

for_menu:
	ccpsx -O3 -Xo$80010000 main.c \psx\lib\none3.obj -oballs.cpe
	cpe2x balls.cpe
