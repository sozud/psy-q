/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				cube
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		  	  Draw cubes
 :		  ���낢��ȗ����̂�`�悷��
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 *	cube vertex database
 :	�����̂̒��_�f�[�^�x�[�X
 */
#define CUBESIZ	196		/* cube size: �����̂̈�ӂ̒��� */

/* vertex */
static SVECTOR P0 = {-CUBESIZ/2,-CUBESIZ/2,-CUBESIZ/2,0};
static SVECTOR P1 = { CUBESIZ/2,-CUBESIZ/2,-CUBESIZ/2,0};
static SVECTOR P2 = { CUBESIZ/2, CUBESIZ/2,-CUBESIZ/2,0};
static SVECTOR P3 = {-CUBESIZ/2, CUBESIZ/2,-CUBESIZ/2,0};

static SVECTOR P4 = {-CUBESIZ/2,-CUBESIZ/2, CUBESIZ/2,0};
static SVECTOR P5 = { CUBESIZ/2,-CUBESIZ/2, CUBESIZ/2,0};
static SVECTOR P6 = { CUBESIZ/2, CUBESIZ/2, CUBESIZ/2,0};
static SVECTOR P7 = {-CUBESIZ/2, CUBESIZ/2, CUBESIZ/2,0};

/* normal */
static SVECTOR N0 = { ONE,   0,    0, 0,};
static SVECTOR N1 = {-ONE,   0,    0, 0,};
static SVECTOR N2 = {0,    ONE,    0, 0,};
static SVECTOR N3 = {0,   -ONE,    0, 0,};
static SVECTOR N4 = {0,      0,  ONE, 0,};
static SVECTOR N5 = {0,      0, -ONE, 0,};

static SVECTOR	*v[6*4] = {
	&P0,&P1,&P2,&P3,
	&P1,&P5,&P6,&P2,
	&P5,&P4,&P7,&P6,
	&P4,&P0,&P3,&P7,
	&P4,&P5,&P1,&P0,
	&P6,&P7,&P3,&P2,
};

static SVECTOR	*n[6] = {	
	&N5, &N0, &N4, &N1, &N3, &N2,
};

/*
 * Calcurate the vertex of the cube and put the primitive which
 * represents a surface of cube
 : �����̂̍��W�l���v�Z���āAOT �ɓo�^����
 */
/* u_long	*ot;		OT pointer				  */
/* int		otsize;		OT size 				  */
/* POLY_F4	*s;		primitive for cube: �����̂̃v���~�e�B�u  */
/* MATRIX	*rottrans;	rot-trans matrix: ��]�E���s�ړ��}�g���b�N�X */
/* CVECTOR	*c;		surface color: ���̐F */

/*
 *	POLY_F4 cube
 :	POLY_F4 �ō\������闧���̂��v�Z����
 */
void add_cubeF4(u_long *ot, POLY_F4 *s, MATRIX *rottrans)
{
	int	i;
	long	p, otz, opz, flg;
	int	isomote;
	SVECTOR	**vp;

	/* GTE has 1 matrix register, so it is better to push the
	 * matrix to preserve the current matrix register.
	 : �֐����Ń}�g���b�N�X�̐ݒ���s���Ƃ��́A�O�̂��߂ɑҔ����Ă���
	 */
	PushMatrix();		

	/* set rotation Matrix: ��]�}�g���b�N�X�̐ݒ� */
	SetRotMatrix(rottrans);		
	
	/* set translation vector: ���s�ړ��x�N�g���̐ݒ� */
	SetTransMatrix(rottrans);	

	/* If the primitive is not back-faceed, then put it into OT.
	 :  �����̂̂��ꂼ��̖ʂ̕\���𒲂ׂāAOT �ɓo�^����
	 */
	vp = v;		/* vp: vertex pointer (work) */
	for (i = 0; i < 6; i++, s++, vp += 4) {
		/* Translate from local coordinates to screen coordinates 
		 * using RotAverageNclip4(). 
		 * otz represents 1/4 value of the average of z value of
		 * each vertex.
		 : ���_�̍��W�l����A�X�N���[�����W���v�Z����B
		 * otz �́A�S���_�̂y�̕��ϒl�� 1/4�B
		 */
		isomote = RotAverageNclip4(vp[0], vp[1], vp[2], vp[3], 
			(long *)&s->x0, (long *)&s->x1, 
			(long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

		/* If not surface, do nothing: �\�łȂ���� break */
		if (isomote <= 0) continue;    

		/* Put into OT:
		 * The length of the OT is assumed to 4096.
		 : OT �̒����� 4096 �ł��邱�Ƃ����肳��Ă���
		 */
		if (otz > 0 && otz < 4096) 	
			/* OT is assumed to be cleared in reversed order.
			 * In this condition, the OT pointer is calculated
			 * as simply ot+otz
			 : OT ���t���Ƀ\�[�g����Ă��邱�Ƃ����肳��Ă���B
			 * ���̏ꍇ�ɂ͓o�^���� OT �̃|�C���^�͒P����
			 * ot+otz �Ōv�Z�ł���
			 */
			AddPrim(ot+otz, s);
	}
	/* pop matrix regsiters which had been pushed.
	 : �Ҕ����Ă����}�g���b�N�X���Đݒ�
	 */
	PopMatrix();		
}

/*
 *	POLY_F4 cube with lighting
 :	POLY_F4 �ō\������闧���̂������v�Z���Ōv�Z����
 */
void add_cubeF4L(u_long *ot, POLY_F4 *s, MATRIX *rottrans, CVECTOR *c)
{
	int	i;
	long	p, otz, opz, flg;
	int	isomote;
	SVECTOR	**vp, **np;

	PushMatrix();		
	SetRotMatrix(rottrans);		
	SetTransMatrix(rottrans);	

	vp = v;		/* vp: vertex pointer (work) */
	np = n;		/* np: normal pointer (work) */

	for (i = 0; i < 6; i++, s++, c++, vp += 4, np++) {

		isomote = RotAverageNclip4(vp[0], vp[1], vp[2], vp[3], 
			(long *)&s->x0, (long *)&s->x1, 
			(long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

		if (isomote <= 0) continue;	

		if (otz > 0 && otz < 4096) {	
			/* Setting local color : ���[�J���J���[�̐ݒ� */
			NormalColorCol(*np, c, (CVECTOR *)&s->r0);
			AddPrim(ot+otz, s); 
		}
	}
	PopMatrix();		
}

/*
 *	POLY_F4 cube with fog
 :	POLY_F4 �ō\������闧���̂��f�v�X�L���[���Ōv�Z����
 */
void add_cubeF4F(u_long *ot, POLY_F4 *s, MATRIX *rottrans, CVECTOR *c)
{
	int	i;
	long	p, otz, opz, flg;
	int	isomote;
	SVECTOR	**vp, **np;
	MATRIX	light;

	PushMatrix();
	SetRotMatrix(rottrans);	
	SetTransMatrix(rottrans);

	vp = v;		/* vp: vertex pointer (work) */
	np = n;		/* np: normal pointer (work) */

	for (i = 0; i < 6; i++, s++, c++, vp += 4, np++) {
		isomote = RotAverageNclip4(vp[0], vp[1], vp[2], vp[3], 
	 		(long *)&s->x0, (long *)&s->x1, 
			(long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

		if (isomote <= 0) continue;	

		if (otz > 0 && otz < 4096) {
			/* depth queue: �f�v�X�L���[�C���O���s�� */
			NormalColorDpq(*np, c, p, (CVECTOR *)&s->r0);
			AddPrim(ot+otz, s);
		}
	}
	PopMatrix();		
}

void add_cubeFT4L(u_long *ot, POLY_FT4 *s, MATRIX *rottrans, CVECTOR *c)
{
	int	i;
	long	p, otz, opz, flg;
	int	isomote;
	SVECTOR	**vp, **np;

	PushMatrix();		
	SetRotMatrix(rottrans);		
	SetTransMatrix(rottrans);	

	vp = v;		/* vp: vertex pointer (work) */
	np = n;		/* np: normal pointer (work) */

	for (i = 0; i < 6; i++, s++, c++, vp += 4, np++) {

		isomote = RotAverageNclip4(vp[0], vp[1], vp[2], vp[3], 
			(long *)&s->x0, (long *)&s->x1, 
			(long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

		if (isomote <= 0) continue;	
		if (otz > 0 && otz < 4096) {	
			NormalColorCol(*np, c, (CVECTOR *)&s->r0);
			AddPrim(ot+otz, s); 
		}
	}
	PopMatrix();		
}

#if 0
/*
 *	POLY_FT4 cube
 :	POLY_FT4 �ō\������闧���̂��v�Z����
 */
void add_cubeFT4(u_long *ot, POLY_FT4 *s, MATRIX *rottrans)
{
	int	i;
	long	p, otz, opz, flg;
	int	isomote;
	SVECTOR	**vp;

	PushMatrix();		
	SetRotMatrix(rottrans);	
	SetTransMatrix(rottrans);

	vp = v;		/* vp: vertex pointer (work) */
	for (i = 0; i < 6; i++, s++, vp += 4) {
		isomote = RotAverageNclip4(vp[0], vp[1], vp[2], vp[3],
			   (long *)&s->x0, (long *)&s->x1,
			   (long *)&s->x3, (long *)&s->x2, &p, &otz, &flg);

		if (isomote < 0) continue;	
		
		if (otz > 0 && otz < 4096)
			AddPrim(ot+otz, s);
	}
	PopMatrix();		
}
#endif