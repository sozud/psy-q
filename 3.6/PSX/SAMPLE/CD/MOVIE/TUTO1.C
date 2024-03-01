/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *          Movie Sample Program
 *
 *      Copyright (C) 1994,5 by Sony Corporation
 *          All rights Reserved
 *
 *  sample for streaming movie from the CD-ROM data.
 *      1) not 16's compliment resolution size
 *         but 24bit mode x's resolution must be even.
 *      2) multi resolution movie data according to it's header
 *  :
 *  CD-ROM ���烀�[�r�[���X�g���[�~���O����T���v��
 *      1) �c�܂��͉����P�U�̔{���łȂ��傫���̓���ł��Đ��ł���B
 *         �������A�Q�S�r�b�g���[�h�ōĐ�����ۂɂ́A
 *         ���ƍĐ��ʒu��X���W�͋����ł���K�v������B
 *         �P�U�r�b�g���[�h�ōĐ�����ۂɂ͂��̐����͂Ȃ��B
 *      2) �����̓���f�[�^�����ꂼ��̃p�����[�^�ɏ]���čĐ�����B     
 *
 *   Version    Date
 *  ------------------------------------------------------------------------
 *  1.10        Jul.20 1995 ume
 *                  (tuto0.c ver1.41 ����̕ύX)
 *                  �c�܂��͉����P�U�̔{���łȂ��傫���̓���ɑΉ��B
 *                  �����̓���f�[�^�����ꂼ��̃p�����[�^�ɏ]���čĐ�
 *                  ����悤�ɉ��ǁB
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>

#define TRUE    1
#define FALSE   0

/*
 * paramter of MDEC movie :
 * MDEC ����̍Đ��p�����[�^
 */
typedef struct {
    char *fileName;
    int is24bit;
    int startFrame;
    int endFrame;
    int posX;
    int posY;
    int scrWidth;
    int scrHeight;
} MovieInfo;

/*
 * information of movie data
 * NOTICE  on 24 bit mode, positon of X and width of movie must be even. :
 *
 * �Đ����� MDEC ����
 * 
 *     ���ӁF
 *         �Q�S�r�b�g���[�h�ōĐ�����Ƃ��́AposX ����� ����f�[�^�̕��͋����łȂ��Ă͂Ȃ�Ȃ��B
 *         �P�U�r�b�g�ōĐ�����Ƃ��͂��̐����͂Ȃ��B
 */
static MovieInfo movieInfo[] = {

    /* file name            24bit,  start,  end,    x,      y,      scrW,   scrH */

    /* 24 bit */
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    0,      18,     256,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    0,      34,     256,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    0,      50,     256,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    12,     18,     320,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    32,     34,     320,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    52,     50,     320,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    12,     18,     512,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    128,    34,     512,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    244,    50,     512,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    12,     18,     640,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    192,    34,     640,    240},
    {"\\DATA\\MOV.STR;1",   TRUE,   1,      100,    372,    50,     640,    240},

    /* 16 bit */
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    0,      18,     256,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    0,      34,     256,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    0,      50,     256,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    12,     18,     320,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    32,     34,     320,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    52,     50,     320,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    12,     18,     512,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    128,    34,     512,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    244,    50,     512,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    12,     18,     640,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    192,    34,     640,    240},
    {"\\DATA\\MOV.STR;1",   FALSE,  1,      100,    372,    50,     640,    240},
};
#define NMOVIE  (sizeof(movieInfo) / sizeof(MovieInfo))

/*
 *  16 bit mode / 24 bit mode :
 *  �f�R�[�h����F���̎w��(16bit/24bit)
 */
#define VRAMPIX(pixels, is24bit)    ((is24bit)? ((pixels) * 3) / 2: (pixels))   /* the actual pixel number of VRAM : VRAM ��ł̃s�N�Z���� */
#define DCT_MODE(is24bit)           ((is24bit)? 3: 2)      /* decode mode */

/*
 *  decode environment :  
 *  �f�R�[�h���ϐ�
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC buffer �idouble�j */
    int vlcid;          /* current decode buffer id :
			   ���� VLC �f�R�[�h���o�b�t�@�� ID */
    u_short *imgbuf[2]; /* decode image buffer �idouble�j*/
    int imgid;          /* corrently use buffer id :
			   ���ݎg�p���̉摜�o�b�t�@��ID */
    RECT    rect[2];    /* double buffer orign(upper left point) address
			   on the VRAM (double buffer) :
			   VRAM����W�l�i�_�u���o�b�t�@�j */
    int rectid;         /* currently translating buffer id :
			   ���ݓ]�����̃o�b�t�@ ID */
    RECT    slice;      /* the region decoded by once DecDCTout() :
			   �P��� DecDCTout �Ŏ��o���̈� */
    int isdone;         /* the flag of decoding whole frame :
			   �P�t���[�����̃f�[�^���ł����� */
    int is24bit;        /* 24bit mode flag : 24�r�b�g���[�h���ǂ��� */
} DECENV;
static DECENV   dec;    /* instance of DECENV :�f�R�[�h���̎��� */

/*
 *  Ring buffer for STREAMING
 *  minmum size is two frame :
 *  �X�g���[�~���O�p�����O�o�b�t�@
 *  CD-ROM����̃f�[�^���X�g�b�N
 *  �Œ�Q�t���[�����̗e�ʂ��m�ۂ���B
 */
#define RING_SIZE   32      /* 32 sectors : �P�ʂ̓Z�N�^ */
static u_long   Ring_Buff[RING_SIZE*SECTOR_SIZE];

/*
 *  VLC buffer(double buffer)
 *  stock the result of VLC decode :
 *  VLC�o�b�t�@�i�_�u���o�b�t�@�j
 *  VLC�f�R�[�h��̒��ԃf�[�^���X�g�b�N
 */
#define VLC_BUFF_SIZE (320/2*256)     /* not correct value :
					 �Ƃ肠�����[���ȑ傫�� */
static u_long   vlcbuf0[VLC_BUFF_SIZE];
static u_long   vlcbuf1[VLC_BUFF_SIZE];

/*                              
 *  image buffer(double buffer)
 *  stock the result of MDEC
 *  rectangle of 16(width) by XX(height) :
 *  �C���[�W�o�b�t�@�i�_�u���o�b�t�@�j
 *  DCT�f�R�[�h��̃C���[�W�f�[�^���X�g�b�N
 *  ����16�s�N�Z���̋�`���Ƀf�R�[�h���]��
 */
#define MAX_SCR_HEIGHT 640
#define SLICE_IMG_SIZE (VRAMPIX(16, TRUE) * MAX_SCR_HEIGHT)
static u_short  imgbuf0[SLICE_IMG_SIZE];
static u_short  imgbuf1[SLICE_IMG_SIZE];

/*
 * macro for playing back not 16's compliment size :
 * 16 �̔{���łȂ��傫���̓����\�����邽�߂̃}�N��
 */
#define bound(val, n)               ((((val) - 1) / (n) + 1) * (n))
#define bound16(val)                (bound((val),16))
static int isFirstSlice;    /* flag of the left end slice data :
			       �ō��[�X���C�X�������t���O */
    
/*
 *  other valiable : ���̑��̕ϐ�
 */  
static int  Rewind_Switch;  /* the end flag set after last frame :
			       �I���t���O:����̃t���[���܂ōĐ�����ƂP�ɂȂ� */

/*
 *  prototypes :  
 *  �֐��̃v���g�^�C�v�錾
 */
static int anim(MovieInfo *movie);
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1, MovieInfo *m);
static void strInit(CdlLOC *loc, void (*callback)(), MovieInfo *m);
static void strCallback();
static void strNextVlc(DECENV *dec, MovieInfo *m);
static void strSync(DECENV *dec, int mode);
static u_long *strNext(DECENV *dec, MovieInfo *m);
static void strKickCD(CdlLOC *loc);

main()
{
    int i;

    ResetCallback();
    CdInit();
    PadInit(0);
    ResetGraph(0);
    SetGraphDebug(0);
    
    while (1) {
        for (i = 0; i < NMOVIE; i++) {
            if(anim(movieInfo + i)==0)
	       return 0;     /* animation subroutine :
				�A�j���[�V�����T�u���[�`�� */
        }
    }
}


/*
 *  animation subroutine forground process :  
 *  �A�j���[�V�����T�u���[�`�� �t�H�A�O���E���h�v���Z�X
 */
static int anim(MovieInfo *movie)
{
    DISPENV disp;       /* display buffer : �\���o�b�t�@ */
    DRAWENV draw;       /* drawing buffer : �`��o�b�t�@ */
    int id;     /* display buffer id : �\���o�b�t�@�� ID */
    CdlFILE file;
    RECT    clearRect;
    
    /* set the left end slice flag : �ō��[�X���C�X�t���O���Z�b�g */
    isFirstSlice = 1;
    
    /* search file :�t�@�C�����T�[�` */
    if (CdSearchFile(&file, movie->fileName) == 0) {
        printf("file not found\n");
        PadStop();
	ResetGraph(3);
        StopCallback();
	return 0;
    }
    
    /* set the position of vram : VRAM��̍��W�l��ݒ� */
    strSetDefDecEnv(&dec, VRAMPIX(movie->posX, movie->is24bit), movie->posY,
                VRAMPIX(movie->posX, movie->is24bit), movie->posY+movie->scrHeight, movie);

    /* init streaming system & kick cd : �X�g���[�~���O���������J�n */
    strInit(&file.pos, strCallback, movie);
    
    /* VLC decode the first frame : �ŏ��̃t���[���� VLC�f�R�[�h���s�Ȃ� */
    strNextVlc(&dec, movie);
    
    Rewind_Switch = 0;

    SetDispMask(0);		/* mask screen : �\���֎~ */
    /* clear screen : ��ʃN���A */
    setRECT(&clearRect, 0, 0, VRAMPIX(movie->scrWidth, movie->is24bit), movie->scrHeight*2);
    if (movie->is24bit) {
        ClearImage(&clearRect, 0, 0, 0);        /* clear black on 24 bit mode : 24 �r�b�g�̂Ƃ��͍��ŃN���A */
    } else {
        ClearImage(&clearRect, 64, 64, 64); /* clear gray on 16 bit mode :16 �r�b�g�̂Ƃ��͊D�F�ŃN���A */
    }

    while (1) {
        /* start DCT decoding the result of VLC decoded data :
	   VLC�̊��������f�[�^��DCT�G���R�[�h�J�n�iMDEC�֑��M�j */
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE(movie->is24bit));
        
        /* prepare for recieving the result of DCT decode :
	   DCT�f�R�[�h���ʂ̎�M�̏���������            */
        /* next DecDCTout is called in DecDCToutCallback :
	   ���̌�̏����̓R�[���o�b�N���[�`���ōs�Ȃ� */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h)/2);
        
        /* decode the next frame's VLC data :
	   ���̃t���[���̃f�[�^�� VLC �f�R�[�h */
        strNextVlc(&dec, movie);
        
        /* wait for whole decode process per 1 frame :
	   �P�t���[���̃f�R�[�h���I������̂�҂� */
        strSync(&dec, 0);
        
        /* wait for V-Blank : V-BLNK ��҂� */
        VSync(0);
        
        /* swap the display buffer :�\���o�b�t�@���X���b�v        */
        /* notice that the display buffer is the opossite side of
	   decoding buffer :
	   �\���o�b�t�@�̓f�R�[�h���o�b�t�@�̋t�ł��邱�Ƃɒ��� */
        id = dec.rectid? 0: 1;
        SetDefDispEnv(&disp, dec.rect[id].x - VRAMPIX(movie->posX, movie->is24bit),
            dec.rect[id].y - movie->posY,
            VRAMPIX(movie->scrWidth, movie->is24bit), movie->scrHeight);
        /* SetDefDrawEnv(&draw, dec.rect[id].x, dec.rect[id].y, movie->scrWidth*PPW, movie->scrHeight); */
        
        if (movie->is24bit) {
            disp.isrgb24 = movie->is24bit;
            disp.disp.w = disp.disp.w * 2/3;
        }
        PutDispEnv(&disp);
        /* PutDrawEnv(&draw); */
        SetDispMask(1);     /* display enable :�\������ */
        
        if(Rewind_Switch == 1)
            break;
        
        if(PadRead(1) & PADk)   /* stop button pressed exit animation routine :
				   �X�g�b�v�{�^���������ꂽ��A�j���[�V������
				   ������ */
            break;
    }
    
    /* post processing of animation routine : �A�j���[�V�����㏈�� */
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause,0,0);
    if(Rewind_Switch==0) {
       PadStop();
       ResetGraph(3);
       StopCallback();
       return 0;
       }
    else
       return 1;
}


