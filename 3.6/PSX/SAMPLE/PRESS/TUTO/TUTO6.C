/* $PSLibId: Runtime Library Release 3.6$ */
/*			tuto6 simplest sample
 *
 *	Complete background on-memory movie decompression (with frame
 *	double buffering). Notice VLC decoding is done in CPU (foreground)
 *	
 :	�o�b�N�O���E���h����̍Đ��i�t���[���_�u���o�b�t�@�t���j
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
 * Decode Environment. notice that image buffer in main memory is single
 *�f�R�[�h��: ���C����������̉𓀉摜�o�b�t�@�͂ЂƂȂ��Ƃɒ���
 */
typedef struct {
	/* VLC buffer (double): VLC �o�b�t�@�i�_�u���o�b�t�@�j */
	u_long	*vlcbuf[2];	
	
	/* current VLC buffer ID: ���� VLC �f�R�[�h���o�b�t�@�� ID */
	int	vlcid;		
	
	/* decoded image buffer (single): �f�R�[�h�摜�o�b�t�@�i�V���O���j*/
	u_long	*imgbuf;	
	
	/* frame buffer area (double):  �]���G���A�i�_�u���o�b�t�@�j */
	RECT	rect[2];	
	
	/* current decompressing buffer ID: ���ݓ]�����̃o�b�t�@ ID */
	int	rectid;		
	
	/* one slice area of one DecDCTout: �P��� DecDCTout �Ŏ��o���̈� */
	RECT	slice;		
	
} DECENV;
static DECENV		dec;		
	
/* flag indicating the enf of decoding. 'volatile' option is necessary.
 :�t���[���̍Ō�ɂȂ�� 1 �ɂȂ�B volatile �w��͕K�{
 */
static volatile int	isEndOfFlame;	

/*
 * background operation. This function is called when DecDCTout is finished
 : �o�b�N�O���E���h�v���Z�X 
 * (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
void out_callback()
{
	/* transfer decoded data to frame buffer
	 :load�f�R�[�h���ʂ��t���[���o�b�t�@�ɓ]��
	 */
	LoadImage(&dec.slice, dec.imgbuf);
	
	/* update sliced image area: �Z���`�̈���ЂƂE�ɍX�V */
	dec.slice.x += 16;

	/* if decoding is not complete: �܂�����Ȃ���΁A*/
	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
		/* get next slice image from MDEC: ���̒Z�����M */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	}
	/* when all frame image is decompressed: �P�t���[�����I������A*/
	else {
		/* notify the end of decompression: �I�������Ƃ�ʒm */
		isEndOfFlame = 1;
		
		/* update ID of each buffer: ID ���X�V */
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
	}
}		

/*
 * foreground operation: �t�H�A�O���E���h�v���Z�X
 */	
/* following work buffer size should be dynamically alocated using malloc().
 * the size of buffer can be known by DecDCTvlcBufSize()
 : �ȉ��̍�Ɨ̈�� malloc() ���g�p���ē��I�Ɋ���t����ׂ��ł��B
 * �o�b�t�@�̃T�C�Y�� DecDCTvlcBufSize() �Ŋl���ł��܂��B
 */	
static u_long	vlcbuf0[256*256];	
static u_long	vlcbuf1[256*256];	
static u_long	imgbuf[16*240/2];	/* 1 slice size: �Z��P�� */

static void StrRewind(void);
static u_long *StrNext(void);

/*
 * VLC maximum count of decoding 
 * DecDCTvlc() is foreground and if it occupies CPU for a long time,
 * set maximum VLC decode size here. DecDCTvlc returns after
 * decompression VLC_SIZE words. You can restart VLC decoding again
 * by calling DecDCTvlc() again
 *
 : VLC �̃f�R�[�h���[�h��:
 * VLC �f�R�[�h�������ԑ��̏������u���b�N����ƕs�s���ȏꍇ�́A����
 * �f�R�[�h���[�h�̍ő�l�������Ŏw�肷��B
 * DecDCTvlc() �� VLC_SIZE���[�h�� VLC ���f�R�[�h����ƈ�U������A�v��
 * �P�[�V�����ɖ߂��B
 */
#define VLC_SIZE	1024	

