/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *       GsOT に プリミティブを AddPrim する例 (GS ベース）
 *
 *	 Version	Date		Design
 *	-----------------------------------------
 *	2.00		Aug,31,1993	masa (original)
 *	2.10		Mar,25,1994	suzu (added addPrimitive())
 *      2.20            Dec,25,1994     yuta (chaned GsDOBJ4)
 *
 *		Copyright (C) 1993 by Sony Corporation
 *			All rights Reserved
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define MODEL_ADDR	(u_long *)0x80040000	/* モデリングデータ情報 */
#define TEX_ADDR	(u_long *)0x80180000	/* テクスチャ情報 */
#define SCRATCH		(u_long *)0x1f800000	/* スクラッチパッド */
	
#define SCR_Z		1000		/* スクリーンまでの距離 */
#define OT_LENGTH	12		/* OT解像度を12bit（大きさ） */
#define OTSIZE		(1<<OT_LENGTH)	/* タグの大きさ */
#define PACKETMAX	4000		/* 1フレームの最大パケット数 */
#define PACKETMAX2	(PACKETMAX*24)	/* 1パケット平均サイズを24Byteとする */

PACKET	GpuPacketArea[2][PACKETMAX2];	/* パケット領域（ダブルバッファ） */
GsOT	WorldOT[2];			/* OT情報（ダブルバッファ） */
SVECTOR	PWorld; 	/* work short vector for making Coordinate parmeter
			   /* 座標系を作るためのローテーションベクター */

GsOT_TAG	OTTags[2][OTSIZE];	/* OTのタグ領域（ダブルバッファ） */
GsDOBJ2		object;			/*  オブジェクト変数 */
GsRVIEW		view;			/* 視点情報 */
GsF_LIGHT	pslt[3];		/* 光源×３個 */
u_long		PadData;		/* コントロールパッド情報 */
GsCOORDINATE2   DWorld;			/* Coordinate for GsDOBJ2
					/* オブジェクトごとの座標系 */

extern MATRIX GsIDMATRIX;

/*
 *  メインルーチン
 */
main()
{
	
	/* Initialize: イニシャライズ */
	initSystem();			/* grobal variables */
	initView();			/* position matrix */
	initLight();			/* light matrix */
	initModelingData(MODEL_ADDR);	/* load model data */
	initTexture(TEX_ADDR);		/* load texture pattern */
	initPrimitives();		/* GPU primitives */
	
	while(1) {
		if ( moveObject() ) break;
		drawObject();
	}
	PadStop();
	StopCallback();
	return 0;
}

/*
 *  3Dオブジェクト描画処理
 */
drawObject()
{
	int activeBuff;
	MATRIX tmpls;
	
	/* ダブルバッファのうちどちらがアクティブか？ */
	activeBuff = GsGetActiveBuff();
	
	/* GPUパケット生成アドレスをエリアの先頭に設定 */
	GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
	
	/* OTの内容をクリア */
	ClearOTagR((u_long *)WorldOT[activeBuff].org, OTSIZE);
	
	/* 3Dオブジェクト（キューブ）のOTへの登録 */
	GsGetLw(object.coord2,&tmpls);		/* マトリクス生成 */
	GsSetLightMatrix(&tmpls);
	GsGetLs(object.coord2,&tmpls);
	GsSetLsMatrix(&tmpls);
	GsSortObject4(&object, &WorldOT[activeBuff],14-OT_LENGTH, SCRATCH);
	
	/* プリミティブを追加 */
	addPrimitives(WorldOT[activeBuff].org);
	
	/* パッドの内容をバッファに取り込む */
	PadData = PadRead(0);

	/* V-BLNKを待つ */
	VSync(0);

	/* 前のフレームの描画作業を強制終了 */
	ResetGraph(1);

	/* ダブルバッファを入れ換える */
	GsSwapDispBuff();

	/* OTの先頭に画面クリア命令を挿入 */
	GsSortClear(0x0, 0x0, 0x0, &WorldOT[activeBuff]);

	/* OTの内容をバックグラウンドで描画開始 */
	/*DumpOTag(WorldOT[activeBuff].org+OTSIZE-1);*/
	DrawOTag((u_long *) (WorldOT[activeBuff].org+OTSIZE-1));
}

/*
 *  コントロールパッドによるオブジェクト移動
 */
moveObject()
{
	/* オブジェクト変数内のローカル座標系の値を更新 */
  
	if(PadData & PADRleft) PWorld.vy -= 5*ONE/360;
	if(PadData & PADRright) PWorld.vy += 5*ONE/360;
	if(PadData & PADRup) PWorld.vx += 5*ONE/360;
	if(PadData & PADRdown) PWorld.vx -= 5*ONE/360;
	
	if(PadData & PADm) DWorld.coord.t[2] -= 200;
	if(PadData & PADo) DWorld.coord.t[2] += 200;
	
	/* Calculate Matrix from Object Parameter and Set Coordinate
	/* オブジェクトのパラメータからマトリックスを計算し座標系にセット */
	set_coordinate(&PWorld,&DWorld);
	

	/* Clear flag of Coordinate for recalculation
	/* 再計算のためのフラグをクリアする */
	  DWorld.flg = 0;
	
	/* Kボタンで終了 */
	if(PadData & PADselect) 
		return -1;		
	return 0;
}

/*
 *  初期化ルーチン群
 */
initSystem()
{
	int	i;

	/* コントロールパッドの初期化 */
	PadInit(0);
	PadData = 0;
	
	/* GPUの初期化 */
	GsInitGraph(320, 240, 0, 0, 0);
	GsDefDispBuff(0, 0, 0, 240);
	
	/* OTの初期化 */
	for (i = 0; i < 2; i++) {
		WorldOT[i].length = OT_LENGTH;
		WorldOT[i].point  = 0;
		WorldOT[i].offset = 0;
		WorldOT[i].org    = OTTags[i];
		WorldOT[i].tag    = OTTags[i] + OTSIZE - 1;
	}
	
	/* 3Dライブラリの初期化 */
	GsInit3D();
}

/*
 *  視点位置の設定
 */
initView()
{
	/* プロジェクション（視野角）の設定 */
	GsSetProjection(SCR_Z);

	/* 視点位置の設定 */
	view.vpx = 0; view.vpy = 0; view.vpz = -1000;
	view.vrx = 0; view.vry = 0; view.vrz = 0;
	view.rz = 0;
	view.super = WORLD;
	GsSetRefView(&view);

	/* Zクリップ値を設定 */
	GsSetNearClip(100);
}

/*
 *  光源の設定（照射方向＆色）
 */
initLight()
{
	/* 光源#0 (100,100,100)の方向へ照射 */
	pslt[0].vx = 100; pslt[0].vy= 100; pslt[0].vz= 100;
	pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
	GsSetFlatLight(0,&pslt[0]);
	
	/* 光源#1（使用せず） */
	pslt[1].vx = 100; pslt[1].vy= 100; pslt[1].vz= 100;
	pslt[1].r=0; pslt[1].g=0; pslt[1].b=0;
	GsSetFlatLight(1,&pslt[1]);
	
	/* 光源#2（使用せず） */
	pslt[2].vx = 100; pslt[2].vy= 100; pslt[2].vz= 100;
	pslt[2].r=0; pslt[2].g=0; pslt[2].b=0;
	GsSetFlatLight(2,&pslt[2]);
	
	/* アンビエント（周辺光）の設定 */
	GsSetAmbient(ONE/2,ONE/2,ONE/2);

	/* 光源モードの設定（通常光源/FOGなし） */
	GsSetLightMode(0);
}

/*
 *  メモリ上のTMDデータの読み込み&オブジェクト初期化
 *		(先頭の１個のみ使用）
 */
initModelingData(addr)
u_long *addr;
{
	u_long *tmdp;
	GsDOBJ *objp;
	
	/* TMDデータ読み込み */
	tmdp = addr;			/* TMDデータの先頭アドレス */
	tmdp++;				/* ファイルヘッダをスキップ */
	GsMapModelingData(tmdp);	/* 実アドレスへマップ */
	tmdp++;				/* フラグ読み飛ばし */
	tmdp++;				/* オブジェクト個数読み飛ばし */
	
	GsLinkObject4((u_long)tmdp,&object,0);
	
	/* Init work vector
        /* マトリックス計算ワークのローテーションベクター初期化 */
        PWorld.vx=PWorld.vy=PWorld.vz=0;
	GsInitCoordinate2(WORLD, &DWorld);
	
	/* 3Dオブジェクト初期化 */
	object.coord2 =  &DWorld;
	object.coord2->coord.t[2] = 4000;
	object.tmd = tmdp;		/* 読み込んだTMDデータをポイントする */
	object.attribute = 0;
}

/*
 *  （メモリ上の）テクスチャデータの読み込み
 */
initTexture(addr)
u_long *addr;
{
	RECT rect1;
	GsIMAGE tim1;

	/* TIMデータの情報を得る */	
	GsGetTimInfo(addr+1, &tim1);	/* ファイルヘッダを飛ばして渡す */

	/* ピクセルデータをVRAMへ送る */	
	rect1.x=tim1.px;
	rect1.y=tim1.py;
	rect1.w=tim1.pw;
	rect1.h=tim1.ph;
	LoadImage(&rect1,tim1.pixel);

	/* CLUTがある場合はVRAMへ送る */
	if((tim1.pmode>>3)&0x01) {
		rect1.x=tim1.cx;
		rect1.y=tim1.cy;
		rect1.w=tim1.cw;
		rect1.h=tim1.ch;
		LoadImage(&rect1,tim1.clut);
	}
}

/* Set coordinte parameter from work vector
/* ローテションベクタからマトリックスを作成し座標系にセットする */
set_coordinate(pos,coor)
SVECTOR *pos;			/* work vector /* ローテションベクタ */
GsCOORDINATE2 *coor;		/* Coordinate /* 座標系 */
{
  MATRIX tmp1;
  SVECTOR v1;
  
  tmp1   = GsIDMATRIX;		/* start from unit matrix
				/* 単位行列から出発する */
    
  /* Set translation /* 平行移動をセットする */
  tmp1.t[0] = coor->coord.t[0];
  tmp1.t[1] = coor->coord.t[1];
  tmp1.t[2] = coor->coord.t[2];
  
  /*/* マトリックスワークにセットされているローテーションを
     ワークのベクターにセット */
  v1 = *pos;
  
  /* Rotate Matrix
  /* マトリックスにローテーションベクタを作用させる */
  RotMatrix(&v1,&tmp1);
  
  /* Set Matrix to Coordinate
  /* 求めたマトリックスを座標系にセットする */
  coor->coord = tmp1;
  
  /* Clear flag becase of changing parameter
  /* マトリックスキャッシュをフラッシュする */
  coor->flg = 0;
}

/*
 *	プリミティブの初期化
 */
#include "balltex.h"
#define NBALL	256		/* ボールの個数 */
#define DIST	SCR_Z/4		/* ボールが散らばっている範囲 */

POLY_FT4	ballprm[2][NBALL];	/* プリミティブバッファ */
SVECTOR		ballpos[NBALL];		/* ボールの位置 */
	
initPrimitives()
{

	int		i, j;
	u_short		tpage, clut[32];
	POLY_FT4	*bp;	
		
	/* ボールのテクスチャページをロードする */
	tpage = LoadTPage(ball16x16, 0, 0, 640, 256, 16, 16);

	/* ボール用の CLUT（３２個）をロードする */
	for (i = 0; i < 32; i++)
		clut[i] = LoadClut(ballcolor[i], 256, 480+i);
	
	/* initialize primiteves */
	for (i = 0; i < 2; i++)
		for (j = 0; j < NBALL; j++) {
			bp = &ballprm[i][j];
			SetPolyFT4(bp);
			SetShadeTex(bp, 1);
			bp->tpage = tpage;
			bp->clut = clut[j%32];
			setUV4(bp, 0, 0, 16, 0, 0, 16, 16, 16);
		}
	
	/* initialize position */
	for (i = 0; i < NBALL; i++) {
		ballpos[i].vx = (rand()%DIST)-DIST/2;
		ballpos[i].vy = (rand()%DIST)-DIST/2;
		ballpos[i].vz = (rand()%DIST)-DIST/2;
	}
}

/*
 *	プリミティブのＯＴへの登録
 */
addPrimitives(ot)
u_long	*ot;
{
	static int	id    = 0;		/* ダブルバッファ ID */
	static VECTOR	trans = {0, 0, SCR_Z};	/* ボールの位置 */
	static SVECTOR	angle = {0, 0, 0};	/* ボールの回転角 */
	static MATRIX	rottrans;		/* 回転行列 */
	int		i, padd;
	long		dmy, flg, otz;
	POLY_FT4	*bp;
	SVECTOR		*sp;
	SVECTOR		dp;
	
	
	id = (id+1)&0x01;		/* ID を スワップ */
	PushMatrix();			/* カレントマトリクスを退避させる */
	
	/* パッドの内容からマトリクス rottrans をアップデート */
	padd = PadRead(1);
	if(padd & PADLup)    angle.vx += 10;
	if(padd & PADLdown)  angle.vx -= 10;
	if(padd & PADLleft)  angle.vy -= 10;
	if(padd & PADLright) angle.vy += 10;
	if(padd & PADl) trans.vz -= 50;
	if(padd & PADn) trans.vz += 50;
	
	RotMatrix(&angle, &rottrans);		/* 回転 */
	TransMatrix(&rottrans, &trans);		/* 並行移動 */
	
	/* マトリクス rottrans をカレントマトリクスに設定 */
	SetTransMatrix(&rottrans);	
	SetRotMatrix(&rottrans);
	
	/* プリミティブをアップデート */
	bp = ballprm[id];
	sp = ballpos;
	for (i = 0; i < NBALL; i++, bp++, sp++) {
		otz = RotTransPers(sp, (long *)&dp, &dmy, &flg);
		if (otz > 0 && otz < OTSIZE) {
			setXY4(bp, dp.vx, dp.vy,    dp.vx+16, dp.vy,
			           dp.vx, dp.vy+16, dp.vx+16, dp.vy+16);

			AddPrim(ot+otz, bp);
		}
	}

	/* 退避させていたマトリクスを引き戻す。*/
	PopMatrix();
}