/*
 * init DECENV    buffer0(x0,y0),buffer1(x1,y1) :
 * �f�R�[�h����������
 *  x0,y0 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�O�j
 *  x1,y1 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�P�j
 *
 */
static void strSetDefDecEnv(DECENV  *dec, int x0, int y0, int x1, int y1, MovieInfo *movie)
{
  static int isFirst = 1;

  if(isFirst == 1)
    {
      dec->vlcbuf[0] = vlcbuf0;
      dec->vlcbuf[1] = vlcbuf1;
      dec->vlcid     = 0;

      dec->imgbuf[0] = imgbuf0;
      dec->imgbuf[1] = imgbuf1;
      dec->imgid     = 0;
      dec->rectid    = 0;
      dec->isdone = 0;
      isFirst = 0;
    }
  
    /* width and height of rect[] are set dynamicaly according to STR data :
       rect[]�̕��^������STR�f�[�^�̒l�ɂ���ăZ�b�g����� */
  dec->rect[0].x = x0;
  dec->rect[0].y = y0;
  dec->rect[1].x = x1;
  dec->rect[1].y = y1;
  dec->slice.w = VRAMPIX(16, movie->is24bit);  
  dec->is24bit = movie->is24bit;   
  
  if(dec->rectid == 0)
    {
      dec->slice.x = x0;
      dec->slice.y = y0;
    }
  else
    {
      dec->slice.x = x1;
      dec->slice.y = y1;
    }
}


