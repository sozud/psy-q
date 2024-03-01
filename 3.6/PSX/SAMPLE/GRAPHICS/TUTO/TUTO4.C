/* $PSLibId: Runtime Library Release 3.6$ */

/*			tuto4: cube with lighting
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *		Draw 3D objects (cube) with lighting
 *			�����̂��闧���̂̕`��
 :
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z	(512)		
#define	OTSIZE	(4096)

/* primitive buffer: �v���~�e�B�u�֘A�̃o�b�t�@ */
typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot[OTSIZE];	/* OT: �I�[�_�����O�e�[�u�� */
	POLY_F4		s[6];		/* sides of cube: �����̂̑��� */
} DB;

/* light source�iLocal Color Matrix�j:
   �����F�i���[�J���J���[�}�g���b�N�X�j */
static MATRIX	cmat = {
/* light source    #0, #1, #2, */
		ONE*3/4,  0,  0, /* R */
		ONE*3/4,  0,  0, /* G */
		ONE*3/4,  0,  0, /* B */
};

/* light vector (local light matrix)
   : �����x�N�g���i���[�J�����C�g�}�g���b�N�X�j */
static MATRIX lgtmat = {
	/*          X     Y     Z */
	          ONE,  ONE, ONE,	/* ���� #0 */
		    0,    0,    0,	/*      #1 */
		    0,    0,    0	/*      #2 */
};

static int pad_read(MATRIX *rottrans, MATRIX *rotlgt);
static void init_prim(DB *db);

main()
{
	DB	db[2];		/* double buffer */
	DB	*cdb	;	/* current buffer */
	MATRIX	rottrans;	/* rot-trans matrix */
	
	/* rot-trans matrix for light source: �����̉�]�}�g���b�N�X */
	MATRIX		rotlgt;		
	
	/* lighint matrix: �ŏI�I�Ȍ����}�g���b�N�X */
	MATRIX		light;

	/* color of cube surface: �����̂̑��ʂ̐F */
	CVECTOR		col[6];	
	
	int		i;		/* work */
	int		dmy, flg;	/* dummy */

	/* initialize environment for double buffer (interlace)
	 :�_�u���o�b�t�@�p�̊��ݒ�i�C���^�[���[�X���[�h�j
	 */
	init_system(320, 240, SCR_Z, 0);
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
	SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
	SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);

	/* set background color: �o�b�N�J���[(�A���r�G���g�F)�̐ݒ� */
	SetBackColor(64, 64, 64);	
	
	/* set local color matrix: ���[�J���J���[�}�g���b�N�X�̐ݒ� */
	SetColorMatrix(&cmat);		

	/* set primitive parametes on buffer: �v���~�e�B�u�o�b�t�@�̏����ݒ� */
	init_prim(&db[0]);	
	init_prim(&db[1]);	

	/* set surface colors. 
	 * NormalColorCol() overwrite the 'code' field of the primitive.
	 * So set the code here.
	 : �����̂̑��ʂ̐F�̐ݒ�
	 * ��� NormalColorCol() �� db[0].s[0].code �̓��e���󂳂��
	 * ���܂����߁A�����őҔ����Ă���
	 */
	for (i = 0; i < 6; i++) {
		col[i].cd = db[0].s[0].code;	/* CODE */
		col[i].r  = rand();		/* R */
		col[i].g  = rand();		/* G */
		col[i].b  = rand();		/* B */
	}

	SetDispMask(1);			/* start displaying: �\���J�n */
	PutDrawEnv(&db[0].draw);	/* set DRAWENV: �`����̐ݒ� */
	PutDispEnv(&db[0].disp);	/* set DISPENV: �\�����̐ݒ� */

	while (pad_read(&rottrans, &rotlgt) == 0) {

		cdb = (cdb==db)? db+1: db;	/* change current buffer */
		ClearOTagR(cdb->ot, OTSIZE);	/* clear OT */

		/* Calcurate Matrix for the light source 
		   : �����}�g���b�N�X�̐ݒ� */
		MulMatrix0(&lgtmat, &rotlgt, &light);
		SetLightMatrix(&light);

		/* apend cubes into OT: �����̂��n�s�ɓo�^���� */
		add_cubeF4L(cdb->ot, cdb->s, &rottrans, col);

		VSync(0);
		ResetGraph(1);
		
		/* clear background: �w�i�̃N���A */
		ClearImage(&cdb->draw.clip, 60, 120, 120);

		/* draw primitives: �`�� */
		/*DumpOTag(cdb->ot+OTSIZE-1);	/* for debug */
		DrawOTag(cdb->ot+OTSIZE-1);	
		FntFlush(-1);
	}
        /* close controller: �R���g���[���̃N���[�Y */
	PadStop();			
	ResetGraph(1);
	StopCallback();
	return;
}

