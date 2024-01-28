/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				db.c
 *			
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Mar,15,1994	suzu
 *		1.10		Jun,19,1995	suzu
 *
 *		      frame double buffer handler
 :		        �_�u���o�b�t�@�n���h��
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * Setup frame double buffer
 *	(0,0)-(xxx, 240), (0, 240)-(xxx, 480)	(xxx = 256/320/512/640)	
 *	(0,0)-(xxx, 480), (0,   0)-(xxx, 480)	(xxx = 256/320/512/640)	
 */	
static int	id = 0;		
static DRAWENV	draw[2];	
static DISPENV	disp[2];	

/* initiaize graphic environment for double buffer
 : �_�u���o�b�t�@�̂��߂̕`��E�\����������������
 */
/* int	  w, h;		drawing/display area size: �`��E�\���G���A */
/* int	  z;		projection: �v���W�F�N�V���� */
/* u_char r0, g0, b0;	background color: �w�i�F */

void db_init(int w, int h, int z, u_char r0, u_char g0, u_char b0)
{
	/*
	 * If width == 480, not to do double buffering
	 : �C���^�[���[�X�łȂ��ꍇ�̓_�u���o�b�t�@����B
	 */	 
	int	oy = (h==480)? 0: 240;
	
	PadInit(0);			
	ResetGraph(0);			
	InitGeom();			

	/* set GTE origin at the center of screen: ���_����ʒ����ɂ��� */
	SetGeomOffset(w/2, h/2);	

	/* set distance to the screen : ���e�ʂ܂ł̋��� */
	SetGeomScreen(z);	
	
	/* inititalize double buffer
	 : �t���[���o�b�t�@��Ń_�u���o�b�t�@���\�����邽�߂̕`����E
	 * �\�����\���̂̃����o��ݒ肷��B
	 */	 
	SetDefDrawEnv(&draw[0], 0,  0, w, h);
	SetDefDispEnv(&disp[0], 0, oy, w, h);
	SetDefDrawEnv(&draw[1], 0, oy, w, h);
	SetDefDispEnv(&disp[1], 0,  0, w, h);

	/* if isbg == 1, PutDrawEnv clears drawing area automatically.
	 : isbg = 1 �ɂ���� PutDrawEnv() �̎��_�ŕ`��G���A���N���A�����
	 */
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0(&draw[0], r0, g0, b0);
	setRGB0(&draw[1], r0, g0, b0);

	/* load font on (960,256)
	 :  �t�H���g�\������ݒ� (VRAM �A�h���X=(960,256))
	 */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(32, 32, 0, 0, 2, 1024));
}

/* swap double buffer: �_�u���o�b�t�@���X���b�v���� */
db_swap(u_long *ot)
{
	id = id? 0: 1;			/* swap id */
	if (draw[id].dfe) {
		DrawSync(0);		/* sync GPU */
		VSync(0);		/* wait for V-BLNK */
	}
	else {
		DrawSync(0);
		VSync(0);		/* wait for V-BLNK */
		ResetGraph(1);		/* reset GPU */
	}
	PutDrawEnv(&draw[id]);		/* update DRAWENV */
	PutDispEnv(&disp[id]);		/* update DISPENV */
	DrawOTag(ot);			/* draw OT */
	FntFlush(-1);			/* flush debug message */
}
