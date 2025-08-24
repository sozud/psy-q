all:
	ccpsx -O3 -Xo$80080000 tuto0.c -otuto0.cpe,tuto0.sym

apd_save:
	ccpsx -O3 -Xo$80080000 -DAPD_SAVE tuto0.c \psx\lib\autopad.obj \
	-ormeshs.cpe

apd_load:
	ccpsx -O3 -Xo$80080000 -DAPD_LOAD tuto0.c \psx\lib\autopad.obj \
	-ormeshl.cpe

for_menu:
        ccpsx -O3 -Xo$80080000 tuto0.c \psx\lib\none3.obj -ormesh.cpe
        cpe2x rmesh.cpe
        del rmesh.cpe

load:
	pqbload rcube.tim 80020000

