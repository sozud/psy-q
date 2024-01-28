/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *		        Movie Sample Program
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date
 *	------------------------------------------
 *	1.00		Jul,14,1994	yutaka
 *	1.10		Sep,01,1994	suzu
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>	
#include <libcd.h>	

/*#define EMULATE*/

#ifdef EMULATE
#define StGetNext	StGetNextS
#define StFreeRing	StFreeRingS
#endif

#define IS_RGB24	0	/* 0:RGB16, 1:RGB24 */

#if IS_RGB24==1		
#define PPW	3/2		/* �P�V���[�g���[�h�ɉ��s�N�Z�����邩 */
#define MODE	3		/* 24bit ���[�h�Ńf�R�[�h */
#else			
#define PPW	1		/* �P�V���[�g���[�h�ɉ��s�N�Z�����邩 */
#define MODE	2		/* 16bit ���[�h�Ńf�R�[�h */
#endif

#define FILENAME	"\\DATA\\MOV.STR;1"
#define FRAME_SIZE	1712	/* �A�j���[�V�����̃t���[���� */
static CdlLOC loc;		/* CDROM���Đ�������|�C���g */

/*
 *	�f�R�[�h��
 */
typedef struct {
	u_long	*vlcbuf[2];	/* VLC �o�b�t�@�i�_�u���o�b�t�@�j */
	int	vlcid;		/* ���� VLC �f�R�[�h���o�b�t�@�� ID */
	u_short	*imgbuf;	/* �f�R�[�h�摜�o�b�t�@�i�V���O���j*/
	RECT	rect[2];	/* �]���G���A�i�_�u���o�b�t�@�j */
	int	rectid;		/* ���ݓ]�����̃o�b�t�@ ID */
	RECT	slice;		/* �P��� DecDCTout �Ŏ��o���̈� */
	int	isdone;		/* �P�t���[�����̃f�[�^���ł����� */
} DECENV;

static DECENV	dec;		/* �f�R�[�h���̎��� */
static int      Rewind_Switch;	/* CD���I��܂ł����ƂP�ɂȂ� */


/*
 *	�t�H�A�O���E���h�v���Z�X
 */	
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1);
static void strInit(CdlLOC *loc, int frame_size, 
		    void (*callback)(), int (*endcallback)());
static void strCallback();
static int endCallback();
static int strNextVlc(DECENV *dec);
static u_long *strNext(DECENV *dec);
static void strSync(DECENV *dec, int mode);
static void strKickCD(CdlLOC *loc);

main()
{
	DISPENV	disp;		
	DRAWENV	draw;		
	
	void	strCallback();		/* �R�[���o�b�N */
	int	endCallback();		/* �R�[���o�b�N */
	int	frame = 0;		/* �t���[���J�E���^ */
	int	id;			/* �\���o�b�t�@ ID */
	volatile u_long  enc_count;	/* �J�E���^ */

	u_char	result[8];		/* CD-ROM status */
	CdlFILE	file;
	
	ResetCallback();
	CdInit();
	PadInit(0);		/* PAD �����Z�b�g */
	CdSetDebug(0);
	ResetGraph(0);		/* GPU �����Z�b�g */
	SetGraphDebug(0);	/* �f�o�b�O���x���ݒ� */
	
	/* �v���~�e�B�u�������� */
	init_graph();
	
	/* �t�H���g�����[�h */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(32, 32, 320, 200, 0, 512));
		
	/* �t�@�C�����T�[�` */
	if (CdSearchFile(&file, FILENAME) == 0) {
		StopCallback();
		PadStop();
		exit();
	}
	loc.minute = file.pos.minute;
	loc.second = file.pos.second;
	loc.sector = file.pos.sector;

	/* ������ */
	strSetDefDecEnv(&dec, 640, 0, 640, 0);
	strInit(&loc, FRAME_SIZE, strCallback, endCallback);
	
	/* ���̃t���[���̃f�[�^���Ƃ��Ă��� VLC �̃f�R�[�h���s�Ȃ� */
	/* ���ʂ́Adec.vlcbuf[dec.vlcid] �Ɋi�[�����B*/
	strNextVlc(&dec);

	SetDispMask(1);		/* �\������ */
	while (1) {

		/* VLC �̊��������f�[�^�𑗐M */
		DecDCTin(dec.vlcbuf[dec.vlcid], MODE);

		/* �ŏ��̃f�R�[�h�u���b�N�̎�M�̏��������� */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
		
		/* ���̃t���[���̃f�[�^�� VLC �f�R�[�h */
		strNextVlc(&dec);

		/* �p�����[�^���o�͂��� */
		if (CdSync(1, 0) == CdlComplete) 
			CdControl(CdlGetlocP, 0, result);

		FntPrint("frame=%d\n", frame++);
		FntPrint("(%02x:%02x:%02X)\n",
			 result[5], result[6], result[7]);
		
		/* �����ɃA�v���P�[�V�����̃R�[�h���͂��� */
		if (graph() != 0) {
			CdControlB(CdlStop, 0, 0);
			PadStop();
			ResetGraph(3);
			StopCallback();
			/* exit(); */
			return 0;
		}
		
		/* �f�[�^���o���オ��̂�҂� */
		strSync(&dec, 0);
	}
}

