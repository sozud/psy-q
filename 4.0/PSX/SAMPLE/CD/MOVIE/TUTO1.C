/*
 * $PSLibId: Run-time Library Release 4.0$
 */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="cd/movie/movie1.cpe";
#endif
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
 *  CD-ROM からムービーをストリーミングするサンプル
 *      1) 縦または横が１６の倍数でない大きさの動画でも再生できる。
 *         ただし、２４ビットモードで再生する際には、
 *         幅と再生位置のX座標は偶数である必要がある。
 *         １６ビットモードで再生する際にはこの制限はない。
 *      2) 複数の動画データをそれぞれのパラメータに従って再生する。     
 *
 *   Version    Date
 *  ------------------------------------------------------------------------
 *  1.10        Jul.20 1995 ume
 *                  (tuto0.c ver1.41 からの変更)
 *                  縦または横が１６の倍数でない大きさの動画に対応。
 *                  複数の動画データをそれぞれのパラメータに従って再生
 *                  するように改良。
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>
#include <r3000.h>
#include <asm.h>
#include <kernel.h>
#include <libpress.h>

#define TRUE    1
#define FALSE   0

/*
 * paramter of MDEC movie :
 * MDEC 動画の再生パラメータ
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
 * 再生する MDEC 動画
 * 
 *     注意：
 *         ２４ビットモードで再生するときは、posX および 動画データの幅は偶数でなくてはならない。
 *         １６ビットで再生するときはこの制限はない。
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
 *  デコードする色数の指定(16bit/24bit)
 */
#define VRAMPIX(pixels, is24bit)    ((is24bit)? ((pixels) * 3) / 2: (pixels))   /* the actual pixel number of VRAM : VRAM 上でのピクセル数 */
#define DCT_MODE(is24bit)           ((is24bit)? 3: 2)      /* decode mode */

/* time out parameter for strNextVlc() and strNext() : strNext(),strNextVlc()
   がタイムアウトさせる繰り返しの数 */
#define MOVIE_WAIT 2000

/*
 *  decode environment :  
 *  デコード環境変数
 */
typedef struct {
    u_long  *vlcbuf[2]; /* VLC buffer （double） */
    int vlcid;          /* current decode buffer id :
			   現在 VLC デコード中バッファの ID */
    u_short *imgbuf[2]; /* decode image buffer （double）*/
    int imgid;          /* corrently use buffer id :
			   現在使用中の画像バッファのID */
    RECT    rect[2];    /* double buffer orign(upper left point) address
			   on the VRAM (double buffer) :
			   VRAM上座標値（ダブルバッファ） */
    int rectid;         /* currently translating buffer id :
			   現在転送中のバッファ ID */
    RECT    slice;      /* the region decoded by once DecDCTout() :
			   １回の DecDCTout で取り出す領域 */
    int isdone;         /* the flag of decoding whole frame :
			   １フレーム分のデータができたか */
    int is24bit;        /* 24bit mode flag : 24ビットモードかどうか */
} DECENV;
static DECENV   dec;    /* instance of DECENV :デコード環境の実体 */

/*
 *  Ring buffer for STREAMING
 *  minmum size is two frame :
 *  ストリーミング用リングバッファ
 *  CD-ROMからのデータをストック
 *  最低２フレーム分の容量を確保する。
 */
#define RING_SIZE   32      /* 32 sectors : 単位はセクタ */
static u_long   Ring_Buff[RING_SIZE*SECTOR_SIZE];

/*  VLC table
 *  memory area for vlc parameters
 *  DecDCTvlcBuild() expands compressed data to that area
 *  compressed size is about 3kbyte and decompressed size is about 64kbyte :
 *  VLCの係数テーブルを格納する領域
 *  DecDCTvlcBuild()で展開されて使用する。
 *  展開前は 約3Kbyte 展開後は 64Kbyte
 */
DECDCTTAB  vlc_table;

/*
 *  VLC buffer(double buffer)
 *  stock the result of VLC decode :
 *  VLCバッファ（ダブルバッファ）
 *  VLCデコード後の中間データをストック
 */
#define VLC_BUFF_SIZE (320/2*256)     /* not correct value :
					 とりあえず充分な大きさ */
static u_long   vlcbuf0[VLC_BUFF_SIZE];
static u_long   vlcbuf1[VLC_BUFF_SIZE];

/*                              
 *  image buffer(double buffer)
 *  stock the result of MDEC
 *  rectangle of 16(width) by XX(height) :
 *  イメージバッファ（ダブルバッファ）
 *  DCTデコード後のイメージデータをストック
 *  横幅16ピクセルの矩形毎にデコード＆転送
 */
