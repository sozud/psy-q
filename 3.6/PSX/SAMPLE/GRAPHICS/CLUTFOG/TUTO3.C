/* $PSLibId: Runtime Library Release 3.6$ */
/***********************************************
 *	
 *	clut �ɂ��e�N�X�`���ւ̃t�H�O����
 *
 *	"tuto3.c"
 *
 *	Copyright (C) 1996 Sony Computer Entertainment Inc.
 *		All Rights Reserved.
 ***********************************************/
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH  4		/* �I�[�_�����O�e�[�u���̉𑜓x */
#define PROJECTION (300*2)

#define TEXTURE_ADDR ((u_long *)0x80100000)

/* �w�i�F */
#define BGR 0x60					/* BG color Red */
#define BGG 0x60					/* BG color Green */
#define BGB 0x60					/* BG color Blue */

/* �t�H�O�̐[���̐ݒ� */
#define FOGNEAR 5000
#define FOGFAR 50000

/* �|���S����u�����W�̌��_�̐[�� */
#define INITZ 1000

/* �|���S���z�u�̏c���̐� */
#define HNUM 3
#define WNUM 3

/* �t�H�O�̉𑜓x */
#define DLNUM (HNUM*WNUM)

/* 1 �� DR_LOAD �œ]���ł���o�C�g�� */
#define SIZEOF_DRLOAD_P 44
/* 256pixel ��]������̂ɕK�v�� DR_LOAD �̐� */
#define PNUM 12

#define Printf(x) FntPrint x

int main(void);
void init_all(void);
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org);
int obj_interactive(u_long padd);
void poly_init(void);
void draw_init(void);
void fnt_init(void);
int view_init(void);
int coord_init(void);
int set_coordinate(SVECTOR *pos, GsCOORDINATE2 *coor);
void texture_init(u_long *addr);
void drload_init(void);
void add_drld(u_long *ot, u_long p, DR_LOAD *dl);
void make_clut_p(u_short *ctim, u_long p, u_short *nc);
void set_clut_drload(u_short *nc, DR_LOAD *dl);

GsOT            Wot[2];			/* �I�[�_�����O�e�[�u���n���h�� */
GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* �I�[�_�����O�e�[�u������ */

GsCOORDINATE2   DWorld;		/* �I�u�W�F�N�g���Ƃ̍��W�n */
SVECTOR PWorld;			/* ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */

POLY_FT4 ft4[2][HNUM][WNUM];			/* polygon */
SVECTOR fpos[HNUM][WNUM][4];			/* polygon vertex positions */

/* CLUT �������̂��߂� DR_LOAD �p�P�b�g */
DR_LOAD drld[2][DLNUM][PNUM];

GsIMAGE tim1;					/* texture for ft4 */

int main(void)
{
    u_long fn;
    u_long padd=0;		/* �R���g���[���̃f�[�^��ێ����� */
    int outbuf_idx=0;

    ResetCallback();
    PadInit(0);			/* �R���g���[�������� */
    SetGraphDebug(2);
    InitGeom();
    
    init_all();

    /* main loop */
    for (fn=0; ;fn++){
#ifdef FNDUMP
	Printf(("%08X\n",fn));
#endif /* FNDUMP */
	outbuf_idx=GsGetActiveBuff();		/* get one of buffers */

	if (obj_interactive(padd)<0) return 0; /* pad data handling */

	GsClearOt(0,0,&Wot[outbuf_idx]);	/* clear ordering table */

	/* draw polygon(s) */
	draw_poly(&DWorld, outbuf_idx, Wot[outbuf_idx].org);

	padd=PadRead(1);			/* read pad */

	DrawSync(0);				/* wait drawing done */

	VSync(0);				/* wait Vertical Sync */

	GsSwapDispBuff();			/* switch double buffers */
	
	/* ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
	GsSortClear(BGR,BGG,BGB,&Wot[outbuf_idx]); 
	/* �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
	GsDrawOt(&Wot[outbuf_idx]);
#ifdef FNDUMP
	FntFlush(-1);				/* draw print-stream */
#endif /* FNDUMP */
    }
}


/* draw polygon(s) */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org)
{
    int i,j;
    long otz, flag;
    MATRIX tmpls;
    POLY_FT4 *f;
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
		&& p<4096){			/* depth cue is not full */
		/* OT �֓o�^ */
		AddPrim(org+(otz>>(14-OT_LENGTH)), f);

		/* �Ή����� CLUT �̍쐬�Ɠo�^ */
		add_drld(((u_long *)org)+(otz>>(14-OT_LENGTH)), p, &drld[idx][i*WNUM+j][0]);
	    }
	    f++;
	}
    }
}

/* pad data handling */
int obj_interactive(u_long padd)
{
#define DT (30*10)
#define VT 25
#define ZT (100*5)
#define FT 50

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

    if (padd & PADk){
	/* �v���O�����I�� */
	PadStop();
	ResetGraph(3);
	StopCallback();
	return -1;
    }

    set_coordinate(&PWorld,&DWorld);
    return 0;
}

/* ���������[�`���Q */
void init_all(void)
{
    ResetGraph(0);		/* GPU���Z�b�g */

    draw_init();
    coord_init();
    view_init();
#ifdef FNDUMP
    fnt_init();
#endif /* FNDUMP */

    SetFogNearFar(FOGNEAR, FOGFAR, PROJECTION);	/* set fog depth */
    SetFarColor(BGR, BGG, BGB);			/* for depth cue */

    texture_init(TEXTURE_ADDR);
    poly_init();
    drload_init();

    return;
}

