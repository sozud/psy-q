# none2.objの代りに2mbyte.objをリンクすれば、単体で起動できる。

all: anim anim2 exe
	
anim:
	ccpsx -O3 -Xo$80040000 main.c ..\..\..\..\lib\none2.obj -oanim.cpe,anim.sym,anim.map
	
anim2:
	ccpsx -O3 -Xo$80010000 -DDEAD_PARENT main.c ..\..\..\..\lib\none2.obj -oanim2.cpe,anim2.sym,anim2.map

exe:
	cpe2x anim.cpe
	cpe2x anim2.cpe
