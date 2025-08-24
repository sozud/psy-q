CC = ccpsx -O3 -Xo$80040000

all: tuto0 tuto1 
	
load:
	pqbload dino.tmd   80100000
	pqbload earth.tmd  80120000
	pqbload earth4.tim 80140000

tuto0.obj : tuto0.c
	$(CC) -c tuto0.c
	dmpsx tuto0.obj

tuto1.obj : tuto1.c
	$(CC) -c tuto1.c
	dmpsx tuto1.obj

tuto0: tuto0.obj
	$(CC) tuto0.obj db.c -otuto0.cpe,tuto0.sym

tuto1: tuto1.obj
	$(CC) tuto1.obj db.c -otuto1.cpe,tuto1.sym

apd_save:
	$(CC) -c -DAPD_SAVE tuto0.c
	dmpsx tuto0.obj
	$(CC) -DAPD_SAVE tuto0.obj db.c \psx\lib\autopad.obj \
	-otmd0s.cpe
	del tuto0.obj
	$(CC) -c -DAPD_SAVE tuto1.c
	dmpsx tuto1.obj
	$(CC) -DAPD_SAVE tuto1.obj db.c \psx\lib\autopad.obj \
	-otmd1s.cpe
	del tuto1.obj

apd_load:
	$(CC) -c -DAPD_LOAD tuto0.c
	dmpsx tuto0.obj
	$(CC) -DAPD_LOAD tuto0.obj db.c \psx\lib\autopad.obj \
	-otmd0l.cpe
	del tuto0.obj
	$(CC) -c -DAPD_LOAD tuto1.c
	dmpsx tuto1.obj
	$(CC) -DAPD_LOAD tuto1.obj db.c \psx\lib\autopad.obj \
	-otmd1l.cpe
	del tuto1.obj

for_menu: tuto0.obj tuto1.obj
	$(CC) \psx\lib\none3.obj tuto0.obj db.c -otuto0.cpe,tuto0.sym
	$(CC) \psx\lib\none3.obj tuto1.obj db.c -otuto1.cpe,tuto1.sym
	cpe2x tuto0.cpe
	cpe2x tuto1.cpe
	del tuto0.cpe
	del tuto1.cpe

