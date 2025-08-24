RUN = run
PQBLOAD = pqbload
RESET = resetps

SRCS = tuto0.c control.c
OBJS = tuto0.obj control.obj
HDRS =  control.h
APDOBJ = \psx\lib\autopad.obj

DOCS = readme_j.txt

MODEL0 = triaxes.hmd
MODEL1 = puyo.hmd
MODEL2 = trirpy.hmd

MODELADDR = 80010000

CC	= ccpsx

CFLAGS = -O2

LDFLAGS = -O2 -Xo$80100000

PROGRAM = tuto0.cpe


all: $(PROGRAM)
	

tuto0.cpe: $(OBJS)
	echo $(OBJS) > objs
	$(CC) $(LDFLAGS) @objs -otuto0.cpe,tuto0.sym

apd_save:
	$(CC) $(LDFLAGS) -DAPD_SAVE tuto0.c control.c $(APDOBJ) -ohmimes.cpe

apd_load:
	$(CC) $(LDFLAGS) -DAPD_LOAD tuto0.c control.c $(APDOBJ) -ohmimel.cpe

for_menu: tuto0_m.exe
	

tuto0_m.exe: $(OBJS)
	echo $(OBJS) > objs
	ccpsx -O3 -Xo$80100000 @objs \psx\lib\none3.obj -otuto0_m.cpe,tuto0_m.sym
	cpe2x tuto0_m.cpe
	del tuto0_m.cpe

reset:
	$(RESET)

run: tuto0.cpe reset load0 list
	$(RUN) tuto0.cpe

run0: tuto0.cpe reset load0 list
	$(RUN) tuto0.cpe

run1: tuto0.cpe reset load1 list
	$(RUN) tuto0.cpe

run2: tuto0.cpe reset load2 list
	$(RUN) tuto0.cpe

load0:
	$(PQBLOAD) $(MODEL0) $(MODELADDR)

load1:
	$(PQBLOAD) $(MODEL1) $(MODELADDR)

load2:
	$(PQBLOAD) $(MODEL2) $(MODELADDR)

clean:
	rm -f $(OBJS)
	rm -f $(PROGRAM)

tuto0.obj:  control.h tuto0.c
	$(CC) -c $(CFLAGS) tuto0.c

control.obj: control.h control.c
	$(CC) -c $(CFLAGS) control.c
