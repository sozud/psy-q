/* $PSLibId: Runtime Library Release 3.6$ */
/***********************************************
 *	
 *	clut �ɂ��e�N�X�`���ւ̃t�H�O����
 *
 *	"tuto2.c" main routine
 *
 *		Version 1.00	Sep.  29, 1995
 *
 *	Copyright (C) 1995 Sony Computer Entertainment Inc.
 *		All Rights Reserved.
 ***********************************************/
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH  10		/* �I�[�_�����O�e�[�u���̉𑜓x */
#define PROJECTION 300

#define BGR 100					/* BG color Red */
#define BGG 100					/* BG color Green */
#define BGB 100					/* BG color Blue */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org);
int obj_interactive(u_long fn, u_long padd);
void main();

GsOT            Wot[2];			/* �I�[�_�����O�e�[�u���n���h�� */
GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* �I�[�_�����O�e�[�u������ */


#define HNUM 5
#define WNUM 5

POLY_FT4 ft4[2][HNUM][WNUM];			/* polygon */
SVECTOR fpos[HNUM][WNUM][4];			/* polygon vertex positions */

GsCOORDINATE2   DWorld;		/* �I�u�W�F�N�g���Ƃ̍��W�n */
SVECTOR PWorld;			/* ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */
extern MATRIX GsIDMATRIX;	/* Unit Matrix /* �P�ʍs�� */



RECT crect1;
CVECTOR cv1[256];				/* original clut */
short clutw;

int  outbuf_idx=0;

#define CBIT 6
#define CNUM (1<<CBIT)

GsIMAGE tim1;
long fognear, fogfar;
static short pbuf[4096];			/*  */

DR_MOVE mvi[2][CNUM];


/************* MAIN START ******************************************/
void main()
{
    u_long fn;
    u_long padd=0;		/* �R���g���[���̃f�[�^��ێ����� */
    int i,j;

    ResetCallback();
    PadInit(0);			/* �R���g���[�������� */
    init_all();


    /* main loop */
    for (fn=0;;fn++){
	outbuf_idx=GsGetActiveBuff();		/* get one of buffers */

	if (obj_interactive(fn, padd)<0) return; /* pad data handling */

	GsClearOt(0,0,&Wot[outbuf_idx]);	/* clear ordering table */

	/* draw polygon(s) */
	draw_poly(&DWorld, outbuf_idx, Wot[outbuf_idx].org);

	padd=PadRead(1);			/* read pad */

	DrawSync(0);				/* wait drawing done */

	VSync(0);				/* wait Vertical Sync */

	GsSwapDispBuff();			/* switch double buffers */
	
	/* ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
	GsSortClear(BGR,BGG,BGB,&Wot[outbuf_idx]); 

	ResetGraph(1);
	/* �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
	GsDrawOt(&Wot[outbuf_idx]);
	FntFlush(-1);				/* draw print-stream */
    }
    return;
}


/* draw polygon(s) */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org)
{
    int i,j;
    long otz, flag;
    MATRIX tmpls;
    POLY_FT4 *f;
    long nclip;
    long p;

    /* ���W���v�Z */
    GsGetLs(co, &tmpls);
    /* GTE�ɃZ�b�g */
    GsSetLsMatrix(&tmpls);

    f= &ft4[idx][0][0];
    for (i=0; i<HNUM; i++){
	for (j=0; j<WNUM; j++){
	    otz=RotTransPers4(&fpos[i][j][0],
			      &fpos[i][j][1],
			      &fpos[i][j][2],
			      &fpos[i][j][3],
			    (long *)(&f->x0),
			    (long *)(&f->x1),
			    (long *)(&f->x2),
			    (long *)(&f->x3),
			    &p, &flag
			    );

	    if (flag >=0				/* no error */
		&& otz>0				/* otz OK */
		&& p<4096){				/* depth queue is not full */

		otz>>=(14-OT_LENGTH);
		p>>=12-CBIT;
		/* �|���S���̑��݂���ꏊ��OTZ��fog�p�����[�^���o���Ă��� */
		if (pbuf[p]<otz) pbuf[p]=otz;
		/* OT �֓o�^ */
		AddPrim(org+otz, f);
	    }
	    f++;
	}
    }


    for (i=0; i<CNUM; i++){
	/* �Y������[���Ƀ|���S�������݂����� CLUT �]���p�P�b�g��o�^ */
	/*(libgs �ɂ��`��ȂǁA�|���S�����ݔ͈͂��킩��Ȃ��Ƃ��͂��ׂēo�^)*/
	if (pbuf[i]>=0){
	    AddPrim(org+pbuf[i], &mvi[idx][i]);
	    pbuf[i]= -1;
	}
    }
}

/* pad data handling */
int obj_interactive(u_long fn, u_long padd)
{
#define DT 30
#define VT 30
#define ZT 100
#define FT 10
    static u_long oldpadd=0;

    if (padd & PADk){
#if 0
	if (padd & PADLdown) fognear-=FT;
	if (padd & PADLup) fognear+=FT;
	if (fognear<0) fognear=0;
	SetFogNearFar(fognear, fogfar, PROJECTION);
#endif /* 0 */
    } else if (padd & PADh){
#if 0
	if (padd & PADLdown) fogfar-=FT;
	if (padd & PADLup) fogfar+=FT;
	if (fogfar<(fognear*2)) fogfar=fognear*2;
	SetFogNearFar(fognear, fogfar, PROJECTION);
#endif /* 0 */
    } else{				/* translate and rotate */
	if (padd & PADLleft) DWorld.coord.t[0]-=DT;
	else if (padd & PADLright) DWorld.coord.t[0]+=DT;
	
	if (padd & PADLup) DWorld.coord.t[1]-=DT;
	else if (padd & PADLdown) DWorld.coord.t[1]+=DT;
	
	if (padd & PADo) DWorld.coord.t[2]+=ZT;
	else if (padd & PADn) DWorld.coord.t[2]-=ZT;
	
	if (padd & PADRleft) PWorld.vy-=VT;
	else if (padd & PADRright) PWorld.vy+=VT;
	
	if (padd & PADRup) PWorld.vx+=VT;
	else if (padd & PADRdown) PWorld.vx-=VT;
	
	if (padd & PADm) PWorld.vz-=VT;
	else if (padd & PADl) PWorld.vz+=VT;
    }

    if (padd & PADk){
	/* �v���O�����I�� */
	PadStop();
	ResetGraph(3);
	StopCallback();
	return -1;
    }
/*    FntPrint("%d,%d\n",fognear, fogfar);*/

    set_coordinate(&PWorld,&DWorld);
    oldpadd=padd;
    return 0;
}

init_all()			/* ���������[�`���Q */
{
    ResetGraph(0);		/* GPU���Z�b�g */

    draw_init();
    coord_init();
    view_init();
    fnt_init();
    
    /* follow 4lines are used by texture_init() */
    fognear=1000;
    fogfar=4000;
    SetFogNearFar(fognear, fogfar, PROJECTION);	/* set fog parameter */
    SetFarColor(BGR, BGG, BGB);

    texture_init((u_long *)0x80100000);
    poly_init();


}

poly_init()
{
    int i,j,k;
    POLY_FT4 *f;
    int z;

    f= &ft4[0][0][0];
    for (k=0; k<2; k++){
	for (i=0; i<HNUM; i++){
	    for (j=0; j<WNUM; j++){
		setPolyFT4(f);			/* initialize primitive */
		SetShadeTex(f,1);			/* shading off */
		setUV4(f,
		       0, 0,
		       255, 0,
		       0, 255,		       
		       255, 255
		       );			/* set texture position */
		f->clut=GetClut(tim1.cx, tim1.cy); /* clut (temporary) */
		/* texture page */
		f->tpage=GetTPage(tim1.pmode,0,tim1.px,tim1.py);
		f++;
	    }
	}
    }
#define SIDELONG 100
#define WSPACE 500
#define HSPACE 500
#define WOFFSET (-(WNUM/2) * WSPACE )
#define HOFFSET (-(HNUM/2) * HSPACE )
#define ZOFFSET 0
#define ZSPACE 750

	    /* set vertex positions */
    for (i=0; i<HNUM; i++){
	for (j=0; j<WNUM; j++){
	    z=abs(i-(HNUM/2))+abs(j-(HNUM/2));
	    fpos[i][j][0].vx= (WSPACE*j)+ WOFFSET-SIDELONG;
	    fpos[i][j][0].vy= (HSPACE*i)+ HOFFSET-SIDELONG;
	    fpos[i][j][0].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][1].vx= (WSPACE*j)+ WOFFSET+SIDELONG;
	    fpos[i][j][1].vy= (HSPACE*i)+ HOFFSET-SIDELONG;
	    fpos[i][j][1].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][2].vx= (WSPACE*j)+ WOFFSET-SIDELONG;
	    fpos[i][j][2].vy= (HSPACE*i)+ HOFFSET+SIDELONG;
	    fpos[i][j][2].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][3].vx= (WSPACE*j)+ WOFFSET+SIDELONG;
	    fpos[i][j][3].vy= (HSPACE*i)+ HOFFSET+SIDELONG;
	    fpos[i][j][3].vz= (ZSPACE*z)+ ZOFFSET;
	}
    }
}


