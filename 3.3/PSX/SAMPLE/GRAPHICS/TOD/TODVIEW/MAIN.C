/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/* 
 * 	todview: sample program to view TOD data
 * 
 *	"main.c" simple TOD viewing routine
 *
 * 		Version 2.00	Feb, 6, 1995
 * 
 * 		Copyright (C) 1994, 1995 by Sony Computer Entertainment
 * 		All rights Reserved
 */

/*****************/
/* Include files */
/*****************/
#include <sys/types.h>
#include <libetc.h>		/* PADを使うためにインクルードする必要あり */
#include <libgte.h>		/* LIGSを使うためにインクルードする必要あり */
#include <libgpu.h>		/* LIGSを使うためにインクルードする必要あり */
#include <libgs.h>		/* グラフィックライブラリ を使うための
				   構造体などが定義されている */
#include "tod.h"		/* TOD処理関数の宣言部 */
#include "te.h"			/* TMD ID テーブル */

/**********/
/* For OT */
/**********/
#define OT_LENGTH	12		 /* OT解像度を12bit（大きさ） */
GsOT		WorldOT[2];		 /* OT情報（ダブルバッファ） */
GsOT_TAG	OTTags[2][1<<OT_LENGTH]; /* OTのタグ領域（ダブルバッファ） */

/***************/
/* For Objects */
/***************/
#define OBJMAX	50			/* 最大オブジェクト数 */
GsDOBJ2		obj_area[OBJMAX];	/* オブジェクト変数領域 */
GsCOORDINATE2	obj_coord[OBJMAX];	/* ローカル座標変数領域 */
GsCOORD2PARAM	obj_cparam[OBJMAX];	/* ローカル座標変数領域 */
TodOBJTABLE	objs;			/* オブジェクトテーブル */

/***************/
/* For Packets */
/***************/
#define PACKETMAX	6000		/* 1フレームの最大パケット数 */
#define PACKETMAX2	(PACKETMAX*24)	/* 1パケット平均サイズを24Byteとする */
PACKET	GpuPacketArea[2][PACKETMAX2];	/* パケット領域（ダブルバッファ） */

/***************/
/* For Viewing */
/***************/
#define VPX	-8000
#define VPY	-2000
#define VPZ	-5000
#define VRX	-900
#define VRY	-1500
#define VRZ	0
GsRVIEW2	view;			/* 視点情報 */

/************************/
/* For Lights ( three ) */
/************************/
GsF_LIGHT	pslt[3];		/* 光源×３個 */

/*********************/
/* For Modeling data */
/*********************/
#define MODEL_ADDR	0x800c0000
u_long		*TmdP;			/* モデリングデータポインタ */

/**********************/
/* For Animation data */
/**********************/
#define TOD_ADDR	0x800e0000
u_long		*TodP;		/* アニメーションデータポインタ配列 */
int		NumFrame;	/* アニメーションデータフレーム数配列 */
int		StartFrameNo;	/* アニメーションスタートフレーム番号配列 */

/******************/
/* For Controller */
/******************/
u_long		padd;

/*
 * Main routine
/* メイン・ルーチン
 *
 */
main()
{
    /* Initialize 
       /* イニシャライズ */
    ResetCallback();
    initSystem();
    initView();
    initLight();
    initModelingData( MODEL_ADDR );
    initTod();

    /* 一度 TODデータを表示 */
    drawTod();

    /* ループ */
    while( 1 ) {

	/* Read the pad data */
	padd = PadRead( 1 );

	/* play TOD data according to the pad data */
	if(obj_interactive()) break;
    }

    PadStop();
    ResetGraph(3);
    StopCallback();
    return 0;
}

/*
 * Draw 3D objects
/* 3Dオブジェクト描画処理
 *
 */
