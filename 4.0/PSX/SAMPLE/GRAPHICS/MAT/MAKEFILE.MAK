all:
	ccpsx -g -Xo$80010000 main.c mat0tex.c mat1tex.c mat2tex.c mat3tex.c \
							-omat.cpe,mat.sym

apd_save:
	ccpsx -O -Xo$80010000 -DAPD_SAVE main.c mat0tex.c mat1tex.c \
	mat2tex.c mat3tex.c \psx\lib\autopad.obj -omats.cpe

apd_load:
	ccpsx -O -Xo$80010000 -DAPD_LOAD main.c mat0tex.c mat1tex.c \
	mat2tex.c mat3tex.c \psx\lib\autopad.obj -omatl.cpe

for_menu:
	ccpsx -O -Xo$80010000 main.c \
	mat0tex.c mat1tex.c mat2tex.c mat3tex.c \psx\lib\none3.obj -o mat.cpe
	cpe2x mat.cpe
	del mat.cpe