#define MAX_SCR_HEIGHT 640
#define SLICE_IMG_SIZE (VRAMPIX(16, TRUE) * MAX_SCR_HEIGHT)
static u_short  imgbuf0[SLICE_IMG_SIZE];
static u_short  imgbuf1[SLICE_IMG_SIZE];

/*
 * macro for playing back not 16's compliment size :
 * 16 の倍数でない大きさの動画を表示するためのマクロ
 */
#define bound(val, n)               ((((val) - 1) / (n) + 1) * (n))
#define bound16(val)                (bound((val),16))
static int isFirstSlice;    /* flag of the left end slice data :
			       最左端スライスを示すフラグ */

/*
 *  other valiable : その他の変数
 */  
static int  Rewind_Switch;  /* the end flag set after last frame :
			       終了フラグ:所定のフレームまで再生すると１になる */

/*
 *  prototypes :  
 *  関数のプロトタイプ宣言
 */
static int anim(MovieInfo *movie);
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1, MovieInfo *m);
static void strInit(CdlLOC *loc, void (*callback)(), MovieInfo *m);
static void strCallback();
static int strNextVlc(DECENV *dec, MovieInfo *m);
static void strSync(DECENV *dec, int mode);
static u_long *strNext(DECENV *dec, MovieInfo *m);
static void strKickCD(CdlLOC *loc);

main()
{
    int i;

    ResetCallback();
    CdInit();
#if defined(APD_SAVE)
	APDSaveInit(0,progname);
#elif defined(APD_LOAD)
	APDLoadInit(0,progname);
#endif
#ifndef APD_LOAD
    PadInit(0);
#endif
    ResetGraph(0);
    SetGraphDebug(0);
    
    while (1) {
        for (i = 0; i < NMOVIE; i++) {
            if(anim(movieInfo + i)==0)
	       return 0;     /* animation subroutine :
				アニメーションサブルーチン */
        }
    }
}


/*
 *  animation subroutine forground process :  
 *  アニメーションサブルーチン フォアグラウンドプロセス
 */
static int anim(MovieInfo *movie)
{
    DISPENV disp;       /* display buffer : 表示バッファ */
    DRAWENV draw;       /* drawing buffer : 描画バッファ */
    int id;     /* display buffer id : 表示バッファの ID */
    CdlFILE file;
    RECT    clearRect;
    CdlLOC  save_loc;		/* retry location : 現在のロケーション */
    int frame_no;		/* retry frame no : 現在のframe_no */
    
    /* set the left end slice flag : 最左端スライスフラグをセット */
    isFirstSlice = 1;
    
    /* search file :ファイルをサーチ */
    if (CdSearchFile(&file, movie->fileName) == 0) {
        printf("file not found\n");
#ifndef APD_LOAD
        PadStop();
#endif
	ResetGraph(3);
        StopCallback();
	return 0;
    }
    
    /* set the position of vram : VRAM上の座標値を設定 */
    strSetDefDecEnv(&dec, VRAMPIX(movie->posX, movie->is24bit), movie->posY,
                VRAMPIX(movie->posX, movie->is24bit), movie->posY+movie->scrHeight, movie);

    /* init streaming system & kick cd : ストリーミング初期化＆開始 */
    strInit(&file.pos, strCallback, movie);
    
    /* expand compressed vlc parameter data : VLCテーブルの展開 */
    DecDCTvlcBuild(vlc_table);
    
    /* VLC decode the first frame : 最初のフレームの VLCデコードを行なう */
    while(strNextVlc(&dec, movie) == -1)
      {
	save_loc = file.pos; /* start position */
	strKickCD(&save_loc);
      }
    
    Rewind_Switch = 0;

    SetDispMask(0);		/* mask screen : 表示禁止 */
    /* clear screen : 画面クリア */
    setRECT(&clearRect, 0, 0, VRAMPIX(movie->scrWidth, movie->is24bit), movie->scrHeight*2);
    if (movie->is24bit) {
        ClearImage(&clearRect, 0, 0, 0);        /* clear black on 24 bit mode : 24 ビットのときは黒でクリア */
    } else {
        ClearImage(&clearRect, 64, 64, 64); /* clear gray on 16 bit mode :16 ビットのときは灰色でクリア */
    }

    while (1) {
        /* start DCT decoding the result of VLC decoded data :
	   VLCの完了したデータをDCTエンコード開始（MDECへ送信） */
        DecDCTin(dec.vlcbuf[dec.vlcid], DCT_MODE(movie->is24bit));
        
        /* prepare for recieving the result of DCT decode :
	   DCTデコード結果の受信の準備をする            */
        /* next DecDCTout is called in DecDCToutCallback :
	   この後の処理はコールバックルーチンで行なう */
        DecDCTout((u_long *)dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h)/2);
        
        /* decode the next frame's VLC data :
	   次のフレームのデータの VLC デコード */
	while(strNextVlc(&dec, movie)== -1)
	  {
	    frame_no = StGetBackloc(&save_loc);	/* get current location */
	    printf("time out in strNext() %d\n",frame_no);
	    if(frame_no>movie->endFrame || frame_no<=0) /* invalid frame no */
	      save_loc = file.pos; /* start position */
	    strKickCD(&save_loc);
	  }
        
        /* wait for whole decode process per 1 frame :
	   １フレームのデコードが終了するのを待つ */
        strSync(&dec, 0);
        
        /* wait for V-Blank : V-BLNK を待つ */
#ifdef APD_LOAD
	APDSetCnt(VSync(0));
#else
        VSync(0);
#endif
        
        /* swap the display buffer :表示バッファをスワップ        */
        /* notice that the display buffer is the opossite side of
	   decoding buffer :
	   表示バッファはデコード中バッファの逆であることに注意 */
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
        SetDispMask(1);     /* display enable :表示許可 */
        
        if(Rewind_Switch == 1)
            break;
        
#if defined(APD_SAVE)
       if(APDSave(PadRead(1)) & PADk)
#elif defined(APD_LOAD)
       if(APDLoad(1) & PADk)
#else
        if(PadRead(1) & PADk)   /* stop button pressed exit animation routine :
				   ストップボタンが押されたらアニメーションを
				   抜ける */
#endif
            break;
    }
    
    /* post processing of animation routine : アニメーション後処理 */
    DecDCToutCallback(0);
    StUnSetRing();
    CdControlB(CdlPause,0,0);
    if(Rewind_Switch==0) {
#ifndef APD_LOAD
       PadStop();
#endif
#if defined(APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDLoadStop(VSync(-1));
#endif
       ResetGraph(3);
       StopCallback();
       return 0;
       }
    else
       return 1;
}


