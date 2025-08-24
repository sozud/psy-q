all:
	ccpsx -O -Xo$80010000 main.c -omain.cpe,main.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE main.c \psx\lib\autopad.obj \
	-oarots.cpe

apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD main.c \psx\lib\autopad.obj \
	-oarotl.cpe

for_menu:
	ccpsx -O -Xo$80010000 main.c \psx\lib\none3.obj -oarot.cpe
	cpe2x arot.cpe
	del arot.cpe
