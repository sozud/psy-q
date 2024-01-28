/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *   ����BG�v���O����: �X�g���[�~���O��
 *
 *   Ver 1.00  1995 Feb 21
 *
 *		Copyright (C) 1995 by Sony Computer Entertainment
 *		All rights Reserved
 *   
 *  ���̃T�u���[�`���Q�� ������O���t�B�b�N�X�Ƒg�ݍ��킹�鎞�ɕ֗���
 *  �T�u���[�`���ł��B
 *
 *  ���C�����[�`�� tuto0.c���� ���J�֐����Ă�ł��܂��B
 *
 *  ���̃T�u���[�`���p�b�P�[�W�� ����̏�����D��I�ɍs�Ȃ��܂��BMDEC��
 *  �x�܂Ȃ��悤�� ���������_�u���o�b�t�@�Ŏ����Ă��܂��B���̂���
 *  ���C����������� �Q��ʕ��� 16bit/pixel �C���[�W�f�[�^�� �������邽�߂�
 *  �Q��ʕ��� VLC�o�b�t�@���Ƃ�܂��B
 *  ����̃X�s�[�h��D��ɂ��� ���C����������ߖ񂵂����ꍇ�� �C���[�W�f�[�^
 *  �̈� �y�� VLC�o�b�t�@�̈�̃_�u���o�b�t�@�͏ȗ��ł��܂��B�ȗ�����
 *  �T�u���[�`���p�b�P�[�W�͎��Ƀ����[�X����\��ł��B
 *
 *  ���C����������Ɋm�ۂ���̈�� ����̃T�C�Y�ɂ���đ������܂��B
 *  WIDTH HIGHT�ɓ���̃T�C�Y������m�ۂ���̈�̑傫�������肵�܂��B
 *  ������ ����̃f�[�^�̒��ɖ��ߍ��܂�Ă��� ����̃T�C�Y�� WIDTH,HIGHT
 *  �𒴂���Ɗm�ۂ����̈�𒴂��ă�������j�󂵖\������̂� ���Ȃ炸
 *  ��������̍ő�̃T�C�Y�� WIDTH,HIGHT�Ŏw�肵�Ă����K�v������܂��B
 *
 *
 *  �����֐��� cdrom_play()�� ����̏ꏊ�ɃV�[�N�� �A�j���[�V�����̍Đ���
 *  �n�߂�֐��ł��� ���̃v���O�����ł̓V�[�N����Ɏ��s�������̃G���[����
 *  �����S�ł͂���܂���B���ӂ��ĉ������B
 */

/*
 * VLC �̃f�R�[�h���[�h��:
 * VLC �f�R�[�h�������ԑ��̏������u���b�N����ƕs�s���ȏꍇ�́A����
 * �f�R�[�h���[�h�̍ő�l�������Ŏw�肷��B
 * DecDCTvlc() �� VLC_SIZE���[�h�� VLC ���f�R�[�h����ƈ�U������A�v��
 * �P�[�V�����ɖ߂��B
 */
#define VLC_SIZE	2048

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <libsnd.h>

/*********************** ���J�֐� *******************/
void init_anim(void);
void start_anim(CdlFILE *,u_long);
void load_image_for_mdec_data(u_long,u_long);
void poll_anim(CdlFILE *);

/*********************** �����֐� *******************/
static void setup_frame();
static void out_callback();
static u_long *get_frame_movie(u_long);
static cdrom_play(CdlLOC *);


#define WIDTH 320		/* �X�g���[�~���O�f�[�^�̍ő�̉� */
#define HIGHT 240		/* �X�g���[�~���O�f�[�^�̍ő�̏c */
#define SLICE  16		/* �^���U�N�̕� */



/*
 *	�f�R�[�h��
 */
typedef struct {
        u_long	*vlcbuf[2];	/* VLC �o�b�t�@�i�_�u���o�b�t�@�j */
	int	vlcid;		/* ���� VLC �f�R�[�h���o�b�t�@�� ID */
	u_short	*imgbuf[2];	/* �f�R�[�h�摜�o�b�t�@�i�_�u���j*/
	RECT	rect[2];	/* �]���G���A�i�_�u���o�b�t�@�j */
	int	rectid;		/* ���ݓ]�����̃o�b�t�@ ID */
	RECT	slice;		/* �P��� DecDCTout �Ŏ��o���̈� */
} DECENV;

static DECENV	dec;		/* �f�R�[�h���̎��� */
static int	MdecFree;	/* MDEC BUSY STATUS */
static int      Frame_ny;   /* STREAMING FLAME DATA NOT READY */
static int      slicew = 0;	/* ��ʂ̉��Əc */
static int      sliceh = 0;
static int      Vlc_size = 0;	/* ��x��VLC����T�C�Y */


static int      Rewind_Switch;	/* CD���I��܂ł����ƂP�ɂȂ� */
static u_long   EndFrame = 0;	/* �X�g���[�~���O���I��点��t���[�� */

#define RING_SIZE 24		/* �X�g���[�~���O���C�u�����Ŏg�p�����
				   �����O�o�b�t�@�̃T�C�Y */

u_long SECT_BUFF[RING_SIZE*SECTOR_SIZE]; /* �X�g���[�~���O���C�u�����Ŏg�p
					    ����郊���O�o�b�t�@�̎��� */


/*
 * �o�b�N�O���E���h�v���Z�X 
 * (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
static void out_callback()
{
  MdecFree = 1;			/* ���̊֐����R�[���o�b�N����鎞�ɂ͂P����
				   �G�̃f�R�[�h���I����Ă��� */
}


/*
 *	�t�H�A�O���E���h�v���Z�X
 */	
#define setRECT(r, _x, _y, _w, _h) \
	(r)->x = (_x),(r)->y = (_y),(r)->w = (_w),(r)->h = (_h)


DISPENV	disp;			/* �\���� */
u_long	vlcbuf0[WIDTH/2*HIGHT/2];	/* �傫���K�� */
u_long	vlcbuf1[WIDTH/2*HIGHT/2];	/* �傫���K�� */
u_short	imgbuf0[WIDTH*HIGHT];	/* 1��ʕ��̃t���[���o�b�t�@ */
u_short	imgbuf1[WIDTH*HIGHT];	/*�P��ʕ��̃t���[���o�b�t�@ */


void init_anim()		/* ���������[�`���Q */
{
  RECT rect;
  int i;
  
  DecDCTReset(0);		/* MDEC �����Z�b�g */
  MdecFree = 0;			/* ���������� */
  Frame_ny = 0;
  Rewind_Switch = 0;		/* �����߂��O */
  Vlc_size = 0;
  
  /* �f�R�[�h�\���̂ɒl��ݒ� */
  dec.vlcbuf[0] = vlcbuf0;
  dec.vlcbuf[1] = vlcbuf1;
  dec.vlcid     = 0;
  dec.imgbuf[0]  = imgbuf0;
  dec.imgbuf[1]  = imgbuf1;
  dec.rectid    = 0;
  setRECT(&dec.rect[0], 0,   0, WIDTH, HIGHT);
  setRECT(&dec.rect[1], 0, HIGHT, WIDTH, HIGHT);
  setRECT(&dec.slice,   0,   0,  SLICE, HIGHT);

  setRECT(&rect, 0, 0, WIDTH, HIGHT*2);	/* clear display buff */
  ClearImage(&rect, 0, 0, 0);

  for(i=0;i<WIDTH*HIGHT;i++)	/* clear imgbuf */
    {
      imgbuf0[i]=0;
      imgbuf1[i]=0;
    }
  
  SsInit();		/* SPU lib init */
  SsSetSerialAttr(SS_SERIAL_A,SS_MIX,SS_SON); /* Audio out enable */
  SsSetSerialVol(SS_SERIAL_A,0x7fff,0x7fff);  /* Audio volume set */
  
  /* �X�g���[�~���O���C�u�����Ŏg���郊���O�o�b�t�@�̐ݒ� */
  StSetRing(SECT_BUFF,RING_SIZE);
  
  /* MDEC���P�f�R�[�h�u���b�N�������������̃R�[���o�b�N���`���� */
  DecDCToutCallback(out_callback);
}


/*
 * CDROM��{���ŃX�^�[�g�����X�g���[�~���O���J�n����
 * CDROM���Ō�܂ł������� end_cdrom() ���R�[���o�b�N�����
 *
 * fp          �Đ�����X�g���[�~���O�t�@�C���̃t�@�C���|�C���^
 * endFrame    �X�g���[�~���O�̍Ō�̃t���[���ԍ��i�{���̏I������
 *             2����3�t���[���O�ɐݒ肵�Ȃ��� �t���[�����ǂݔ�΂��ꂽ����
 *             �I��肪���o�ł��Ȃ��̂Œ���
 */
void start_anim(fp,endFrame)
CdlFILE *fp;
u_long  endFrame;
{
  int ret_val;
  u_long *frame_addr;
  
  StSetStream(0,1,0xffffffff,0,0);
  
  cdrom_play(&fp->pos);

  EndFrame = endFrame;
  
  /* �ŏ��̂P�t���[�����Ƃ��Ă��� */
  while((frame_addr = get_frame_movie(EndFrame))==0);
  DecDCTvlcSize(0);  
  /* VLC�f�R�[�h���s�Ȃ� */
  DecDCTvlc(frame_addr, dec.vlcbuf[dec.vlcid]);
  
  /* VLC���f�R�[�h���ꂽ�烊���O�o�b�t�@�̂��̃t���[���̗̈��
     �K�v�Ȃ��̂Ń����O�o�b�t�@�̃t���[���̗̈��������� */
  StFreeRing(frame_addr);
  
  /* �P�t���[���S�Ẵf�[�^��MDEC�֓��͂���悤�ɐݒ肷�� */
  DecDCTin(dec.vlcbuf[dec.vlcid], 2);
  
  /* 1�t���[���S�Ẵf�[�^��MDEC����Ƃ�o���悤�ɐݒ肷�� */
  DecDCTout(dec.imgbuf[dec.rectid], slicew*sliceh/2);
  
  /* ID ���X���b�v */
  dec.vlcid = dec.vlcid? 0: 1;
  
  /* ���̃t���[���̃f�[�^���Ƃ��Ă��� ���� ���̃t���[����
     �f�[�^���G���[�������� 1���Ԃ� */
  /* �G���[�������烊���O�o�b�t�@���N���A����
     ���̃t���[���̃f�[�^���Ƃ��Ă��� */
  while((frame_addr=get_frame_movie(EndFrame))==0);
  DecDCTvlc(frame_addr, dec.vlcbuf[dec.vlcid]);
  StFreeRing(frame_addr);
}

/*
 * ���̂P�t���[����MOVIE�t�H�[�}�b�g�̃f�[�^�������O�o�b�t�@����Ƃ��Ă���
 * 1�t���[�����̃f�[�^�������Ă���� �P�t���[���̃f�[�^�̐擪�A�h���X��
 * �܂������ĂȂ���� NULL��Ԃ�
 */
static u_long *get_frame_movie(endFrame)
u_long endFrame;
{
  u_long   *addr;
  StHEADER *sector;
  
  if(StGetNext(&addr,(u_long **)&sector))
    return(0);			/* �܂��P�t���[���̃f�[�^�������O�o�b�t�@
				   ��ɑ����Ă��Ȃ� */
  
  if(sector->frameCount > endFrame)
    Rewind_Switch = 1;
  
  /* ��ʂ̉𑜓x�� �O�̃t���[���ƈႤ�Ȃ�� ClearImage �����s */
  if(slicew != sector->width || sliceh != sector->height) {
    RECT    rect;
    setRECT(&rect, 0, 0, WIDTH, HIGHT);
    ClearImage(&rect, 0, 0, 0);
    slicew  = sector->width;
    sliceh  = sector->height;
  }
  
  /* �~�j�w�b�_�ɍ��킹�ăf�R�[�h����ύX���� */
  dec.rect[0].w = dec.rect[1].w = slicew;
  dec.rect[0].h = dec.rect[1].h = sliceh;
  dec.slice.h   = sliceh;
  
  return(addr);
}


/*
 * ���C���������Ƀ^���U�N���łȂ��ł���MDEC�ɂ���ăf�R�[�h���ꂽ
 * �C���[�W�f�[�^�� �^���U�N���� VRAM�֓]������
 * ������ VRAM�̃A�h���X������
 */
void load_image_for_mdec_data(xo,yo)
u_long xo,yo;
{
  int i,x;
  RECT tmprect;
  
  for(i=0;i<slicew/16;i++)
    {
      x = i*16;
      tmprect.x = x+xo;
      tmprect.y = yo;
      tmprect.w = 16;
      tmprect.h = sliceh;
      
      LoadImage(&tmprect, (u_long*)(dec.imgbuf[dec.rectid? 0: 1]+x*sliceh));
      DrawSync(0);		/* �]�����I���܂ő҂� */
    }
}


/* 
 *  ���̃t���[���̃f�R�[�h���J�n����
 *  �X�e�C�^�X�t���O�Ƃ��� MdecFree��Frame_ny������
 *  MdecFree�� MDEC���摜�̂ŃR�[�h�����Ă��Ȃ����ɂP�ɂȂ�
 *  Frame_ny�� �����O�o�b�t�@�ɂP�t���[�����̃f�[�^���p�ӂł��Ă��Ȃ�����
 *  �P�ɂȂ�
 *  
 */
static void setup_frame()
{
  static u_long *frame_addr;
  
  if(Vlc_size == 0)
    {
      if(Frame_ny == 0)
	{
	  dec.rectid = dec.rectid? 0: 1;
	  
	  /* VLC �̊��������f�[�^�𑗐M */
	  DecDCTin(dec.vlcbuf[dec.vlcid], 2);
	  
	  /* �ŏ��̃f�R�[�h�u���b�N�̎�M�̏��������� */
	  DecDCTout(dec.imgbuf[dec.rectid], slicew*sliceh/2);
	  
	  /* ID ���X���b�v */
	  dec.vlcid = dec.vlcid? 0: 1;
	  MdecFree = 0;
	}
      
      if((frame_addr=get_frame_movie(EndFrame))==0)
	{
	  Frame_ny = 1;
	  return;
	}
      Frame_ny = 0;      
      
      DecDCTvlcSize(VLC_SIZE);
      Vlc_size = DecDCTvlc(frame_addr, dec.vlcbuf[dec.vlcid]);
    }
  else
    Vlc_size = DecDCTvlc(0,0);

  if(Vlc_size==0)
    if(StFreeRing(frame_addr))
      printf("FREE ERROR\n");
}


/*
 * �|�[�����O�ŃA�j���[�V�����̃��[�`�����N������
 * MDEC���f�[�^�̃f�R�[�h���Ŏ��̃t���[����VLC���������Ă���ꍇ ���Ȃ킿
 * MdecFree == 0 ���� Frame_ny == 0 �̏ꍇ�� �������Ȃ��Ŕ�����
 */
void poll_anim(fp)
CdlFILE *fp;
{
  if(Rewind_Switch) /* �����߂����ݒ肳��Ă����� �ŏ��ɃW�����v */
    {
      cdrom_play(&fp->pos);
      Rewind_Switch = 0;
    }
  
  /* �X�g���[�~���O���[�`�����N������K�v������ꍇ�� setup_frame()���Ă� */  
  if(MdecFree || Frame_ny || Vlc_size)
    setup_frame();
}


/*
 * loc�ɃV�[�N���A�j���[�V�������X�^�[�g������
 * ������ CdRead2()�ŃV�[�N�Ɏ��s�����ꍇ�̃G���[�����������Ă��Ȃ��̂�
 * ���ӂ��邱�Ɓi���i�łł� �����K�v������j
 */
static cdrom_play(loc)
CdlLOC *loc;
{
  while(CdControl(CdlSetloc, (u_char *)loc,0) ==0);
  while(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) ==0);
}