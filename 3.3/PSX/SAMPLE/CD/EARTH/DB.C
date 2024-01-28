/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				db.c
 *			
 *		  (640x480) �_�u���o�b�t�@�n���h��
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Mar,15,1994	suzu
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * �_�u���o�b�t�@�����Z�b�g�A�b�v����
 * �t���[���_�u���o�b�t�@�́A�ȉ��̂ǂ��炩�B
 *	(0,0)-(xxx, 240), (0, 240)-(xxx, 480)	(xxx = 256/320/512/640)	
 *	(0,0)-(xxx, 480), (0,   0)-(xxx, 480)	(xxx = 256/320/512/640)	
 */	
static int	id = 0;		/* �o�b�t�@ ID */
static DRAWENV	draw[2];	/* �`��� */
static DISPENV	disp[2];	/* �\���� */
static int	font;		/* �f�o�b�O�v�����g�� */

db_init(w, h, z)
int	w, h;	/* �`��G���A */
int	z;	/* �v���W�F�N�V���� */
{
	/*
	 * �C���^�[���[�X�łȂ��ꍇ�̓_�u���o�b�t�@����B
	 */	 
	int	oy = (h==480)? 0: 240;
	
	/*
	 * �V�X�e���̏�����
	 */
	/*PadInit(0);			/* �R���g���[���p�b�h�̏����� */
	/*ResetGraph(0);		/* GPU �������� */
	InitGeom();			/* GTE �̏����� */
	SetGeomOffset(w/2, h/2);	/* �����_�̍��W��ݒ�i��ʒ����j */
	SetGeomScreen(h);		/* ���e�ʂ܂ł̋��� */
	
	/*
	 * �t���[���o�b�t�@��Ń_�u���o�b�t�@���\�����邽�߂̕`����E
	 * �\�����\���̂̃����o��ݒ肷��B
	 */	 
	SetDefDrawEnv(&draw[0], 0,  0, w, h);
	SetDefDispEnv(&disp[0], 0, oy, w, h);
	SetDefDrawEnv(&draw[1], 0, oy, w, h);
	SetDefDispEnv(&disp[1], 0,  0, w, h);

	/* isbg = 1 �ɂ���� PutDrawEnv() �̎��_�ŕ`��G���A���N���A����� */
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0(&draw[0], 60, 120, 120);
	setRGB0(&draw[1], 60, 120, 120);

	/* �t�H���g�\������ݒ� (VRAM �A�h���X=(960,256)) */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(32, 32, 512, 256, 0, 512));
}

/*
 * �_�u���o�b�t�@���X���b�v����
 */	
db_swap(ot)
u_long	*ot;
{
	id = id? 0: 1;
	DrawSync(0);
	VSync(0);
	PutDrawEnv(&draw[id]);		/* �`������X�V */
	PutDispEnv(&disp[id]);		/* �\�������X�V */
	DrawOTag(ot);			/* �n�s��̃v���~�e�B�u��`�� */
	/*DrawSync(0);*/
	FntFlush(-1);			/* �f�o�b�O�v�����g */
}
