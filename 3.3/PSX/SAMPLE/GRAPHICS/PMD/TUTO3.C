/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				tuto3
 *			
 *	   TMD-PMD �r���[�A�v���g�^�C�v�i�����v�Z����  FT3 �^�j
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	2.00		Jul,14,1994	suzu	(using PMD)
 *	2.10		Jul,14,1994	suzu	(with lighting)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z		1024		/* ���e�ʂ܂ł̋��� */
#define OTLENGTH	12		/* �n�s�̒i�� */
#define OTSIZE		(1<<OTLENGTH)	
#define MAX_POLY	3000		/* �\���|���S���̍ő�l */
#define MODELADDR	(0x80120000)	/* TMD �̊i�[����Ă���A�h���X */
#define TEXADDR		(0x80140000)	/* TIM �̊i�[����Ă���A�h���X */

/*
 * �R�p�`�|���S���p�P�b�g
 * ���_�E�p�P�b�g�o�b�t�@�́Amalloc() �œ��I�Ɋ���t����ׂ��ł��B	
 */
typedef struct {
	int	n;
	struct {			
		POLY_FT3	prim[2];
		SVECTOR v0, v1, v2;
	} p[MAX_POLY];
	SVECTOR	n0[MAX_POLY];			/* �@�� */
} OBJ_FT3;

static void loadTIM(u_long *addr);
static void setSTP(u_long *col, int n);
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj);
static int lightOBJ_FT3(OBJ_FT3 *obj, MATRIX *cmat, SVECTOR *lang, int ambi);
static int pad_read(OBJ_FT3 *obj);

main()
{
	static OBJ_FT3	obj;			/* �I�u�W�F�N�g */
	static u_long	otbuf[2][OTSIZE];	/* �n�s �o�b�t�@*/
	int		id = 0;			/* �p�P�b�g ID */
	int		i, nprim;
		
	/* �_�u���o�b�t�@�̏����� */
	db_init(640, 480, SCR_Z, 60, 120, 120);	
	loadTIM((u_long *)TEXADDR);	/* TIM ���t���[���o�b�t�@�ɓ]������ */

	/* TMD ���΂�΂�ɂ��ēǂݍ��� */
	nprim = loadTMD_FT3((u_long *)MODELADDR, &obj);

	/* �\���J�n */
	SetDispMask(1);			

	/* ���C�����[�v */
	while (pad_read(&obj) == 0) {
		
		/* �p�P�b�g�o�b�t�@ ID���X���b�v */
		id = id? 0: 1;
		
		/* �n�s���N���A */
		ClearOTagR(otbuf[id], OTSIZE);			

		/* �v���~�e�B�u�̐ݒ� */
		RotPMD_FT3((long *)&obj, otbuf[id], OTLENGTH, id, 0);

		/* �f�o�b�O�X�g�����O�̐ݒ� */
		FntPrint("total=%d\n", obj.n);
		
		/* �_�u���o�b�t�@�̃X���b�v�Ƃn�s�`�� */
		db_swap(otbuf[id]+OTSIZE-1);
	}
	PadStop();
	StopCallback();
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
	int		i, n_prim = 0;

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
		copyVector(&obj->n0[i],   &tmdprim.n0);
		
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

/*
 * �����v�Z
 */
static int lightOBJ_FT3(OBJ_FT3 *obj, MATRIX *cmat, SVECTOR *lang, int ambi)
{
	int	i;	
	MATRIX	lmat;
	P_CODE	csrc, cdst;
	SVECTOR	*n;
	
	/* �����̃}�g���N�X���v�Z */
	SetBackColor(ambi, ambi, ambi);	/* ���ӌ� */
	SetColorMatrix(cmat);		/* �����F */
	RotMatrix(lang, &lmat);		/* �����x�N�g�� */
	SetLightMatrix(&lmat);		
	
	/*setRGB0(&csrc, 128, 128, 128);	/* �f�ސF�́A(128,128,128) */
	setRGB0(&csrc, 255, 255, 255);		/* �f�ސF�́A(255,255,255) */
	csrc.code = obj->p[0].prim[0].code;	/* �R�[�h��ۑ� */

	/* �����v�Z */
	for (i = 0, n = obj->n0; i < obj->n; i++, n++) {
		NormalColorCol(n, (CVECTOR *)&csrc, (CVECTOR *)&cdst);
		*(u_long *)(&obj->p[i].prim[0].r0) = *(u_long *)(&cdst);
		*(u_long *)(&obj->p[i].prim[1].r0) = *(u_long *)(&cdst);
	}
	return(0);
}
	
	       
/*
 * �p�b�h�̉��
 */	
static MATRIX	cmat = {			/* �����F */
/* 	 #0,	#1,	#2, */
	ONE,	0,	0,	/* R */
	0,	ONE,	0, 	/* G */
	0,	0,	ONE,	/* B */
};
	
static int pad_read(OBJ_FT3 *obj)
{
	static int	light = 1;
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*2};	/* position */
	static SVECTOR	lang  = {ONE, ONE, ONE};	/* light angle */
	static MATRIX	m;				/* matrix */
	static int	scale = 2*ONE;			/* scale */
	static int	frame = 0;
	
	VECTOR	svec;
	int 	padd = PadRead(1);

	if (padd & PADselect)	return(-1);
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  += ONE/256;
	if (padd & PADR2)	scale  -= ONE/256;
	
	if (frame%2 == 0 && (padd & (PADLup|PADLdown|PADLleft|PADLright)))
		light = 1;
	    
	if (padd & PADLup)	lang.vx += 32;
	if (padd & PADLdown)	lang.vx -= 32;
	if (padd & PADLleft) 	lang.vy += 32;
	if (padd & PADLright)	lang.vy -= 32;
	
	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	/* �����v�Z������i�ŏ��̈��ƃp�����[�^���ύX�����ꂽ�Ƃ��j*/
	/* �x���Ȃ�̂ŁA����v�Z����ׂ��ł͂Ȃ� */
	if (light) {
		light = 0;
		lightOBJ_FT3(obj, &cmat, &lang, 0);
	}

	/* ���^�[�� */
	frame++;
	return(0);
}		