/*
 * �f�R�[�h����������
 * imgbuf, vlcbuf �͍œK�ȃT�C�Y�����蓖�Ă�ׂ��ł��B	
 */
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1)
{
	static u_long	vlcbuf0[320/2*256];		/* �傫���K�� */
	static u_long	vlcbuf1[320/2*256];		/* �傫���K�� */
	static u_short	imgbuf[16*PPW*240*2];		/* �Z��P�� */

	dec->vlcbuf[0] = vlcbuf0;
	dec->vlcbuf[1] = vlcbuf1;
	dec->vlcid     = 0;
	dec->imgbuf    = imgbuf;
	dec->rectid    = 0;
	dec->isdone    = 0;	
	setRECT(&dec->rect[0], x0, y0, 640*PPW, 240);	
	setRECT(&dec->rect[1], x1, y1, 640*PPW, 240);
	setRECT(&dec->slice,   x0, y0,  16*PPW, 240);
}

/*
 * �X�g���[�~���O����������
 */
#define RING_SIZE 	32		/* �����O�o�b�t�@�T�C�Y */

static void strInit(CdlLOC *loc, int frame_size, 
		    void (*callback)(), int (*endcallback)())
{
	static u_long	sect_buff[RING_SIZE*SECTOR_SIZE];/* �����O�o�b�t�@ */
	
	DecDCTReset(0);		/* MDEC �����Z�b�g */
	Rewind_Switch = 0;	/* �����߂��O */

	/* MDEC���P�f�R�[�h�u���b�N�������������̃R�[���o�b�N���`���� */
	DecDCToutCallback(callback);

	/* �����O�o�b�t�@�̐ݒ� */
	StSetRing(sect_buff, RING_SIZE);
	
	/* �X�g���[�~���O���Z�b�g�A�b�v */
	/* �Ō�܂ł������� endcallback() ���R�[���o�b�N����� */
	StSetStream(IS_RGB24, 1, frame_size, 0, endcallback);

	/* �ŏ��̂P�t���[�����Ƃ��Ă��� */
	strKickCD(loc);
}


/*
 * �o�b�N�O���E���h�v���Z�X 
 * (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
static void strCallback()
{
#if IS_RGB24==1
	extern StCdIntrFlag;	
	if(StCdIntrFlag) {
		StCdInterrupt();	/* RGB24�̎��͂����ŋN������ */
		StCdIntrFlag = 0;
	}
