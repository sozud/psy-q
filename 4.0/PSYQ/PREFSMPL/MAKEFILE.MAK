
OBJS = menu.obj snd.obj fonttex1.obj balls.obj anim.obj rcube.obj address.obj

all: $(OBJS)
	psylink /v /c /m /n512 @menu.lnk,menu.cpe,menu.sym,menu.map
	cpe2x menu.cpe
	del overmenu.exe
	ren menu.exe overmenu.exe

fonttex1.obj : fonttex1.c
	ccpsx -g -O3 -c fonttex1.c
menu.obj : menu.c
	ccpsx -g -O3 -c menu.c
snd.obj : snd.c
	ccpsx -g -O3 -c snd.c	

balls.obj : balls.c
	ccpsx -g -O3 -G0 -c -Wa,sballs -DOVERLAY balls.c	
rcube.obj : rcube.c
	ccpsx -g -O3 -G0 -c -Wa,srcube -DOVERLAY rcube.c
anim.obj : anim.c
	ccpsx -g -O3 -G0 -c -Wa,sanim -DOVERLAY anim.c	

address.obj: address.s
	asmpsx /l address,address

