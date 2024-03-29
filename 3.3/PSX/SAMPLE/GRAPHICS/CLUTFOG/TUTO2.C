/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/***********************************************
 *	
 *	clut によるテクスチャへのフォグ効果
 *
 *	"tuto2.c" main routine
 *
 *		Version 1.00	Sep.  29, 1995
 *
 *	Copyright (C) 1995 Sony Computer Entertainment Inc.
 *		All Rights Reserved.
 ***********************************************/
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH  10		/* オーダリングテーブルの解像度 */
#define PROJECTION 300

#define BGR 100					/* BG color Red */
#define BGG 100					/* BG color Green */
#define BGB 100					/* BG color Blue */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org);
int obj_interactive(u_long fn, u_long padd);
void main();

GsOT            Wot[2];			/* オーダリングテーブルハンドラ */
GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* オーダリングテーブル実体 */


#define HNUM 5
#define WNUM 5

POLY_FT4 ft4[2][HNUM][WNUM];			/* polygon */
SVECTOR fpos[HNUM][WNUM][4];			/* polygon vertex positions */

GsCOORDINATE2   DWorld;		/* オブジェクトごとの座標系 */
SVECTOR PWorld;			/* 座標系を作るためのローテーションベクター */
extern MATRIX GsIDMATRIX;	/* Unit Matrix /* 単位行列 */



RECT crect1;
CVECTOR cv1[256];				/* original clut */
CVECTOR bgc;					/* background color */
short clutw;

int  outbuf_idx=0;

#define CBIT 6
#define CNUM (1<<CBIT)
u_short fclut[CNUM];

GsIMAGE tim1;
long fognear, fogfar;
static short pbuf[4096];			/*  */

DR_MOVE mvi[2][CNUM];


/************* MAIN START ******************************************/
void main()
{
    u_long fn;
    u_long padd=0;		/* コントローラのデータを保持する */
    int i,j;

    ResetCallback();
    PadInit(0);			/* コントローラ初期化 */
    init_all();


    /* main loop */
    for (fn=0;;fn++){
	outbuf_idx=GsGetActiveBuff();		/* get one of buffers */

	if (obj_interactive(fn, padd)<0) return; /* pad data handling */

	GsClearOt(0,0,&Wot[outbuf_idx]);	/* clear ordering table */

	/* draw polygon(s) */
	draw_poly(&DWorld, outbuf_idx, Wot[outbuf_idx].org);

	padd=PadRead(1);			/* read pad */

	DrawSync(0);				/* wait drawing done */

	VSync(0);				/* wait Vertical Sync */

	GsSwapDispBuff();			/* switch double buffers */
	
	/* 画面のクリアをオーダリングテーブルの最初に登録する */
	GsSortClear(BGR,BGG,BGB,&Wot[outbuf_idx]); 

	ResetGraph(1);
	/* オーダリングテーブルに登録されているパケットの描画を開始する */
	GsDrawOt(&Wot[outbuf_idx]);
	FntFlush(-1);				/* draw print-stream */
    }
    return;
}


/* draw polygon(s) */
void draw_poly(GsCOORDINATE2 *co, int idx, GsOT_TAG *org)
{
    int i,j;
    long otz, flag;
    MATRIX tmpls;
    POLY_FT4 *f;
    long nclip;
    long p;

    /* 座標を計算 */
    GsGetLs(co, &tmpls);
    /* GTEにセット */
    GsSetLsMatrix(&tmpls);

    f= &ft4[idx][0][0];
    for (i=0; i<HNUM; i++){
	for (j=0; j<WNUM; j++){
	    otz=RotTransPers4(&fpos[i][j][0],
			      &fpos[i][j][1],
			      &fpos[i][j][2],
			      &fpos[i][j][3],
			    (long *)(&f->x0),
			    (long *)(&f->x1),
			    (long *)(&f->x2),
			    (long *)(&f->x3),
			    &p, &flag
			    );

	    if (flag >=0				/* error OK */
		&& otz>0				/* otz OK */
		&& p<4096){				/* fog is not full */

		otz>>=(14-OT_LENGTH);
		p>>=12-CBIT;
		/* ポリゴンの存在する場所のOTZとfogパラメータを覚えておく */
		if (pbuf[p]<otz) pbuf[p]=otz;
		/* OT へ登録 */
		AddPrim(org+otz, f);
	    }
	    f++;
	}
    }


    for (i=0; i<CNUM; i++){
	/* 該当する深さにポリゴンが存在したら CLUT 転送パケットを登録 */
	/*(libgs による描画など、ポリゴン存在範囲がわからないときはすべて登録)*/
	if (pbuf[i]>=0){
	    AddPrim(org+pbuf[i], &mvi[idx][i]);
	}
	pbuf[i]= -1;
    }
}

