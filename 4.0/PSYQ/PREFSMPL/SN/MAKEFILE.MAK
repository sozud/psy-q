
OBJS = c:\psx\lib\2mbyte.obj menu.obj snd.obj fonttex1.obj balls.obj rcube.obj anim.obj address.obj mmatrix.obj oobjt.obj oobjt2.obj gs__001.obj gs__002.obj gs__010.obj gs__101.obj gs__103.obj gs__104.obj gs__105.obj gs__106.obj gs__107.obj gs__108.obj gs__109.obj gs__110.obj gs__111.obj gs__113.obj gs__122.obj gs__124.obj gs__131.obj gs__134.obj gglobal.obj ddag3gf0.obj ddag3gf1.obj ddag3gf2.obj ddaf3gf0.obj ddaf3gf1.obj ddaf3gf2.obj gs__003.obj gs__119.obj gs__121.obj gs__123.obj gs__133.obj

all: $(OBJS)
	psylink /c /m @menu.lnk,menu.cpe,menu.sym,menu.map
        cpe2x menu.cpe
        del overmenu.exe
        ren menu.exe overmenu.exe

fonttex1.obj : fonttex1.c
	ccpsx -O3 -c fonttex1.c
menu.obj : menu.c
	ccpsx -O3 -c -DSOUND menu.c
snd.obj : snd.c
	ccpsx -O3 -c snd.c	

balls.obj : balls.c
	ccpsx -O3 -G0 -c -Wa,sballs -DOVERLAY balls.c	
# <-G0> 		not use global pointer(GP)
#			グローバルポインタ(GP)を使用しない。
# <-Wa,s[name]>		define this module as group 'name'
#			このモジュールを group name として定義する。
rcube.obj : rcube.c
        ccpsx -O3 -G0 -c -Wa,srcube -DOVERLAY rcube.c
anim.obj : anim.c
	ccpsx -O3 -G0 -c -Wa,sanim -DOVERLAY anim.c	

address.obj: address.s
	asmpsx /l address,address

ddag3gf0.obj: dag3gf00.obj
        prefsect dag3gf00.obj ddag3gf0.obj ddag3gf0

ddag3gf1.obj: dag3gf01.obj
        prefsect dag3gf01.obj ddag3gf1.obj ddag3gf1

ddag3gf2.obj: dag3gf02.obj
        prefsect dag3gf02.obj ddag3gf2.obj ddag3gf2

ddaf3gf0.obj: daf3gf00.obj
        prefsect daf3gf00.obj ddaf3gf0.obj ddaf3gf0

ddaf3gf1.obj: daf3gf01.obj
        prefsect daf3gf01.obj ddaf3gf1.obj ddaf3gf1

ddaf3gf2.obj: daf3gf02.obj
        prefsect daf3gf02.obj ddaf3gf2.obj ddaf3gf2

gglobal.obj: global.obj
        prefsect global.obj gglobal.obj gglobal

mmatrix.obj: matrix.obj
        prefsect matrix.obj mmatrix.obj mmatrix

oobjt.obj: objt.obj
        prefsect objt.obj oobjt.obj oobjt

oobjt2.obj: objt2.obj
        prefsect objt2.obj oobjt2.obj oobjt2

gs__001.obj: gs_001.obj
        prefsect gs_001.obj gs__001.obj 001

gs__002.obj: gs_002.obj
        prefsect gs_002.obj gs__002.obj 002

gs__003.obj: gs_003.obj
        prefsect gs_003.obj gs__003.obj 003

gs__010.obj: gs_010.obj
        prefsect gs_010.obj gs__010.obj 010

gs__101.obj: gs_101.obj
        prefsect gs_101.obj gs__101.obj 101

gs__103.obj: gs_103.obj
        prefsect gs_103.obj gs__103.obj 103

gs__104.obj: gs_104.obj
        prefsect gs_104.obj gs__104.obj 104

gs__105.obj: gs_105.obj
        prefsect gs_105.obj gs__105.obj 105

gs__106.obj: gs_106.obj
        prefsect gs_106.obj gs__106.obj 106

gs__107.obj: gs_107.obj
        prefsect gs_107.obj gs__107.obj 107

gs__108.obj: gs_108.obj
        prefsect gs_108.obj gs__108.obj 108

gs__109.obj: gs_109.obj
        prefsect gs_109.obj gs__109.obj 109

gs__110.obj: gs_110.obj
        prefsect gs_110.obj gs__110.obj 110

gs__111.obj: gs_111.obj
        prefsect gs_111.obj gs__111.obj 111

gs__113.obj: gs_113.obj
        prefsect gs_113.obj gs__113.obj 113

gs__119.obj: gs_119.obj
        prefsect gs_119.obj gs__119.obj 119

gs__121.obj: gs_121.obj
        prefsect gs_121.obj gs__121.obj 121

gs__122.obj: gs_122.obj
        prefsect gs_122.obj gs__122.obj 122

gs__123.obj: gs_123.obj
        prefsect gs_123.obj gs__123.obj 123

gs__124.obj: gs_124.obj
        prefsect gs_124.obj gs__124.obj 124

gs__131.obj: gs_131.obj
        prefsect gs_131.obj gs__131.obj 131

gs__133.obj: gs_133.obj
        prefsect gs_133.obj gs__133.obj 133

gs__134.obj: gs_134.obj
        prefsect gs_134.obj gs__134.obj 134