/*
 * init DECENV    buffer0(x0,y0),buffer1(x1,y1) :
 * デコード環境を初期化
 *  x0,y0 デコードした画像の転送先座標（バッファ０）
 *  x1,y1 デコードした画像の転送先座標（バッファ１）
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
       rect[]の幅／高さはSTRデータの値によってセットされる */
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
 * ストリーミング環境を初期化して開始
 */
static void strInit(CdlLOC  *loc, void (*callback)(), MovieInfo *movie)
{
    /* cold reset mdec : MDEC をリセット */
    DecDCTReset(0);

    /* set the callback after 1 block MDEC decoding :
       MDECが１デコードブロックを処理した時のコールバックを定義する */
    DecDCToutCallback(callback);
    
    /* set the ring buffer : リングバッファの設定 */
    StSetRing(Ring_Buff, RING_SIZE);
    
    /* init the streaming library : ストリーミングをセットアップ */
    /* end frame is set endless : 終了フレーム=∞に設定   */
    StSetStream(movie->is24bit, movie->startFrame, 0xffffffff, 0, 0);
    
    /* start the cdrom : ストリーミングリード開始 */
    strKickCD(loc);
}


/*
 *  back ground process
 *  callback of DecDCTout() :
 *  バックグラウンドプロセス 
 *  (DecDCTout() が終った時に呼ばれるコールバック関数)
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
			       RGB24の時はここで起動する */
	  StCdIntrFlag = 0;
        }
    }
/*    
    LoadImage(&dec.slice, (u_long *)dec.imgbuf[dec.imgid]);
*/
    id = dec.imgid;
    snap_rect = dec.slice;
    
    /* switch the id of decoding buffer : 画像デコード領域の切替え */
    dec.imgid = dec.imgid? 0:1;

    /* update slice(rectangle) position :
      スライス（短柵矩形）領域をひとつ右に更新 */
    if (isFirstSlice && (mod = dec.rect[dec.rectid].w % dec.slice.w)) {
        dec.slice.x += mod;
        isFirstSlice = 0;
    } else {
        dec.slice.x += dec.slice.w;
    }
    
    /* remaining slice ? : 残りのスライスがあるか？ */
    if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
        /* prepare for next slice : 次のスライスをデコード開始 */
        DecDCTout((u_long *)dec.imgbuf[dec.imgid], dec.slice.w * bound16(dec.slice.h)/2);
    }
    /* last slice ; end of 1 frame : 最終スライス＝１フレーム終了 */
    else {
        /* set the decoding done flag : 終ったことを通知 */
        dec.isdone = 1;
        isFirstSlice = 1;
        
        /* update the position on VRAM : 転送先座標値を更新 */
        dec.rectid = dec.rectid? 0: 1;
        dec.slice.x = dec.rect[dec.rectid].x;
        dec.slice.y = dec.rect[dec.rectid].y;
    }
  /* transfer the decoded data to VRAM :
     デコード結果をフレームバッファに転送 */
    LoadImage(&snap_rect, (u_long *)dec.imgbuf[id]);
}


