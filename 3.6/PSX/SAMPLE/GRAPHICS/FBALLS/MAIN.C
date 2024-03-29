/* $PSLibId: Runtime Library Release 3.6$ */
/*				Fast balls
 *
 *		画面内をバウンドする複数のボールを描画する
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * definition for speed up: 高速化のための定義 
 */
#define FASTVBLNK	/* shorten VBLNK: VBLNK を短くする */
#define SMALLDRAWAREA	/* decrease drawing: 描画を減らす */
#define OTENV		/* do PutDrawEnv and DrawOTag at the same time
			   : PutDrawEnv と DrawOTag を同時に行う */

/*
 * Kaji Printf: 漢字をプリントするための定義
 */
#define KANJI

/*#define DEBUG */
/*
 * Primitive Buffer: プリミティブ関連のバッファ
 */
#define OTSIZE		1		/* size of ordering table
					   : オーダリングテーブルの数 */
#define MAXOBJ		4000		/* max sprite number :
					   スプライト（ボール）数の上限 */
typedef struct {
	DRAWENV		draw;		/* drawing environment : 描画環境 */
	DISPENV		disp;		/* display environment : 表示環境 */
	u_long		ot[OTSIZE];	/* ordering table:
					   : オーダリングテーブル */
	SPRT_16		sprt[MAXOBJ];	/* 16x16 fixed-size sprite:
					   16x16固定サイズのスプライト */
} DB;

/*
 * Position Buffer: スプライトの動きに関するバッファ
 */
typedef struct {
	u_short x, y;			/* current point: 現在の位置 */
	u_short dx, dy;			/* verocity: 速度 */
} POS;

/*
 * 表示領域
 */
#define	FRAME_X		640		/* frame size:表示領域サイズ(320x240)*/
#define	FRAME_Y		480
#define WALL_X		(FRAME_X-16)	/* reflection point
					   : スプライトの可動領域サイズ */
#define WALL_Y		(FRAME_Y-16)

/* preset unchanged primitve members: プリミティブバッファの初期設定 */
static void init_prim(DB *db);	

/* parse controller: コントローラの解析 */
static int  pad_read(int n);	

/* callback for VSync: V-Sync時のコールバックルーチン */
static void  cbvsync(void);	

/* intitialze position table : ボールのスタート位置と移動距離の設定 */
static int  init_point(POS *pos);

main()
{
	/* ボールの座標値と移動距離を格納するバッファ */
	POS	pos[MAXOBJ];	
	
	/* double buffer: ダブルバッファのため２つ用意する */
	DB	db[2];		
	
	/* current double buffer: 現在のダブルバッファバッファのアドレス */
	DB	*cdb;		

	/* object number: 表示するスプライトの数（最初は１つから）*/
	int	nobj = 1;	
	
	/* current OT: 現在のＯＴのアドレス */
	u_long	*ot;		
	
	SPRT_16	*sp;		/* work */
	POS	*pp;		/* work */
	int	i, cnt, x, y;	/* work */

	/* reset PAD: コントローラのリセット */
	PadInit(0);		
	
	/* reset graphics sysmtem (0:cold,1:warm); 描画・表示環境のリセット */
	ResetGraph(0);		
	
	/* set debug mode (0:off,1:monitor,2:dump): デバッグモードの設定 */
	SetGraphDebug(0);	
	
	/* set callback: V-sync時のコールバック関数の設定 */
	VSyncCallback(cbvsync);	

	/* inititlalize environment for double buffer
	: 描画・表示環境をダブルバッファ用に設定
	(0,  0)-(320,240)に描画しているときは(0,240)-(320,480)を表示(db[0])
	(0,240)-(320,480)に描画しているときは(0,  0)-(320,240)を表示(db[1])
	*/
#ifdef SMALLDRAWAREA
	{
		RECT rect;

		setRECT(&rect, 0, 0, 640, 512);
		ClearImage(&rect, 0, 0, 0);
	}
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 464);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 464);
#else /* SMALLDRAWAREA */
	SetDefDrawEnv(&db[0].draw, 0, 0, 640, 480);
	SetDefDrawEnv(&db[1].draw, 0, 0, 640, 480);
#endif /* SMALLDRAWAREA */
	SetDefDispEnv(&db[0].disp, 0, 0, 640, 480);
	SetDefDispEnv(&db[1].disp, 0, 0, 640, 480);
#ifdef FASTVBLNK
	db[0].disp.screen.h = 242;
	db[1].disp.screen.h = 242;
#endif /* FASTVBLNK */

	/* init font environment; フォントの設定 */
#ifdef KANJI	/* KANJI */
	KanjiFntOpen(160, 16, 256, 200, 704, 0, 768, 256, 0, 512);