/* pad data handling */
int obj_interactive(u_long fn, u_long padd)
{
#define DT 30
#define VT 30
#define ZT 100
#define FT 10
    static u_long oldpadd=0;

    if (padd & PADk){
#if 0
	if (padd & PADLdown) fognear-=FT;
	if (padd & PADLup) fognear+=FT;
	if (fognear<0) fognear=0;
	SetFogNearFar(fognear, fogfar, PROJECTION);
#endif /* 0 */
    } else if (padd & PADh){
#if 0
	if (padd & PADLdown) fogfar-=FT;
	if (padd & PADLup) fogfar+=FT;
	if (fogfar<(fognear*2)) fogfar=fognear*2;
	SetFogNearFar(fognear, fogfar, PROJECTION);
#endif /* 0 */
    } else{				/* translate and rotate */
	if (padd & PADLleft) DWorld.coord.t[0]-=DT;
	else if (padd & PADLright) DWorld.coord.t[0]+=DT;
	
	if (padd & PADLup) DWorld.coord.t[1]-=DT;
	else if (padd & PADLdown) DWorld.coord.t[1]+=DT;
	
	if (padd & PADo) DWorld.coord.t[2]+=ZT;
	else if (padd & PADn) DWorld.coord.t[2]-=ZT;
	
	if (padd & PADRleft) PWorld.vy-=VT;
	else if (padd & PADRright) PWorld.vy+=VT;
	
	if (padd & PADRup) PWorld.vx+=VT;
	else if (padd & PADRdown) PWorld.vx-=VT;
	
	if (padd & PADm) PWorld.vz-=VT;
	else if (padd & PADl) PWorld.vz+=VT;
    }

    if (padd & PADk){
	/* プログラム終了 */
	PadStop();
	ResetGraph(3);
	StopCallback();
	return -1;
    }
/*    FntPrint("%d,%d\n",fognear, fogfar);*/

    set_coordinate(&PWorld,&DWorld);
    oldpadd=padd;
    return 0;
}

init_all()			/* 初期化ルーチン群 */
{
    ResetGraph(0);		/* GPUリセット */

    draw_init();
    coord_init();
    view_init();
    fnt_init();
    texture_init((u_long *)0x80100000);
    poly_init();

    fognear=1000;
    fogfar=4000;
    SetFogNearFar(fognear, fogfar, PROJECTION);	/* set fog parameter */
    bgc.r=BGR; bgc.g=BGG; bgc.b=BGB;

}

