
OBJS = ..\..\..\lib\2mbyte.obj main.obj address.obj
OBJS2= \psx\lib\none2.obj main.obj address.obj

all: $(OBJS)
	psylink /c /m @main.lnk,main.cpe,main.sym,main.map

for_menu: $(OBJS2)
	psylink /c /m @scratch.lnk,scratch.cpe
	cpe2x scratch.cpe
	del   scratch.cpe

main.obj : main.c
	ccpsx -O0 -c main.c
#最適化レベルを変えて試してみて下さい。

address.obj: address.s
	asmpsx /l address,address
