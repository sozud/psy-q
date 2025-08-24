TADDR	 =80040000

APDOBJ   = \psx\lib\autopad.obj

SRCS = main.c model.c control.c matrix.c preset.c ctlfc.c a_intr.c rpy_intr.c

all: tuto0
	
tuto0: 
	ccpsx -Xo$$(TADDR) $(SRCS) -otuto0.cpe,tuto0.sym,tuto0.map

apd_save: 
	copy \psx\lib\autopad.obj autopad.obj
	ccpsx -Xo$$(TADDR) $(SRCS) -DAPD_SAVE autopad.obj -ojmimes.cpe
	del autopad.obj

apd_load: 
	copy \psx\lib\autopad.obj autopad.obj
	ccpsx -Xo$$(TADDR) $(SRCS) -DAPD_LOAD autopad.obj -ojmimel.cpe
	del autopad.obj

load:
	pqbload torso.tmd 80100000
	pqbload leg.tmd 80104000
	pqbload arm.tmd 80108000
	pqbload head.tmd 8010C000

for_menu: tuto0_m
	
tuto0_m:
	echo $(SRCS) > srcs.tmp
	ccpsx -Xo$$(TADDR) @srcs.tmp c:\psx\lib\none3.obj -otuto0_m.cpe,tuto0_m.sym
	cpe2x tuto0_m.cpe
	del tuto0_m.cpe
