/* $PSLibId: Runtime Library Release 3.6$ */
/*		  tuto7: many cubes with local coordinates
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		      Drawing many 3D objects.
 :		    ������ 3D �I�u�W�F�N�g�̕`��
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z	1024		
#define	OTSIZE	4096
#define NCUBE	256		/* max number of cubes:
				   �\�����闧���̂̐��̍ő�l */

typedef struct {
	POLY_F4		s[6];		/* surface of cube: �����̂̑��� */
} CUBE;

typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot[OTSIZE];	/* OT: �I�[�_�����O�e�[�u�� */
	CUBE		c[NCUBE];	/* pointer to CUBE: �����̂̃f�[�^ */
} DB;

typedef struct {
	CVECTOR	col[6];			/* color of cube surface: ���ʂ̐F */
	SVECTOR	trans;			/* translation vector (local coord.)
					   : ���s�ړ��x�N�g���i���[�J�����W�j*/
} CUBEATTR;

/* �����F�i���[�J���J���[�}�g���b�N�X�j */
static MATRIX	cmat = {
/*  light source   #0, #1, #2,
	 : ����    #0, #1, #2, */
		ONE*3/4,  0,  0, /* R */
		ONE*3/4,  0,  0, /* G */
		ONE*3/4,  0,  0, /* B */
};

/* the vector of the light source (Local light Matrix�j 
   : �����x�N�g���i���[�J�����C�g�}�g���b�N�X�j */
static MATRIX lgtmat = {
	/*          X     Y     Z */
	          ONE,  ONE, ONE,	/* ���� #0 */
		    0,    0,    0,	/* #1 */
		    0,    0,    0	/* #2 */
};

static int pad_read(int *ncube, 
		MATRIX *world, MATRIX *local, MATRIX *rotlgt);
static void init_attr(CUBEATTR *attr, int nattr);
static init_prim(DB *db);

main()
{
	
	DB	db[2];		/* double buffer */
	DB	*cdb;		/* current buffer */
	
	/* attribute of cube: �����̂̑��� */
	CUBEATTR	attr[NCUBE];	
	
	/* world-screen matrix: ���[���h�X�N���[���}�g���N�X */
	MATRIX		ws;

	/* local-screen: ���[�J���X�N���[���}�g���N�X */
	MATRIX		ls;

	/* light source matrix: �����̃��[�J���X�N���[���}�g���b�N�X */
	MATRIX		lls;		
	
	/* lighint matrix: �ŏI�I�Ȍ����}�g���b�N�X */
	MATRIX		light;

	/* number of cubes: �\�����闧���̂̐� */
	int		ncube = NCUBE/2;
	
	int		i;		/* work */
	long		dmy, flg;	/* dummy */

	/* set double buffer environment
	 :  �_�u���o�b�t�@�p�̊��ݒ�i�C���^�[���[�X���[�h�j*/
	init_system(320, 240, SCR_Z, 0);
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
	SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
	SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);

	/* set background color: �w�i�F�̐ݒ� */
	SetBackColor(64, 64, 64);	
	
	/* set local
	   color matrix: ���[�J���J���[�}�g���b�N�X�̐ݒ� */
	SetColorMatrix(&cmat);		

	/* �v���~�e�B�u�o�b�t�@�̏����ݒ� */
	init_prim(&db[0]);
	init_prim(&db[1]);
	init_attr(attr, NCUBE);

	/* display enable: �\���J�n */
	SetDispMask(1);		

	PutDrawEnv(&db[0].draw); /* set DRAWENV: �`����̐ݒ� */
	PutDispEnv(&db[0].disp); /* set DISPENV: �\�����̐ݒ� */

	while (pad_read(&ncube, &ws, &ls, &lls) == 0) {

		cdb = (cdb==db)? db+1: db;	
		ClearOTagR(cdb->ot, OTSIZE);	

		/* Calcurate Matrix for the light source;
		 * Notice that MulMatrix() destroys current matrix.
		 : �����}�g���b�N�X�̐ݒ�
		 * Mulmatrix() �̓J�����g�}�g���N�X��j�󂷂邱�Ƃɒ���
		 */
		PushMatrix();
		MulMatrix0(&lgtmat, &lls, &light);
		PopMatrix();
		SetLightMatrix(&light);
		
		/* put the primitives of cubes int OT: �����̂̂n�s�ւ̓o�^ */
		limitRange(ncube, 1, NCUBE);
		
		/* add cubes to the OT
		 * note that only translation vector of the local
		 * screen matrix is changed   ncube �̗����̂�o�^
		 : OT �ɓo�^
		 * ���[�J���X�N���[���}�g���N�X�̈ړ��x�N�g����������
		 * ���X�V���Ă��邱�Ƃɒ���
		 */ 
		for (i = 0; i < ncube; i++) {
			RotTrans(&attr[i].trans, (VECTOR *)ls.t, &flg);
			add_cubeF4L(cdb->ot, cdb->c[i].s, &ls, attr[i].col);
		}

		VSync(0);
		ResetGraph(1);

		/* clear background: �w�i�N���A */
		ClearImage(&cdb->draw.clip, 60, 120, 120);

		DrawOTag(cdb->ot+OTSIZE-1);	/* draw: �`�� */
		FntFlush(-1);			/* for debug */
	}
	/* close controller: �R���g���[���̃N���[�Y */
	PadStop();			
	ResetGraph(1);
	StopCallback();
	return;
}


/* Analyzing PAD and Calcurating Matrix
 : �R���g���[���̉�͂ƁA�ϊ��}�g���b�N�X�̌v�Z
 */
