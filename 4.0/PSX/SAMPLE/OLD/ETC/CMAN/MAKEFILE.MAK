all:
	ccpsx -O -Xo$80010000 cman.c -ocman.cpe,cman.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE cman.c \psx\lib\autopad.obj \
	-ocmans.cpe
apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD cman.c \psx\lib\autopad.obj \
	-ocmanl.cpe
	ccpsx -O -Xo$80010000 -DCMAN ..\..\autopad\message.c -oapdmsg.cpe

for_menu:
	ccpsx -O -Xo$80010000 cman.c \psx\lib\none3.obj -ocman.cpe,cman.sym
	cpe2x cman.cpe
	del cman.cpe

