OBJ = fonttex1.obj ..\..\..\lib\2mbyte.obj

all: $(OBJ)
	ccpsx -O3 -Xo$80010000 menu.c $(OBJ) -omenu.cpe,menu.sym,menu.map
	cpe2x menu.cpe
	del execmenu.exe
	ren menu.exe execmenu.exe

apd_save: $(OBJ)
	ccpsx -O3 -Xo$80010000 menu.c $(OBJ) -DAPD_SAVE \psx\lib\autopad.obj \
	-oexecmens.cpe

apd_load: $(OBJ)
	ccpsx -O3 -Xo$80010000 menu.c $(OBJ) -DAPD_LOAD \psx\lib\autopad.obj \
	-oexecmenl.cpe

fonttex1.obj : fonttex1.c
	ccpsx -O3 -c fonttex1.c