drawObject()
{
    int		i;
    int		activeBuff;
    GsDOBJ2	*objp;
    MATRIX	LsMtx;	

    activeBuff = GsGetActiveBuff();

    /* Set the address at which drawing command will be put
       /* 描画コマンドを格納するアドレスを設定する */
    GsSetWorkBase( (PACKET*)GpuPacketArea[activeBuff] );

    /* Initialize OT 
       /* OT の初期化 */
    GsClearOt( 0, 0, &WorldOT[activeBuff] );

    /* Set the pointer to the object array 
       /* オブジェクト配列へのポインタをセットする */
    objp = objs.top;

    for( i = 0; i < objs.nobj; i++ ) { 

	/* flag whether the coord has changed or not
	   /* coord が書き換えられたかどうかのフラグ */
	objp->coord2->flg = 0;

	if ( ( objp->id  != GsOBJ_UNDEF ) && ( objp->tmd != 0 ) ) {

	    /* Calculate the local screen matrix 
	       /* ローカルスクリーンマトリックスを計算 */
	    GsGetLs( objp->coord2, &LsMtx );

	    /* Set the local screen matrix to GTE 
	       /* ローカルスクリーンマトリックスを GTE にセットする */
	    GsSetLsMatrix( &LsMtx );

	    /* Set the light matrix to GTE 
	       /* ライトマトリックスを GTE にセットする */
	    GsSetLightMatrix( &LsMtx );

	    /* Transform the object perspectively and assign it to the OT
	       /* オブジェクトを透視変換しオーダリングテーブルに割り付ける */
	    GsSortObject2( objp, 		/* Pointer to the object */
			   &WorldOT[activeBuff],/* Pointer to the OT */
			   14-OT_LENGTH );	/* number of bits to be shifted*/

	}

	objp++;
    }

    VSync( 0 );				/* Wait for V-BLNK */
    ResetGraph( 1 );			/* Cancel the current display */
    GsSwapDispBuff();			/* Switch the double buffer */

    /* Resister the clear command to OT 
       /* 画面クリアコマンドを OT に登録 */
    GsSortClear( 0x0,				/* R of the background */
		 0x0,				/* G of the background */
		 0x0,				/* B of the background */
		 &WorldOT[activeBuff] );	/* Pointer to the OT */

    /* Do the drawing commands assigned to the OT 
       /* OT に割り付けられた描画コマンドの描画 */
    GsDrawOt( &WorldOT[activeBuff] );
}

/*
 * Initialize system
/* 初期化ルーチン群
 *
 */
initSystem()
{
  /* Initialize the controll pad
     /* コントロールパッドの初期化 */
  PadInit( 0 );
  padd = 0;
	
  /* Initialize the GPU 
     /* GPUの初期化 */
  GsInitGraph( 640, 240, 0, 1, 0 );
  GsDefDispBuff( 0, 0, 0, 240 );
	
  /* Initialize the OT 
     /* OTの初期化 */
  WorldOT[0].length = OT_LENGTH;
  WorldOT[0].org = OTTags[0];
  WorldOT[0].offset = 0;
  WorldOT[1].length = OT_LENGTH;
  WorldOT[1].org = OTTags[1];
  WorldOT[1].offset = 0;
	
  /* Initialize the 3D library 
     /* 3Dライブラリの初期化 */
  GsInit3D();
}

/*
 * Set the location of the view point
/* 視点位置の設定
 *
 */
initView()
{
    /* Set the view angle 
       /* プロジェクション（視野角）の設定 */
    GsSetProjection( 1000 );

    /* Set the view point and the reference point 
       /* 視点位置の設定 */
    view.vpx = VPX; view.vpy = VPY; view.vpz = VPZ;
    view.vrx = VRX; view.vry = VRY; view.vrz = VRZ;
    view.rz = 0;
    view.super = WORLD;
    GsSetRefView2( &view );

    /* Set the value of Z clip 
       /* Zクリップ値を設定 */
    GsSetNearClip( 100 );
}

/*
 * Set the light ( the direction and the color )
/* 光源の設定（照射方向＆色）
 *
 */
