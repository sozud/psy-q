/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto3: simple cube
 *
 *         Copyright (C) 1993-1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		  	  Draw 3D objects (cube) 
 :		  	  ��]���闧���̂�`�悷�� 
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/* libgte functions return 'otz', which indicates the OT linking position.
 * 'otz' is a quater value of the Z in the screen cordinate, and libgte
 * has 15bit (0-0x7fff) z range. Therefore maximum OT size should be 4096
 * (2^14)
 : OT�Ƀv���~�e�B�u��o�^����Ƃ��ɎQ�Ƃ��� otz �̒l��Ԃ�
 * libgte �֐��̑����́A���ۂ̉��s��(z)�̒l��1/4�̒l��Ԃ����߁A
 * OT�̃T�C�Y�����ۂ̕���\��1/4 (4096) �ł悢�B
 */
#define SCR_Z	(512)		/* screen depth: �X�N���[���̐[�� */
#define	OTSIZE	(4096)		/* OT size: �n�s�̃T�C�Y */

typedef struct {
	DRAWENV		draw;		/* drawing environment: �`��� */
	DISPENV		disp;		/* display environment: �\���� */
	u_long		ot[OTSIZE];	/* OT: �I�[�_�����O�e�[�u�� */
	POLY_F4		s[6];		/* cube surface: �����̂̑��� */
} DB;

static int pad_read(MATRIX *rottrans);
static void init_prim(DB *db, CVECTOR *c);

main()
{
	/* double buffer: �_�u���o�b�t�@ */
	DB		db[2];		
	
	/* current db: ���݂̃o�b�t�@�A�h���X */
	DB		*cdb;		
	
	/* rotation-translation matrix: ��]�E���s�ړ��}�g���b�N�X */
	MATRIX		rottrans;	
	
	/* color of cube: �����̂̐F */
	CVECTOR		col[6];		
	
	int		i;		/* work */
	int		dmy, flg;	/* work */

	/* initialize environment for double buffer (interlace)
	 : �_�u���o�b�t�@�p�̊��ݒ�i�C���^�[���[�X���[�h�j*/
	/*	buffer #0	(0,  0)-(640,480)
	 *	buffer #1	(0,  0)-(640,480)
	 */
	init_system(320, 240, SCR_Z, 0);
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
	SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
	SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);

	/* set surface colors : �����̂̑��ʂ̐F��ݒ肷�� */
	for (i = 0; i < 6; i++) {
		col[i].r = rand();	/* R */
		col[i].g = rand();	/* G */
		col[i].b = rand();	/* B */
	}

	/* set primitive parameters on buffer #0/#1
	   : �v���~�e�B�u�o�b�t�@�̏����ݒ� #0/#1 */
	init_prim(&db[0], col);	
	init_prim(&db[1], col);	

	/* enable to display: �\���J�n */
	SetDispMask(1);			

	/*
	 * When using interrace mode, there is no need to changing 
	 * draw/display environment for every frames because the same
	 * same area is used for both drawing and displaying images.
	 * Therefore, the environment should be set only at the first time.
	 * Even in this case, 2 primitive buffers are needed since drawing
	 * process runs parallely with CPU adn GPU.
	 :
	 * �C���^�[���[�X���[�h�̏ꍇ�A�`��^�\���Ƃ��t���[���o�b�t�@����
	 * �����̈���g�p���Ă��邽�߁A�P�t���[�����Ƃɕ`����^�\������
	 * �ؑւ����s���K�v������܂���B
	 * ���������Ċ��ݒ�͍ŏ��Ɉ�x�����s���܂��B
	 * �������A�`��̓v���O�����̏����ƕ���ɍs���邽�߁A
         *  �v���~�e�B�u�̓_�u���Ŏ��K�v������܂��B
	 */
	PutDrawEnv(&db[0].draw);	/*: �`����̐ݒ� */
	PutDispEnv(&db[0].disp);	/*: �\�����̐ݒ� */
	
	/* loop while [select] key: select �L�[���������܂Ń��[�v���� */
	while (pad_read(&rottrans) == 0) {	

		/* swap double buffer: �_�u���o�b�t�@�|�C���^�̐؂�ւ� */
		cdb = (cdb==db)? db+1: db;	
		
		/* clear OT.
		 * ClearOTagR() clears OT as reversed order. This is natural
		 * for 3D type application, because OT pointer to be linked
		 * is simply related to Z value of the primivie. ClearOTagR()
		 * is faster than ClearOTag because it uses hardware DMA
		 * channel to clear.
		 : OT ���N���A����B
		 * ClearOTagR() �͋t���� OT �𐶐�����B����� 3D �̃A�v��
		 * �P�[�V�����̏ꍇ�ɂ͎��R�ȏ��ԂɂȂ�B�܂� ClearOTagR()
		 * �̓n�[�h�E�F�A�� OT ���N���A����̂ō���
		 */
		ClearOTagR(cdb->ot, OTSIZE);	

		/* add cube int OT: �����̂��n�s�ɓo�^���� */
		add_cubeF4(cdb->ot, cdb->s, &rottrans);

		/* When using interlaced single buffer, all drawing have to be
		 * finished in 1/60 sec. Therefore we have to reset the drawing
		 * procedure at the timing of VSync by calling ResetGraph(1)
		 * instead of DrawSync(0)
		 : �C���^�[���[�X���[�h�̏ꍇ�́A���ׂĂ̕`�揈���� 1/60sec ��
		 * �I�����Ȃ��Ă͂Ȃ�Ȃ��B���̂��߁ADrawSync(0) �̑����
		 * ResetGraph(1) ���g�p����VSync �̃^�C�~���O�ŕ`���ł��؂�
		 */
		VSync(0);	
		ResetGraph(1);	

		/* clear background: �w�i�̃N���A */
		ClearImage(&cdb->draw.clip, 60, 120, 120);
		
		/* Draw Otag.
		 * Since ClearOTagR() clears the OT as reversed order, the top
		 * pointer of the table is ot[OTSIZE-1]. Notice that drawing
		 * start point is not ot[0] but ot[OTSIZE-1].
		 : ClearOTagR() �� OT ���t���ɃN���A����̂� OT �̐擪�|�C
		 * ���^�� ot[0] �ł͂Ȃ��� ot[OTSIZE-1] �ɂȂ�B���̂��߁A
		 * DrawOTag() �� ot[OTSIZE-1] ����J�n���Ȃ��Ă͂Ȃ�Ȃ�
		 * ���Ƃɒ���
		 */
		/*DumpOTag(cdb->ot+OTSIZE-1);	/* for debug */
		DrawOTag(cdb->ot+OTSIZE-1);	
		FntFlush(-1);
	}
        /* close controller: �R���g���[�����N���[�Y */
	PadStop();	
	ResetGraph(1);
	StopCallback();
	return;
}