#endif	
	/* :基本フォントパターンをフレームバッファにロード */
	FntLoad(960, 256);	
	
	/* :フォントの表示位置の設定 */
	SetDumpFnt(FntOpen(16, 16, 256, 200, 0, 512));	

	/* initialize primitive buffer: プリミティブバッファの初期設定(db[0])*/
	init_prim(&db[0]);	
	
	/* initialize primitive buffer: プリミティブバッファの初期設定(db[1])*/
	init_prim(&db[1]);	
	
	/* set initial geometries: ボールの動きに関する初期設定 */
	init_point(pos);	

	/* enable to display: ディスプレイへの表示開始 */
	SetDispMask(1);		/* 0:inhibit,1:enable: ０：不可  １：可 */

	/* ; メインループ */
	while ((nobj = pad_read(nobj)) > 0) {
		/* swap double buffer ID: ダブルバッファポインタの切り替え */
		cdb  = (cdb==db)? db+1: db;	
#ifdef DEBUG
		/* dump DB environment */
		DumpDrawEnv(&cdb->draw);
		DumpDispEnv(&cdb->disp);
		DumpTPage(cdb->draw.tpage);
#endif

 		/* clear ordering table: オーダリングテーブルのクリア */
		ClearOTag(cdb->ot, OTSIZE);

		/* update sprites :
		   ボールの位置を１つずつ計算してＯＴに登録していく */
		ot = cdb->ot;
		sp = cdb->sprt;
		pp = pos;
		for (i = 0; i < nobj; i++, sp++, pp++) {
			/* detect reflection:
			   座標値の更新および画面上での位置の計算 */
			if ((x = (pp->x += pp->dx) % WALL_X*2) >= WALL_X)
				x = WALL_X*2 - x;
			if ((y = (pp->y += pp->dy) % WALL_Y*2) >= WALL_Y)
				y = WALL_Y*2 - y;

			/* update vertex: 計算した座標値をセット */
			setXY0(sp, x, y);	
			
			/* apend to OT: ＯＴへ登録 */
			AddPrim(ot, sp);	
		}
		/* wait for end of drawing: 描画の終了待ち */
		DrawSync(0);		
		
		/* cnt = VSync(1);	/* check for count */
		/* cnt = VSync(2);	/* wait for V-BLNK (1/30) */
		cnt = VSync(0);		/* wait for V-BLNK (1/60) */

		/*: ダブルバッファの切替え */
		/* update display environment: 表示環境の更新 */
		PutDispEnv(&cdb->disp); 
		
		/* update drawing environment: 描画環境の更新 */
#ifdef OTENV
		DrawOTagEnv(cdb->ot, &cdb->draw); 
#else /* OTENV */
		PutDrawEnv(&cdb->draw); 
		
		/*: ＯＴに登録されたプリミティブの描画 */
		DrawOTag(cdb->ot);	
#endif /* OTENV */
#ifdef DEBUG
		DumpOTag(cdb->ot);
#endif
		/*: ボールの数と経過時間のプリント */
#ifdef KANJI
		KanjiFntPrint("玉の数＝%d\n", nobj);
		KanjiFntPrint("時間=%d\n", cnt);
		KanjiFntFlush(-1);
#endif
		FntPrint("sprite = %d\n", nobj);
		FntPrint("total time = %d\n", cnt);
		FntFlush(-1);
	}
	PadStop();	/*: コントローラのクローズ */
	StopCallback();
	return(0);
}

/*
 * Initialize drawing Primitives: プリミティブバッファの初期設定
 */
#include "balltex.h"	/* ボールのテクスチャパターンが入っているファイル */

/* DB *db; プリミティブバッファ*/
static void init_prim(DB *db)
{
	u_short	clut[32];		/* CLUT entry: テクスチャ CLUT */
	SPRT_16	*sp;			/* work */
	int	i;			/* work */

	/* set bg color: 背景色のセット */
	db->draw.isbg = 1;
	setRGB0(&db->draw, 60, 120, 120);

	/* load texture pattern: テクスチャのロード */
	db->draw.tpage = LoadTPage(ball16x16, 0, 0, 640, 0, 16, 16);
#ifdef DEBUG
	DumpTPage(db->draw.tpage);
#endif
	/* load texture CLUT: テクスチャ CLUTのロード */
	for (i = 0; i < 32; i++) {
		clut[i] = LoadClut(ballcolor[i], 640, 480+i);
#ifdef DEBUG
		DumpClut(clut[i]);
#endif
	}

	/* initialize sprite: スプライトの初期化 */
	for (sp = db->sprt, i = 0; i < MAXOBJ; i++, sp++) {
		/* set SPRT_16: 16x16スプライトプリミティブの初期化 */
		SetSprt16(sp);		
		
		/* semi-ambient is ON: 半透明属性� ONt */
		SetSemiTrans(sp, 1);	
		
		/* shaded texture is OFF: シェーディングを行わない */
		SetShadeTex(sp, 1);	
		
		/* texture point is (0,0): u,vを(0,0)に設定 */
		setUV0(sp, 0, 0);	
		
		/* set CLUT: CLUT のセット */
		sp->clut = clut[i%32];	
	}
}

/*
 * Initialize sprite position and verocity:
 : ボールのスタート位置と移動量を設定する
 */

/* POS	*pos;		ボールの動きに関する構造体 */
static init_point(POS *pos)	
{
	int	i;
	for (i = 0; i < MAXOBJ; i++) {
		pos->x  = rand();		/*: スタート座標 Ｘ */
		pos->y  = rand();		/*: スタート座標 Ｙ */
		pos->dx = (rand() % 4) + 1;	/*: 移動距離 Ｘ (1<=x<=4) */
		pos->dy = (rand() % 4) + 1;	/*: 移動距離 Ｙ (1<=y<=4) */
		pos++;
	}
}

/*
 * Read controll-pad: コントローラの解析
 */
/* int n; スプライトの数 */
static int pad_read(int	n)		
{
	u_long	padd = PadRead(1);		/*: コントローラの読み込み */

	if(padd & PADLup)	n += 4;		/*: 左の十字キーアップ */
	if(padd & PADLdown)	n -= 4;		/*: 左の十字キーダウン */

	if (padd & PADL1) 			/*: pause */
		while (PadRead(1)&PADL1);

	if(padd & PADselect) 	return(-1);	/*: プログラムの終了 */

	limitRange(n, 1, MAXOBJ-1);	/*: nを1<=n<=(MAXOBJ-1)の値にする */
					/* see libgpu.h: libgpu.hに記載 */
	return(n);
}

/*
 * callback: コールバック
 */
static void cbvsync(void)
{
	/* print absolute VSync count */
	FntPrint("V-BLNK(%d)\n", VSync(-1));	
}


