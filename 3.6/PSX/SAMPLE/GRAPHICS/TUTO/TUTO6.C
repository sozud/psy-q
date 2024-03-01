/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto6: cube with depth-queue
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *		    rotating cubes with depth queue
 :		�f�v�X�L���[�C���O���ʂ�p���������̂̕`��
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z	(512)		
#define	OTSIZE	(4096)

/* start point of fog effect: �t�H�O�i�f�v�X�L���[�j�̂�����X�^�[�g�ʒu */
#define FOGNEAR (300)		
					   
/* primitive buffer: �v���~�e�B�u�֘A�̃o�b�t�@ */
typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot[OTSIZE];	/* OT: �I�[�_�����O�e�[�u�� */
	POLY_F4		s[6];		/* sides of cube: �����̂̑��� */
} DB;

static int pad_read(MATRIX *rottrans);
static void init_prim(DB *db);
main()
{
	DB	db[2];		/* double buffer */
	DB	*cdb	;	/* current buffer */
	MATRIX	rottrans;	/* rot-trans matrix */
	CVECTOR	col[6];		/* color of cube surface */
	
	int	i;		/* work */
	int	dmy, flg;	/* dummy */

	/* initialize environment for double buffer (interlace)
	 :�_�u���o�b�t�@�p�̊��ݒ�i�C���^�[���[�X���[�h�j
	 */
	init_system(320, 240, SCR_Z, 0);
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
	SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
	SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);

	/* FarColor have to be the same as background:
	   �t�@�[�J���[�i�����F�j�̐ݒ�w�i�F�Ɠ��F�ɂ��� */
	SetFarColor(60,120,120);	
	
	/* start point of depth quweue: �f�v�X�L���[�C���O���J�n����n�_ */
	SetFogNear(FOGNEAR,SCR_Z);	

	/* set primitive parametes on buffer: �v���~�e�B�u�o�b�t�@�̏����ݒ� */
	init_prim(&db[0]);	/* buffer #0 */
	init_prim(&db[1]);	/* buffer #1 */

	/* set surface colors: �����̂̑��ʂ̐F�̐ݒ� */
	for (i = 0; i < 6; i++) {
		col[i].cd = db[0].s[0].code;	/* code */
		col[i].r = rand();		/* R */
		col[i].g = rand();		/* G */
		col[i].b = rand();		/* B */
	}

	SetDispMask(1);		/* start displaying: �\���J�n */

	PutDrawEnv(&db[0].draw);
	PutDispEnv(&db[0].disp);

	while (pad_read(&rottrans) == 0) {

		cdb = (cdb==db)? db+1: db;	
		ClearOTagR(cdb->ot, OTSIZE);	
		
		/* apend cubes into OT: �����̂��n�s�ɓo�^���� */
		add_cubeF4F(cdb->ot, cdb->s, &rottrans, col);

		VSync(0);
		ResetGraph(1);
		
		/* clear background: �w�i�̃N���A */
		ClearImage(&cdb->draw.clip, 60, 120, 120);

		/* draw: �`�� */
		DrawOTag(cdb->ot+OTSIZE-1);
		FntFlush(-1);
	}
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
static int pad_read(MATRIX *rottrans)
{
	/* angle of rotation for the cube: angle �����̂̉�]�p�x */
	static SVECTOR	ang  = { 0, 0, 0};	
	
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

	/* distance from screen : ���_����̋��� */
	if (padd & PADR1)	vec.vz += 8;
	if (padd & PADR2) 	vec.vz -= 8;

	/* matrix calcuration: �}�g���b�N�X�̌v�Z */
	/* rotation angle of the cube: �����̂̉�]�p�x */
	RotMatrix(&ang, rottrans);	
	
	/* translation vector of the cube: �����̂̕��s�ړ��x�N�g�� */
	TransMatrix(rottrans, &vec);	
	
	FntPrint("tuto6: fog angle=(%d,%d,%d)\n",
		 ang.vx, ang.vy, ang.vz);
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

	for(i = 0;i < 6; i++)
		SetPolyF4(&db->s[i]);
}