/* 
 *  Analyzing PAD and setting Matrix
 : �R���g���[���̉�͂ƁA�ϊ��}�g���b�N�X�̐ݒ���s���B
 */
/* MATRIX *rottrans;	��]�E���s�ړ��}�g���b�N�X */
static int pad_read(MATRIX *rottrans)
{
	
	/* angle of rotation: ��]�p�x( 360��= 4096 ) */
	static SVECTOR	ang  = { 0, 0, 0};	
	
	/* translation vertex: ���s�ړ��x�N�g�� */
	static VECTOR	vec  = {0, 0, SCR_Z};	

	/* read from controller: �R���g���[������f�[�^��ǂݍ��� */
	u_long	padd = PadRead(1);	
	
	int	ret = 0;

	/* quit: �I�� */
	if (padd & PADselect) 	ret = -1;	

	/* change rotation angle: ��]�p�x�̕ύX�iz, y, x �̏��ɉ�]�j */
	if (padd & PADRup)	ang.vz += 32;
	if (padd & PADRdown)	ang.vz -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;

	/* distance from screen : ���_����̋��� */
	if (padd & PADL1)	vec.vz += 8;
	if (padd & PADR1) 	vec.vz -= 8;

	/* calculate matrix: �}�g���b�N�X�̌v�Z */
	RotMatrix(&ang, rottrans);	/* rotation: ��] */
	TransMatrix(rottrans, &vec);	/* translation: ���s�ړ� */

	/* set matrix: �}�g���b�N�X�̐ݒ� */
	SetRotMatrix(rottrans);		/* rotation: ��] */
	SetTransMatrix(rottrans);	/* translation: ���s�ړ� */

	/* print status: ���݂̃W�I���g���󋵂��v�����g */
	FntPrint("tuto3: simple cube angle=(%d,%d,%d)\n",
		 ang.vx, ang.vy, ang.vz);
		
	return(ret);
}

/*
 *	Initialization of Primitives
 :	�v���~�e�B�u�̏�����
 */
/*DB	*db;	primitive buffer: �v���~�e�B�u�o�b�t�@ */
/*CVECTOR *c;	coloer of cube surface: ���ʂ̐F */
static void init_prim(DB *db, CVECTOR *c)
{
	int	i;

	/* initialize for side polygon: ���ʂ̏����ݒ� */
	for (i = 0; i < 6; i++) {
		/* initialize POLY_FT4: �t���b�g�S�p�`�v���~�e�B�u�̏����� */
		SetPolyF4(&db->s[i]);	
		setRGB0(&db->s[i], c[i].r, c[i].g, c[i].b);
	}
}