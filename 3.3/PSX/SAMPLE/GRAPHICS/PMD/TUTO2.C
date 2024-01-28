/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			    tuto3
 *			
 *	   TMD-PMD �r���[�A�v���g�^�C�v�i�����v�Z�Ȃ�  FT3 �^�j
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	----------------------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	2.00		Jul,14,1994	suzu	(using PMD)
 */

#include <sys/types.h>	
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * �R�p�`�|���S���p�P�b�g
 * ���_�E�p�P�b�g�o�b�t�@�́Amalloc() �œ��I�Ɋ���t����ׂ��ł��B	
 * ���̗�ł̓I�u�W�F�N�g������̃|���S�����́AMAX_POLY ��	
 */
#define MAX_POLY	400			/* �|���S���̍ő�l */

typedef struct {
	MATRIX		m;			/* ���[�J���}�g���N�X */
	SVECTOR		v;			/* �ړ��x�N�g�� */
	int		n;			/* �|���S���� */
	struct {			
		POLY_FT3	prim[2];
		SVECTOR		v0, v1, v2;
	} p[MAX_POLY];
} OBJ_FT3;

#define SCR_Z		1024		/* ���e�ʂ܂ł̋��� */
#define OTSIZE		8192		/* �n�s�̒i���i�Q�ׂ̂���j */
#define OTLENGTH	12		/* �n�s�̒i�� */
#define MAX_OBJ		16		/* �I�u�W�F�N�g�̌� */
#define MODELADDR	(0x80120000)	/* TMD �̊i�[����Ă���A�h���X */
#define TEXADDR		(0x80140000)	/* TIM �̊i�[����Ă���A�h���X */

static int pad_read(OBJ_FT3 *obj, int nobj, MATRIX *m);
static void set_position(OBJ_FT3 *obj);
static void add_OBJ_FT3(MATRIX *world, u_long *ot, OBJ_FT3 *obj, int id);
static void loadTIM(u_long *addr);
static void setSTP(u_long *col, int n);
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj);


main()
{
	MATRIX	world;			/* ���[���h�}�g���N�X */
	OBJ_FT3	obj[MAX_OBJ];	/* �I�u�W�F�N�g */
	u_long	ot[2][OTSIZE];		/* �n�s �o�b�t�@*/
	int	nobj = 1;		/* �I�u�W�F�N�g�̌� */
	int	id   = 0;		/* �p�P�b�g ID */
	int	i, n; 			/* work */
	
	db_init(640, 480, SCR_Z, 60, 120, 120);	/* �_�u���o�b�t�@�̏����� */
	loadTIM((u_long *)TEXADDR);	/* TIM ���t���[���o�b�t�@�ɓ]������ */

	/* TMD ���΂�΂�ɂ��ēǂݍ��� */
	for (i = 0; i < MAX_OBJ; i++) 
		loadTMD_FT3((u_long *)MODELADDR, &obj[i]);	
	
	set_position(obj);		/* �I�u�W�F�N�g�ʒu�����C�A�E�g */
	SetDispMask(1);			/* �\���J�n */
	
	/* ���C�����[�v */
	while ((nobj = pad_read(obj, nobj, &world)) != -1) {
		
		/* �p�P�b�g�o�b�t�@ ID���X���b�v */
		id = id? 0: 1;
		
		/* �n�s���N���A */
		ClearOTagR(ot[id], OTSIZE);			

		/* �v���~�e�B�u�̐ݒ� */
		for (i = 0; i < nobj; i++) 
			add_OBJ_FT3(&world, ot[id], &obj[i], id);
		
		/* �f�o�b�O�X�g�����O�̐ݒ� */
		FntPrint("polygon=%d\n", obj[0].n);
		FntPrint("objects=%d\n", nobj);
		FntPrint("total  =%d\n", obj[0].n*nobj);
		
		/* �_�u���o�b�t�@�̃X���b�v�Ƃn�s�`�� */
		db_swap(&ot[id][OTSIZE-1]);
	}
	PadStop();
	StopCallback();
}


/*
 * �p�b�h�̓ǂݍ���
 * ��]�̒��S�ʒu�Ɖ�]�p�x��ǂݍ���
 * 	
 */	
static int pad_read(OBJ_FT3 *obj, int nobj, MATRIX *m)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*2};	
	static int	scale = ONE;
	static int	opadd = 0;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADselect)	return(-1);
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;
	
	if ((opadd==0) && (padd & PADLup))	set_position(&obj[nobj++]);
	if ((opadd==0) && (padd & PADLdown))	nobj--;
	
	limitRange(nobj, 1, MAX_OBJ);
	opadd = padd;
	
	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, m);	
	TransMatrix(m, &vec);	
	ScaleMatrix(m, &svec);
	
	SetRotMatrix(m);
	SetTransMatrix(m);

	return(nobj);
}

