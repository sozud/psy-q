all:
	ccpsx -O -Xo$80040000 main.c -orgb24.cpe,rgb24.sym

apd_save:
	ccpsx -O -Xo$80040000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-orgb24s.cpe

apd_load:
	ccpsx -O -Xo$80040000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-orgb24l.cpe

for_menu:
	ccpsx -O -Xo$80040000 main.c \psx\lib\none3.obj -orgb24.cpe
	cpe2x rgb24.cpe
	del rgb24.cpe

