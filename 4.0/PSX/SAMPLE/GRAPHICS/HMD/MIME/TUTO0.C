/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="graphics/hmd/mime/hmime.cpe";
#endif
/*
 *	"tuto0.c" HMD viewer with MIMe
 *
 *		Copyright (C) 1997  Sony Computer Entertainment
 *		All rights Reserved
 */

/*#define DEBUG		/**/

/*
#define INTER		/* Interlace mode */

#include <sys/types.h>

#include <libetc.h>		/* for Control Pad :
				   PADを使うためにインクルードする必要あり */
#include <libgte.h>		/* LIBGS uses libgte :
				   LIGSを使うためにインクルードする必要あり*/
#include <libgpu.h>		/* LIBGS uses libgpu :
				   LIGSを使うためにインクルードする必要あり */
#include <libgs.h>		/* for LIBGS :
				   グラフィックライブラリ を使うための
				   構造体などが定義されている */
#include "control.h"		/* ctlfc 計算用ルーチン */

#define MIMENUM 8
static u_long mimepr[MIMENUM];	/* MIMe 重み係数 */
static int mimenum=0;

static int cnv0[24] = {		/* ctlfc 計算で使う伝達関数 */
    -40, -50, 0,
    200, 325, 450, 475, 500, 475, 450, 375,
    300, 250, 200, 150, 100,  80,  55,   0,
    -20, -50, -40, -25,
};



#define OBJECTMAX 100		/* Max Objects :
                                   ３Dのモデルは論理的なオブジェクトに
                                   分けられるこの最大数 を定義する */

#define MODEL_ADDR ((u_long *)0x80010000)	/* Top Address of modeling data (TMD FORMAT) :
				   モデリングデータ（TMDフォーマット）
				   がおかれるアドレス */

#define OT_LENGTH 10		/* Area of OT : オーダリングテーブルの解像度 */

#define PACKETMAX (10000*24)


GsOT            Wot[2];		/* Handler of OT
                                   Uses 2 Hander for Dowble buffer :
				   オーダリングテーブルハンドラ
				   ダブルバッファのため２つ必要 */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH];
				/* Area of OT : オーダリングテーブル実体 */

GsUNIT		object[OBJECTMAX];
				/* Array of Object Handler :
				   オブジェクトハンドラ
				   オブジェクトの数だけ必要 */

u_long		bnum;		/* valibable of number of Objects :
				   モデリングデータのオブジェクトの数を
				   保持する */

GsCOORDUNIT	*DModel;	/* Coordinate for GsUNIT : 
				   オブジェクトごとの座標系 */

/* work short vector for making Coordinate parmeter :
   オブジェクトごとの座標系を作るための元データ */

GsRVIEW2	view;		/* View Point Handler :
				   視点を設定するための構造体 */
GsF_LIGHT	pslt[3];	/* Flat Lighting Handler :
				   平行光源を設定するための構造体 */
u_long		padd;		/* Controler data :
				   コントローラのデータを保持する */

PACKET          out_packet[2][PACKETMAX];
				/* GPU PACKETS AREA */

/* 
 * prototype
 */
void init_all(void);
int  obj_interactive(void);
void mime_interactive(u_long padd);
void set_coordinate(GsCOORDUNIT *coor);
void model_init(void);
void view_init(void);
void light_init(void);
void set_mimepr(GsMIMePrimHeader *mph, long *pr, int num);