/*
 * �I�u�W�F�N�g�̃��C�A�E�g
 * �����ł͗������g�p���ēK���ɔz�u���Ă���	
 * ���̂��߁A��̃I�u�W�F�N�g�������ʒu�ɂԂ��邱�Ƃ�����B	
 */	
#define DIST	SCR_Z		/* �����̕��U */
#define UNIT	256		/* �ŏ��𑜓x */

static void set_position(OBJ_FT3 *obj)
{
	SVECTOR	ang;
		
	ang.vx = rand()%4096;
	ang.vy = rand()%4096;
	ang.vz = rand()%4096;
	RotMatrix(&ang, &obj->m);	/* ���[�J���}�g���N�X */
		
	obj->v.vx = (rand()/UNIT*UNIT%DIST)-DIST/2;
	obj->v.vy = (rand()/UNIT*UNIT%DIST)-DIST/2;
	obj->v.vz = (rand()/UNIT*UNIT%DIST)-DIST/2;

}
/*
 * �I�u�W�F�N�g�̓o�^
 */
static void add_OBJ_FT3(MATRIX *world, u_long *ot, OBJ_FT3 *obj, int id)
{
	MATRIX	screen;			/* �X�N���[���}�g���N�X */
	long	flg;			/* work */
	
	RotTrans(&obj->v, (VECTOR *)screen.t, &flg);	/* ���s�ړ� */
	PushMatrix();				/* �J�����g�}�g���N�X��ޔ� */
	MulMatrix0(world, &obj->m, &screen);	/* ��]�i���Ԓ��Ӂj */
	SetRotMatrix(&screen);			/* ��]�}�g���N�X�̐ݒ� */
	SetTransMatrix(&screen);		/* �ړ��x�N�g���̐ݒ� */
	
	/* ��]�E�ړ��E�����ϊ� */
	RotPMD_FT3((long *)&obj->n, ot, OTLENGTH, id, 0);

	/* �}�g���N�X�����ɂ��ǂ��ă��^�[�� */
	PopMatrix();
}


/*
 * TIM �����[�h����
 */	
static void loadTIM(u_long *addr)
{
	TIM_IMAGE	image;		/* TIM �w�b�_ */
	
	OpenTIM(addr);			/* TIM ���I�[�v�� */
	while (ReadTIM(&image)) {
		if (image.caddr) {	/* CLUT�i��������΁j�����[�h */
			setSTP(image.caddr, image.crect->w);
			LoadImage(image.crect, image.caddr);
		}
		if (image.paddr) 	/* �p�^�[�������[�h */
			LoadImage(image.prect, image.paddr);
	}
}
	
/*
 * STP bit �� 1 �ɂ���
 */	
static void setSTP(u_long *col, int n)
{
	n /= 2;  
	while (n--)
		*col++ |= 0x80008000;
}

/*
 * TMD �̉��
 */
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj)
{
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	/* TMD �̃I�[�v�� */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY) 
		n_prim = MAX_POLY;
	
	/*
	 * �v���~�e�B�u�o�b�t�@�̂����A���������Ȃ����̂�
	 * ���炩���ߐݒ肵�Ă���
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {

		/* �v���~�e�B�u�̏����� */
		SetPolyFT3(&obj->p[i].prim[0]);

		/* �@���ƒ��_�x�N�g�����R�s�[���� */
		copyVector(&obj->p[i].v0, &tmdprim.x0);
		copyVector(&obj->p[i].v1, &tmdprim.x1);
		copyVector(&obj->p[i].v2, &tmdprim.x2);
		
		/* �����v�Z�i�ŏ��̈��̂݁j*/
		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(&obj->p[i].prim[0], col, col, col);
		
		/* �e�N�X�`�����W���R�s�[ */
		setUV3(&obj->p[i].prim[0], 
		       tmdprim.u0, tmdprim.v0,
		       tmdprim.u1, tmdprim.v1,
		       tmdprim.u2, tmdprim.v2);
		
		/* �e�N�X�`���y�[�W�^�e�N�X�`�� CLUT ID ���R�s�[ */
		obj->p[i].prim[0].tpage = tmdprim.tpage;
		obj->p[i].prim[0].clut  = tmdprim.clut;

		/* �_�u���o�b�t�@���g�p����̂Ńv���~�e�B�u�͂Q�g�K�v */
		memcpy(&obj->p[i].prim[1],
		       &obj->p[i].prim[0], sizeof(POLY_FT3));
		
	}
	return(obj->n = i);
}