/*
 * init the streaming environment and start the cdrom :
 * �X�g���[�~���O�������������ĊJ�n
 */
static void strInit(CdlLOC  *loc, void (*callback)(), MovieInfo *movie)
{
    /* cold reset mdec : MDEC �����Z�b�g */
    DecDCTReset(0);

    /* set the callback after 1 block MDEC decoding :
       MDEC���P�f�R�[�h�u���b�N�������������̃R�[���o�b�N���`���� */
    DecDCToutCallback(callback);
    
    /* set the ring buffer : �����O�o�b�t�@�̐ݒ� */
    StSetRing(Ring_Buff, RING_SIZE);
    
    /* init the streaming library : �X�g���[�~���O���Z�b�g�A�b�v */
    /* end frame is set endless : �I���t���[��=���ɐݒ�   */
    StSetStream(movie->is24bit, movie->startFrame, 0xffffffff, 0, 0);
    
    /* start the cdrom : �X�g���[�~���O���[�h�J�n */
    strKickCD(loc);
}


/*
 *  back ground process
 *  callback of DecDCTout() :
 *  �o�b�N�O���E���h�v���Z�X 
 *  (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
static void strCallback()
{
    int mod;
    int id;
    RECT snap_rect;
      
    if (dec.is24bit) {
        extern StCdIntrFlag;
        if(StCdIntrFlag) {
	  StCdInterrupt();  /* on the RGB24 bit mode , call
			       StCdInterrupt manually at this timing :
			       RGB24�̎��͂����ŋN������ */
	  StCdIntrFlag = 0;
        }
    }
