/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="graphics/hmd/basic/basic2.cpe";
#endif
/*
 *	"tuto2.c" main routine (shuttle)
 *
 *		Copyright (C) 1997  Sony Computer Entertainment
 *		All rights Reserved
 */

#define DEBUG		/**/

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

#define OBJECTMAX 100		/* Max Objects :
                                   ３Dのモデルは論理的なオブジェクトに
                                   分けられるこの最大数 を定義する */

#define MODEL_ADDR 0x80010000	/* Top Address of modeling data (HMD FORMAT) :
				   モデリングデータ（HMDフォーマット）
				   がおかれるアドレス */

#define OT_LENGTH 12		/* Area of OT : オーダリングテーブルの解像度 */

#define PACKETMAX (10000*24)

#define S_MAXID		9		/* shuttle max number of ID */
#define S_BODY		0		/* shuttle body ID */
#define S_HATCHR	1		/* shuttle right hatch ID */
#define S_HATCHL	2		/* shuttle left hatch ID */
#define S_SAT		3		/* shuttle satellite ID */
#define S_WING0		4		/* shuttle satellite wing 0 ID */
#define S_WING1		5		/* shuttle satellite wing 1 ID */
#define S_WING2		6		/* shuttle satellite wing 2 ID */
#define S_WING3		7		/* shuttle satellite wing 3 ID */
#define S_WING4		8		/* shuttle satellite wing 4 ID */
#define S_WING5		9		/* shuttle satellite wing 5 ID */

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

GsRVIEWUNIT	view;		/* View Point Handler :
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
void set_coordinate(GsCOORDUNIT *coor);
void set_coordinate2(GsCOORDUNIT *coor);
void model_init(void);
void view_init(void);
void light_init(void);

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
	FntOpen(16, 16, 256, 200, 0, 512); 

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
		GsSetRefViewUnit(&view);/* Calculate World-Screen Matrix :
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
#ifdef APD_LOAD
		FntPrint("vcount=%d\n", VSync(-1));
		APDSetCnt(VSync(0));
#else
		FntPrint("vcount=%d\n", GsGetVcount());
		VSync(0);		/* Wait for VSYNC : Vブランクを待つ */
		GsClearVcount();
#endif
		ResetGraph(1);		/* Reset GPU */
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

int obj_interactive()
{
	int i;

	if (padd & PADRleft)	DModel[S_BODY].rot.vy += 5*ONE/360;
	if (padd & PADRright)	DModel[S_BODY].rot.vy -= 5*ONE/360;
	if (padd & PADRup)	DModel[S_BODY].rot.vx += 5*ONE/360;
	if (padd & PADRdown)	DModel[S_BODY].rot.vx -= 5*ONE/360;
	
	if (padd & PADLleft)	DModel[S_BODY].matrix.t[0] -= 10;
	if (padd & PADLright)	DModel[S_BODY].matrix.t[0] += 10;
	if (padd & PADLup)	DModel[S_BODY].matrix.t[1] -= 10;
	if (padd & PADLdown)	DModel[S_BODY].matrix.t[1] += 10;

	if (padd & PADstart)	DModel[S_BODY].matrix.t[2] -= 50;
	if (padd & PADselect)	DModel[S_BODY].matrix.t[2] += 50;

	if (padd & PADL1) {
		DModel[S_HATCHR].rot.vz += 2*ONE/360;
		DModel[S_HATCHL].rot.vz += 2*ONE/360;
		set_coordinate(&DModel[S_HATCHR]);
		set_coordinate(&DModel[S_HATCHL]);
	}
	if (padd & PADR1) {
		DModel[S_HATCHR].rot.vz -= 2*ONE/360;
		DModel[S_HATCHL].rot.vz -= 2*ONE/360;
		set_coordinate(&DModel[S_HATCHR]);
		set_coordinate(&DModel[S_HATCHL]);
	}

	if (padd & PADL2) {
		DModel[S_SAT].matrix.t[1] -= 10;
		DModel[S_SAT].rot.vy += 2*ONE/360;
		set_coordinate(&DModel[S_SAT]);
		for (i = 0; i < 6; i++) {
			DModel[S_WING0+i].rot.vx -= 1*ONE/360;
			set_coordinate2(&DModel[S_WING0+i]);
		}
	}
	if (padd & PADR2) {
		DModel[S_SAT].matrix.t[1] += 10;
		DModel[S_SAT].rot.vy -= 2*ONE/360;
		set_coordinate(&DModel[S_SAT]);
		for (i = 0; i < 6; i++) {
			DModel[S_WING0+i].rot.vx += 1*ONE/360;
			set_coordinate2(&DModel[S_WING0+i]);
		}
	}

	if ((padd & PADselect) && (padd & PADstart)) {
		return(0);
	}
	
	set_coordinate(&DModel[S_BODY]);

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
	GsInitGraph(640, 480, 2, 1, 0);
				/* rezolution set , non interrace mode :
				   解像度設定（インターレースモード） */
	GsDefDispBuff(0, 0, 0, 0);
				/* Double buffer setting :
				   ダブルバッファ指定 */

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
	GsSetRefViewUnit(&view);
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

/* Set coordinte parameter from work vector :
   マトリックス計算ワークからマトリックスを作成し座標系にセットする */
void set_coordinate2(GsCOORDUNIT *coor)
{
	/* Rotate Matrix :
	   マトリックスにローテーションベクタを作用させる */
	RotMatrixYXZ(&coor->rot, &coor->matrix);

	/* Clear flag becase of changing parameter :
	   マトリックスキャッシュをフラッシュする */
	coor->flg = 0;
}

/* Read modeling data (HMD FORMAT) : モデリングデータの読み込み */
void model_init(void)
{
	u_long	*dop;
	GsUNIT	*objp;		/* handler of object :
				   モデリングデータハンドラ */
	int	i;
	u_long	*oppp;

	dop = (u_long *)MODEL_ADDR;
				/* Top Address of MODELING DATA(HMD FORMAT) :
				   モデリングデータが格納されているアドレス */
	GsMapUnit(dop);		/* Mappipng real address :
				   モデリングデータ（HMDフォーマット）を
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
				DModel = GsMapCoordUnit((u_long *)MODEL_ADDR, 
						ut.ptr);
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
			case GsUG3:
				*(ut.ptr) = (u_long)GsU_0000000c;
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
}