poly_init()
{
    int i,j,k;
    POLY_FT4 *f;
    int z;

    f= &ft4[0][0][0];
    for (k=0; k<2; k++){
	for (i=0; i<HNUM; i++){
	    for (j=0; j<WNUM; j++){
		setPolyFT4(f);			/* initialize primitive */
		SetShadeTex(f,1);			/* shading off */
		setUV4(f,
		       0, 0,
		       255, 0,
		       0, 255,		       
		       255, 255
		       );			/* set texture position */
		f->clut=GetClut(tim1.cx, tim1.cy); /* clut (temporary) */
		/* texture page */
		f->tpage=GetTPage(tim1.pmode,0,tim1.px,tim1.py);
		f++;
	    }
	}
    }
#define SIDELONG 100
#define WSPACE 500
#define HSPACE 500
#define WOFFSET (-(WNUM/2) * WSPACE )
#define HOFFSET (-(HNUM/2) * HSPACE )
#define ZOFFSET 0
#define ZSPACE 750

	    /* set vertex positions */
    for (i=0; i<HNUM; i++){
	for (j=0; j<WNUM; j++){
	    z=abs(i-(HNUM/2))+abs(j-(HNUM/2));
	    fpos[i][j][0].vx= (WSPACE*j)+ WOFFSET-SIDELONG;
	    fpos[i][j][0].vy= (HSPACE*i)+ HOFFSET-SIDELONG;
	    fpos[i][j][0].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][1].vx= (WSPACE*j)+ WOFFSET+SIDELONG;
	    fpos[i][j][1].vy= (HSPACE*i)+ HOFFSET-SIDELONG;
	    fpos[i][j][1].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][2].vx= (WSPACE*j)+ WOFFSET-SIDELONG;
	    fpos[i][j][2].vy= (HSPACE*i)+ HOFFSET+SIDELONG;
	    fpos[i][j][2].vz= (ZSPACE*z)+ ZOFFSET;
	    fpos[i][j][3].vx= (WSPACE*j)+ WOFFSET+SIDELONG;
	    fpos[i][j][3].vy= (HSPACE*i)+ HOFFSET+SIDELONG;
	    fpos[i][j][3].vz= (ZSPACE*z)+ ZOFFSET;
	}
    }
}


draw_init()
{
/*  GsInitGraph(640,480,2,1,0);	/* 解像度設定（インターレースモード） */
/*  GsDefDispBuff(0,0,0,0);	/* ダブルバッファ指定 */
    GsInitGraph(640,240,GsOFSGPU|GsINTER,1,0);	/* 解像度設定（インターレースモード） */
    GsDefDispBuff(0,0,0,240);	/* ダブルバッファ指定 */
    GsInit3D();			/* ３Dシステム初期化 */

    Wot[0].length=OT_LENGTH;	/* オーダリングテーブルハンドラに解像度設定 */
    Wot[0].org=zsorttable[0];	/* オーダリングテーブルハンドラに
					   オーダリングテーブルの実体設定 */
    /* ダブルバッファのためもう一方にも同じ設定 */
    Wot[1].length=OT_LENGTH;
    Wot[1].org=zsorttable[1];
}

fnt_init()
{
    FntLoad(960, 256);				/* font load */
    SetDumpFnt(FntOpen(-290,-100,400,100,0,200)); /* stream open & define */
}

int view_init()
{
    GsRVIEW2  view;		/* View Point Handler*/

    /*---- Set projection,view ----*/
    GsSetProjection(PROJECTION);	/* Set projection /* プロジェクション設定 */
  
    /* Setting view point location /* 視点パラメータ設定 */
    view.vpx = 0; view.vpy = 0; view.vpz = -2000;
  
    /* Setting focus point location /* 注視点パラメータ設定 */
    view.vrx = 0; view.vry = 0; view.vrz = 0;
  
    /* Setting bank of SCREEN /* 視点の捻りパラメータ設定 */
    view.rz=0;

    /* Setting parent of viewing coordinate /* 視点座標パラメータ設定 */
    view.super = WORLD;
  
    /* Calculate World-Screen Matrix from viewing paramter*/
    /* 視点パラメータを群から視点を設定する*/
    /*ワールドスクリーンマトリックスを計算する */
    GsSetRefView2(&view);
  
    GsSetNearClip(100);           /* Set Near Clip /* ニアクリップ設定 */
}


int coord_init()
{
    /* 座標の定義 */
    GsInitCoordinate2(WORLD,&DWorld);
    DWorld.coord.t[2]= -1000;

    /* マトリックス計算ワークのローテーションベクター初期化 */
    PWorld.vx=PWorld.vy=PWorld.vz=0;
/*    PWorld.vx=2048;*/

    set_coordinate(&PWorld,&DWorld);
}

