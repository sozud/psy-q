
walk:	
	ccpsx -O3 -Xo$80100000 walk.c onpol.c -owalk.cpe,walk.sym

apd_save:
	ccpsx -O3 -Xo$80100000 -DAPD_SAVE walk.c onpol.c \psx\lib\autopad.obj \
	-owalks.cpe
apd_load:
	ccpsx -O3 -Xo$80100000 -DAPD_LOAD walk.c onpol.c \psx\lib\autopad.obj \
	-owalkl.cpe

for_menu:
        ccpsx -O3 -Xo$80100000 walk.c onpol.c \psx\lib\none3.obj -owalk.cpe
        cpe2x walk.cpe
        del walk.cpe

load:
	pqbload venus.pmd 80040000
	#pqbload m2.pmd 80040000
	pqbload c0sv1.pmd 80080000
	pqbload chara0.sv 80090000
	pqbload chara1.sv 800a0000
	pqbload chara2.sv 800b0000
	pqbload chara3.sv 800c0000

