/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto0
 *			
 *	   TMD ビューアプロトタイプ（光源計算なし  FT3 型）
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	1.00		Mar,15,1994	suzu
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <inline.h>
#include <gtemac.h>
#include "rtp.h"

#define SCR_Z		1024		/* 投影面までの距離 */
#define OTSIZE		8192		/* ＯＴの段数（２のべき乗） */
#define MAX_POLY	3000		/* 表示ポリゴンの最大値 */

#define MODELADDR	((u_long *)0x80100000)	/* TMD のアドレス */

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
 */
typedef struct {
	int		n;			/* ポリゴン数 */
	VERT_F3		vert[MAX_POLY];		/* 頂点バッファ （１個）*/
	POLY_F3		prim[2][MAX_POLY];	/* パケットバッファ（２個）*/
} OBJ_F3;

static int pad_read(int nprim);
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj);

main()
{
	static OBJ_F3	obj;			/* オブジェクト */
	static u_long	otbuf[2][OTSIZE];	/* ＯＴ バッファ*/
	u_long		*ot;			/* カレントＯＴ */
	int		id = 0;			/* パケット ID */
	VERT_F3		*vp;			/* work */
	POLY_F3		*pp;			/* work */
	int		nprim;			/* work */
	int		i; 			/* work */
	/*long		otz, flg, clp;*/

	db_init(640, 480, SCR_Z, 60, 120, 120);	/* ダブルバッファの初期化 */
	loadTMD_F3(MODELADDR, &obj);		/* TMD を読み込む */
	SetDispMask(1);				/* 表示開始 */

	/* メインループ */
	nprim = obj.n;
	while ((nprim = pad_read(nprim)) != -1) {
		
		/* [0,max_nprim] で nprim をクリップ */
		limitRange(nprim, 0, obj.n);

		/* パケットバッファ IDをスワップ */
		id = id? 0: 1;
		ot = otbuf[id];
		
		/* ＯＴをクリア */
		ClearOTagR(ot, OTSIZE);			

		/* プリミティブの設定 */
		vp = obj.vert;
		pp = obj.prim[id];
		
		for (i = 0; i < nprim; i++, vp++, pp++) {
	
			/* ３Ｄ表示の根幹は、この２行に集約される */
			pp = &obj.prim[id][i];
			rotTransPers3(ot, OTSIZE, pp,
				      &vp->v0, &vp->v1, &vp->v2);

		}
		/* デバッグストリングの設定 */
		FntPrint("total=%d\n", i);
		
		/* ダブルバッファのスワップとＯＴ描画 */
		db_swap(ot+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static int pad_read(int nprim)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE/4;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADselect)	return(-1);
	if (padd & PADLup) 	nprim += 4;
	if (padd & PADLdown)	nprim -= 4;
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;

	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	return(nprim);
}		

/*
 * TMD の解析
 */
int loadTMD_F3(u_long *tmd, OBJ_F3 *obj)
{
	VERT_F3		*vert;
	POLY_F3		*prim0, *prim1;
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
		SetPolyF3(prim0);

		/* 法線と頂点ベクトルをコピーする */
		copyVector(&vert->n0, &tmdprim.n0);
		copyVector(&vert->v0, &tmdprim.x0);
		copyVector(&vert->v1, &tmdprim.x1);
		copyVector(&vert->v2, &tmdprim.x2);
		
		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(prim0, col, col, col);
		
		/* ダブルバッファを使用するのでプリミティブは２組必要 */
		memcpy(prim1, prim0, sizeof(POLY_F3));
		vert++, prim0++, prim1++;
	}
	return(obj->n = i);
}