/* 
 * Analyzing PAD and Calcurating Matrix
 : �R���g���[���̉�͂ƁA�ϊ��}�g���b�N�X�̌v�Z
 */
/* MATRIX *rottrans; 	rot-trans matrix: �����̂̉�]�E���s�ړ��}�g���b�N�X */
/* MATRIX *rotlgt;	light source matrix: �����}�g���b�N�X */
static int pad_read(MATRIX *rottrans, MATRIX *rotlgt)
{
	/* angle of rotation for the cube: angle �����̂̉�]�p�x */
	static SVECTOR	ang  = { 0, 0, 0};	
	
	/* �����̉�]�p�x: angle of rotation for the light source */
	static SVECTOR	lgtang = {1024, -512, 1024};	
	
	/* translation vector: ���s�ړ��x�N�g�� */
	static VECTOR	vec  = {0, 0, SCR_Z};	

	/* read from controller: �R���g���[������f�[�^��ǂݍ��� */
	u_long	padd = PadRead(1);	

	int	ret = 0;
	
	/* quit program: �v���O�����̏I�� */
	if (padd & PADselect) 	ret = -1;	

	/* change the rotation angles for the cube and the light source
	   : �����Ɨ����̂̉�]�p�x�̕ύX */
	if (padd & PADRup)	ang.vz += 32;
	if (padd & PADRdown)	ang.vz -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	
	/* change the rotation angles only for the light source 
	   : �����݂̂̉�]�p�x�̕ύX */
	if (padd & PADLup)	lgtang.vx += 32;
	if (padd & PADLdown)	lgtang.vx -= 32;
	if (padd & PADLleft) 	lgtang.vy += 32;
	if (padd & PADLright)	lgtang.vy -= 32;

	/* distance from screen : ���_����̋��� */
	if (padd & PADL1)	vec.vz += 8;
	if (padd & PADR1) 	vec.vz -= 8;

	/* rotation matrix of the light source: �����̉�]�}�g���N�X�v�Z */
	RotMatrix(&lgtang, rotlgt);	
	MulMatrix(rotlgt, rottrans);

	/* set rot-trans-matrix of the cube: �����̂̉�]�}�g���N�X�v�Z */
	RotMatrix(&ang, rottrans);	
	TransMatrix(rottrans, &vec);	

	FntPrint("tuto4: lighting angle=(%d,%d,%d)\n",
		 lgtang.vx, lgtang.vy, lgtang.vz);
	return(ret);
}

/*
 *	Initialization assosiate with Primitives.
 :	�v���~�e�B�u�֘A�̏����ݒ�
 */
/* DB	*db;	primitive buffer: �v���~�e�B�u�o�b�t�@ */
static void init_prim(DB *db)
{
	int i;

	/* Flat shading 4 point Polygon declared
	   : �t���b�g�V�F�[�f�B���O�S�p�`�v���~�e�B�u�̏����ݒ� */
	for(i = 0;i < 6;i++)
		SetPolyF4(&db->s[i]);	
}