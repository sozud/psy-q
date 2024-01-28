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
#define PPW	3/2		/* １ショートワードに何ピクセルあるか */
#define MODE	3		/* 24bit モードでデコード */
#else			
#define PPW	1		/* １ショートワードに何ピクセルあるか */
#define MODE	2		/* 16bit モードでデコード */
#endif

#define FILENAME	"\\DATA\\MOV.STR;1"
#define FRAME_SIZE	1712	/* アニメーションのフレーム数 */
static CdlLOC loc;		/* CDROMを再生させるポイント */

/*
 *	デコード環境
 */
typedef struct {
	u_long	*vlcbuf[2];	/* VLC バッファ（ダブルバッファ） */
	int	vlcid;		/* 現在 VLC デコード中バッファの ID */
	u_short	*imgbuf;	/* デコード画像バッファ（シングル）*/
	RECT	rect[2];	/* 転送エリア（ダブルバッファ） */
	int	rectid;		/* 現在転送中のバッファ ID */
	RECT	slice;		/* １回の DecDCTout で取り出す領域 */
	int	isdone;		/* １フレーム分のデータができたか */
} DECENV;

static DECENV	dec;		/* デコード環境の実体 */
static int      Rewind_Switch;	/* CDが終りまでいくと１になる */


/*
 *	フォアグラウンドプロセス
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
	
	void	strCallback();		/* コールバック */
	int	endCallback();		/* コールバック */
	int	frame = 0;		/* フレームカウンタ */
	int	id;			/* 表示バッファ ID */
	volatile u_long  enc_count;	/* カウンタ */

	u_char	result[8];		/* CD-ROM status */
	CdlFILE	file;
	
	ResetCallback();
	CdInit();
	PadInit(0);		/* PAD をリセット */
	CdSetDebug(0);
	ResetGraph(0);		/* GPU をリセット */
	SetGraphDebug(0);	/* デバッグレベル設定 */
	
	/* プリミティブを初期化 */
	init_graph();
	
	/* フォントをロード */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(32, 32, 320, 200, 0, 512));
		
	/* ファイルをサーチ */
	if (CdSearchFile(&file, FILENAME) == 0) {
		StopCallback();
		PadStop();
		exit();
	}
	loc.minute = file.pos.minute;
	loc.second = file.pos.second;
	loc.sector = file.pos.sector;

	/* 初期化 */
	strSetDefDecEnv(&dec, 640, 0, 640, 0);
	strInit(&loc, FRAME_SIZE, strCallback, endCallback);
	
	/* 次のフレームのデータをとってきて VLC のデコードを行なう */
	/* 結果は、dec.vlcbuf[dec.vlcid] に格納される。*/
	strNextVlc(&dec);

	SetDispMask(1);		/* 表示許可 */
	while (1) {

		/* VLC の完了したデータを送信 */
		DecDCTin(dec.vlcbuf[dec.vlcid], MODE);

		/* 最初のデコードブロックの受信の準備をする */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
		
		/* 次のフレームのデータの VLC デコード */
		strNextVlc(&dec);

		/* パラメータを出力する */
		if (CdSync(1, 0) == CdlComplete) 
			CdControl(CdlGetlocP, 0, result);

		FntPrint("frame=%d\n", frame++);
		FntPrint("(%02x:%02x:%02X)\n",
			 result[5], result[6], result[7]);
		
		/* ここにアプリケーションのコードがはいる */
		if (graph() != 0) {
			CdControlB(CdlStop, 0, 0);
			PadStop();
			ResetGraph(3);
			StopCallback();
			/* exit(); */
			return 0;
		}
		
		/* データが出来上がるのを待つ */
		strSync(&dec, 0);
	}
}

/*
 * デコード環境を初期化
 * imgbuf, vlcbuf は最適なサイズを割り当てるべきです。	
 */
