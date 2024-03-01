/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*
 *          Movie Sample Program
 *
 *      Copyright (C) 1994,5 by Sony Corporation
 *          All rights Reserved
 *
 *  CD-ROM ���烀�[�r�[���X�g���[�~���O����T���v��
 *
 *   Version    Date
 *  ------------------------------------------
 *  1.00        Jul,14,1994     yutaka
 *  1.10        Sep,01,1994     suzu
 *  1.20        Oct,24,1994     yutaka(anim subroutine��)
 *  1.30        Jun,02,1995     yutaka(�㏈��)
 *  1.40        Jul,10,1995     masa(imgbuf�_�u���o�b�t�@��)
 *  1.50        Jul,20,1995     ume(��ʃN���A����)
 *  1.50a	Aug,03,1996     yoshi (for overlay)
 *  1.50b	Mar,04,1996     yoshi (added English comments)
 *====================================================================
 *    This was rewritten as a child process. Compile conditionally with OVERLAY.
 *    Called while parent is emitting sound.
 *    Playback is performed at 24bits. :
 * �q�v���Z�X�p�ɏ����������BOVERLAY�ŏ����R���p�C������B
 * �e�ŃT�E���h��炵���܂܂̏�ԂŌĂ΂��B
 * �Đ��� 24bit  �ōs���Ă���B
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>


#define FILE_NAME   "\\DATA\\MOV.STR;1"
#define START_FRAME 1
#define END_FRAME   577
#define POS_X       36
#define POS_Y       24
#define SCR_WIDTH   320
#define SCR_HEIGHT  240

/*
 *  specification of the number of colors to decode (16bit/24bit) : 
 *  �f�R�[�h����F���̎w��(16bit/24bit)
 */
#define IS_RGB24    1   /* 0:RGB16, 1:RGB24 */
#if IS_RGB24==1
#define PPW 3/2     /* how many pixels in 1 short word? : 
                       �P�V���[�g���[�h�ɉ��s�N�Z�����邩 */
#define DCT_MODE    3   /* decode in 24bit mode : 24bit ���[�h�Ńf�R�[�h */
#else
#define PPW 1       /* how many pixels in 1 short word? : 
                       �P�V���[�g���[�h�ɉ��s�N�Z�����邩 */
#define DCT_MODE    2   /* decode in 16 bit mode : 16bit ���[�h�Ńf�R�[�h */
#endif

/*
 *  decode environment variable : �f�R�[�h���ϐ�
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC buffer (double buffer) : VLC �o�b�t�@�i�_�u���o�b�t�@�j */
    int vlcid;      /* Cureent buffer ID for VLC decode operation : 
                       ���� VLC �f�R�[�h���o�b�t�@�� ID */
    u_short *imgbuf[2]; /* decode image buffer (double buffer) : 
                           �f�R�[�h�摜�o�b�t�@�i�_�u���o�b�t�@�j*/
    int imgid;      /* ID of image buffer being used currently : 
                       ���ݎg�p���̉摜�o�b�t�@��ID */
    RECT    rect[2];    /* coordinates on VRAM (double buffer) : 
                           VRAM����W�l�i�_�u���o�b�t�@�j */
    int rectid;     /* area retrieved for one DecDCTout : ���ݓ]�����̃o�b�t�@ ID */
    RECT    slice;      /* area retrieved for one DecDCTout : 
                           �P��� DecDCTout �Ŏ��o���̈� */
    int isdone;     /* is one frame's worth of data prepared? : 
                       �P�t���[�����̃f�[�^���ł����� */
} DECENV;
static DECENV   dec;        /* actual decode environment : 
                               �f�R�[�h���̎��� */

/*
 * ring buffer for streaming
 * stocks data from the CD-ROM
 * reserve capacity for at least 2 frames' worth
 *  �X�g���[�~���O�p�����O�o�b�t�@
 *  CD-ROM����̃f�[�^���X�g�b�N
 *  �Œ�Q�t���[�����̗e�ʂ��m�ۂ���B
 */
#define RING_SIZE   32      /* �P�ʂ̓Z�N�^ */
static u_long   Ring_Buff[RING_SIZE*SECTOR_SIZE];

/*
 *  VLC buffer (double buffer)
 *  stock intermediate data after VLC decoding
 *  VLC�o�b�t�@�i�_�u���o�b�t�@�j
 *  VLC�f�R�[�h��̒��ԃf�[�^���X�g�b�N
 */
#define VLC_BUFF_SIZE 320/2*256     /* �Ƃ肠�����[���ȑ傫�� */
static u_long   vlcbuf0[VLC_BUFF_SIZE];
static u_long   vlcbuf1[VLC_BUFF_SIZE];

/*
 * image buffer (double buffer)
 * stock image data from after DCT decoding
 * decode and transfer for rectangles 16 pixels wide
 *  �C���[�W�o�b�t�@�i�_�u���o�b�t�@�j
 *  DCT�f�R�[�h��̃C���[�W�f�[�^���X�g�b�N
 *  ����16�s�N�Z���̋�`���Ƀf�R�[�h���]��
 */
#define SLICE_IMG_SIZE 16*PPW*SCR_HEIGHT
static u_short  imgbuf0[SLICE_IMG_SIZE];
static u_short  imgbuf1[SLICE_IMG_SIZE];
    
/*
 *  other variables : ���̑��̕ϐ�
 */
static int  StrWidth  = 0;  /* size of movie image (horizontal and vertical) : 
                               ���[�r�[�摜�̑傫���i���Əc�j */
static int  StrHeight = 0;  
static int  Rewind_Switch;  /* end flag: set to 1 when playback has reached a specified frame : 
                               �I���t���O:����̃t���[���܂ōĐ�����ƂP�ɂȂ� */

/*
 *  function prototype declaration : �֐��̃v���g�^�C�v�錾
 */
static void anim();
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1);
static void strInit(CdlLOC *loc, void (*callback)());
static void strCallback();
static void strNextVlc(DECENV *dec);
static void strSync(DECENV *dec, int mode);
static u_long *strNext(DECENV *dec);
static void strKickCD(CdlLOC *loc);

#ifdef OVERLAY
child_anim()
#else
main()
#endif
{
    RECT rct;

    /* initialization other than animation are all the same : 
       �A�j���[�V���� ����ȊO ���ʂ̏����� */
#ifdef OVERLAY
/* some tricks are used to allow smooth screen translation : 
   ��ʐ؂�ւ����X���[�Y�ɍs���悤�ɏ����H�v���Ă��� */
    VSync(0);
    SetDispMask(0);
    ResetGraph(1);
    setRECT(&rct,0,0,1024,512);
    ClearImage(&rct,0,0,0);
    DrawSync(0);
#else
    ResetCallback();
    CdInit();
    PadInit(0);     /* reset PAD : PAD �����Z�b�g */
    ResetGraph(0);      /* reset GPU : GPU �����Z�b�g */
    SetGraphDebug(0);   /* set debug level : �f�o�b�O���x���ݒ� */
#endif
    
    anim();     /* animation subroutine : �A�j���[�V�����T�u���[�`�� */

	return(0);
}


/*
 *  animation subroutine foreground process : 
 *  �A�j���[�V�����T�u���[�`�� �t�H�A�O���E���h�v���Z�X
 */
static void anim()
{
    DISPENV disp;       /* display buffer : �\���o�b�t�@ */
    DRAWENV draw;       /* drawing buffer : �`��o�b�t�@ */
    int id;     /* ID of display buffer : �\���o�b�t�@�� ID */
    CdlFILE file;
    
    /* search file : �t�@�C�����T�[�` */
    if (CdSearchFile(&file, FILE_NAME) == 0) {
        printf("file not found\n");
        StopCallback();
        PadStop();
        exit();
    }
    
    /* set coordinates on VRAM : VRAM��̍��W�l��ݒ� */
    strSetDefDecEnv(&dec, POS_X, POS_Y, POS_X, POS_Y+SCR_HEIGHT);

    /* initalize and begin streaming : �X�g���[�~���O���������J�n */
    strInit(&file.pos, strCallback);
    
    /* peform VLC decoding of the initial frame : 
       �ŏ��̃t���[���� VLC�f�R�[�h���s�Ȃ� */
    strNextVlc(&dec);
    
    Rewind_Switch = 0;
    
    while (1) {
        /* DCT encoding is begun on the data for which VLC has been completed 
           (send to MDEC) : 
           VLC�̊��������f�[�^��DCT�G���R�[�h�J�n�iMDEC�֑��M�j */
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE);
        
        /* prepare to receive DCT decode results*/
        /* processing after this is performed with call back routines : 
           DCT�f�R�[�h���ʂ̎�M�̏���������            
             ���̌�̏����̓R�[���o�b�N���[�`���ōs�Ȃ� */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
        
        /* VLC decoding of the data for the next frame : 
           ���̃t���[���̃f�[�^�� VLC �f�R�[�h */
        strNextVlc(&dec);
        
        /* wait for decoding of one frame to finish :
           �P�t���[���̃f�R�[�h���I������̂�҂� */
        strSync(&dec, 0);
        
        /* wait for V-BLNK : V-BLNK ��҂� */
        VSync(0);
        
        /* swap display buffer  
           note that the display buffer is the reverse of the buffer being decoded :
           �\���o�b�t�@���X���b�v                                
             �\���o�b�t�@�̓f�R�[�h���o�b�t�@�̋t�ł��邱�Ƃɒ��� */
        id = dec.rectid? 0: 1;
        SetDefDispEnv(&disp, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);
/*      SetDefDrawEnv(&draw, 0, (id)*240, SCR_WIDTH*PPW, SCR_HEIGHT);*/
        
#if IS_RGB24==1
        disp.isrgb24 = IS_RGB24;
        disp.disp.w = disp.disp.w*2/3;
#endif
        PutDispEnv(&disp);
/*      PutDrawEnv(&draw);*/
        SetDispMask(1);     /* display permission : �\������ */
        
        if(Rewind_Switch == 1)
            break;
        
        if(PadRead(1) & PADk)   /* exit animation if the stop button is pressed : 
                                   �X�g�b�v�{�^���������ꂽ��A�j���[�V����
                       �𔲂��� */
            break;
    }
    
    
    /* post-processing of animation :
       �A�j���[�V�����㏈�� */
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause,0,0);
}


/*
 *    initialize decode environment
 *  the transfer coordinates of the image decoded from x0,y0 (buffer 0)
 *  the transfer coordinates of the image decoded from x1,y1 (buffer 1)
 * �f�R�[�h����������
 *  x0,y0 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�O�j
 *  x1,y1 �f�R�[�h�����摜�̓]������W�i�o�b�t�@�P�j
 *
 */
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1)
{

    dec->vlcbuf[0] = vlcbuf0;
    dec->vlcbuf[1] = vlcbuf1;
    dec->vlcid     = 0;

    dec->imgbuf[0] = imgbuf0;
    dec->imgbuf[1] = imgbuf1;
    dec->imgid     = 0;

    /* the width/height of rect[] is set from the STR data values :
       rect[]�̕��^������STR�f�[�^�̒l�ɂ���ăZ�b�g����� */
    dec->rect[0].x = x0;
    dec->rect[0].y = y0;
    dec->rect[1].x = x1;
    dec->rect[1].y = y1;
    dec->rectid    = 0;

    dec->slice.x = x0;
    dec->slice.y = y0;
    dec->slice.w = 16*PPW;

    dec->isdone    = 0;
}


/*
 * initialize streming environment and begin : 
   �X�g���[�~���O�������������ĊJ�n
 */
static void strInit(CdlLOC *loc, void (*callback)())
{
    /* reset MDEC : MDEC �����Z�b�g */
    DecDCTReset(0);

    /* define call back for when MDEC has processed one decode block :
       MDEC���P�f�R�[�h�u���b�N�������������̃R�[���o�b�N���`���� */
    DecDCToutCallback(callback);
    
    /* set ring buffer : 
       �����O�o�b�t�@�̐ݒ� */
    StSetRing(Ring_Buff, RING_SIZE);
    
    /* set up streaming  
       set final frame = infinity :
       �X�g���[�~���O���Z�b�g�A�b�v   
        �I���t���[��=���ɐݒ�   */
    StSetStream(IS_RGB24, START_FRAME, 0xffffffff, 0, 0);
    
    
    /* begin streaming read : 
       �X�g���[�~���O���[�h�J�n */
    strKickCD(loc);
}


/*
 *    background process
 *  (call back function that is called when DecDCTout() is finished) :
 *  �o�b�N�O���E���h�v���Z�X 
 *  (DecDCTout() ���I�������ɌĂ΂��R�[���o�b�N�֐�)
 */
static void strCallback()
{
#if IS_RGB24==1
    extern StCdIntrFlag;
    if(StCdIntrFlag) {
        StCdInterrupt();    /* run here if RGB24 : RGB24�̎��͂����ŋN������ */
        StCdIntrFlag = 0;
    }
#endif
    
    /* transfer decode results to the frame buffer :
       �f�R�[�h���ʂ��t���[���o�b�t�@�ɓ]�� */
    LoadImage(&dec.slice, (u_long *)dec.imgbuf[dec.imgid]);
    
    /* switch image decode area :
       �摜�f�R�[�h�̈�̐ؑւ� */
    dec.imgid = dec.imgid? 0:1;

    /* update slice (rectangular strip) area to next one on the right :
       �X���C�X�i�Z���`�j�̈���ЂƂE�ɍX�V */
    dec.slice.x += dec.slice.w;
    
    /* any remaining slices? :
       �c��̃X���C�X�����邩�H */
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
	    /* begin decoding next slice : 
           ���̃X���C�X���f�R�[�h�J�n */
        DecDCTout(dec.imgbuf[dec.imgid], dec.slice.w*dec.slice.h/2);
    }
    /* final slice = 1 frame completed : �ŏI�X���C�X���P�t���[���I�� */
    else {
        /* indicate completion : �I�������Ƃ�ʒm */
        dec.isdone = 1;
        
        /* update transfer coordinates : �]������W�l���X�V */
        dec.rectid = dec.rectid? 0: 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
}


/*
 *  perform VLC decoding
 *  perform VLC decoding for next frame :
 *  VLC�f�R�[�h�̎��s
 *  ����1�t���[���̃f�[�^��VLC�f�R�[�h���s�Ȃ�
 */
static void strNextVlc(DECENV *dec)
{
    int cnt = WAIT_TIME;
    u_long  *next;
    static u_long *strNext();


    /* retrieve one frame's worth of data : �f�[�^���P�t���[�������o�� */
    while ((next = strNext(dec)) == 0) {
        if (--cnt == 0)
            return;
    }
    
    /* switch VLC decode area : VLC�f�R�[�h�̈�̐ؑւ� */
    dec->vlcid = dec->vlcid? 0: 1;

    /* VLC decode : VLC�f�R�[�h */
    DecDCTvlc(next, dec->vlcbuf[dec->vlcid]);

    /* free up frame area of ring buffer : 
       �����O�o�b�t�@�̃t���[���̗̈��������� */
    StFreeRing(next);

    return;
}

/*
 *    retrieve data from the ring buffer
 *    (return value)      normal completion = starting address of ring buffer
 *    error occurred = NULL
 *  �����O�o�b�t�@����̃f�[�^�̎��o��
 *  �i�Ԃ�l�j  ����I�����������O�o�b�t�@�̐擪�A�h���X
 *          �G���[��������NULL
 */
static u_long *strNext(DECENV *dec)
{
    u_long      *addr;
    StHEADER    *sector;
    int     cnt = WAIT_TIME;


    /* retrieve data (with timeout) :  
       �f�[�^�����o���i�^�C���A�E�g�t���j */  
    while(StGetNext((u_long **)&addr,(u_long **)&sector)) {
        if (--cnt == 0)
            return(0);
    }

    /* if current frame number is the specified value, then end :   
       ���݂̃t���[���ԍ����w��l�Ȃ�I��  */   
    if(sector->frameCount >= END_FRAME) {
        Rewind_Switch = 1;
    }
    
    /* if the resolution of the frame is different from that of the previous frame,
          perform ClearImage : 
       ��ʂ̉𑜓x�� �O�̃t���[���ƈႤ�Ȃ�� ClearImage �����s */
    if(StrWidth != sector->width || StrHeight != sector->height) {
        
        RECT    rect;
        setRECT(&rect, 0, 0, SCR_WIDTH * PPW, SCR_HEIGHT*2);
        ClearImage(&rect, 0, 0, 0);
        
        StrWidth  = sector->width;
        StrHeight = sector->height;
    }
    
    
    /* change decode environment to match the STR format header :
       STR�t�H�[�}�b�g�̃w�b�_�ɍ��킹�ăf�R�[�h����ύX���� */
    dec->rect[0].w = dec->rect[1].w = StrWidth*PPW;
    dec->rect[0].h = dec->rect[1].h = StrHeight;
    dec->slice.h   = StrHeight;
    
    return(addr);
}


/*
 *  wait for the decoding of one frame to finish
 *  monitor time and check for a timeout
 *  �P�t���[���̃f�R�[�h�I����҂�
 *  ���Ԃ��Ď����ă^�C���A�E�g���`�F�b�N
 */
static void strSync(DECENV *dec, int mode /* VOID */)
{
    volatile u_long cnt = WAIT_TIME;

    /* wait until the background process sets 'is done' :      
       �o�b�N�O���E���h�v���Z�X��isdone�𗧂Ă�܂ő҂� */      
    while (dec->isdone == 0) {
        if (--cnt == 0) {
            /* timeout:  forcibly switch : timeout: �����I�ɐؑւ��� */
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
 *  begin streaming the CDROM from the specified position :
 *  CDROM���w��ʒu����X�g���[�~���O�J�n����
 */
static void strKickCD(CdlLOC *loc)
{
 loop:
  /* Seek the target position : �ړI�n�܂� Seek ���� */
  while (CdControl(CdlSetloc, (u_char *)loc, 0) == 0);
    
    /* add streaming mode and issue the command : 
       �X�g���[�~���O���[�h��ǉ����ăR�}���h���s */
  if(CdRead2(CdlModeStream|CdlModeSpeed|CdlModeRT) == 0)
    goto loop;
}