/************* MAIN START ******************************************/
main()
{
	GsUNIT	*op;		/* pointer of Object Handler :
				   オブジェクトハンドラへのポインタ */
	int	outbuf_idx;
	MATRIX	tmpls;
	int	i;
	
	ResetCallback();
	init_all();

	FntLoad(960, 256);
#ifdef INTER
	FntOpen(-300, -200, 256, 200, 0, 512); 
#else /* INTER */
	FntOpen(-150, -100, 256, 200, 0, 512); 
#endif /* INTER */

#ifndef APD_LOAD
	GsInitVcount();
#endif
	while(1) {
		/* interactive parameter get :
		   パッドデータから動きのパラメータを入れる */
		if(obj_interactive() == 0) {
#ifndef APD_LOAD
			PadStop();
#endif
#if defined (APD_SAVE)
			APDSaveStop();
#elif defined(APD_LOAD)
			APDLoadStop(VSync(-1));
#endif
			ResetGraph(3);
			StopCallback();
			return 0;
		}
		GsSetRefView2(&view);	/* Calculate World-Screen Matrix :
					   ワールドスクリーンマトリックス計算 */
		outbuf_idx=GsGetActiveBuff();
					/* Get double buffer index :
					   ダブルバッファのどちらかを得る */
		GsSetWorkBase((PACKET*)out_packet[outbuf_idx]);
			
		GsClearOt(0, 0, &Wot[outbuf_idx]);
					/* Clear OT for using buffer :
					   オーダリングテーブルをクリアする */
			
		for(i=0,op=object; i<bnum; i++,op++) {
			if (op->primtop == NULL)
				continue;
			if (op->coord) {
				/* Calculate Local-World Matrix :
				   スクリーン／ローカルマトリックスを計算する */
				GsGetLwUnit(op->coord, &tmpls);
	
				/* Set LWMATRIX to GTE Lighting Registers :
				   ライトマトリックスをGTEにセットする */
				GsSetLightMatrix(&tmpls);
	
				/* Set LSAMTRIX to GTE Registers :
				   スクリーン／ローカルマトリックスをGTEに
				   セットする */
				GsGetLsUnit(op->coord, &tmpls);
	
				/* Set LSMATRIX to GTE Lighting Registers :
				   ライトマトリックスをGTEにセットする */
				GsSetLsMatrix(&tmpls);
			}

			/* Perspective Translate Object and Set OT :
			   オブジェクトを透視変換しオーダリングテーブルに
			   登録する */
			GsSortUnit(op, &Wot[outbuf_idx], getScratchAddr(0));
		}
			
#if defined(APD_SAVE)
		padd = APDSave(PadRead(1));
#elif defined(APD_LOAD)
		padd = APDLoad(1);
#else
		padd = PadRead(0);	/* Readint Control Pad data :
					   パッドのデータを読み込む */
#endif
#ifndef INTER
		DrawSync(0);
#endif /* !INTER */
#ifdef APD_LOAD
		APDSetCnt(VSync(0));
#else
		VSync(0);		/* Wait for VSYNC : Vブランクを待つ */
		GsClearVcount();
#endif
#ifdef INTER
		ResetGraph(1);		/* Reset GPU */
#endif /* INTER */
		GsSwapDispBuff();	/* Swap double buffer :
					   ダブルバッファを切替える */
		/* Set SCREEN CLESR PACKET to top of OT :
	 	   画面のクリアをオーダリングテーブルの最初に登録する */
		GsSortClear(0x0, 0x0, 0x0, &Wot[outbuf_idx]);

		/* Drawing OT :
	 	   オーダリングテーブルに登録されているパケットの描画を
		   開始する */
		GsDrawOt(&Wot[outbuf_idx]);

		FntFlush(-1);
	}
}

/* convert pad data to mimepr */
void mime_interactive(u_long padd)
{
    int i;
    static u_long fn=0;

    for (i=0; i<mimenum; i++){
	ctlfc[i].in=0;
    }

    if (padd & PADL1) ctlfc[0].in+=4096;
    if (padd & PADL2) ctlfc[1].in+=4096;
    if (padd & PADR1) ctlfc[2].in+=4096;
    if (padd & PADR2) ctlfc[3].in+=4096;
    if (padd & PADLleft) ctlfc[4].in+=4096;
    if (padd & PADLright) ctlfc[5].in+=4096;
    if (padd & PADLup) ctlfc[6].in+=4096;
    if (padd & PADLdown) ctlfc[7].in+=4096;

    set_cntrl(fn++);

    for (i=0; i<mimenum; i++){
	mimepr[i]=ctlfc[i].out;
	FntPrint("mime [%d]:%4d\n",i,mimepr[i]);
    }

    return;
}

