TADDR	 =80010000

SRCS0 =main.c control.c matrix.c preset.c ctlfc.c rpy_intr.c a_intr.c tuto0.c nmime.c
SRCS1 =main.c control.c matrix.c preset.c ctlfc.c rpy_intr.c a_intr.c tuto1.c

all: tuto0 tuto1
	

tuto0: 
	ccpsx -O2 -Xo$$(TADDR) $(SRCS0) -otuto0.cpe,tuto0.sym

tuto1: 
	ccpsx -O2 -Xo$$(TADDR) $(SRCS1) -otuto1.cpe,tuto1.sym


load0:
	pqbload base.tmd 80100000
	pqbload base.vdf 80108000

load1:
	pqbload base.tmd 80100000

base.vdf: base.tmd  rotate.tmd yokonobi.tmd bend1.tmd bend2.tmd
	mimefilt -o base.vdf base.tmd  rotate.tmd yokonobi.tmd bend1.tmd bend2.tmd

apd_save: 
	copy \psx\lib\autopad.obj ap.obj
	ccpsx -O2 -Xo$$(TADDR) -DS0 $(SRCS0) ap.obj -ovjmime0s.cpe
	ccpsx -O2 -Xo$$(TADDR) -DS1 $(SRCS1) ap.obj -ovjmime1s.cpe
	del ap.obj

apd_load: 
	copy \psx\lib\autopad.obj ap.obj
	ccpsx -O2 -Xo$$(TADDR) $(SRCS0) -DL0 ap.obj -ovjmime0l.cpe
	ccpsx -O2 -Xo$$(TADDR) $(SRCS1) -DL1 ap.obj -ovjmime1l.cpe
	del ap.obj

for_menu: tuto0_m tuto1_m
	

tuto0_m:
	echo $(SRCS0) > srcs.tmp
	ccpsx -O2 -Xo$80040000 @srcs.tmp c:\psx\lib\none3.obj -otuto0_m.cpe
	cpe2x tuto0_m.cpe
	del srcs.tmp
	del tuto0_m.cpe

tuto1_m:
	echo $(SRCS1) > srcs.tmp
	ccpsx -O2 -Xo$80040000 @srcs.tmp c:\psx\lib\none3.obj -otuto1_m.cpe
	cpe2x tuto1_m.cpe
	del srcs.tmp
	del tuto1_m.cpe
