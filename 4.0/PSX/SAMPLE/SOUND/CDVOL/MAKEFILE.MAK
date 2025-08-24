all:
	ccpsx -O -Xo$80010000 -Xm main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80010000 -Xm -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-ocdvols.cpe

apd_load:
	ccpsx -O -Xo$80010000 -Xm -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-ocdvoll.cpe

for_menu:
	ccpsx -O -Xo$80010000 -DFOR_MENU -Xm main.c c:\psx\lib\none3.obj -ocdvol.cpe,cdvol.sym
	cpe2x cdvol.cpe

clean:
	del *.exe
	del *.sym
	del *.map
	del *.cpe
	del *.?~
	del *.??~

load:
	
