/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			    tuto1
 *			
 *	   TMD ビューアプロトタイプ（光源計算なし  FT3 型）
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 */

#include <sys/types.h>	
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <inline.h>
#include "rtp.h"

/*
 * ３角形ポリゴンの頂点情報
 */
typedef struct {
	SVECTOR	n0;		/* 法線 */
	SVECTOR	v0, v1, v2;	/* 頂点 */
} VERT_F3;

/*
 * ３角形ポリゴンパケット
 * 頂点・パケットバッファは、malloc() で動的に割り付けるべきです。	
 * この例ではオブジェクト当たりのポリゴン数は、MAX_POLY 個	
 */
#define MAX_POLY	400			/* ポリゴンの最大値 */
typedef struct {
	MATRIX		m;			/* ローカルマトリクス */
	SVECTOR		v;			/* 移動ベクトル */
	int		n;			/* ポリゴン数 */
	VERT_F3		vert[MAX_POLY];		/* 頂点バッファ （１個）*/
	POLY_FT3	prim[2][MAX_POLY];	/* パケットバッファ（２個）*/
} OBJ_FT3;

#define SCR_Z		1024		/* 投影面までの距離 */
#define OTSIZE		8192		/* ＯＴの段数（２のべき乗） */
#define OTMASK		(OTSIZE-1)	/* ＯＴの段数のマスク */
#define MAX_OBJ		9		/* オブジェクトの個数 */
#define MODELADDR	((u_long *)0x80120000)	/* TMD のアドレス */
#define TEXADDR		((u_long *)0x80140000)	/* TIM のアドレス */

static int pad_read(OBJ_FT3 *obj, int nobj, MATRIX *m);
static void set_position(OBJ_FT3 *obj);
static void add_OBJ_FT3(MATRIX *world, u_long *ot, OBJ_FT3 *obj, int id);
static void loadTIM(u_long *addr);
static void setSTP(u_long *col, int n);
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj);

main()
{
	MATRIX	world;			/* ワールドマトリクス */
	OBJ_FT3	obj[MAX_OBJ];		/* オブジェクト */
	u_long	ot[2][OTSIZE];		/* ＯＴ バッファ*/
	int	nobj = 1;		/* オブジェクトの個数 */
	int	id   = 0;		/* パケット ID */
	int	i, n; 			/* work */
	
	db_init(640, 480, SCR_Z, 60, 120, 120);	/* ダブルバッファの初期化 */
	loadTIM(TEXADDR);		/* TIM をフレームバッファに転送する */

	/* TMD をばらばらにして読み込む */
	for (i = 0; i < MAX_OBJ; i++) 
		loadTMD_FT3(MODELADDR, &obj[i]);	
	
	set_position(obj);		/* オブジェクト位置をレイアウト */
	SetDispMask(1);			/* 表示開始 */
	
	/* メインループ */
	while ((nobj = pad_read(obj, nobj, &world)) != -1) {
		
		/* パケットバッファ IDをスワップ */
		id = id? 0: 1;
		
		/* ＯＴをクリア */
		ClearOTagR(ot[id], OTSIZE);			

		/* プリミティブの設定 */
		for (n = i = 0; i < nobj; i++) 
			add_OBJ_FT3(&world, ot[id], &obj[i], id);
		
		/* デバッグストリングの設定 */
		FntPrint("polygon=%d\n", obj[0].n);
		FntPrint("objects=%d\n", nobj);
		FntPrint("total  =%d\n", obj[0].n*nobj);
		
		/* ダブルバッファのスワップとＯＴ描画 */
		db_swap(&ot[id][OTSIZE-1]);
	}
	StopCallback();
	PadStop();
	return;
}


/*
 * パッドの読み込み
 * 回転の中心位置と回転角度を読み込む
 * 	
 */	
static int pad_read(OBJ_FT3 *obj, int nobj, MATRIX *m)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*2};	
	static int	scale = ONE;
	static int	opadd = 0;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADk)	return(-1);
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;
	
	if ((opadd==0) && (padd & PADLup))	set_position(&obj[nobj++]);
	if ((opadd==0) && (padd & PADLdown))	nobj--;
	
	limitRange(nobj, 1, MAX_OBJ);
	opadd = padd;
	
	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, m);	
	TransMatrix(m, &vec);	
	ScaleMatrix(m, &svec);
	
	SetRotMatrix(m);
	SetTransMatrix(m);

	return(nobj);
}

