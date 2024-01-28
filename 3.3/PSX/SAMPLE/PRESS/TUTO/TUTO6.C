/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto6 simplest sample
 *
 *	バックグラウンド動画の再生（フレームダブルバッファ付き）
 *		ただし、VLC のデコードは表で実行する
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
 *	デコード環境
 */
typedef struct {
	u_long	*vlcbuf[2];	/* VLC バッファ（ダブルバッファ） */
	int	vlcid;		/* 現在 VLC デコード中バッファの ID */
	u_long	*imgbuf;	/* デコード画像バッファ（シングル）*/	
	RECT	rect[2];	/* 転送エリア（ダブルバッファ） */
	int	rectid;		/* 現在転送中のバッファ ID */
	RECT	slice;		/* １回の DecDCTout で取り出す領域 */
} DECENV;

static DECENV		dec;		/* デコード環境の実体 */
static volatile int	isEndOfFlame;	/* フレームの最後になると 1 になる */

/*
 * バックグラウンドプロセス 
 * (DecDCTout() が終った時に呼ばれるコールバック関数)
 */
void out_callback()
{
	/* デコード結果をフレームバッファに転送 */
	LoadImage(&dec.slice, dec.imgbuf);
	/*DrawSync(0);*/
	
	/* 短柵矩形領域をひとつ右に更新 */
	dec.slice.x += 16;

	/* まだ足りなければ、*/
	if (dec.slice.x < dec.rect[dec.rectid].x + dec.rect[dec.rectid].w) {
		/* 次の短柵を受信 */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	}
	/* １フレーム分終ったら、*/
	else {
		/* 終ったことを通知 */
		isEndOfFlame = 1;
		
		/* ID を更新 */
		dec.rectid = dec.rectid? 0: 1;
		dec.slice.x = dec.rect[dec.rectid].x;
		dec.slice.y = dec.rect[dec.rectid].y;
	}
}		

/*
 *	フォアグラウンドプロセス
 */	
static u_long	vlcbuf0[256*256];	/* 大きさ適当 */
static u_long	vlcbuf1[256*256];	/* 大きさ適当 */
static u_long	imgbuf[16*240/2];	/* 短柵１個 */

static void StrRewind(void);
static u_long *StrNext(void);

/*
 * VLC のデコードワード数:
 * VLC デコードが長時間他の処理をブロックすると不都合な場合は、一回の
 * デコードワードの最大値をここで指定する。
 * DecDCTvlc() は VLC_SIZEワードの VLC をデコードすると一旦制御をアプリ
 * ケーションに戻す。
 */
#define VLC_SIZE	1024	

main()
{
	DISPENV	disp;			/* 表示環境 */
	DRAWENV	draw;			/* 描画環境 */
	RECT	rect;
	void	out_callback();		/* コールバック */
	int	id;
	u_long	padd;
	u_long	*next, *StrNext();	/* CD-ROM の代わり */
	int	isvlcLeft;		/* VLC デコード終了フラグ */
	
	PadInit(0);		/* PAD をリセット */
	ResetGraph(0);		/* GPU をリセット */
	DecDCTReset(0);		/* MDEC をリセット */
	SetGraphDebug(0);	/* デバッグレベル設定 */
	SetDispMask(1);		/* 表示許可 */
	isEndOfFlame = 0;	/* 旗を下げる */
	
	/* フレームバッファをクリア */
	setRECT(&rect, 0, 0, 640,480);
	ClearImage(&rect, 0, 0, 0);

	/* フォントロード */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 16, 256, 16, 1, 512));
	
	/* デコード構造体に値を設定 */
	dec.vlcbuf[0] = vlcbuf0;
	dec.vlcbuf[1] = vlcbuf1;
	dec.vlcid     = 0;
	dec.imgbuf    = imgbuf;
	dec.rectid    = 0;
	
	setRECT(&dec.rect[0], 0,  32, 256, 176);
	setRECT(&dec.rect[1], 0, 272, 256, 176);
	setRECT(&dec.slice,   0,  32,  16, 176);
		
	/* コールバックを定義する */
	DecDCToutCallback(out_callback);
	
	/* フレームを巻き戻す */
	StrRewind();
	
	/* まず最初の VLC を解く */
	DecDCTvlcSize(0);
	DecDCTvlc(StrNext(), dec.vlcbuf[dec.vlcid]);

	/* メインループ */
	while (1) {
		/* VLC の結果を送信する */
		DecDCTin(dec.vlcbuf[dec.vlcid], 0);	/* run-level を転送 */
		dec.vlcid = dec.vlcid? 0: 1;		/* ID をスワップ */

		/* 最初の短柵の受信の準備をする */
		/* ２発目からは、callback() 内で行なう */
		DecDCTout(dec.imgbuf, dec.slice.w*dec.slice.h/2);
	
		/* 次のサンプルを取り出す。*/
		if ((next = StrNext()) == 0)	
			break;
		
		/* vlc の最初の VLC_SIZE ワードをデコードする */ 
		DecDCTvlcSize(VLC_SIZE);
		isvlcLeft = DecDCTvlc(next, dec.vlcbuf[dec.vlcid]);	

		/* 経過時間を表示 */
		FntPrint("slice=%d,", VSync(1));
		
		/* データが出来上がるのを待つ */
		do {
			/* vlc の残りの VLC_SIZE ワードをデコードする */ 
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
			
		/* V-BLNK を待つ */
		VSync(0);
		
		/* 表示バッファをスワップ */
		/* 表示バッファは、描画バッファの反対側なことに注意 */
		id = dec.rectid? 0: 1;
		
		PutDispEnv(SetDefDispEnv(&disp, 0, id==0? 0:240, 256, 240));
		PutDrawEnv(SetDefDrawEnv(&draw, 0, id==0? 0:240, 256, 240));
		FntFlush(-1);
	}
}

/*
 *	次のストリーミングデータを読み込む（本当は CD-ROM から来る）
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

