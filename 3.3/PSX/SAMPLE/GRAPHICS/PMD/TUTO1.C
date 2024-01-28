/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto1
 *			
 *	   PMD-TMD ビューアプロトタイプ（光源計算なし  F3 型）
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	2.00		Jul,14,1994	suzu	(using PMD)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z		1024		/* 投影面までの距離 */
#define OTSIZE		8192		/* ＯＴの段数（２のべき乗） */
#define OTLENGTH	12		/* ＯＴの段数 */
#define MAX_POLY	3000		/* 表示ポリゴンの最大値 */

#define MODELADDR	((u_long *)0x80100000)	/* TMD のアドレス */

/*
 * ３角形ポリゴンパケット
 * 頂点・パケットバッファは、malloc() で動的に割り付けるべきです。	
 */
typedef struct {
	int	n;
	struct {			
		POLY_F3	prim[2];
		SVECTOR v0, v1, v2;
	} p[MAX_POLY];
} OBJ_F3;

static int pad_read(int nprim);
static int loadTMD_F3(u_long *tmd, OBJ_F3 *obj);
main()
{
	static OBJ_F3	obj;			/* オブジェクト */
	static u_long	otbuf[2][OTSIZE];	/* ＯＴ バッファ*/
	int		id = 0;			/* パケット ID */
	int		i, nprim;		/* work */
	
	db_init(640, 480, SCR_Z, 60, 120, 120);	/* ダブルバッファの初期化 */
	nprim = loadTMD_F3(MODELADDR, &obj);	/* TMD を読み込む */
	SetDispMask(1);				/* 表示開始 */

	/* メインループ */
	while ((obj.n = pad_read(obj.n)) != -1) {
		
		/* [0,max_nprim] で nprim をクリップ */
		limitRange(obj.n, 0, nprim);

		/* パケットバッファ IDをスワップ */
		id = id? 0: 1;
		
		/* ＯＴをクリア */
		ClearOTagR(otbuf[id], OTSIZE);			

		/* プリミティブの設定 */
		RotPMD_F3((long *)&obj, otbuf[id], OTLENGTH, id, 0);
		
		/* デバッグストリングの設定 */
		FntPrint("total=%d\n", obj.n);
		
		/* ダブルバッファのスワップとＯＴ描画 */
		db_swap(otbuf[id]+OTSIZE-1);
	}
	PadStop();
	StopCallback();
	return;
}

static int pad_read(int nprim)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*8};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE;
	
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
	if (padd & PADLup)	nprim  += 32;
	if (padd & PADLdown)	nprim  -= 32;

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
static int loadTMD_F3(u_long *tmd, OBJ_F3 *obj)
{
	TMD_PRIM	tmdprim;
	int		col, i, n_prim = 0;

	/* TMD のオープン */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY)
		n_prim = MAX_POLY;

	/*
	 * プリミティブバッファのうち、書き換えないものを
	 * あらかじめ設定しておく
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {
		
		/* プリミティブの初期化 */
		SetPolyF3(&obj->p[i].prim[0]);
		
		/* 法線と頂点ベクトルをコピーする */
		copyVector(&obj->p[i].v0, &tmdprim.x0);
		copyVector(&obj->p[i].v1, &tmdprim.x1);
		copyVector(&obj->p[i].v2, &tmdprim.x2);

		col = (tmdprim.n0.vx+tmdprim.n0.vy)*128/ONE/2+128;
		setRGB0(&obj->p[i].prim[0], col, col, col);
		
		/* ダブルバッファを使用するのでプリミティブは２組必要 */
		memcpy(&obj->p[i].prim[1],
		       &obj->p[i].prim[0], sizeof(POLY_F3));
	}
	return(obj->n = i);
}
