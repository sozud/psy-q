all:
	ccpsx -O2 -G0 -Xo$80080000 main.c -o main.cpe,main.sym

apd_save:
	ccpsx -O2 -G0 -Xo$80080000 -DAPD_SAVE main.c \
	\psx\lib\autopad.obj -omcrd0s.cpe

apd_load:
	ccpsx -O2 -G0 -Xo$80080000 -DAPD_LOAD main.c \
	\psx\lib\autopad.obj -omcrd0l.cpe

for_menu:
	ccpsx -O2 -G0 -Xo$80080000 main.c \psx\lib\none3.obj -o main.cpe,main.sym
	cpe2x main.cpe
	del   main.cpe

clean:
	del *.obj
	del *.cpe
	del *.sym