/*
 *  execute VLC decoding
 *  the decoding data is the next frame's :
 *  VLCデコードの実行
 *  次の1フレームのデータのVLCデコードを行なう
 */
static int strNextVlc(DECENV  *dec, MovieInfo *movie)
{
    int cnt = MOVIE_WAIT;
    u_long  *next;
    static u_long *strNext();
    
    /* get the 1 frame streaming data : データを１フレーム分取り出す */
    while ((next = strNext(dec, movie)) == 0) {
        if (--cnt == 0)
            return -1;
    }

    /* switch the decoding area : VLCデコード領域の切替え */
    dec->vlcid = dec->vlcid? 0: 1;

    /* VLC decode */
    DecDCTvlc2(next, dec->vlcbuf[dec->vlcid],vlc_table);

    /* free the ring buffer : リングバッファのフレームの領域を解放する */
    StFreeRing(next);

    return;
}

/*
 *  get the 1 frame streaming data
 *  return vale     normal end -> top address of 1 frame streaming data
 *                  error      -> NULL : 
 *  リングバッファからのデータの取り出し
 *  （返り値）  正常終了時＝リングバッファの先頭アドレス
 *          エラー発生時＝NULL
 */
static u_long *strNext(DECENV  *dec, MovieInfo *movie)
{
    static int  strWidth  = 0;  /* width of the previous frame :
				   １つ前のフレームの大きさ（幅） */
    static int  strHeight = 0;  /* height of the previous frame :
				   １つ前のフレームの大きさ（高さ） */
    u_long      *addr;
    StHEADER    *sector;
    int cnt = MOVIE_WAIT;

    /* get the 1 frame streaming data withe TIME-OUT :
       データを取り出す（タイムアウト付き） */  
    while(StGetNext((u_long **)&addr,(u_long **)&sector)) {
        if (--cnt == 0)
            return(0);
    }

    /* if the frame number greater than the end frame, set the end switch :
       現在のフレーム番号が指定値なら終了  */
    if(sector->frameCount >= movie->endFrame) {
        Rewind_Switch = 1;
    }
    
    /* if the resolution is differ to previous frame, clear frame buffer :    
       画面の解像度が前のフレームと違うならば ClearImage を実行 */
    if(strWidth != sector->width || strHeight != sector->height) {
        
        RECT    rect;
        setRECT(&rect, 0, 0, VRAMPIX(movie->scrWidth, movie->is24bit), movie->scrHeight*2);
        if (movie->is24bit) {
            ClearImage(&rect, 0, 0, 0);     /* clear black on 24 bit mode :
					       24 ビットのときは黒でクリア */
        } else {
            ClearImage(&rect, 64, 64, 64);  /* clear gray on 16 bit mode :
					       16 ビットのときは灰色でクリア */
        }

        strWidth  = sector->width;
        strHeight = sector->height;
    }
    
    /* set DECENV according to the data on the STR format :    
       STRフォーマットのヘッダに合わせてデコード環境を変更する */
    dec->rect[0].w = dec->rect[1].w = VRAMPIX(strWidth, movie->is24bit);
    dec->rect[0].h = dec->rect[1].h = strHeight;
    dec->slice.h   = strHeight;
    
    return(addr);
}


/*
 *  wait for finish decodeing 1 frame with TIME-OUT :  
 *  １フレームのデコード終了を待つ
 *  時間を監視してタイムアウトをチェック
 */
static void strSync(DECENV  *dec, int mode /* VOID */)
{
    volatile u_long cnt = WAIT_TIME;

    /* wait for the decod is done flag set by background process :    
       バックグラウンドプロセスがisdoneを立てるまで待つ */
    while (dec->isdone == 0) {
        if (--cnt == 0) {
            /* if timeout force to switch buffer : 強制的に切替える */	  
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
 *  CDROMを指定位置からストリーミング開始する
 */
static void strKickCD(CdlLOC *loc)
{
  u_char param;

  param = CdlModeSpeed;
  
 loop:
  /* seek to the destination : 目的地まで Seek する */
  while (CdControl(CdlSetloc, (u_char *)loc, 0) == 0);
  while (CdControl(CdlSetmode, &param, 0) == 0);
#ifdef APD_LOAD
	APDSetCnt(VSync(3));
#else
  VSync(3);  /* wait for 3 VSync when changing the speed :
		倍速に切り替えてから ３V待つ必要がある */
#endif
    /* out the read command with streaming mode :
       ストリーミングモードを追加してコマンド発行 */
  if(CdRead2(CdlModeStream2|CdlModeSpeed|CdlModeRT) == 0)
    goto loop;
}