/* Initialize for HMD MIMe  */
void set_mimepr(GsMIMePrimHeader *mph, long *pr, int num)
{
    if (mph->mapped==0 /* mimepr にアドレスがまだ入っていない場合 mapped==0 */
	&& mph->internal==0){/*mimeprをプログラム側で用意する場合 internal==0 */

	/* 必要に応じて mph->mimeid もチェックする(ここではチェックなし) */

	mph->mimepr=pr;		/* mimepr の場所指定 */
	mph->mapped=1;		/* miempr をマップしたのでフラグを立てる */

	/* これはサンプルなので、一応 mimepr 領域の長さをチェック */
	if (mph->mimenum>num) printf("too short mimepr area!\n");

	if (mph->mimenum>mimenum) mimenum=mph->mimenum;
    }
}


int obj_interactive()
{
	static int num = 0;
	static u_long opadd = 0;

		
	if (padd & PADRleft)	DModel[num].rot.vy += 5*ONE/360;
	if (padd & PADRright)	DModel[num].rot.vy -= 5*ONE/360;
	if (padd & PADRup)	DModel[num].rot.vx += 5*ONE/360;
	if (padd & PADRdown)	DModel[num].rot.vx -= 5*ONE/360;
	
	
#if 0
	if (padd & PADLleft)	DModel[num].matrix.t[0] -= 10;
	if (padd & PADLright)	DModel[num].matrix.t[0] += 10;

	if (padd & PADLdown)	DModel[num].matrix.t[1] += 10;
	if (padd & PADLup)	DModel[num].matrix.t[1] -= 10;
#endif /* 0 */

	if (padd & PADstart)	DModel[num].matrix.t[2] += 100;
	if (padd & PADselect)	DModel[num].matrix.t[2] -= 100;

	if ((padd & PADselect) && (padd & PADstart)) {
		return(0);
	}
	
	set_coordinate(&DModel[num]);

	mime_interactive(padd);

	opadd = padd;
	return(1);
}

void init_all(void)		/* Initialize routine : 初期化ルーチン群 */
{
	ResetGraph(0);		/* reset GPU */
#if defined (APD_SAVE)
	APDSaveInit(0,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0,progname) ) return;
#endif
#ifndef APD_LOAD
	PadInit(0);		/* Reset Controller : コントローラ初期化 */
#endif
	padd = 0;		/* controller value initialize :
				   コントローラ値初期化 */
#ifdef INTER
	GsInitGraph(640, 480, 5, 1, 0);
				/* rezolution set , interrace mode :
				   解像度設定（インターレースモード） */
	GsDefDispBuff(0, 0, 0, 0);
				/* Double buffer setting :
				   ダブルバッファ指定 */
#else /* INTER */
	GsInitGraph(320, 240, 4, 1, 0);
				/* rezolution set , non interrace mode :
				   解像度設定（ノンインターレースモード） */
	GsDefDispBuff(0, 0, 0, 240);
				/* Double buffer setting :
				   ダブルバッファ指定 */
#endif /* INTER */

	GsInit3D();		/* Init 3D system : ３Dシステム初期化 */
	
	Wot[0].length = OT_LENGTH;/* Set bit length of OT handler :
				   オーダリングテーブルハンドラに解像度設定 */
	Wot[0].org = zsorttable[0];
				/* Set Top address of OT Area to OT handler :
				   オーダリングテーブルハンドラに
				   オーダリングテーブルの実体設定 */
	/* same setting for anoter OT handler :
	   ダブルバッファのためもう一方にも同じ設定 */
	Wot[1].length = OT_LENGTH;
	Wot[1].org = zsorttable[1];

	model_init();		/* Reading modeling data :
				   モデリングデータ読み込み */
	view_init();		/* Setting view point : 視点設定 */
	light_init();		/* Setting Flat Light : 平行光源設定 */

	init_cntrlarry(cnv0,24);
}

void view_init(void)		/* Setting view point : 視点設定 */
{
	/*---- Set projection,view ----*/
	GsSetProjection(1000);	/* Set projection : プロジェクション設定 */
	
	/* Setting view point location : 視点パラメータ設定 */
	view.vpx = 0; view.vpy = 0; view.vpz = -2000;

	/* Setting focus point location : 注視点パラメータ設定 */
	view.vrx = 0; view.vry = 0; view.vrz = 0;

	/* Setting bank of SCREEN : 視点の捻りパラメータ設定 */
	view.rz=0;

	/* Setting parent of viewing coordinate : 視点座標パラメータ設定 */	
	view.super = WORLD;

	/* Calculate World-Screen Matrix from viewing paramter :	
	   視点パラメータを群から視点を設定する
	   ワールドスクリーンマトリックスを計算する */
	GsSetRefView2(&view);
}


