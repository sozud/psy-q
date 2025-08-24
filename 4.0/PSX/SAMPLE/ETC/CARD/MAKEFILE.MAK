all:
	ccpsx -O -Xo$80010000 card.c -ocard.cpe,card.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE card.c \psx\lib\autopad.obj \
	-ocards.cpe
apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD card.c \psx\lib\autopad.obj \
	-ocardl.cpe
	ccpsx -O -Xo$80010000 -DCARD ..\..\autopad\message.c -oapdmsg.cpe

for_menu:
	ccpsx -O -Xo$80010000 card.c \psx\lib\none3.obj -ocard.cpe,card.sym
	cpe2x card.cpe
	del card.cpe

