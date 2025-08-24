all:
	ccpsx -O3 -c text.c text.obj
	dmpsx text.obj
	ccpsx -O3 -Xo$80010000 main.c divide.c text.obj -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -c text.c text.obj
	dmpsx text.obj
	ccpsx -O3 -Xo$80010000 -DAPD_SAVE main.c divide.c text.obj \
	\psx\lib\autopad.obj -ojimens.cpe
	del text.obj

apd_load:
	ccpsx -O3 -c text.c text.obj
	dmpsx text.obj
	ccpsx -O3 -Xo$80010000 -DAPD_LOAD main.c divide.c text.obj \
	\psx\lib\autopad.obj -ojimenl.cpe
	del text.obj

for_menu:
	ccpsx -O3 -c text.c text.obj
	dmpsx text.obj
	ccpsx -O3 -Xo$80010000 \psx\lib\none3.obj main.c divide.c text.obj \
	-ojimen.cpe,jimen.sym
	cpe2x jimen.cpe
	del jimen.cpe

load:
	pqbload stones.tim 801a0000
