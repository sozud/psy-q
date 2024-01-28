/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto6 simplest sample
 *
 *	�o�b�N�O���E���h����̍Đ��i�t���[���_�u���o�b�t�@�t���j
 *		�������AVLC �̃f�R�[�h�͕\�Ŏ��s����
 *	
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libpress.h>

/*
 *	�f�R�[�h��
 */
typedef struct {
	u_long	*vlcbuf[2];	/* VLC �o�b�t�@�i�_�u���o�b�t�@�j */
	int	vlcid;		/* ���� VLC �f�R�[�h���o�b�t�@�� ID */
	u_long	*imgbuf;	/* �f�R�[�h�摜�o�b�t�@�i�V���O���j*/	
	RECT	rect[2];	/* �]���G���A�i�_�u���o�b�t�@�j */
	int	rectid;		/* ���ݓ]�����̃o�b�t�@ ID */
	RECT	slice;		/* �P��� DecDCTout �Ŏ��o���̈� */
} DECENV;

static DECENV		dec;		/* �f�R�[�h���̎��� */
static volatile int	isEndOfFlame;	/* �t���[���̍Ō�ɂȂ�� 1 �ɂȂ� */

/*
 * �o�b�N�O���E���h�v���Z�X 
 * (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
void out_callback()
{
	/* �f�R�[�h���ʂ��t���[���o�b�t�@�ɓ]�� */
	LoadImage(&dec.slice, dec.imgbuf);
	/*DrawSync(0);*/
	
	/* �Z���`�̈���ЂƂE�ɍX�V */
	dec.slice.x += 16;

	/* �܂�����Ȃ���΁A*/
	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
		/* ���̒Z�����M */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	}
	/* �P�t���[�����I������A*/
	else {
		/* �I�������Ƃ�ʒm */
		isEndOfFlame = 1;
		
		/* ID ���X�V */
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
	}
}		

/*
 *	�t�H�A�O���E���h�v���Z�X
 */	
static u_long	vlcbuf0[256*256];	/* �傫���K�� */
static u_long	vlcbuf1[256*256];	/* �傫���K�� */
static u_long	imgbuf[16*240/2];	/* �Z��P�� */

static void StrRewind(void);
static u_long *StrNext(void);

/*
 * VLC �̃f�R�[�h���[�h��:
 * VLC �f�R�[�h�������ԑ��̏������u���b�N����ƕs�s���ȏꍇ�́A����
 * �f�R�[�h���[�h�̍ő�l�������Ŏw�肷��B
 * DecDCTvlc() �� VLC_SIZE���[�h�� VLC ���f�R�[�h����ƈ�U������A�v��
 * �P�[�V�����ɖ߂��B
 */
#define VLC_SIZE	1024	

main()
{
	DISPENV	disp;			/* �\���� */
	DRAWENV	draw;			/* �`��� */
	RECT	rect;
	void	out_callback();		/* �R�[���o�b�N */
	int	id;
	u_long	padd;
	u_long	*next, *StrNext();	/* CD-ROM �̑��� */
	int	isvlcLeft;		/* VLC �f�R�[�h�I���t���O */
	
	PadInit(0);		/* PAD �����Z�b�g */
	ResetGraph(0);		/* GPU �����Z�b�g */
	DecDCTReset(0);		/* MDEC �����Z�b�g */
	SetGraphDebug(0);	/* �f�o�b�O���x���ݒ� */
	SetDispMask(1);		/* �\������ */
	isEndOfFlame = 0;	/* ���������� */
	
	/* �t���[���o�b�t�@���N���A */
	setRECT(&rect, 0, 0, 640,480);
	ClearImage(&rect, 0, 0, 0);

	/* �t�H���g���[�h */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 16, 256, 16, 1, 512));
	
	/* �f�R�[�h�\���̂ɒl��ݒ� */
	dec.vlcbuf[0] = vlcbuf0;
	dec.vlcbuf[1] = vlcbuf1;
	dec.vlcid     = 0;
	dec.imgbuf    = imgbuf;
	dec.rectid    = 0;
	
	setRECT(&dec.rect[0], 0,  32, 256, 176);
	setRECT(&dec.rect[1], 0, 272, 256, 176);
	setRECT(&dec.slice,   0,  32,  16, 176);
		
	/* �R�[���o�b�N���`���� */
	DecDCToutCallback(out_callback);
	
	/* �t���[���������߂� */
	StrRewind();
	
	/* �܂��ŏ��� VLC ������ */
	DecDCTvlcSize(0);
	DecDCTvlc(StrNext(), dec.vlcbuf[dec.vlcid]);

	/* ���C�����[�v */
	while (1) {
		/* VLC �̌��ʂ𑗐M���� */
		DecDCTin(dec.vlcbuf[dec.vlcid], 0);	/* run-level ��]�� */
		dec.vlcid = dec.vlcid? 0: 1;		/* ID ���X���b�v */

		/* �ŏ��̒Z��̎�M�̏��������� */
		/* �Q���ڂ���́Acallback() ���ōs�Ȃ� */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	
		/* ���̃T���v�������o���B*/
		if ((next = StrNext()) == 0)	
			break;
		
		/* vlc �̍ŏ��� VLC_SIZE ���[�h���f�R�[�h���� */ 
		DecDCTvlcSize(VLC_SIZE);
		isvlcLeft = DecDCTvlc(next, dec.vlcbuf[dec.vlcid]);	

		/* �o�ߎ��Ԃ�\�� */
		FntPrint("slice=%d,", VSync(1));
		
		/* �f�[�^���o���オ��̂�҂� */
		do {
			/* vlc �̎c��� VLC_SIZE ���[�h���f�R�[�h���� */ 
			if (isvlcLeft) {
				isvlcLeft = DecDCTvlc(0, 0);
				FntPrint("%d,", VSync(1));
			}
			
			if ((padd = PadRead(1)) & PADk) {
				PadStop();
				StopCallback();
				return(0);
			}
		} while (isvlcLeft || isEndOfFlame == 0);
		FntPrint("%d\n", VSync(1));
		isEndOfFlame = 0;
			
		/* V-BLNK ��҂� */
		VSync(0);
		
		/* �\���o�b�t�@���X���b�v */
		/* �\���o�b�t�@�́A�`��o�b�t�@�̔��Α��Ȃ��Ƃɒ��� */
		id = dec.rectid? 0: 1;
		
		PutDispEnv(SetDefDispEnv(&disp, 0, id==0? 0:240, 256, 240));
		PutDrawEnv(SetDefDrawEnv(&draw, 0, id==0? 0:240, 256, 240));
		FntFlush(-1);
	}
}

/*
 *	���̃X�g���[�~���O�f�[�^��ǂݍ��ށi�{���� CD-ROM ���痈��j
 */
static int frame_no = 0;
static void StrRewind(void)
{
	frame_no = 0;
}

static u_long *StrNext(void)
{
	extern	u_long	*mdec_frame[];

	FntPrint("%4d: %4d byte\n",
		 frame_no,
		 mdec_frame[frame_no+1]-mdec_frame[frame_no]);
	
	if (mdec_frame[frame_no] == 0) 
		return(mdec_frame[frame_no = 0]);
	else
		return(mdec_frame[frame_no++]);
}