draw_init()
{
/*  GsInitGraph(640,480,2,1,0);	/* �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
/*  GsDefDispBuff(0,0,0,0);	/* �_�u���o�b�t�@�w�� */
    GsInitGraph(640,240,GsOFSGPU|GsINTER,1,0);	/* �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
    GsDefDispBuff(0,0,0,240);	/* �_�u���o�b�t�@�w�� */
    GsInit3D();			/* �RD�V�X�e�������� */

    Wot[0].length=OT_LENGTH;	/* �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */
    Wot[0].org=zsorttable[0];	/* �I�[�_�����O�e�[�u���n���h����
					   �I�[�_�����O�e�[�u���̎��̐ݒ� */
    /* �_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
    Wot[1].length=OT_LENGTH;
    Wot[1].org=zsorttable[1];
}

fnt_init()
{
    FntLoad(960, 256);				/* font load */
    SetDumpFnt(FntOpen(-290,-100,400,100,0,200)); /* stream open & define */
}

int view_init()
{
    GsRVIEW2  view;		/* View Point Handler*/

    /*---- Set projection,view ----*/
    GsSetProjection(PROJECTION);	/* Set projection /* �v���W�F�N�V�����ݒ� */
  
    /* Setting view point location /* ���_�p�����[�^�ݒ� */
    view.vpx = 0; view.vpy = 0; view.vpz = -2000;
  
    /* Setting focus point location /* �����_�p�����[�^�ݒ� */
    view.vrx = 0; view.vry = 0; view.vrz = 0;
  
    /* Setting bank of SCREEN /* ���_�̔P��p�����[�^�ݒ� */
    view.rz=0;

    /* Setting parent of viewing coordinate /* ���_���W�p�����[�^�ݒ� */
    view.super = WORLD;
  
    /* Calculate World-Screen Matrix from viewing paramter*/
    /* ���_�p�����[�^���Q���王�_��ݒ肷��*/
    /*���[���h�X�N���[���}�g���b�N�X���v�Z���� */
    GsSetRefView2(&view);
  
    GsSetNearClip(100);           /* Set Near Clip /* �j�A�N���b�v�ݒ� */
}