void poly_init(void)
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
		f->clut=GetClut(tim1.cx, tim1.cy); /* clut set */
		/* texture page */
		f->tpage=GetTPage(tim1.pmode,0,tim1.px,tim1.py);

		setRGB0(f, 128,128,128);
		f++;
	    }
	}
    }

    /* �|���S���z�u */
#define SIDELONG (100*5)
#define WSPACE (500*5)
#define HSPACE (500*5)
#define WOFFSET (-(WNUM/2) * WSPACE )
#define HOFFSET (-(HNUM/2) * HSPACE )
#define ZOFFSET 0
#define ZSPACE (1500)

    /* set vertex positions */
    for (i=0; i<HNUM; i++){
	for (j=0; j<WNUM; j++){
	    z= ((HNUM-((i+(HNUM/2))%HNUM))*WNUM)+(WNUM-((j+(WNUM/2))%WNUM));
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


void draw_init(void)
{
    GsInitGraph(640,240,GsOFSGPU|GsINTER,1,0);
    GsDefDispBuff(0,0,0,240);	/* �_�u���o�b�t�@�w�� */
    GsInit3D();			/* �RD�V�X�e�������� */

    Wot[0].length=OT_LENGTH;	/* �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */
    Wot[0].org=zsorttable[0];	/* �I�[�_�����O�e�[�u���n���h����
					   �I�[�_�����O�e�[�u���̎��̐ݒ� */
    /* �_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
    Wot[1].length=OT_LENGTH;
    Wot[1].org=zsorttable[1];
}

void fnt_init(void)
{
    FntLoad(960, 256);				/* font load */
    SetDumpFnt(FntOpen(-290,-100,400,100,0,200)); /* stream open & define */
}

int view_init(void)
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


int coord_init(void)
{
    /* ���W�̒�` */
    GsInitCoordinate2(WORLD,&DWorld);
    DWorld.coord.t[2]= INITZ;

    /* �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
    PWorld.vx=PWorld.vy=PWorld.vz=0;

    set_coordinate(&PWorld,&DWorld);
}

/* ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
int set_coordinate(SVECTOR *pos, GsCOORDINATE2 *coor)
{
    MATRIX tmp1;
    SVECTOR v1;

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


void texture_init(u_long *addr)
{
    RECT rect1;

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

    /* CLUT �� OT ���s���� DR_LOAD �œ]������̂ł����ł͂Ȃɂ����Ȃ� */
/*    LoadClut(tim1.clut, tim1.cx, tim1.cy);*/

}

void drload_init(void)
{
    RECT rect;
    int i,j,k;
    

    rect.y=tim1.cy;
    rect.h=tim1.ch;
    
    for (k=0; k<PNUM; k++){
	rect.x=tim1.cx+ (k*SIZEOF_DRLOAD_P/2);
	if (k<PNUM-1) rect.w=SIZEOF_DRLOAD_P/2;
	else rect.w=tim1.cw-((PNUM-1)*SIZEOF_DRLOAD_P/2);
	for (i=0; i<2; i++){
	    for (j=0; j<DLNUM; j++){
		SetDrawLoad(&drld[i][j][k], &rect);
	    }
	}
    }
    return;
}


void add_drld(u_long *ot, u_long p, DR_LOAD *dl)
{
    int i;
    static u_short newclut[256];

    /* �[���ɉ������t�H�O�̂������� clut ���쐬 */
    make_clut_p((u_short *)tim1.clut, p, newclut);

    /* DR_LOAD �p�P�b�g�ɃZ�b�g */
    set_clut_drload(newclut, dl);

    /* CLUT ���������p DR_LOAD �p�P�b�g�� OT �֓o�^ */
    for (i=0; i<PNUM; i++){
	AddPrim(ot, &dl[i]);
    }

}

/* depth cue �̂������� clut ���쐬���� */
void make_clut_p(u_short *ctim, u_long p, u_short *nc)
{
    int i;
    CVECTOR orgc, newc;
    u_short stp;

    for (i=0; i<tim1.cw; i++){
	if (ctim[i]==0){
	    /* as it is when transparent clut: */
	    /* �����̂Ƃ��͓��}���Ȃ� */
	    nc[i]=ctim[i];
	} else{
	    /* decompose the clut into RGB : �� CLUT �� R,G,B �ɕ��� */
	    orgc.r= (ctim[i] & 0x1f)<<3;
	    orgc.g= ((ctim[i] >>5) & 0x1f)<<3;
	    orgc.b= ((ctim[i] >>10) & 0x1f)<<3;
	    /* �������r�b�g�̕ۑ� */
	    stp= ctim[i] & 0x8000;

	    /* FarColor �ƌ� clut ����}���� */
	    DpqColor(&orgc, p, &newc);

	    /* reconstruct depth cued clut : FOG �� CLUT �̍č\�� */
	    nc[i]=
		stp
		    | (newc.r>>3)
			| (((unsigned long)(newc.g&0xf8))<<2)
			    | (((unsigned long)(newc.b&0xf8))<<7);
	}
    }
}

/* depth cue �̂������� CLUT �� DR_LOAD �p�P�b�g�ɃZ�b�g���� */
void set_clut_drload(u_short *nc, DR_LOAD *dl)
{
    int i,j,k;

    i=0;
    k=0;
    for (j=0; j<tim1.cw; j+=2){
	dl[i].p[k]=
	    (((u_long)nc[j])&0xffff) |
		((((u_long)nc[j+1])&0xffff)<<16);
	k++;
	if (k==SIZEOF_DRLOAD_P/sizeof(long)){
	    k=0;
	    i++;
	}
    }
}