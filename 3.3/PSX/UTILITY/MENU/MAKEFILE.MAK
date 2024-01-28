
OPT  = -g -O
OPT2 = -g -O -DCDEXEC
OPT3 = -g -O -DCDEXEC2

#all: pcmenu
all: cdmenu
	
pcmenu:  menu.obj pcexec.obj pcload.obj printerr.obj \
	 string.obj sound.obj sdata.obj
	 psylink /c /m @pcmenu.lnk,pcmenu.cpe,pcmenu.sym,pcmenu.map

cdmenu:  menu.obj cdexec.obj cdload.obj printerr.obj \
	 string.obj sound.obj sdata.obj cdsfile.obj
	 psylink /c /m @cdmenu.lnk,cdmenu.cpe,cdmenu.sym,cdmenu.map

cdmenu2: menu.obj cdexec.obj cdload2.obj printerr.obj \
	 string.obj sound.obj sdata.obj cdsfile.obj
	 psylink /c /m @cdmenu2.lnk,cdmenu2.cpe,cdmenu2.sym,cdmenu2.map

menu.obj : menu.c menu.h errmsg.h
	ccpsx $(OPT) -c menu.c

pcexec.obj : exec.c menu.h errmsg.h
	ccpsx $(OPT) -c exec.c -o pcexec.obj

cdexec.obj : exec.c menu.h errmsg.h
	ccpsx $(OPT2) -c exec.c -o cdexec.obj

pcload.obj : loadmenu.c menu.h loadmenu.h
	ccpsx $(OPT) -c loadmenu.c -o pcload.obj

cdload.obj : loadmenu.c menu.h loadmenu.h
	ccpsx $(OPT2) -c loadmenu.c -o cdload.obj

cdload2.obj : loadmenu.c menu.h loadmenu.h
	ccpsx $(OPT3) -c loadmenu.c -o cdload2.obj

printerr.obj : printerr.c menu.h errmsg.h
	ccpsx $(OPT) -c printerr.c

string.obj : string.c
	ccpsx $(OPT) -c string.c

sound.obj : sound.c
	ccpsx $(OPT) -c sound.c

sdata.obj : sdata.c
	ccpsx $(OPT) -c sdata.c

clean:
	del *.obj
	del *.cpe
