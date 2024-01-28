/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				db.c
 *			
 *		  (640x480) ダブルバッファハンドラ
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Mar,15,1994	suzu
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

/*
 * ダブルバッファ環境をセットアップする
 * フレームダブルバッファは、以下のどちらか。
 *	(0,0)-(xxx, 240), (0, 240)-(xxx, 480)	(xxx = 256/320/512/640)	
 *	(0,0)-(xxx, 480), (0,   0)-(xxx, 480)	(xxx = 256/320/512/640)	
 */	
static int	id = 0;		/* バッファ ID */
static DRAWENV	draw[2];	/* 描画環境 */
static DISPENV	disp[2];	/* 表示環境 */
static int	font;		/* デバッグプリント環境 */

db_init(w, h, z)
int	w, h;	/* 描画エリア */
int	z;	/* プロジェクション */
{
	/*
	 * インターレースでない場合はダブルバッファする。
	 */	 
	int	oy = (h==480)? 0: 240;
	
	/*
	 * システムの初期化
	 */
	/*PadInit(0);			/* コントロールパッドの初期化 */
	/*ResetGraph(0);		/* GPU を初期化 */
	InitGeom();			/* GTE の初期化 */
	SetGeomOffset(w/2, h/2);	/* 消失点の座標を設定（画面中央） */
	SetGeomScreen(h);		/* 投影面までの距離 */
	
	/*
	 * フレームバッファ上でダブルバッファを構成するための描画環境・
	 * 表示環境構造体のメンバを設定する。
	 */	 
	SetDefDrawEnv(&draw[0], 0,  0, w, h);
	SetDefDispEnv(&disp[0], 0, oy, w, h);
	SetDefDrawEnv(&draw[1], 0, oy, w, h);
	SetDefDispEnv(&disp[1], 0,  0, w, h);

	/* isbg = 1 にすると PutDrawEnv() の時点で描画エリアがクリアされる */
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0(&draw[0], 60, 120, 120);
	setRGB0(&draw[1], 60, 120, 120);

	/* フォント表示環境を設定 (VRAM アドレス=(960,256)) */
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(32, 32, 512, 256, 0, 512));
}

/*
 * ダブルバッファをスワップする
 */	
db_swap(ot)
u_long	*ot;
{
	id = id? 0: 1;
	DrawSync(0);
	VSync(0);
	PutDrawEnv(&draw[id]);		/* 描画環境を更新 */
	PutDispEnv(&disp[id]);		/* 表示環境を更新 */
	DrawOTag(ot);			/* ＯＴ上のプリミティブを描画 */
	/*DrawSync(0);*/
	FntFlush(-1);			/* デバッグプリント */
}

