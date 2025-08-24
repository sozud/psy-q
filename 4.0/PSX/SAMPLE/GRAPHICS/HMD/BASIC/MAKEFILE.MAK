all: tuto0 tuto1 tuto2 tuto3
    
APDOBJ = \psx\lib\autopad.obj
	
tuto0:
	ccpsx -O3 -Xo$80100000 tuto0.c -otuto0.cpe,tuto0.sym

tuto1:
	ccpsx -O3 -Xo$80100000 tuto1.c -otuto1.cpe,tuto1.sym

tuto2:
	ccpsx -O3 -Xo$80100000 tuto2.c -otuto2.cpe,tuto2.sym

tuto3:
	ccpsx -O3 -Xo$80100000 tuto3.c -otuto3.cpe,tuto3.sym

apd_save:
	ccpsx -O3 -Xo$80100000 -DAPD_SAVE tuto0.c $(APDOBJ) \
	-obasic0s.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_SAVE tuto1.c $(APDOBJ) \
	-obasic1s.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_SAVE tuto2.c $(APDOBJ) \
	-obasic2s.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_SAVE tuto3.c $(APDOBJ) \
	-obasic3s.cpe

apd_load:
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto0.c $(APDOBJ) \
	-obasic0l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto1.c $(APDOBJ) \
	-obasic1l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto2.c $(APDOBJ) \
	-obasic2l.cpe
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD tuto3.c $(APDOBJ) \
	-obasic3l.cpe

for_menu:
	ccpsx -O3 -Xo$80100000 tuto0.c \psx\lib\none3.obj -otuto0.cpe,tuto0.sym
	cpe2x tuto0.cpe
	del tuto0.cpe

	ccpsx -O3 -Xo$80100000 tuto1.c \psx\lib\none3.obj -otuto1.cpe,tuto1.sym
	cpe2x tuto1.cpe
	del tuto1.cpe

	ccpsx -O3 -Xo$80100000 tuto2.c \psx\lib\none3.obj -otuto2.cpe,tuto2.sym
	cpe2x tuto2.cpe
	del tuto2.cpe

	ccpsx -O3 -Xo$80100000 tuto3.c \psx\lib\none3.obj -otuto3.cpe,tuto3.sym
	cpe2x tuto3.cpe
	del tuto3.cpe

load0:
	pqbload tri.hmd 80010000
#	pqbload dino.hmd 80010000
#	pqbload cube.hmd 80010000
#	pqbload tile.hmd 80010000

load1:
	pqbload flag.hmd 80010000

load2:
	pqbload shuttle.hmd 80010000

load3:
	pqbload jimen.hmd 80010000