/*
 * オブジェクトのレイアウト
 * ここでは乱数を使用して適当に配置している	
 * そのため、二つのオブジェクトが同じ位置にぶつかることもある。	
 */	
#define DIST	SCR_Z		/* 乱数の分散 */
#define UNIT	256		/* 最小解像度 */

static void set_position(OBJ_FT3 *obj)
{
	SVECTOR	ang;
		
	ang.vx = rand()%4096;
	ang.vy = rand()%4096;
	ang.vz = rand()%4096;
	RotMatrix(&ang, &obj->m);	/* ローカルマトリクス */
		
	obj->v.vx = (rand()/UNIT*UNIT%DIST)-DIST/2;
	obj->v.vy = (rand()/UNIT*UNIT%DIST)-DIST/2;
	obj->v.vz = (rand()/UNIT*UNIT%DIST)-DIST/2;

}
/*
 * オブジェクトの登録
 */
static void add_OBJ_FT3(MATRIX *world, u_long *ot, OBJ_FT3 *obj, int id)
{
	MATRIX		screen;			/* スクリーンマトリクス */
	VERT_F3		*vp;			/* work */
	POLY_FT3	*pp;			/* work */
	int		i;
	long		flg;
	
	vp = obj->vert;
	pp = obj->prim[id];
		
	RotTrans(&obj->v, (VECTOR *)screen.t, &flg);	/* 並行移動 */
	PushMatrix();				/* カレントマトリクスを退避 */
	MulMatrix0(world, &obj->m, &screen);	/* 回転（順番注意） */
	SetRotMatrix(&screen);			/* 回転マトリクスの設定 */
	SetTransMatrix(&screen);		/* 移動ベクトルの設定 */
	
	/* 回転・移動・透視変換 */
	for (i = 0; i < obj->n; i++, vp++, pp++) {
		
		/* ３Ｄ表示の根幹は、この行に集約される */
		rotTransPers3(ot, OTSIZE, pp,
			      &vp->v0, &vp->v1, &vp->v2);
	}
	
	/* マトリクスを元にもどしてリターン */
	PopMatrix();
}


/*
 * TIM をロードする
 */	
static void loadTIM(u_long *addr)
{
	TIM_IMAGE	image;		/* TIM ヘッダ */
	
	OpenTIM(addr);			/* TIM をオープン */
	while (ReadTIM(&image)) {
		if (image.caddr) {	/* CLUT（もしあれば）をロード */
			setSTP(image.caddr, image.crect->w);
			LoadImage(image.crect, image.caddr);
		}
		if (image.paddr) 	/* パターンをロード */
			LoadImage(image.prect, image.paddr);
	}
}
	
/*
 * STP bit を 1 にする
 */	
static void setSTP(u_long *col, int n)
{
	n /= 2;  
	while (n--) 
		*col++ |= 0x80008000;
}

/*
 * TMD の解析
 */
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj)
{
	VERT_F3		*vert;
	POLY_FT3	*prim0, *prim1;
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	vert  = obj->vert;
	prim0 = obj->prim[0];
	prim1 = obj->prim[1];
	
	/* TMD のオープン */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY) 
		n_prim = MAX_POLY;
	
	/*
	 * プリミティブバッファのうち、書き換えないものを
	 * あらかじめ設定しておく
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {

		/* プリミティブの初期化 */
		SetPolyFT3(prim0);

		/* 法線と頂点ベクトルをコピーする */
		copyVector(&vert->n0, &tmdprim.n0);
		copyVector(&vert->v0, &tmdprim.x0);
		copyVector(&vert->v1, &tmdprim.x1);
		copyVector(&vert->v2, &tmdprim.x2);
		
		/* 光源計算（最初の一回のみ）*/
		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(prim0, col, col, col);
		
		/* テクスチャ座標をコピー */
		setUV3(prim0,
		       tmdprim.u0, tmdprim.v0,
		       tmdprim.u1, tmdprim.v1,
		       tmdprim.u2, tmdprim.v2);
		
		/* テクスチャページ／テクスチャ CLUT ID をコピー */
		prim0->tpage = tmdprim.tpage;
		prim0->clut  = tmdprim.clut;

		/* ダブルバッファを使用するのでプリミティブは２組必要 */
		memcpy(prim1, prim0, sizeof(POLY_FT3));
		
		vert++, prim0++, prim1++;
	}
	return(obj->n = i);
}