main()
{
	DISPENV	disp;			/* display environment: �\���� */
	DRAWENV	draw;			/* drawing environment: �`��� */
	RECT	rect;
	void	out_callback();		/* callback: �R�[���o�b�N */
	int	id;
	u_long	padd;
	u_long	*next, *StrNext();	/* CD-ROM simulator: CD-ROM �̑��� */
	int	isvlcLeft;		/* flag indicating end of VLC decode
					 :VLC �f�R�[�h�I���t���O
					 */
	
	PadInit(0);		/* reset controler: PAD �����Z�b�g */
	ResetGraph(0);		/* reset GPU: GPU �����Z�b�g */
	DecDCTReset(0);		/* reset MDEC: MDEC �����Z�b�g */
	SetDispMask(1);		/* start display: �\������ */
	isEndOfFlame = 0;	/* clear flag: ���������� */
	
	/* clear frame buffer: �t���[���o�b�t�@���N���A */
	setRECT(&rect, 0, 0, 640,480);
	ClearImage(&rect, 0, 0, 0);

	/* initialize font environment: �t�H���g���[�h */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 16, 256, 16, 1, 512));
	
	/* intialize decoding environment: �f�R�[�h�\���̂ɒl��ݒ� */
	dec.vlcbuf[0] = vlcbuf0;
	dec.vlcbuf[1] = vlcbuf1;
	dec.vlcid     = 0;
	dec.imgbuf    = imgbuf;
	dec.rectid    = 0;
	
	setRECT(&dec.rect[0], 0,  32, 256, 176);
	setRECT(&dec.rect[1], 0, 272, 256, 176);
	setRECT(&dec.slice,   0,  32,  16, 176);
		
	/* define callback: �R�[���o�b�N���`���� */
	DecDCToutCallback(out_callback);
	
	/* rewind CD-ROM simulator: �t���[���������߂� */
	StrRewind();
	
	/* decode the first VLC: �܂��ŏ��� VLC ������ */
	DecDCTvlcSize(0);
	DecDCTvlc(StrNext(), dec.vlcbuf[dec.vlcid]);

	/* main loop: ���C�����[�v */
	while (1) {
		/* send run-level (result of VLC decoding) to MDEC
		 : VLC �̌��ʁi�������x���j�𑗐M����
		 */
		DecDCTin(dec.vlcbuf[dec.vlcid], 0);	
	
		/* swap VLC ID: ID ���X���b�v */
		dec.vlcid = dec.vlcid? 0: 1;		

		/* start recieving the first decoded slice image.
		 * next one is called in the callback.
		 : �ŏ��̒Z��̎�M�̏��������� 
		 * �Q���ڂ���́Acallback() ���ōs�Ȃ�
		 */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	
		/* read next BS from CD-ROM simulator:
		 * ���� BS ���^�� CD-ROM ���ǂݏo���B*/
		if ((next = StrNext()) == 0)	
			break;
		
		/* decode first VLC_SIZE words of VLC buffer
		 : vlc �̍ŏ��� VLC_SIZE ���[�h���f�R�[�h����
		 */ 
		DecDCTvlcSize(VLC_SIZE);
		isvlcLeft = DecDCTvlc(next, dec.vlcbuf[dec.vlcid]);	

		/* display consumed time: �o�ߎ��Ԃ�\�� */
		FntPrint("slice=%d,", VSync(1));
		
		/* wait for end of decoding: �f�[�^���o���オ��̂�҂� */
		do {
			/* decode next VLC_SIZE words of VLC buffer
			 : vlc �̎c��� VLC_SIZE ���[�h���f�R�[�h����
			 */ 
			if (isvlcLeft) {
				isvlcLeft = DecDCTvlc(0, 0);
				FntPrint("%d,", VSync(1));
			}

			/* watch controler: �R���g���[�����Ď����� */
			if ((padd = PadRead(1)) & PADk) {
				PadStop();
				StopCallback();
				return(0);
			}
		} while (isvlcLeft || isEndOfFlame == 0);
		FntPrint("%d\n", VSync(1));
		isEndOfFlame = 0;
			
		/* wait for V-BLNK: V-BLNK ��҂� */
		VSync(0);
		
		/* swap disply buffer.
		 * Notice that drawing buffer (tranfering buffer) is
		 * always opposite one of displaying buffer.
		 * �\���o�b�t�@���X���b�v 
		 * �\���o�b�t�@�́A�`��o�b�t�@�̔��Α��Ȃ��Ƃɒ���
		 */
		id = dec.rectid? 0: 1;
		
		PutDispEnv(SetDefDispEnv(&disp, 0, id==0? 0:240, 256, 240));
		PutDrawEnv(SetDefDrawEnv(&draw, 0, id==0? 0:240, 256, 240));
		FntFlush(-1);
	}
}

/*
 *	read next bitstream data (pseudo CD-ROM simulator)	
 :	���̃r�b�g�X�g���[����ǂݍ��ށi�{���� CD-ROM ���痈��j
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