#endif

	/* �f�R�[�h���ʂ��t���[���o�b�t�@�ɓ]�� */
	LoadImage(&dec.slice, (u_long *)dec.imgbuf);
	
	/* �Z���`�̈���ЂƂE�ɍX�V */
	dec.slice.x += dec.slice.w;
	
	/* �܂�����Ȃ���΁A*/
	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
		/* ���̒Z�����M */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	}
	/* �P�t���[�����I������A*/
	else {
		/* �I�������Ƃ�ʒm */
		dec.isdone = 1;
		
		/* ID ���X�V */
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
		
		/*DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);*/
	}
}

/*
 * �o�b�N�O���E���h�v���Z�X 
 * CDROM���I��܂ł�������Ă΂��֐�
 */
static int endCallback()
{
	Rewind_Switch = 1;
}


/* ���̃t���[���̃f�[�^����݂��� VLC �̉𓀂��s�Ȃ�
 * �G���[�Ȃ�΃����O�o�b�t�@���N���A���Ď��̃t���[�����l������ 
 * VLC���f�R�[�h���ꂽ�烊���O�o�b�t�@�̂��̃t���[���̗̈��
 * �K�v�Ȃ��̂Ń����O�o�b�t�@�̃t���[���̗̈��������� 
 */
static int strNextVlc(DECENV *dec)
{
	int cnt = WAIT_TIME;
	u_long	*next, *strNext();
	
	while ((next = strNext(dec)) == 0) {		/* get next frame */
		if (--cnt == 0)
			return(-1);
		StFreeRing(next);
	}
	dec->vlcid = dec->vlcid? 0: 1;			/* swap ID */
	DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);	/* VLC decode */
	StFreeRing(next);				/* free used frame */
	return(0);
}


/*
 * ���̂P�t���[����MOVIE�t�H�[�}�b�g�̃f�[�^�������O�o�b�t�@����Ƃ��Ă���
 * �f�[�^�����킾������ �����O�o�b�t�@�̐擪�A�h���X��
 * ����łȂ���� NULL��Ԃ�
 */  

static u_long *strNext(DECENV *dec)
{
	u_long		*addr;
	StHEADER	*sector;
	int		cnt = WAIT_TIME;
	static 	int      width  = 0;	        /* ��ʂ̉��Əc */
	static 	int      height = 0;	

	while(StGetNext(&addr, (u_long **)&sector)) {
		if (--cnt == 0)
			return(0);
		/* �����߂����ݒ肳��Ă�����擪�ɃW�����v */
		if(Rewind_Switch) {
			strKickCD(&loc);
			Rewind_Switch = 0;
		}
	}

	/* ��ʂ̉𑜓x�� �O�̃t���[���ƈႤ�Ȃ�� ClearImage �����s */
	if(width != sector->width || height != sector->height) {
		
		RECT	rect;
		setRECT(&rect, 0, 0, 640, 480);
		ClearImage(&rect, 0, 0, 0);
		
		width  = sector->width;
		height = sector->height;
	}

	/* �~�j�w�b�_�ɍ��킹�ăf�R�[�h����ύX���� */
	dec->rect[0].w = dec->rect[1].w = width*PPW;
	dec->rect[0].h = dec->rect[1].h = height;
	dec->slice.h   = height;

	return(addr);
}

static void strSync(DECENV *dec, int mode)
{
	volatile u_long	cnt = WAIT_TIME;
	
	while (dec->isdone == 0) {
		if (--cnt == 0) {
			/* timeout: �����I�ɐؑւ��� */
			printf("time out in decoding !\n");
			dec->isdone = 1;
			dec->rectid = dec->rectid? 0: 1;
			dec->slice.x = dec->rect[dec->rectid].x;
			dec->slice.y = dec->rect[dec->rectid].y;
		}
	}
	dec->isdone = 0;
}

/*
 * CDROM��loc����READ����B
 */
static void strKickCD(CdlLOC *loc)
{
	/* �ړI�n�܂� Seek ���� */
	while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0);	
  
	/* �X�g���[�~���O���[�h��ǉ����ăR�}���h���s */
	while(CdRead2(CdlModeStream|CdlModeSpeed) == 0);	
}