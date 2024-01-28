# none2.objの代りに2mbyte.objをリンクすれば、単体で起動できる。

all:	balls balls2 exe
	
balls:
	ccpsx -O3 -Xo$80040000 main.c ..\..\..\..\lib\none2.obj -oballs.cpe,balls.sym,balls.map

balls2:
	ccpsx -O3 -Xo$80010000 -DDEAD_PARENT main.c ..\..\..\..\lib\none2.obj -oballs2.cpe,balls2.sym,balls2.map

exe:
	cpe2x balls.cpe
	cpe2x balls2.cpe