void light_init(void)		/* init Flat light : 平行光源設定 */
{
	/* Setting Light ID 0 : ライトID０ 設定 */	
	/* Setting direction vector of Light0 : 平行光源方向パラメータ設定 */
	pslt[0].vx = 100; pslt[0].vy = 100; pslt[0].vz = 100;

	/* Setting color of Light0 : 平行光源色パラメータ設定 */
	pslt[0].r = 0xd0; pslt[0].g = 0xd0; pslt[0].b = 0xd0;

	/* Set Light0 from parameters : 光源パラメータから光源設定 */
	GsSetFlatLight(0, &pslt[0]);
	
	/* Setting Light ID 1 : ライトID１ 設定 */
	pslt[1].vx = 20; pslt[1].vy = -50; pslt[1].vz = -100;
	pslt[1].r = 0x80; pslt[1].g = 0x80; pslt[1].b = 0x80;
	GsSetFlatLight(1, &pslt[1]);
	
	/* Setting Light ID 2 : ライトID２ 設定 */
	pslt[2].vx = -20; pslt[2].vy = 20; pslt[2].vz = 100;
	pslt[2].r = 0x60; pslt[2].g = 0x60; pslt[2].b = 0x60;
	GsSetFlatLight(2, &pslt[2]);
	
	/* Setting Ambient : アンビエント設定 */
	GsSetAmbient(0, 0, 0);
	
	/* Setting default light mode : 光源計算のデフォルトの方式設定 */	
	GsSetLightMode(0);
}

/* Set coordinte parameter from work vector :
   マトリックス計算ワークからマトリックスを作成し座標系にセットする */
void set_coordinate(GsCOORDUNIT *coor)
{
	/* Rotate Matrix :
	   マトリックスにローテーションベクタを作用させる */
	RotMatrix(&coor->rot, &coor->matrix);

	/* Clear flag becase of changing parameter :
	   マトリックスキャッシュをフラッシュする */
	coor->flg = 0;
}

