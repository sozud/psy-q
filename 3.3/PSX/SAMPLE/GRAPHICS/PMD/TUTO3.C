/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*				tuto3
 *			
 *	   TMD-PMD ビューアプロトタイプ（光源計算あり  FT3 型）
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------	
 *	1.00		Mar,15,1994	suzu
 *	2.00		Jul,14,1994	suzu	(using PMD)
 *	2.10		Jul,14,1994	suzu	(with lighting)
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define SCR_Z		1024		/* 投影面までの距離 */
#define OTLENGTH	12		/* ＯＴの段数 */
#define OTSIZE		(1<<OTLENGTH)	
#define MAX_POLY	3000		/* 表示ポリゴンの最大値 */
#define MODELADDR	(0x80120000)	/* TMD の格納されているアドレス */
#define TEXADDR		(0x80140000)	/* TIM の格納されているアドレス */

/*
 * ３角形ポリゴンパケット
 * 頂点・パケットバッファは、malloc() で動的に割り付けるべきです。	
 */
typedef struct {
	int	n;
	struct {			
		POLY_FT3	prim[2];
		SVECTOR v0, v1, v2;
	} p[MAX_POLY];
	SVECTOR	n0[MAX_POLY];			/* 法線 */
} OBJ_FT3;

static void loadTIM(u_long *addr);
static void setSTP(u_long *col, int n);
static int loadTMD_FT3(u_long *tmd, OBJ_FT3 *obj);
static int lightOBJ_FT3(OBJ_FT3 *obj, MATRIX *cmat, SVECTOR *lang, int ambi);
static int pad_read(OBJ_FT3 *obj);

main()
{
	static OBJ_FT3	obj;			/* オブジェクト */
	static u_long	otbuf[2][OTSIZE];	/* ＯＴ バッファ*/
	int		id = 0;			/* パケット ID */
	int		i, nprim;
		
	/* ダブルバッファの初期化 */
	db_init(640, 480, SCR_Z, 60, 120, 120);	
	loadTIM((u_long *)TEXADDR);	/* TIM をフレームバッファに転送する */

	/* TMD をばらばらにして読み込む */
	nprim = loadTMD_FT3((u_long *)MODELADDR, &obj);

	/* 表示開始 */
	SetDispMask(1);			

	/* メインループ */
	while (pad_read(&obj) == 0) {
		
		/* パケットバッファ IDをスワップ */
		id = id? 0: 1;
		
		/* ＯＴをクリア */
		ClearOTagR(otbuf[id], OTSIZE);			

		/* プリミティブの設定 */
		RotPMD_FT3((long *)&obj, otbuf[id], OTLENGTH, id, 0);

		/* デバッグストリングの設定 */
		FntPrint("total=%d\n", obj.n);
		
		/* ダブルバッファのスワップとＯＴ描画 */
		db_swap(otbuf[id]+OTSIZE-1);
	}
	PadStop();
	StopCallback();
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
	TMD_PRIM	tmdprim;
	int		i, n_prim = 0;

	/* TMD のオープン */
	if ((n_prim = OpenTMD(tmd, 0)) > MAX_POLY)
		n_prim = MAX_POLY;

	/*
	 * プリミティブバッファのうち、書き換えないものを
	 * あらかじめ設定しておく
	 */	 
	for (i = 0; i < n_prim && ReadTMD(&tmdprim) != 0; i++) {
		
		/* プリミティブの初期化 */
		SetPolyFT3(&obj->p[i].prim[0]);
		
		/* 法線と頂点ベクトルをコピーする */
		copyVector(&obj->p[i].v0, &tmdprim.x0);
		copyVector(&obj->p[i].v1, &tmdprim.x1);
		copyVector(&obj->p[i].v2, &tmdprim.x2);
		copyVector(&obj->n0[i],   &tmdprim.n0);
		
		/* テクスチャ座標をコピー */
		setUV3(&obj->p[i].prim[0], 
		       tmdprim.u0, tmdprim.v0,
		       tmdprim.u1, tmdprim.v1,
		       tmdprim.u2, tmdprim.v2);
		
		/* テクスチャページ／テクスチャ CLUT ID をコピー */
		obj->p[i].prim[0].tpage = tmdprim.tpage;
		obj->p[i].prim[0].clut  = tmdprim.clut;
		
		/* ダブルバッファを使用するのでプリミティブは２組必要 */
		memcpy(&obj->p[i].prim[1],
		       &obj->p[i].prim[0], sizeof(POLY_FT3));
	}
	return(obj->n = i);
}

/*
 * 光源計算
 */
static int lightOBJ_FT3(OBJ_FT3 *obj, MATRIX *cmat, SVECTOR *lang, int ambi)
{
	int	i;	
	MATRIX	lmat;
	P_CODE	csrc, cdst;
	SVECTOR	*n;
	
	/* 光源のマトリクスを計算 */
	SetBackColor(ambi, ambi, ambi);	/* 周辺光 */
	SetColorMatrix(cmat);		/* 光源色 */
	RotMatrix(lang, &lmat);		/* 光源ベクトル */
	SetLightMatrix(&lmat);		
	
	/*setRGB0(&csrc, 128, 128, 128);	/* 素材色は、(128,128,128) */
	setRGB0(&csrc, 255, 255, 255);		/* 素材色は、(255,255,255) */
	csrc.code = obj->p[0].prim[0].code;	/* コードを保存 */

	/* 光源計算 */
	for (i = 0, n = obj->n0; i < obj->n; i++, n++) {
		NormalColorCol(n, (CVECTOR *)&csrc, (CVECTOR *)&cdst);
		*(u_long *)(&obj->p[i].prim[0].r0) = *(u_long *)(&cdst);
		*(u_long *)(&obj->p[i].prim[1].r0) = *(u_long *)(&cdst);
	}
	return(0);
}
	
	       
/*
 * パッドの解析
 */	
static MATRIX	cmat = {			/* 光源色 */
/* 	 #0,	#1,	#2, */
	ONE,	0,	0,	/* R */
	0,	ONE,	0, 	/* G */
	0,	0,	ONE,	/* B */
};
	
static int pad_read(OBJ_FT3 *obj)
{
	static int	light = 1;
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*2};	/* position */
	static SVECTOR	lang  = {ONE, ONE, ONE};	/* light angle */
	static MATRIX	m;				/* matrix */
	static int	scale = 2*ONE;			/* scale */
	static int	frame = 0;
	
	VECTOR	svec;
	int 	padd = PadRead(1);

	if (padd & PADselect)	return(-1);
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  += ONE/256;
	if (padd & PADR2)	scale  -= ONE/256;
	
	if (frame%2 == 0 && (padd & (PADLup|PADLdown|PADLleft|PADLright)))
		light = 1;
	    
	if (padd & PADLup)	lang.vx += 32;
	if (padd & PADLdown)	lang.vx -= 32;
	if (padd & PADLleft) 	lang.vy += 32;
	if (padd & PADLright)	lang.vy -= 32;
	
	ang.vz += 8;
	ang.vy += 8;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	/* 光源計算をする（最初の一回とパラメータが変更がされたとき）*/
	/* 遅くなるので、毎回計算するべきではない */
	if (light) {
		light = 0;
		lightOBJ_FT3(obj, &cmat, &lang, 0);
	}

	/* リターン */
	frame++;
	return(0);
}		


