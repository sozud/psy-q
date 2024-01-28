/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto0
 *			
 *	   TMD �r���[�A�v���g�^�C�v�i�����v�Z�Ȃ�  FT3 �^�j
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	1.00		Mar,15,1994	suzu
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <inline.h>
#include <gtemac.h>
#include "rtp.h"

#define SCR_Z		1024		/* ���e�ʂ܂ł̋��� */
#define OTSIZE		8192		/* �n�s�̒i���i�Q�ׂ̂���j */
#define MAX_POLY	3000		/* �\���|���S���̍ő�l */

#define MODELADDR	((u_long *)0x80100000)	/* TMD �̃A�h���X */

/*
 * �R�p�`�|���S���̒��_���
 */
typedef struct {
	SVECTOR	n0;		/* �@�� */
	SVECTOR	v0, v1, v2;	/* ���_ */
} VERT_F3;

/*
 * �R�p�`�|���S���p�P�b�g
 * ���_�E�p�P�b�g�o�b�t�@�́Amalloc() �œ��I�Ɋ���t����ׂ��ł��B	
 */
typedef struct {
	int		n;			/* �|���S���� */
	VERT_F3		vert[MAX_POLY];		/* ���_�o�b�t�@ �i�P�j*/
	POLY_F3		prim[2][MAX_POLY];	/* �p�P�b�g�o�b�t�@�i�Q�j*/
} OBJ_F3;

static int pad_read(int nprim);
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj);

main()
{
	static OBJ_F3	obj;			/* �I�u�W�F�N�g */
	static u_long	otbuf[2][OTSIZE];	/* �n�s �o�b�t�@*/
	u_long		*ot;			/* �J�����g�n�s */
	int		id = 0;			/* �p�P�b�g ID */
	VERT_F3		*vp;			/* work */
	POLY_F3		*pp;			/* work */
	int		nprim;			/* work */
	int		i; 			/* work */
	/*long		otz, flg, clp;*/

	db_init(640, 480, SCR_Z, 60, 120, 120);	/* �_�u���o�b�t�@�̏����� */
	loadTMD_F3(MODELADDR, &obj);		/* TMD ��ǂݍ��� */
	SetDispMask(1);				/* �\���J�n */

	/* ���C�����[�v */
	nprim = obj.n;
	while ((nprim = pad_read(nprim)) != -1) {
		
		/* [0,max_nprim] �� nprim ���N���b�v */
		limitRange(nprim, 0, obj.n);

		/* �p�P�b�g�o�b�t�@ ID���X���b�v */
		id = id? 0: 1;
		ot = otbuf[id];
		
		/* �n�s���N���A */
		ClearOTagR(ot, OTSIZE);			

		/* �v���~�e�B�u�̐ݒ� */
		vp = obj.vert;
		pp = obj.prim[id];
		
		for (i = 0; i < nprim; i++, vp++, pp++) {
	
			/* �R�c�\���̍����́A���̂Q�s�ɏW�񂳂�� */
			pp = &obj.prim[id][i];
			rotTransPers3(ot, OTSIZE, pp,
				      &vp->v0, &vp->v1, &vp->v2);

		}
		/* �f�o�b�O�X�g�����O�̐ݒ� */
		FntPrint("total=%d\n", i);
		
		/* �_�u���o�b�t�@�̃X���b�v�Ƃn�s�`�� */
		db_swap(ot+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static int pad_read(int nprim)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE/4;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADselect)	return(-1);
	if (padd & PADLup) 	nprim += 4;
	if (padd & PADLdown)	nprim -= 4;
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;

	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	return(nprim);
}		

/*
 * TMD �̉��
 */
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj)
{
	VERT_F3		*vert;
	POLY_F3		*prim0, *prim1;
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	vert  = obj->vert;
	prim0 = obj->prim[0];
	prim1 = obj->prim[1];
	
	/* TMD �̃I�[�v�� */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY)
		n_prim = MAX_POLY;

	/*
	 * �v���~�e�B�u�o�b�t�@�̂����A���������Ȃ����̂�
	 * ���炩���ߐݒ肵�Ă���
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {
		
		/* �v���~�e�B�u�̏����� */
		SetPolyF3(prim0);

		/* �@���ƒ��_�x�N�g�����R�s�[���� */
		copyVector(&vert->n0, &tmdprim.n0);
		copyVector(&vert->v0, &tmdprim.x0);
		copyVector(&vert->v1, &tmdprim.x1);
		copyVector(&vert->v2, &tmdprim.x2);
		
		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(prim0, col, col, col);
		
		/* �_�u���o�b�t�@���g�p����̂Ńv���~�e�B�u�͂Q�g�K�v */
		memcpy(prim1, prim0, sizeof(POLY_F3));
		vert++, prim0++, prim1++;
	}
	return(obj->n = i);
}