/* int	  *ncube	number of cubes: �\�����闧���̂̐� */
/* MATRIX *ws	 	rottrans matrix for all: �S�̂̉�]�}�g���b�N�X */
/* MATRIX *ls	 	rottrans matrix for each: �e�����̂̉�]�}�g���b�N�X */
/* MATRIX *lls		light local-screen matrix: �����̉�]�}�g���b�N�X */
static int pad_read(int *ncube, 
		MATRIX *ws, MATRIX *ls, MATRIX *lls)
{
	/* PlayStation treats angle like as follows: 360��= 4096
	   : Play Station �ł́A�p�x�� 360��= 4096 �ň����܂��B*/
	
	/*; ��������]����Ƃ������Ƃ͎����ȊO�̐��E���t�����ɉ�]����
	   ���ƂƓ����Ȃ��Ƃɒ��� */
	 
	/*  rotation angle of world: �S�̂̉�]�p�x */
	static SVECTOR	wang    = {0,  0,  0};	
	
	/* rotation angle for each cube: �X�̗����̂̉�]�p�x */
	static SVECTOR	lang   = {0,  0,  0};	
	
	/* lotation angle for light source: �����̉�]�p�x */
	static SVECTOR	lgtang = {1024, -512, 1024};	
	
	/* translation vector for all objects: �S�̂̕��s�ړ��x�N�g�� */
	static VECTOR	vec    = {0,  0,  SCR_Z};

	/* scale of the each cube: �e�����̂̊g��E�k���� */
	static VECTOR	scale  = {1024, 1024, 1024, 0};
	
	SVECTOR	dwang, dlang;
	int	ret = 0;
	u_long	padd = PadRead(0);

	/* quit: �I�� */
	if (padd & PADselect) 	ret = -1;	

	/* rotate all cubes : �����́i�S�́j����]����B*/
	if (padd & PADRup)	wang.vz += 32;
	if (padd & PADRdown)	wang.vz -= 32;
	if (padd & PADRleft) 	wang.vy += 32;
	if (padd & PADRright)	wang.vy -= 32;

	/* rotate each cube: �����݂̂���]���� */
	if (padd & PADLup)	lang.vx += 32;
	if (padd & PADLdown)	lang.vx -= 32;
	if (padd & PADLleft) 	lang.vy += 32;
	if (padd & PADLright)	lang.vy -= 32;
	
	/* distance from screen : ���_����̋��� */
	if (padd & PADL1)	vec.vz += 8;
	if (padd & PADL2) 	vec.vz -= 8;

	/* change number of displayed cubes : �\�����闧���̂̐��̕ύX */
	if (padd & PADR1)       (*ncube)++;
	if (padd & PADR2)	(*ncube)--;
	limitRange(*ncube, 1, NCUBE-1);

	FntPrint("objects = %d\n", *ncube);
	
	/* calcurate world-screen matrix
	  ���[���h�X�N���[���}�g���b�N�X�̌v�Z */
	RotMatrix(&wang, ws);	
	TransMatrix(ws, &vec);

	/* calcurate matrix for each cubes.
	 * In this case, each local-screen matrix is the same because each
	 * cube rotates to the same direction.
	 : �X�̗����̂̃��[�J���}�g���b�N�X�̌v�Z 
	 * ���̏ꍇ�́A�����͓̂��������ɉ�]����̂Ń��[�J���}�g���N�X�͓���
	 */
	RotMatrix(&lang, ls);
	
	/* make local-screen matrix:
	 * Notice the difference between  MulMatrix() and MulMatrix2()
	 : ���[�J���X�N���[���}�g���b�N�X������ 
	 * MulMatrix() �� MulMatrix2() �̈Ⴂ�ɒ���
	 */
	MulMatrix2(ws, ls); 
	
	/*: make light matrix:  �����̉�]�}�g���b�N�X�̌v�Z */
	RotMatrix(&lgtang, lls);
	MulMatrix(lls, ls);

	/* scale of the local matrix represents the scale of the ls object.
	 : �I�u�W�F�N�g�̃X�P�[���̓��[�J���}�g���N�X�̃X�P�[���ŕ\���ł��� */
	ScaleMatrix(ls, &scale);
	
	/*Setting Matrix for all the objects�j
	  : �}�g���b�N�X�̐ݒ�i�S�́j */
	SetRotMatrix(ws);	/* rotation: ��]�}�g���b�N�X */
	SetTransMatrix(ws);	/* translation: ���s�ړ��x�N�g�� */

	return(ret);
}

#define MIN_D 	64		/* minumus distance between each cube */
#define MAX_D	(SCR_Z/2)	/* maximum distance */
/* CUBEATTR	*attr,	attribute of cube: �����̂̑��� */
/* int		nattr	number of cube: �����̂̐� */
static void init_attr(CUBEATTR *attr, int nattr)
{
	int	i;
	POLY_F4	templ;

	SetPolyF4(&templ);

	for (; nattr; nattr--, attr++) {
		for (i = 0; i < 6; i++) {
			attr->col[i].cd = templ.code;	/* sys code */
			attr->col[i].r  = rand();	/* R */
			attr->col[i].g  = rand();	/* G */
			attr->col[i].b  = rand();	/* B */
		}
		/* Set initial coordinates; �X�^�[�g�ʒu�̐ݒ� */
		attr->trans.vx = (rand()/MIN_D*MIN_D)%MAX_D-(MAX_D/2);
		attr->trans.vy = (rand()/MIN_D*MIN_D)%MAX_D-(MAX_D/2);
		attr->trans.vz = (rand()/MIN_D*MIN_D)%MAX_D-(MAX_D/2);
	}
}
	
/* DB	*db;	primivie buffer: �v���~�e�B�u�o�b�t�@ */
static init_prim(DB *db)
{
	int	i, j;

	for (i = 0; i < NCUBE; i++) 
		for (j = 0; j < 6; j++) 
			SetPolyF4(&db->c[i].s[j]);
}