int coord_init()
{
    /* ���W�̒�` */
    GsInitCoordinate2(WORLD,&DWorld);
    DWorld.coord.t[2]= -1000;

    /* �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
    PWorld.vx=PWorld.vy=PWorld.vz=0;
/*    PWorld.vx=2048;*/

    set_coordinate(&PWorld,&DWorld);
}

/* ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
int set_coordinate(SVECTOR *pos, GsCOORDINATE2 *coor)
{
    MATRIX tmp1;
    SVECTOR v1;
    int i,j;

    tmp1   = GsIDMATRIX;		/* �P�ʍs�񂩂�o������ */
    /* ���s�ړ����Z�b�g���� */
    tmp1.t[0] = coor->coord.t[0];
    tmp1.t[1] = coor->coord.t[1];
    tmp1.t[2] = coor->coord.t[2];

    /* �}�g���b�N�X���[�N�ɃZ�b�g����Ă��郍�[�e�[�V������ */
    /* ���[�N�̃x�N�^�[�ɃZ�b�g */
    v1 = *pos;

    /* �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ */
    RotMatrix(&v1,&tmp1);

    /* ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
    coor->coord = tmp1;

    /* �}�g���b�N�X�L���b�V�����t���b�V������ */
    coor->flg = 0;
}


texture_init(u_long *addr)
{
    RECT rect1;
    int i, j;
    RECT crect_org;

    /* Get texture information of TIM FORMAT*/
       /* TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */  
    GsGetTimInfo(addr+1,&tim1);
    
    rect1.x=tim1.px;		/* X point of image data on VRAM*/
				   /* �e�N�X�`�������VRAM�ł�X���W */
    rect1.y=tim1.py;		/* Y point of image data on VRAM*/
				   /* �e�N�X�`�������VRAM�ł�Y���W */
    rect1.w=tim1.pw;		/* Width of image /* �e�N�X�`���� */
    rect1.h=tim1.ph;		/* Height of image /* �e�N�X�`������ */
  
    /* Load texture to VRAM *//* VRAM�Ƀe�N�X�`�������[�h���� */
    LoadImage(&rect1,tim1.pixel);

    LoadClut(tim1.clut, tim1.cx, tim1.cy);
    crect1.x=640; crect1.y=256; crect1.w=256; crect1.h=1;
    make_clut(&crect1, (u_short *)tim1.clut, CNUM);

    crect_org.w=tim1.cw;
    crect_org.h=tim1.ch;
    crect_org.x=crect1.x;

    for (i=0; i<2; i++){
	for (j=0; j<CNUM; j++){
	    crect_org.y=crect1.y+j;
	    SetDrawMove(&mvi[i][j], &crect_org, tim1.cx, tim1.cy);
	}
    }

    for (i=0; i<4096; i++){
	pbuf[i]= -1;
    }
}