initLight()
{
    /* Light #0 , direction (100,100,100) 
       /* 光源#0 (100,100,100)の方向へ照射 */
    pslt[0].vx = 100; pslt[0].vy= 100; pslt[0].vz= 100;
    pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
    GsSetFlatLight( 0,&pslt[0] );
	
    /* Light #1 ( not used ) 
       /* 光源#1（使用せず） */
    pslt[1].vx = 100; pslt[1].vy= 100; pslt[1].vz= 100;
    pslt[1].r=0; pslt[1].g=0; pslt[1].b=0;
    GsSetFlatLight( 1,&pslt[1] );
	
    /* Light #2 ( not used ) 
       /* 光源#2（使用せず） */
    pslt[2].vx = 100; pslt[2].vy= 100; pslt[2].vz= 100;
    pslt[2].r=0; pslt[2].g=0; pslt[2].b=0;
    GsSetFlatLight( 2,&pslt[2] );
	
    /* Set the ambient light 
       /* アンビエント（周辺光）の設定 */
    GsSetAmbient( ONE/2,ONE/2,ONE/2 );

    /* Set the light mode 
       /* 光源モードの設定（通常光源/FOGなし） */
    GsSetLightMode( 0 );
}

/*
 * Get TMD data from the memory ( use only the top one )
/* メモリ上のTMDデータの読み込み (先頭の１個のみ使用）
 *
 */
initModelingData( addr )
u_long	*addr;
{
    /* Top address of the TMD data 
       /* TMDデータの先頭アドレス */
    TmdP = addr;

    /* Skip the file header 
       /* ファイルヘッダをスキップ */
    TmdP++;

    /* Map to the real address 
       /* 実アドレスへマップ */
    GsMapModelingData( TmdP );

    /* Initialize the object table 
       /* オブジェクトテーブルの初期化 */
    TodInitObjTable( &objs, obj_area, obj_coord, obj_cparam, OBJMAX );
}


/*
 * Set TOD data from the memory
/* メモリ上のTODデータの読み込み
 *
 */
initTod()
{
    TodP = ( u_long * )TOD_ADDR;
    TodP++;
    NumFrame = *TodP++;
    StartFrameNo = *( TodP + 1 );
}

/*
 * play TOD data according to the pad data 
/* パッドにより、TODデータを表示する
 *
 */
obj_interactive()
{
    int		i;

    if ( ( ( padd & PADRright ) > 0 ) ||
	 ( ( padd & PADRleft )  > 0 ) || 
	 ( ( padd & PADRup )    > 0 ) ||
	 ( ( padd & PADRdown )  > 0 ) ) {

	drawTod();
    }

    /* 視点の変更 */
    if ( ( padd & PADn ) > 0 ) {
	view.vpz += 100;
	GsSetRefView2( &view );
	drawObject();
    }
    if ( ( padd & PADo ) > 0 ) {
	view.vpz -= 100;
	GsSetRefView2( &view );
	drawObject();
    }
    if ( ( padd & PADLleft ) > 0 ) {
	view.vpx -= 100;
	GsSetRefView2( &view );
	drawObject();
    }
    if ( ( padd & PADLright ) > 0 ) {
	view.vpx += 100;
	GsSetRefView2( &view );
	drawObject();
    }
    if ( ( padd & PADLdown ) > 0 ) {
	view.vpy += 100;
	GsSetRefView2( &view );
	drawObject();
    }
    if ( ( padd & PADLup ) > 0 ) {
	view.vpy -= 100;
	GsSetRefView2( &view );
	drawObject();
    }
    
    /* Move the view point to the initial place 
       /* 視点を元に戻す */
    if ( ( padd & PADh ) > 0 ) {
	initView();
	drawObject();
    }

    /* Exit the program 
       /* プログラムを終了する */
    if ( ( padd & PADk ) > 0 ) {
	return -1;
    }

    return 0;
}


drawTod()
{
    int		i;
    int		j;
    u_long	*TodPtmp;
    int		frameNo;
    int		oldFrameNo;

    TodPtmp = TodP;
    TodPtmp = TodSetFrame( StartFrameNo, TodPtmp, &objs, te_list,
			   TmdP, GsTOD_CREATE );
    drawObject();

    oldFrameNo = StartFrameNo;

    for ( i = StartFrameNo + 1 ; i < NumFrame + StartFrameNo ; i++ ) {
	    frameNo = *( TodPtmp + 1 );

        for ( j = 0 ; j < frameNo - oldFrameNo - 1 ; j++ ) {
            drawObject();
        }

        TodPtmp = TodSetFrame( frameNo, TodPtmp, &objs, te_list,
			       TmdP, GsTOD_CREATE );
        drawObject();

	oldFrameNo = frameNo;
    }
    drawObject();
}

