# If you link 2mbyte.obj instead of none3.obj, this program can start alone.
# none3.objの代りに2mbyte.objをリンクすれば、単体で起動できる。

all: anim anim2 exe
	
anim:
	ccpsx -O3 -Xo$80050000 main.c ..\..\..\..\lib\none3.obj -oanim.cpe,anim.sym,anim.map
	
anim2:
	ccpsx -O3 -Xo$80010000 -DDEAD_PARENT main.c ..\..\..\..\lib\none3.obj -oanim2.cpe,anim2.sym,anim2.map

apd_save:
	ccpsx -O3 -Xo$80050000 -DAPD_SAVE main.c ..\..\..\..\lib\none3.obj \
	\psx\lib\autopad.obj -oanims.cpe
	cpe2x anims.cpe

apd_load:
	ccpsx -O3 -Xo$80050000 -DAPD_LOAD main.c ..\..\..\..\lib\none3.obj \
	\psx\lib\autopad.obj -oaniml.cpe
	cpe2x animl.cpe

exe:
	cpe2x anim.cpe
	cpe2x anim2.cpe