/* make depth queued clut */
make_clut(RECT *rtim, u_short *ctim, long num)
{
    long i,j;
    CVECTOR orgc, newc;
    RECT rect;
    long stp;
    long p;
    u_short newclut[256];

    rect.x=rtim->x;
    rect.w=rtim->w;
    rect.h=rtim->h;

    for (i=0; i<num; i++){
	p=i*ONE/num;				/* p= depth queue parameter */

	for (j=0; j<rtim->w; j++){
	    if (ctim[j]==0){		/* as it is when transparent clut
					   : �����̂Ƃ��͓��}���Ȃ� */
		newclut[j]=ctim[j];
	    } else{
		/* decompose the clut into RGB : �� CLUT �� R,G,B �ɕ��� */
		orgc.r= (ctim[j] & 0x1f)<<3;
		orgc.g= ((ctim[j] >>5) & 0x1f)<<3;
		orgc.b= ((ctim[j] >>10) & 0x1f)<<3;
		stp= ctim[j] & 0x8000;

		/* interpolate each RGB and BG color
		   : R,G,B ���ꂼ���w�i�F�Ɠ��} */
		DpqColor(&orgc, p, &newc);
		/* reconstruct depth queued clut : FOG �� CLUT �̍č\�� */
		newclut[j]=
		    stp
			| (newc.r>>3)
			    | (((unsigned long)(newc.g&0xf8))<<2)
				| (((unsigned long)(newc.b&0xf8))<<7);
	    }
	}
	/* next Y axis */
	rect.y=rtim->y+i;

	/* load clut to VRAM : VRAM �� load */
	LoadImage(&rect,(u_long *)newclut);
    }
}