/* ローテションベクタからマトリックスを作成し座標系にセットする */
int set_coordinate(SVECTOR *pos, GsCOORDINATE2 *coor)
{
    MATRIX tmp1;
    SVECTOR v1;
    int i,j;

    tmp1   = GsIDMATRIX;		/* 単位行列から出発する */
    /* 平行移動をセットする */
    tmp1.t[0] = coor->coord.t[0];
    tmp1.t[1] = coor->coord.t[1];
    tmp1.t[2] = coor->coord.t[2];

    /* マトリックスワークにセットされているローテーションを */
    /* ワークのベクターにセット */
    v1 = *pos;

    /* マトリックスにローテーションベクタを作用させる */
    RotMatrix(&v1,&tmp1);

    /* 求めたマトリックスを座標系にセットする */
    coor->coord = tmp1;

    /* マトリックスキャッシュをフラッシュする */
    coor->flg = 0;
}


texture_init(u_long *addr)
{
    RECT rect1;
    CVECTOR bgc;
    int i, j;

    /* Get texture information of TIM FORMAT*/
       /* TIMデータのヘッダからテクスチャのデータタイプの情報を得る */  
    GsGetTimInfo(addr+1,&tim1);
    
    rect1.x=tim1.px;		/* X point of image data on VRAM*/
				   /* テクスチャ左上のVRAMでのX座標 */
    rect1.y=tim1.py;		/* Y point of image data on VRAM*/
				   /* テクスチャ左上のVRAMでのY座標 */
    rect1.w=tim1.pw;		/* Width of image /* テクスチャ幅 */
    rect1.h=tim1.ph;		/* Height of image /* テクスチャ高さ */
  
    /* Load texture to VRAM *//* VRAMにテクスチャをロードする */
    LoadImage(&rect1,tim1.pixel);

    LoadClut(tim1.clut, tim1.cx, tim1.cy);
    crect1.x=640; crect1.y=256; crect1.w=256; crect1.h=1;
    bgc.r=BGR;
    bgc.g=BGG;
    bgc.b=BGB;
    make_clut(&crect1, (u_short *)tim1.clut, (u_short *)fclut, CNUM, &bgc);


    for (i=0; i<2; i++){
	for (j=0; j<CNUM; j++){
	    SetDrawMove(&mvi[i][j]);
	    mvi[i][j].x0=tim1.cx;
	    mvi[i][j].y0=tim1.cy;
	    mvi[i][j].w=tim1.cw;
	    mvi[i][j].h=tim1.ch;
	    mvi[i][j].sx=(fclut[j]&0x3f)<<4;
	    mvi[i][j].sy=fclut[j]>>6;
	}
    }

    for (i=0; i<4096; i++){
	pbuf[i]= -1;
    }
}

/* make fogged clut */
make_clut(RECT *rtim, u_short *ctim, u_short *cp, long num, CVECTOR *bgc)
{
    long r,g,b,stp;
    long p;
    long i,j;
    u_short newclut[256];
    RECT rect;

    rect.x=rtim->x;
    rect.w=rtim->w;
    rect.h=rtim->h;

    for (i=0; i<num; i++){
	p=i*ONE/num;

	for (j=0; j<rtim->w; j++){
	    if (ctim[j]==0){			/* 透明のときは内挿しない */
		newclut[j]=ctim[j];
	    } else{
		/* 元 CLUT を R,G,B に分解 */
		r= (ctim[j] & 0x1f)<<3;
		g= ((ctim[j] >>5) & 0x1f)<<3;
		b= ((ctim[j] >>10) & 0x1f)<<3;
		stp= ctim[j] & 0x8000;

		/* R,G,B それぞれを背景色と内挿 */
		/* 原色と背景色の内挿計算 */
		r= (((long)((u_long)r * (4096-p))) + (((u_long)bgc->r * p))>>15);
		g= (((long)((u_long)g * (4096-p))) + (((u_long)bgc->g * p))>>15);
		b= (((long)((u_long)b * (4096-p))) + (((u_long)bgc->b * p))>>15);
		/* FOG つき CLUT の再構成 */
		newclut[j]= stp |r| (g<<5) | (b<<10);
	    }
	}
	rect.y=rtim->y+i;

	/* VRAM に load */
	LoadImage(&rect,(u_long *)newclut);

	cp[i]= GetClut(rect.x, rect.y); /* x,y 転送先アドレス */
    }
}
