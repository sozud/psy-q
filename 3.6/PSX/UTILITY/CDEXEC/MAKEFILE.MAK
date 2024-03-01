all:
	ccpsx -O -Xo$801ff000 cdexec.c -ocdexec.cpe

for_menu:
        ccpsx -O -Xo$80100000 cdexec.c \psx\lib\none2.obj -ocdexec.cpe
        cpe2x cdexec.cpe
        del cdexec.cpe
