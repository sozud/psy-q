/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *       GsOT �� �v���~�e�B�u�� AddPrim ����� (GPU �x�[�X�j
 *
 *	 Version	Date		Design
 *	-----------------------------------------
 *	2.00		Aug,31,1993	masa (original)
 *	2.10		Mar,25,1994	suzu (added addPrimitive())
 *      2.20            Dec,25,1994     suzu (chaned GsDOBJ4)
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define MODEL_ADDR	(u_long *)0x80040000	/* ���f�����O�f�[�^��� */
#define TEX_ADDR	(u_long *)0x80180000	/* �e�N�X�`����� */
#define SCRATCH		(u_long *)0x1f800000	/* �X�N���b�`�p�b�h */
	
#define SCR_Z		1000		/* �X�N���[���܂ł̋��� */
#define OT_LENGTH	12		/* OT�𑜓x��12bit�i�傫���j */
#define OTSIZE		(1<<OT_LENGTH)	/* �^�O�̑傫�� */
#define PACKETMAX	4000		/* 1�t���[���̍ő�p�P�b�g�� */
#define PACKETMAX2	(PACKETMAX*24)	/* 1�p�P�b�g���σT�C�Y��24Byte�Ƃ��� */

GsDOBJ2		object;		/*  �I�u�W�F�N�g�ϐ� */
GsRVIEW		view;		/* ���_��� */
GsF_LIGHT	pslt[3];	/* �����~�R�� */
u_long		PadData;	/* �R���g���[���p�b�h��� */

SVECTOR         PWorld; /* work short vector for making Coordinate parmeter
			   /* ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */
GsCOORDINATE2   DWorld;	/* Coordinate for GsDOBJ2
			   /* �I�u�W�F�N�g���Ƃ̍��W�n */
	
extern MATRIX	GsIDMATRIX;

/*
 *  �_�u���o�b�t�@��
 */
typedef struct {
	DRAWENV		draw;			/* �`��� */
	DISPENV		disp;			/* �\���� */		
	GsOT 		gsot;			/* GS OT��� */
	u_long		ot[OTSIZE];		/* OT */
	PACKET		gsprim[PACKETMAX2];	/* �v���~�e�B�u�o�b�t�@ */
} DB;

void initSystem(DB *db);
void initPrimitives();
void drawObject(DB *db);

/*
 *  ���C�����[�`��
 */
main()
{
	/* �_�u���o�b�t�@ */
	DB	db[2];
	DB	*cdb;

	/* Initialize: �C�j�V�����C�Y */
	initSystem(db);			/* grobal variables */
	initView();			/* position matrix */
	initLight();			/* light matrix */
	initModelingData(MODEL_ADDR);	/* load model data */
	initTexture(TEX_ADDR);		/* load texture pattern */
	initPrimitives();		/* GPU primitives */
	
	/* �t�H���g�p�^�[���̃��[�h */
	FntLoad(960, 256);		
	SetDumpFnt(FntOpen(16, 16, 256, 200, 0, 512));

	
	/* ���C�����[�v */	
	while(1) {
		cdb  = (cdb==db)? db+1: db;	/* swap double buffer ID */
		if ( moveObject() ) break;
		drawObject(cdb);
	}
	PadStop();
	StopCallback();
	return 0;
}

/*
 *  3D�I�u�W�F�N�g�`�揈��
 */
void drawObject(DB *cdb)
{
	MATRIX tmpls;
	
	/* GPU�p�P�b�g�����A�h���X���G���A�̐擪�ɐݒ� */
	GsSetWorkBase(cdb->gsprim);
	
	/* OT�̓��e���N���A */
	ClearOTagR(cdb->ot, OTSIZE);
	
	/* 3D�I�u�W�F�N�g�i�L���[�u�j��OT�ւ̓o�^ */
	GsGetLw(object.coord2,&tmpls);		/* �}�g���N�X���� */
	GsSetLightMatrix(&tmpls);
	GsGetLs(object.coord2,&tmpls);
	GsSetLsMatrix(&tmpls);
	GsSortObject4(&object, &cdb->gsot,14-OT_LENGTH,SCRATCH);
	
	/* �v���~�e�B�u��ǉ� */
	addPrimitives(cdb->ot);
	
	/* �p�b�h�̓��e���o�b�t�@�Ɏ�荞�� */
	PadData = PadRead(0);

	/* V-BLNK��҂� */
	VSync(0);

	/* �_�u���o�b�t�@����ꊷ���� */
	PutDispEnv(&cdb->disp);
	PutDrawEnv(&cdb->draw);
	SetGeomOffset(cdb->draw.clip.w/2, cdb->draw.clip.h/2); 

	/* OT�̓��e���o�b�N�O���E���h�ŕ`��J�n */
	DrawOTag(cdb->ot+OTSIZE-1);
	FntFlush(-1);
	
	SetDispMask(1);
}

/*
 *  �R���g���[���p�b�h�ɂ��I�u�W�F�N�g�ړ�
 */
moveObject()
{
	/* �I�u�W�F�N�g�ϐ����̃��[�J�����W�n�̒l���X�V */
  
	if(PadData & PADRleft) PWorld.vy -= 5*ONE/360;
	if(PadData & PADRright) PWorld.vy += 5*ONE/360;
	if(PadData & PADRup) PWorld.vx += 5*ONE/360;
	if(PadData & PADRdown) PWorld.vx -= 5*ONE/360;
	
	if(PadData & PADm) DWorld.coord.t[2] -= 200;
	if(PadData & PADo) DWorld.coord.t[2] += 200;
	
	/* Calculate Matrix from Object Parameter and Set Coordinate
	/* �I�u�W�F�N�g�̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
	set_coordinate(&PWorld,&DWorld);
	

	/* Clear flag of Coordinate for recalculation
	/* �Čv�Z�̂��߂̃t���O���N���A���� */
	  DWorld.flg = 0;
	
	/* K�{�^���ŏI�� */
	if(PadData & PADselect) 
		return -1;
	return 0;
}

/*
 *  ���������[�`���Q
 */
void initSystem(DB *db)
{
	int	i;
	
	/* �R���g���[���p�b�h�̏����� */
	PadInit(0);
	PadData = 0;
	
	/* GPU�̏����� */
	GsInitGraph(640, 480, 0, 0, 0);	/* �A�X�y�N�g���ݒ� */
	GsDefDispBuff(0, 0, 0, 0);

	/* �_�u���o�b�t�@�̈ʒu�����߂� */
	SetDefDrawEnv(&db[0].draw, 0,   0, 320, 240);
	SetDefDrawEnv(&db[1].draw, 0, 240, 320, 240);
	SetDefDispEnv(&db[0].disp, 0, 240, 320, 240);
	SetDefDispEnv(&db[1].disp, 0,   0, 320, 240);
	
	/* OT�̏����� */
	for (i = 0; i < 2; i++) {
		
		db[i].draw.isbg = 1;
		setRGB0(&db[i].draw, 0, 0, 0);

		db[i].gsot.length = OT_LENGTH;
		db[i].gsot.point  = 0;
		db[i].gsot.offset = 0;
		db[i].gsot.org    = (GsOT_TAG *)db[i].ot;
		db[i].gsot.tag    = (GsOT_TAG *)db[i].ot + OTSIZE - 1;
	}
	
	/* 3D���C�u�����̏����� */
	GsInit3D();
}

/*
 *  ���_�ʒu�̐ݒ�
 */
initView()
{
	/* �v���W�F�N�V�����i����p�j�̐ݒ� */
	GsSetProjection(SCR_Z);

	/* ���_�ʒu�̐ݒ� */
	view.vpx = 0; view.vpy = 0; view.vpz = -1000;
	view.vrx = 0; view.vry = 0; view.vrz = 0;
	view.rz = 0;
	view.super = WORLD;
	GsSetRefView(&view);

	/* Z�N���b�v�l��ݒ� */
	GsSetNearClip(100);
}

/*
 *  �����̐ݒ�i�Ǝ˕������F�j
 */
initLight()
{
	/* ����#0 (100,100,100)�̕����֏Ǝ� */
	pslt[0].vx = 100; pslt[0].vy= 100; pslt[0].vz= 100;
	pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
	GsSetFlatLight(0,&pslt[0]);
	
	/* ����#1�i�g�p�����j */
	pslt[1].vx = 100; pslt[1].vy= 100; pslt[1].vz= 100;
	pslt[1].r=0; pslt[1].g=0; pslt[1].b=0;
	GsSetFlatLight(1,&pslt[1]);
	
	/* ����#2�i�g�p�����j */
	pslt[2].vx = 100; pslt[2].vy= 100; pslt[2].vz= 100;
	pslt[2].r=0; pslt[2].g=0; pslt[2].b=0;
	GsSetFlatLight(2,&pslt[2]);
	
	/* �A���r�G���g�i���ӌ��j�̐ݒ� */
	GsSetAmbient(ONE/2,ONE/2,ONE/2);

	/* �������[�h�̐ݒ�i�ʏ����/FOG�Ȃ��j */
	GsSetLightMode(0);
}

/*
 *  ���������TMD�f�[�^�̓ǂݍ���&�I�u�W�F�N�g������
 *		(�擪�̂P�̂ݎg�p�j
 */
initModelingData(addr)
u_long *addr;
{
	u_long *tmdp;
	GsDOBJ *objp;
	
	/* TMD�f�[�^�ǂݍ��� */
	tmdp = addr;			/* TMD�f�[�^�̐擪�A�h���X */
	tmdp++;				/* �t�@�C���w�b�_���X�L�b�v */
	GsMapModelingData(tmdp);	/* ���A�h���X�փ}�b�v */
	tmdp++;				/* �t���O�ǂݔ�΂� */
	tmdp++;				/* �I�u�W�F�N�g���ǂݔ�΂� */
	
	GsLinkObject4((u_long)tmdp,&object,0);
	
	/* Init work vector
        /* �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
        PWorld.vx=PWorld.vy=PWorld.vz=0;
	GsInitCoordinate2(WORLD, &DWorld);
	
	/* 3D�I�u�W�F�N�g������ */
	object.coord2 =  &DWorld;
	object.coord2->coord.t[2] = 4000;
	object.tmd = tmdp;		/* �ǂݍ���TMD�f�[�^���|�C���g���� */
	object.attribute = 0;
}

/*
 *  �i��������́j�e�N�X�`���f�[�^�̓ǂݍ���
 */
initTexture(addr)
u_long *addr;
{
	RECT rect1;
	GsIMAGE tim1;

	/* TIM�f�[�^�̏��𓾂� */	
	GsGetTimInfo(addr+1, &tim1);	/* �t�@�C���w�b�_���΂��ēn�� */

	/* �s�N�Z���f�[�^��VRAM�֑��� */	
	rect1.x=tim1.px;
	rect1.y=tim1.py;
	rect1.w=tim1.pw;
	rect1.h=tim1.ph;
	LoadImage(&rect1,tim1.pixel);

	/* CLUT������ꍇ��VRAM�֑��� */
	if((tim1.pmode>>3)&0x01) {
		rect1.x=tim1.cx;
		rect1.y=tim1.cy;
		rect1.w=tim1.cw;
		rect1.h=tim1.ch;
		LoadImage(&rect1,tim1.clut);
	}
}

/* Set coordinte parameter from work vector
/* ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
set_coordinate(pos,coor)
SVECTOR *pos;			/* work vector /* ���[�e�V�����x�N�^ */
GsCOORDINATE2 *coor;		/* Coordinate /* ���W�n */
{
  MATRIX tmp1;
  SVECTOR v1;
  
  tmp1   = GsIDMATRIX;		/* start from unit matrix
				/* �P�ʍs�񂩂�o������ */
    
  /* Set translation /* ���s�ړ����Z�b�g���� */
  tmp1.t[0] = coor->coord.t[0];
  tmp1.t[1] = coor->coord.t[1];
  tmp1.t[2] = coor->coord.t[2];
  
  /*/* �}�g���b�N�X���[�N�ɃZ�b�g����Ă��郍�[�e�[�V������
     ���[�N�̃x�N�^�[�ɃZ�b�g */
  v1 = *pos;
  
  /* Rotate Matrix
  /* �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ */
  RotMatrix(&v1,&tmp1);
  
  /* Set Matrix to Coordinate
  /* ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
  coor->coord = tmp1;
  
  /* Clear flag becase of changing parameter
  /* �}�g���b�N�X�L���b�V�����t���b�V������ */
  coor->flg = 0;
}

/*
 *	�v���~�e�B�u�̏�����
 */
#include "balltex.h"
#define NBALL	256		/* �{�[���̌� */
#define DIST	SCR_Z/4		/* �{�[�����U��΂��Ă���͈� */

POLY_FT4	ballprm[2][NBALL];	/* �v���~�e�B�u�o�b�t�@ */
SVECTOR		ballpos[NBALL];		/* �{�[���̈ʒu */
	
void initPrimitives()
{

	int		i, j;
	u_short		tpage, clut[32];
	POLY_FT4	*bp;	
		
	/* �{�[���̃e�N�X�`���y�[�W�����[�h���� */
	tpage = LoadTPage(ball16x16, 0, 0, 640, 256, 16, 16);

	/* �{�[���p�� CLUT�i�R�Q�j�����[�h���� */
	for (i = 0; i < 32; i++)
		clut[i] = LoadClut(ballcolor[i], 256, 480+i);
	
	/* initialize primiteves */
	for (i = 0; i < 2; i++)
		for (j = 0; j < NBALL; j++) {
			bp = &ballprm[i][j];
			SetPolyFT4(bp);
			SetShadeTex(bp, 1);
			bp->tpage = tpage;
			bp->clut = clut[j%32];
			setUV4(bp, 0, 0, 16, 0, 0, 16, 16, 16);
		}
	
	/* initialize position */
	for (i = 0; i < NBALL; i++) {
		ballpos[i].vx = (rand()%DIST)-DIST/2;
		ballpos[i].vy = (rand()%DIST)-DIST/2;
		ballpos[i].vz = (rand()%DIST)-DIST/2;
	}
}

/*
 *	�v���~�e�B�u�̂n�s�ւ̓o�^
 */
addPrimitives(ot)
u_long	*ot;
{
	static int	id    = 0;		/* �_�u���o�b�t�@ ID */
	static VECTOR	trans = {0, 0, SCR_Z};	/* �{�[���̈ʒu */
	static SVECTOR	angle = {0, 0, 0};	/* �{�[���̉�]�p */
	static MATRIX	rottrans;		/* ��]�s�� */
	int		i, padd;
	long		dmy, flg, otz;
	POLY_FT4	*bp;
	SVECTOR		*sp;
	SVECTOR		dp;
	
	
	id = (id+1)&0x01;		/* ID �� �X���b�v */
	PushMatrix();			/* �J�����g�}�g���N�X��ޔ������� */
	
	/* �p�b�h�̓��e����}�g���N�X rottrans ���A�b�v�f�[�g */
	padd = PadRead(1);
	if(padd & PADLup)    angle.vx += 10;
	if(padd & PADLdown)  angle.vx -= 10;
	if(padd & PADLleft)  angle.vy -= 10;
	if(padd & PADLright) angle.vy += 10;
	if(padd & PADl) trans.vz -= 50;
	if(padd & PADn) trans.vz += 50;
	
	RotMatrix(&angle, &rottrans);		/* ��] */
	TransMatrix(&rottrans, &trans);		/* ���s�ړ� */
	
	/* �}�g���N�X rottrans ���J�����g�}�g���N�X�ɐݒ� */
	SetTransMatrix(&rottrans);	
	SetRotMatrix(&rottrans);
	
	/* �v���~�e�B�u���A�b�v�f�[�g */
	bp = ballprm[id];
	sp = ballpos;
	for (i = 0; i < NBALL; i++, bp++, sp++) {
		otz = RotTransPers(sp, (long *)&dp, &dmy, &flg);
		if (otz > 0 && otz < OTSIZE) {
			setXY4(bp, dp.vx, dp.vy,    dp.vx+16, dp.vy,
			           dp.vx, dp.vy+16, dp.vx+16, dp.vy+16);

			AddPrim(ot+otz, bp);
		}
	}

	/* �ޔ������Ă����}�g���N�X�������߂��B*/
	PopMatrix();
}
