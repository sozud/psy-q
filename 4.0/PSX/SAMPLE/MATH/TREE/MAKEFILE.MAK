
tree:
	ccpsx -O3 -Xo$80010000 main.c ../../../lib/2mbyte.obj -omain.cpe,main.sym

apd_save:
	ccpsx -O3 -Xo$80010000 -DAPD_SAVE main.c ../../../lib/2mbyte.obj \
	\psx\lib\autopad.obj -otrees.cpe

apd_load:
	ccpsx -O3 -Xo$80010000 -DAPD_LOAD main.c ../../../lib/2mbyte.obj \
	\psx\lib\autopad.obj -otreel.cpe

for_menu:
	ccpsx -O3 -Xo$80010000 \psx\lib\none3.obj main.c -otree.cpe
	cpe2x tree.cpe
	del   tree.cpe
