/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			    tuto0
 *			
 *	        PMD ビューアプロトタイプ (関数テーブル型)
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	----------------------------------------------------	
 *	1.00		Jul, 7,1994	oka
 *	2.00		Jul,21,1994	suzu	
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>	

#define TEX_ADDR	0x801c0000	
#define PMD_ADDR	0x801a0000	
#define OTLENGTH	8		
#define OTSIZE		(1<<OTLENGTH)
#define SCR_Z		1024

static int pad_read(void);
static void RotPMD(u_long *ot, int id);
static void loadTIM(u_long *addr);

main()
{
	u_long	otbuf[2][1<<OTLENGTH]; 	/* オーダリングテーブル実体 */
	int     id;			/* ダブルバッファ ID */

	db_init(640, 480, SCR_Z, 60, 120, 120);	/* ダブルバッファ初期化 */
	loadTIM((u_long *)TEX_ADDR);		/* テクスチャロード */
	SetDispMask(1);				/* 表示開始 */

	while (pad_read() == 0) {
		id = id? 0: 1;			/* バッファスワップ */
		ClearOTagR(otbuf[id], OTSIZE);	/* ＯＴクリア */
		RotPMD(otbuf[id], id);		/* PMD を登録 */
		db_swap(otbuf[id]+OTSIZE-1);	/* ＯＴ描画 */
	}
	PadStop();				/* 終了 */
	StopCallback();
	return(0);
}

/*
 * GTE マトリクスを設定する
 */
static int pad_read(void)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*5};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE;
	
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

	ang.vz += 4;
	ang.vy += 4;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	return(0);
}		

/*
 * TIM をロードする
 */	
static void loadTIM(u_long *addr)
{
	TIM_IMAGE	image;		/* TIM ヘッダ */
	
	OpenTIM(addr);			/* TIM をオープン */
	while (ReadTIM(&image)) {
		if (image.caddr)	/* CLUT（もしあれば）をロード */
			LoadImage(image.crect, image.caddr);
		if (image.paddr) 	/* パターンをロード */
			LoadImage(image.prect, image.paddr);
	}
}


/*
 * PMD 描画関数
 *	プリミティブの型と共有頂点／独立頂点に応じて１６の関数が
 *	ある。
 */	
	
/* independent vertex type (8 type) */
static void (*PMD_func[])() = {
	RotPMD_FT3,	RotPMD_FT4,	RotPMD_GT3,	RotPMD_GT4,
	RotPMD_F3,	RotPMD_F4,	RotPMD_G3,	RotPMD_G4,
};
/* shared vertex type (8 type) */
static void (*PMD_SV_func[])() = {
	RotPMD_SV_FT3,	RotPMD_SV_FT4,	RotPMD_SV_GT3,	RotPMD_SV_GT4,
	RotPMD_SV_F3,	RotPMD_SV_F4,	RotPMD_SV_G3,	RotPMD_SV_G4,
};

/*
 * PMD を描画する
 */	
static void RotPMD(u_long *ot, int id)
{
	int     i,j;
	long	*PMDTOP;	/*PMD file PRIMITIVE Gp top address*/
	long	*PMDWC;		/*PMD file word counter*/
	long	POINTER;	/*PMD file POINTER*/
	long	*PTOP;		/*PMD file PRIMITIVE Gp Block top address*/
	long	ID;		/*PMD file ID*/
	long	*PRIMTOP;	/*PMD file PRIMITIVE Gp top address*/
	long	NOBJ;		/*PMD file NOBJ*/
	long	NPTR;		/*PMD file NPTR*/
	long	TYPE_NPACKET;	/*PMD file PRIMITIVE Gp header*/
	short	TYPE;		/*PMD file PRIMITIVE Gp type*/
	short	type;		/*PMD file PRIMITIVE Gp local type*/
	long	*SVTOP;		/*PMD file Vertex top address*/
	long	BACKC;		/*PMD file Back clip ON/OFF flag*/

	PMDWC = PMDTOP = (long *)PMD_ADDR;

	ID    = *PMDWC;			PMDWC++;
	PTOP  = PMDTOP + ((*PMDWC)>>2);	PMDWC++;
	SVTOP = PMDTOP + ((*PMDWC)>>2);	PMDWC++;
	NOBJ= *PMDWC;			PMDWC++;

	for(i = 0;i < NOBJ; i++) {
		NPTR= *PMDWC;
		PMDWC++;
		for(j = 0;j < NPTR; j++){
			POINTER= *PMDWC;
			PRIMTOP= PMDTOP+(POINTER>>2);
			TYPE_NPACKET= *(PRIMTOP);	
			TYPE= TYPE_NPACKET>>16;
			type= TYPE&0x000f;
			BACKC= (TYPE&0x0020)>>5;
			if (type < 0x8)
				(*PMD_func[type])(PRIMTOP,
						     ot, OTLENGTH, id, BACKC);
			else if (type < 0x10)
				(*PMD_func[type])(PRIMTOP, SVTOP,
						     ot, OTLENGTH, id, BACKC);
			PMDWC++;
			}
		}
}