/*    
    LoadImage(&dec.slice, (u_long *)dec.imgbuf[dec.imgid]);
*/
    id = dec.imgid;
    snap_rect = dec.slice;
    
    /* switch the id of decoding buffer : �摜�f�R�[�h�̈�̐ؑւ� */
    dec.imgid = dec.imgid? 0:1;

    /* update slice(rectangle) position :
      �X���C�X�i�Z���`�j�̈���ЂƂE�ɍX�V */
    if (isFirstSlice && (mod = dec.rect[dec.rectid].w % dec.slice.w)) {
        dec.slice.x += mod;
        isFirstSlice = 0;
    } else {
        dec.slice.x += dec.slice.w;
    }
    
    /* remaining slice ? : �c��̃X���C�X�����邩�H */
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        /* prepare for next slice : ���̃X���C�X���f�R�[�h�J�n */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h)/2);
    }
    /* last slice ; end of 1 frame : �ŏI�X���C�X���P�t���[���I�� */
    else {
        /* set the decoding done flag : �I�������Ƃ�ʒm */
        dec.isdone = 1;
        isFirstSlice = 1;
        
        /* update the position on VRAM : �]������W�l���X�V */
        dec.rectid = dec.rectid? 0: 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
  /* transfer the decoded data to VRAM :
     �f�R�[�h���ʂ��t���[���o�b�t�@�ɓ]�� */
    LoadImage(&snap_rect, (u_long *)dec.imgbuf[id]);
}


/*
 *  execute VLC decoding
 *  the decoding data is the next frame's :
 *  VLC�f�R�[�h�̎��s
 *  ����1�t���[���̃f�[�^��VLC�f�R�[�h���s�Ȃ�
 */
