all: balls
	
balls:
	ccpsx -O3 -Xo$80010000 main.c -oballs.cpe,balls.sym

apd_save:
	ccpsx -O3 -Xo$80010000 main.c \psx\lib\autopad.obj -DAPD_SAVE -oballss.cpe

apd_load:
	ccpsx -O3 -Xo$80010000 main.c \psx\lib\autopad.obj -DAPD_LOAD -oballsl.cpe,ballsl.sym

for_menu:
	ccpsx -O3 -Xo$80010000 main.c \psx\lib\none3.obj -oballs.cpe
	cpe2x balls.cpe
	del balls.cpe