static void strSetDefDecEnv(DECENV *dec, int x0, int y0, int x1, int y1)
{
	static u_long	vlcbuf0[320/2*256];		/* 大きさ適当 */
	static u_long	vlcbuf1[320/2*256];		/* 大きさ適当 */
	static u_short	imgbuf[16*PPW*240*2];		/* 短柵１個 */

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
 * ストリーミング環境を初期化
 */
#define RING_SIZE 	32		/* リングバッファサイズ */

static void strInit(CdlLOC *loc, int frame_size, 
		    void (*callback)(), int (*endcallback)())
{
	static u_long	sect_buff[RING_SIZE*SECTOR_SIZE];/* リングバッファ */
	
	DecDCTReset(0);		/* MDEC をリセット */
	Rewind_Switch = 0;	/* 巻き戻し０ */

	/* MDECが１デコードブロックを処理した時のコールバックを定義する */
	DecDCToutCallback(callback);

	/* リングバッファの設定 */
	StSetRing(sect_buff, RING_SIZE);
	
	/* ストリーミングをセットアップ */
	/* 最後までいったら endcallback() がコールバックされる */
	StSetStream(IS_RGB24, 1, frame_size, 0, endcallback);

	/* 最初の１フレームをとってくる */
	strKickCD(loc);
}


/*
 * バックグラウンドプロセス 
 * (DecDCTout() が終った時に呼ばれるコールバック関数)
 */
static void strCallback()
{
#if IS_RGB24==1
	extern StCdIntrFlag;	
	if(StCdIntrFlag) {
		StCdInterrupt();	/* RGB24の時はここで起動する */
		StCdIntrFlag = 0;
	}
#endif

	/* デコード結果をフレームバッファに転送 */
	LoadImage(&dec.slice, (u_long *)dec.imgbuf);
	
	/* 短柵矩形領域をひとつ右に更新 */
	dec.slice.x += dec.slice.w;
	
	/* まだ足りなければ、*/
	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
		/* 次の短柵を受信 */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	}
	/* １フレーム分終ったら、*/
	else {
		/* 終ったことを通知 */
		dec.isdone = 1;
		
		/* ID を更新 */
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
		
		/*DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);*/
	}
}

/*
 * バックグラウンドプロセス 
 * CDROMが終りまでいったら呼ばれる関数
 */
static int endCallback()
{
	Rewind_Switch = 1;
}


/* 次のフレームのデータをよみだし VLC の解凍を行なう
 * エラーならばリングバッファをクリアして次のフレームを獲得する 
 * VLCがデコードされたらリングバッファのそのフレームの領域は
 * 必要ないのでリングバッファのフレームの領域を解放する 
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
 * 次の１フレームのMOVIEフォーマットのデータをリングバッファからとってくる
 * データが正常だったら リングバッファの先頭アドレスを
 * 正常でなければ NULLを返す
 */  

static u_long *strNext(DECENV *dec)
{
	u_long		*addr;
	StHEADER	*sector;
	int		cnt = WAIT_TIME;
	static 	int      width  = 0;	        /* 画面の横と縦 */
	static 	int      height = 0;	

	while(StGetNext(&addr, (u_long **)&sector)) {
		if (--cnt == 0)
			return(0);
		/* 巻き戻しが設定されていたら先頭にジャンプ */
		if(Rewind_Switch) {
			strKickCD(&loc);
			Rewind_Switch = 0;
		}
	}

	/* 画面の解像度が 前のフレームと違うならば ClearImage を実行 */
	if(width != sector->width || height != sector->height) {
		
		RECT	rect;
		setRECT(&rect, 0, 0, 640, 480);
		ClearImage(&rect, 0, 0, 0);
		
		width  = sector->width;
		height = sector->height;
	}

	/* ミニヘッダに合わせてデコード環境を変更する */
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
			/* timeout: 強制的に切替える */
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
 * CDROMをlocからREADする。
 */
static void strKickCD(CdlLOC *loc)
{
	/* 目的地まで Seek する */
	while (CdControl(CdlSeekL, (u_char *)loc, 0) == 0);	
  
	/* ストリーミングモードを追加してコマンド発行 */
	while(CdRead2(CdlModeStream|CdlModeSpeed) == 0);	
}