static void strNextVlc(DECENV  *dec, MovieInfo *movie)
{
    int cnt = WAIT_TIME;
    u_long  *next;

    /* get the 1 frame streaming data : �f�[�^���P�t���[�������o�� */
    while ((next = strNext(dec, movie)) == 0) {
        if (--cnt == 0)
            return;
    }
    
    /* switch the decoding area : VLC�f�R�[�h�̈�̐ؑւ� */
    dec->vlcid = dec->vlcid? 0: 1;

    /* VLC decode */
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);

    /* free the ring buffer : �����O�o�b�t�@�̃t���[���̗̈��������� */
    StFreeRing(next);

    return;
}

/*
 *  get the 1 frame streaming data
 *  return vale     normal end -> top address of 1 frame streaming data
 *                  error      -> NULL : 
 *  �����O�o�b�t�@����̃f�[�^�̎��o��
 *  �i�Ԃ�l�j  ����I�����������O�o�b�t�@�̐擪�A�h���X
 *          �G���[��������NULL
 */
static u_long *strNext(DECENV  *dec, MovieInfo *movie)
{
    static int  strWidth  = 0;  /* width of the previous frame :
				   �P�O�̃t���[���̑傫���i���j */
    static int  strHeight = 0;  /* height of the previous frame :
				   �P�O�̃t���[���̑傫���i�����j */
    u_long      *addr;
    StHEADER    *sector;
    int     cnt = WAIT_TIME;

    /* get the 1 frame streaming data withe TIME-OUT :
       �f�[�^�����o���i�^�C���A�E�g�t���j */  
    while(StGetNext((u_long **)&addr,(u_long **)&sector)) {
        if (--cnt == 0)
            return(0);
    }

    /* if the frame number greater than the end frame, set the end switch :
       ���݂̃t���[���ԍ����w��l�Ȃ�I��  */
    if(sector->frameCount >= movie->endFrame) {
        Rewind_Switch = 1;
    }
    
    /* if the resolution is differ to previous frame, clear frame buffer :    
       ��ʂ̉𑜓x���O�̃t���[���ƈႤ�Ȃ�� ClearImage �����s */
    if(strWidth != sector->width || strHeight != sector->height) {
        
        RECT    rect;
        setRECT(&rect, 0, 0, VRAMPIX(movie->scrWidth, movie->is24bit), movie->scrHeight*2);
        if (movie->is24bit) {
            ClearImage(&rect, 0, 0, 0);     /* clear black on 24 bit mode :
					       24 �r�b�g�̂Ƃ��͍��ŃN���A */
        } else {
            ClearImage(&rect, 64, 64, 64);  /* clear gray on 16 bit mode :
					       16 �r�b�g�̂Ƃ��͊D�F�ŃN���A */
        }

        strWidth  = sector->width;
        strHeight = sector->height;
    }
    
    /* set DECENV according to the data on the STR format :    
       STR�t�H�[�}�b�g�̃w�b�_�ɍ��킹�ăf�R�[�h����ύX���� */
    dec->rect[0].w = dec->rect[1].w = VRAMPIX(strWidth, movie->is24bit);
    dec->rect[0].h = dec->rect[1].h = strHeight;
    dec->slice.h   = strHeight;
    
    return(addr);
}


/*
 *  wait for finish decodeing 1 frame with TIME-OUT :  
 *  �P�t���[���̃f�R�[�h�I����҂�
 *  ���Ԃ��Ď����ă^�C���A�E�g���`�F�b�N
 */
static void strSync(DECENV  *dec, int mode /* VOID */)
{
    volatile u_long cnt = WAIT_TIME;

    /* wait for the decod is done flag set by background process :    
       �o�b�N�O���E���h�v���Z�X��isdone�𗧂Ă�܂ő҂� */
    while (dec->isdone == 0) {
        if (--cnt == 0) {
            /* if timeout force to switch buffer : �����I�ɐؑւ��� */	  
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
 *  start streaming :
 *  CDROM���w��ʒu����X�g���[�~���O�J�n����
 */
static void strKickCD(CdlLOC *loc)
{
  u_char param;

  param = CdlModeSpeed;
  
 loop:
  /* seek to the destination : �ړI�n�܂� Seek ���� */
  while (CdControl(CdlSetloc, (u_char *)loc, 0) == 0);
  while (CdControl(CdlSetmode, &param, 0) == 0);
  VSync(3);  /* wait for 3 VSync when changing the speed :
		�{���ɐ؂�ւ��Ă��� �RV�҂K�v������ */
    /* out the read command with streaming mode :
       �X�g���[�~���O���[�h��ǉ����ăR�}���h���s */
  if(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) == 0)
    goto loop;
}