/* Read modeling data (TMD FORMAT) : モデリングデータの読み込み */
void model_init(void)
{
	u_long	*dop;
	GsUNIT	*objp;		/* handler of object :
				   モデリングデータハンドラ */
	int	i;
	u_long	*oppp;
	GsMIMePrimHeader *mph;	/* GsMapMIMe の返り値型 */

	dop = MODEL_ADDR;
				/* Top Address of MODELING DATA(TMD FORMAT) :
				   モデリングデータが格納されているアドレス */
	GsMapUnit(dop);		/* Mappipng real address :
				   モデリングデータ（TMDフォーマット）を
				   実アドレスにマップする */
	dop++;			/* ID skip */
	dop++;			/* flag skip */
	dop++;			/* headder top skip */
	bnum = *dop;		/* Get number of Blocks :
				   ブロック数を HMD のヘッダから得る */
	dop++;			/* skip block number */

	for(i=0,objp=object; i<bnum; i++,dop++,objp++) {
		GsTYPEUNIT	ut;

		objp->primtop = (u_long *)*dop;
		if (objp->primtop == NULL)
			continue;
		GsScanUnit(objp->primtop, 0, 0, 0);
		while(GsScanUnit(0, &ut, &Wot[0], getScratchAddr(0))) {
			if (((ut.type>>24 == 0x00) || (ut.type>>24 == 0x01)) &&
			    (ut.type & 0x00800000)) {
				DModel = GsMapCoordUnit(MODEL_ADDR, ut.ptr);
				ut.type &= (~0x00800000);
			}
			switch(ut.type) {
			  case 0:
#ifdef DEBUG
			    printf("DEBUG:pointer is already set\n");
#endif /* DEBUG */
			    break;
			  case GsUF3:
			    *(ut.ptr) = (u_long)GsU_00000008;
			    break;
			  case GsUFT3: 
			    *(ut.ptr) = (u_long)GsU_00000009;
			    break;
			  case GsUG3:
			    *(ut.ptr) = (u_long)GsU_0000000c;
			    break;
			  case GsUGT3:
			    *(ut.ptr) = (u_long)GsU_0000000d;
			    break;
			  case GsUF4:
			    *(ut.ptr) = (u_long)GsU_00000010;
			    break;
			  case GsUFT4:
			    *(ut.ptr) = (u_long)GsU_00000011;
			    break;
			  case GsUG4:
			    *(ut.ptr) = (u_long)GsU_00000014;
			    break;
			  case GsUGT4:
			    *(ut.ptr) = (u_long)GsU_00000015;
			    break;
			  case GsUFF3:
			    *(ut.ptr) = (u_long)GsU_00020008;
			    break;
			  case GsUFFT3: 
			    *(ut.ptr) = (u_long)GsU_00020009;
			    break;
			  case GsUFG3:
			    *(ut.ptr) = (u_long)GsU_0002000c;
			    break;
			  case GsUFGT3:
			    *(ut.ptr) = (u_long)GsU_0002000d;
			    break;
			  case GsUFF4:
			    *(ut.ptr) = (u_long)GsU_00020010;
			    break;
			  case GsUFFT4:
			    *(ut.ptr) = (u_long)GsU_00020011;
			    break;
			  case GsUFG4:
			    *(ut.ptr) = (u_long)GsU_00020014;
			    break;
			  case GsUFGT4:
			    *(ut.ptr) = (u_long)GsU_00020015;
			    break;
			  case GsUCF3:
			    *(ut.ptr) = (u_long)GsU_0000000a;
			    break;
			  case GsUCFT3: 
			    *(ut.ptr) = (u_long)GsU_0000000b;
			    break;
			  case GsUCG3:
			    *(ut.ptr) = (u_long)GsU_0000000e;
			    break;
			  case GsUCGT3:
			    *(ut.ptr) = (u_long)GsU_0000000f;
			    break;
			  case GsUCF4:
			    *(ut.ptr) = (u_long)GsU_00000012;
			    break;
			  case GsUCFT4:
			    *(ut.ptr) = (u_long)GsU_00000013;
			    break;
			  case GsUCG4:
			    *(ut.ptr) = (u_long)GsU_00000016;
			    break;
			  case GsUCGT4:
			    *(ut.ptr) = (u_long)GsU_00000017;
			    break;
			  case GsUNF3: 
			    *(ut.ptr) = (u_long)GsU_00040048;
			    break;
			  case GsUNFT3: 
			    *(ut.ptr) = (u_long)GsU_00040049;
			    break;
			  case GsUNG3: 
			    *(ut.ptr) = (u_long)GsU_0004004c;
			    break;
			  case GsUNGT3: 
			    *(ut.ptr) = (u_long)GsU_0004004d;
			    break;
			  case GsUNF4: 
			    *(ut.ptr) = (u_long)GsU_00040050;
			    break;
			  case GsUNFT4: 
			    *(ut.ptr) = (u_long)GsU_00040051;
			    break;
			  case GsUNG4: 
			    *(ut.ptr) = (u_long)GsU_00040054;
			    break;
			  case GsUNGT4: 
			    *(ut.ptr) = (u_long)GsU_00040055;
			    break;
			  case GsUDF3:
			    *(ut.ptr) = (u_long)GsU_00010008;
			    break;
			  case GsUDFT3: 
			    *(ut.ptr) = (u_long)GsU_00010009;
			    break;
			  case GsUDG3:
			    *(ut.ptr) = (u_long)GsU_0001000c;
			    break;
			  case GsUDGT3:
			    *(ut.ptr) = (u_long)GsU_0001000d;
			    break;
			  case GsUDF4:
			    *(ut.ptr) = (u_long)GsU_00010010;
			    break;
			  case GsUDFT4:
			    *(ut.ptr) = (u_long)GsU_00010011;
			    break;
			  case GsUDG4:
			    *(ut.ptr) = (u_long)GsU_00010014;
			    break;
			  case GsUDGT4:
			    *(ut.ptr) = (u_long)GsU_00010015;
			    break;
			  case GsUDFF3:
			    *(ut.ptr) = (u_long)GsU_00030008;
			    break;
			  case GsUDFFT3: 
			    *(ut.ptr) = (u_long)GsU_00030009;
			    break;
			  case GsUDFG3:
			    *(ut.ptr) = (u_long)GsU_0003000c;
			    break;
			  case GsUDFGT3:
			    *(ut.ptr) = (u_long)GsU_0003000d;
			    break;
			  case GsUDFF4:
			    *(ut.ptr) = (u_long)GsU_00030010;
			    break;
			  case GsUDFFT4:
			    *(ut.ptr) = (u_long)GsU_00030011;
			    break;
			  case GsUDFG4:
			    *(ut.ptr) = (u_long)GsU_00030014;
			    break;
			  case GsUDFGT4:
			    *(ut.ptr) = (u_long)GsU_00030015;
			    break;
			  case GsUDNF3: 
			    *(ut.ptr) = (u_long)GsU_00050048;
			    break;
			  case GsUDNFT3: 
			    *(ut.ptr) = (u_long)GsU_00050049;
			    break;
			  case GsUDNG3: 
			    *(ut.ptr) = (u_long)GsU_0005004c;
			    break;
			  case GsUDNGT3: 
			    *(ut.ptr) = (u_long)GsU_0005004d;
			    break;
			  case GsUDNF4: 
			    *(ut.ptr) = (u_long)GsU_00050050;
			    break;
			  case GsUDNFT4: 
			    *(ut.ptr) = (u_long)GsU_00050051;
			    break;
			  case GsUDNG4: 
			    *(ut.ptr) = (u_long)GsU_00050054;
			    break;
			  case GsUDNGT4: 
			    *(ut.ptr) = (u_long)GsU_00050055;
			    break;
			  case GsUSCAL:
			    *(ut.ptr) = (u_long)GsU_01000000;
			    break;
			  case GsUSG3:
			    *(ut.ptr) = (u_long)GsU_0100000c;
			    break;
			  case GsUSGT3:
			    *(ut.ptr) = (u_long)GsU_0100000d;
			    break;
			  case GsUSG4:
			    *(ut.ptr) = (u_long)GsU_01000014;
			    break;
			  case GsUSGT4:
			    *(ut.ptr) = (u_long)GsU_01000015;
			    break;
			  case GsUSNF3:
			    *(ut.ptr) = (u_long)GsU_01040048;
			    break;
			  case GsUSNFT3:
			    *(ut.ptr) = (u_long)GsU_01040049;
			    break;
			  case GsUSNG3:
			    *(ut.ptr) = (u_long)GsU_0104004c;
			    break;
			  case GsUSNGT3:
			    *(ut.ptr) = (u_long)GsU_0104004d;
			    break;
			  case GsUSNF4:
			    *(ut.ptr) = (u_long)GsU_01040050;
			    break;
			  case GsUSNFT4:
			    *(ut.ptr) = (u_long)GsU_01040051;
			    break;
			  case GsUSNG4:
			    *(ut.ptr) = (u_long)GsU_01040054;
			    break;
			  case GsUSNGT4:
			    *(ut.ptr) = (u_long)GsU_01040055;
			    break;
			  case GsUMF3:
			    *(ut.ptr) = (u_long)GsU_00000018;
			    break;
			  case GsUMFT3:
			    *(ut.ptr) = (u_long)GsU_00000019;
			    break;
			  case GsUMG3:
			    *(ut.ptr) = (u_long)GsU_0000001c;
			    break;
			  case GsUMGT3:
			    *(ut.ptr) = (u_long)GsU_0000001d;
			    break;
			  case GsUMNF3:
			    *(ut.ptr) = (u_long)GsU_00040058;
			    break;
			  case GsUMNFT3:
			    *(ut.ptr) = (u_long)GsU_00040059;
			    break;
			  case GsUMNG3:
			    *(ut.ptr) = (u_long)GsU_0004005c;
			    break;
			  case GsUMNGT3:
			    *(ut.ptr) = (u_long)GsU_0004005d;
			    break;
			  case GsUTFT3:
			    *(ut.ptr) = (u_long)GsU_00000209;
			    break;
			  case GsUTGT3:
			    *(ut.ptr) = (u_long)GsU_0000020d;
			    break;
			  case GsUTFT4:
			    *(ut.ptr) = (u_long)GsU_00000211;
			    break;
			  case GsUTGT4:
			    *(ut.ptr) = (u_long)GsU_00000215;
			    break;
			  case GsUPNF3:
			    *(ut.ptr) = (u_long)GsU_00040148;
			    break;
			  case GsUPNFT3:
			    *(ut.ptr) = (u_long)GsU_00040149;
			    break;
			  case GsUPNG3:
			    *(ut.ptr) = (u_long)GsU_0004014c;
			    break;
			  case GsUPNGT3:
			    *(ut.ptr) = (u_long)GsU_0004014d;
			    break;
			  case GsUPNF4:
			    *(ut.ptr) = (u_long)GsU_00040150;
			    break;
			  case GsUPNFT4:
			    *(ut.ptr) = (u_long)GsU_00040151;
			    break;
			  case GsUPNG4:
			    *(ut.ptr) = (u_long)GsU_00040154;
			    break;
			  case GsUPNGT4:
			    *(ut.ptr) = (u_long)GsU_00040155;
			    break;
			  case GsUIMG0:
			    *(ut.ptr) = (u_long)GsU_00000000;
			    GsU_02000000((GsARGUNIT *)getScratchAddr(0));
			    break;
			  case GsUIMG1:
			    *(ut.ptr) = (u_long)GsU_00000000;
			    GsU_02000001((GsARGUNIT *)getScratchAddr(0));
			    break;
			  case GsVtxMIMe:
			    *(ut.ptr) = (u_long)GsU_04010020;
			    mph=GsMapVtxMIMe(ut.ptr, MODEL_ADDR, GsGetHeadpUnit());
			    set_mimepr(mph, mimepr, MIMENUM);
			    break;
			  case GsRstVtxMIMe:
			    *(ut.ptr) = (u_long)GsU_04010028;
			    GsMapRstVtxMIMe(ut.ptr,  GsGetHeadpUnit());
			    break;
			  case GsNrmMIMe:
			    *(ut.ptr) = (u_long)GsU_04010021;
			    mph=GsMapNrmMIMe(ut.ptr, MODEL_ADDR, GsGetHeadpUnit());
			    set_mimepr(mph, mimepr, MIMENUM);
			    break;
			  case GsRstNrmMIMe:
			    *(ut.ptr) = (u_long)GsU_04010029;
			    GsMapRstNrmMIMe(ut.ptr,  GsGetHeadpUnit());
			    break;
			  case GsJntAxesMIMe:
			    *(ut.ptr) = (u_long)GsU_04010010;
			    mph=GsMapJntAxesMIMe(ut.ptr,  GsGetHeadpUnit());
			    set_mimepr(mph, mimepr, MIMENUM);
			    break;
			  case GsRstJntAxesMIMe:
			    GsMapRstJntAxesMIMe(ut.ptr,  GsGetHeadpUnit());
			    *(ut.ptr) = (u_long)GsU_04010018;
			    break;
			  case GsJntRPYMIMe:
			    *(ut.ptr) = (u_long)GsU_04010011;
			    mph=GsMapJntRPYMIMe(ut.ptr,  GsGetHeadpUnit());
			    set_mimepr(mph, mimepr, MIMENUM);
			    break;
			  case GsRstJntRPYMIMe:
			    GsMapRstJntRPYMIMe(ut.ptr,  GsGetHeadpUnit());
			    *(ut.ptr) = (u_long)GsU_04010019;
			    break;
			  default:
			    *(ut.ptr) = (u_long)GsU_00000000;
			    printf("Unknown type:0x%08x\n",
				   ut.type);
			    break;
			}
#ifdef DEBUG
			printf("DEBUG:block:%d, Code:0x%08x\n", i, ut.type);
#endif /* DEBUG */
		}
		if ((i > 0) && (i < bnum-1))
			objp->coord =	&DModel[i-1];
		else
			objp->coord =	NULL;
	}

	return;
}
