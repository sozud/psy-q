all:
	ccpsx -O -Xo$80010000 main.c -odiffuse.cpe,diffuse.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-odiffuses.cpe

apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-odiffusel.cpe

for_menu:
	ccpsx -O -Xo$80010000 main.c \psx\lib\none3.obj -odiffuse.cpe
	cpe2x diffuse.cpe
	del diffuse.cpe
