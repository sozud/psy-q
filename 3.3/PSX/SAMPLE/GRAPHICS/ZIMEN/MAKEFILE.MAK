CC	 = ccpsx -O3 -Xo$80010000
SRCS     = clip.c pad.c mesh.c debug.c bgtex.c db.c div.obj

all:	tuto0 tuto1 tuto2 tuto3 tuto4 tuto5
	
none2.obj:
	copy \psx\lib\none2.obj .
div.obj: div.c
	$(CC) -c div.c
	dmpsx div.obj
tuto0: div.obj 
	$(CC) tuto0.c $(SRCS) -otuto0.cpe,tuto0.sym
tuto1: div.obj 
	$(CC) -DVIEW_CLIP -DNO_DIV tuto1.c $(SRCS) -otuto1.cpe,tuto1.sym
tuto2: div.obj 
	$(CC) -DNO_DIV tuto1.c $(SRCS) -otuto2.cpe,tuto2.sym
tuto3: div.obj 
	$(CC) -DRAP_ROUND -DNO_DIV tuto1.c $(SRCS) -otuto3.cpe,tuto3.sym
tuto4: div.obj 
	$(CC) -DRAP_ROUND tuto1.c $(SRCS) -otuto4.cpe,tuto4.sym
tuto5: div.obj 
	$(CC) tuto5.c bg.c bgtex8.c -otuto5.cpe,tuto5.sym

for_menu: div.obj none2.obj 
	$(CC) tuto0.c $(SRCS) none2.obj -otuto0.cpe
	$(CC) -DVIEW_CLIP -DNO_DIV tuto1.c $(SRCS) none2.obj -otuto1.cpe
	$(CC) -DNO_DIV tuto1.c $(SRCS) none2.obj -otuto2.cpe
	$(CC) -DRAP_ROUND -DNO_DIV tuto1.c $(SRCS) none2.obj -otuto3.cpe
	$(CC) -DRAP_ROUND tuto1.c $(SRCS) none2.obj -otuto4.cpe
	$(CC) tuto5.c bg.c bgtex8.c none2.obj -otuto5.cpe
	cpe2x tuto0.cpe
	cpe2x tuto1.cpe
	cpe2x tuto2.cpe
	cpe2x tuto3.cpe
	cpe2x tuto4.cpe
	cpe2x tuto5.cpe

