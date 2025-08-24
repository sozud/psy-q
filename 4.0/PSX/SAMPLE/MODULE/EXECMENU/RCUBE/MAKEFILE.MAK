# If you link 2mbyte.obj instead of none3.obj, this program can start alone.
# none3.objの代りに2mbyte.objをリンクすれば、単体で起動できる。

all: rcube rcube2 exe
	
rcube:
	ccpsx -O3 -Xo$80050000 -DCDREAD main.c ..\..\..\..\lib\none3.obj -orcube.cpe,rcube.sym,rcube.map

rcube2:
	ccpsx -O3 -Xo$80010000 -DCDREAD -DDEAD_PARENT main.c ..\..\..\..\lib\none3.obj -orcube2.cpe,rcube2.sym,rcube2.map

apd_save:
	ccpsx -O3 -Xo$80050000 -DCDREAD -DAPD_SAVE main.c \
	..\..\..\..\lib\none3.obj \psx\lib\autopad.obj -orcubes.cpe
	cpe2x rcubes.cpe

apd_load:
	ccpsx -O3 -Xo$80050000 -DCDREAD -DAPD_LOAD main.c \
	..\..\..\..\lib\none3.obj \psx\lib\autopad.obj -orcubel.cpe
	cpe2x rcubel.cpe

exe:
	cpe2x rcube.cpe
	cpe2x rcube2.cpe
