/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *          Program Execute MENU Sample Program
 *
 *      Copyright (C) 1995 by Sony Computer Entertainment Inc.
 *          All rights Reserved
 *
 *   Version    Date
 *  ------------------------------------------
 *  1.00        Mar,09,1995 yoshi
 *  1.01        Mar,15,1995 suzu
 *
 *===============================================================
 * 子プログラムの実行サンプルプログラム。ここでは、
 *	CD関数でリード後、Exec()で実行する方法（親と別の領域にロード）
 *	LoadExec()で実行する方法（親と重なる領域にロード）
 * の、２種類の方法を提示します。
 *---------------------------------------------------------------
 * child program execute sample. In this example, we present two way.
 *      1) use Exec() after loading by CD function.
 *      2) use LoadExec(). (child program is loaded over parent.)
 *===============================================================
 */
#include <sys/types.h>
#include <sys/file.h>
#include <kernel.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#define TITLE_X 120
#define TITLE_Y 32
#define MENU_X 260
#define MENU_Y 64
#define FSIZE 16
#define MARG 40

#define PAD_A_SELECT 0x00000100
#define PAD_A_START 0x00000800
#define PAD_A_UP 0x00001000
#define PAD_A_DOWN 0x00004000

#define H_SIZE 2048 /*  EXEfile header size :: EXEファイルのヘッダサイズ */

/* stack pointer of child EXE. :: 呼び出す子EXEのスタックポインタ */
#define STACKP 0x8017fff0	/*　over parent's stack :: 
					親のスタックと重ならない位置 */
#define STACKP2 0x801ffff0	/*  not over parent's stack ::
					親のスタックと重なる位置 */


static struct XF_HDR *head;     /*  header of child EXE :: 子EXEのヘッダ */

typedef struct demo {
  char *s;		/* demo title :: 画面表示デモタイトル */
  char *fn;		/* execute file name :: 実行ファイル名 */
  int	loadflag;	/* 0: run child EXE by cdload() & Exec() ::
				cdload() & Exec() で子EXEを起動 */
			/* 1: run child EXE by LoadExec() ::
				LoadExec()で子EXEを起動 */
} DMENU;

#define MENU_MAX 6

static DMENU dm[MENU_MAX] = {
  { "BALLS","\\EXECMENU\\BALLS.EXE;1",0 },
  { "RCUBE","\\EXECMENU\\RCUBE.EXE;1",0 },
  { "ANIM","\\EXECMENU\\ANIM.EXE;1",0 },
  { "BALLS2","\\EXECMENU\\BALLS2.EXE;1",1 },
  { "RCUBE2","\\EXECMENU\\RCUBE2.EXE;1",1 },
  { "ANIM2","\\EXECMENU\\ANIM2.EXE;1",1 }
/* { demo title, EXEfile name, load-flag } ::
	{ デモタイトル名、デモ起動ファイル名、起動種別フラグ } */
};

typedef struct {
  DRAWENV draw;
  DISPENV disp;
  long x,y;
} DB;


main()
{
  int	x, y, c;
  long pad,padd,padr;
  long i,j;
  char title[40];
  DB db[2];
  DB *cdb;
  unsigned char co1,co2,co3;	
  POLY_G4 g4;
  char fname[128];

  while(1){
    ResetCallback();
    CdInit();

    PadInit(0);
    ResetGraph(0);		/* reset graphic subsystem (0:cold,1:warm) */
    SetGraphDebug(0);		/* set debug mode */

    SetDefDrawEnv(&(db[0].draw), 0,   0, 640, 240);
    SetDefDrawEnv(&(db[1].draw), 0, 240, 640, 240);
    SetDefDispEnv(&(db[0].disp), 0, 240, 640, 240);
    SetDefDispEnv(&(db[1].disp), 0,   0, 640, 240);
    db[0].x = 0;
    db[0].y = 0;
    db[1].x = 0;
    db[1].y = 240;

    SetDispMask(1);

    padd = PadRead(1);
    InitFont(640, 0, 0, 481);
    strcpy(title,"PlayStation EXEC MENU");
    cdb = &db[0];
    co1 = 255;
    co2 = 0;
    co3 = 0;
    i = 0;

    while(1) {
      cdb = ( cdb == &db[1] ) ? &db[0] : &db[1];  
      PutDrawEnv(&cdb->draw);
      PutDispEnv(&cdb->disp);

      SetPolyG4(&g4);
      setXYWH(&g4, 0, 0, 640, 240);
      setRGB0(&g4, co1, co2, co3);
      setRGB1(&g4, co3, co1, co2);
      setRGB2(&g4, co2, co3, co1);
      setRGB3(&g4, co1, co2, co3);
      DrawPrim(&g4);

      if ( co3 == 0 && co1 > 0 ){
	co1--;
	co2++;
      }
      if ( co1 == 0 && co2 > 0 ){
	co2--;
	co3++;
      }
      if ( co2 == 0 && co3 > 0 ){
	co3--;
	co1++;
      }

      Puts(TITLE_X,TITLE_Y,title);
      for( j=0;j<MENU_MAX;j++ ){
	y = j*FSIZE + MENU_Y;
	Puts(MENU_X,y,dm[j].s);
      }

      if( i < 0 ) i = MENU_MAX-1;
      if( i >= MENU_MAX ) i = 0;
      y = i*FSIZE + MENU_Y;
      Puts(MENU_X-MARG,y,">>");

      padr = PadRead(1);
      pad = padr & ~padd;
      padd = padr;

      if (pad & PAD_A_START){
	if( !dm[i].loadflag ){
	  printf("cdload:%s\n",dm[i].fn);
	  /* load EXEfile by CD function. :: CD関数を用いてプログラムをロードする */
	  if (cdload(dm[i].fn)) goto error;
	  ResetGraph(3);
	  PadStop();
	  StopCallback();
	  /* child's stack pointer = STACKP :: 子のスタックポインタ = STACKP */
	  (head->exec).s_addr = STACKP;
	  (head->exec).s_size = 0;
	  EnterCriticalSection();
	  printf("EXEC!\n");
	  Exec(&(head->exec),1,0);
	  /* If escape from child's main(), it's returned here. ::
	     子のメインでリターンするとここに戻ってくる。しかし、その時
	     親が既に壊されてしまっている場合はハングアップする。*/
	  printf("RET!\n");
	  /* do again from initialization.::
	     初期化からやり直す */
	  break;
	}
	else{
	  strcpy(fname,"cdrom:");	/* need 'cdrom:' ::
						LoadExec()の場合は cdrom: が必要 */
	  strcat(fname,dm[i].fn);
	  ResetGraph(3);
	  PadStop();
	  StopCallback();
	  /* need _96_init(), because open() & read() are called in LoadExec(). ::
	     LoadExec()内部でopen(),read()を呼ぶため、ここで_96_init()が必要 */
	  _96_init();
	  printf("LoadExec:%s\n",fname);
	  /* child's stack pointer = STACKP2 :: 子のスタックポインタ = STACKP2 */
	  LoadExec(fname,STACKP2,0);
	  /* If escape from child's main(), it's returned here. ::
	     子のメインでリターンするとここに戻ってくる。しかし、その時
	     親が既に壊されてしまっている場合はハングアップする。*/
	  break;
	}
      }
      if (pad & PAD_A_UP)
	i--;
      if ( (pad & PAD_A_DOWN) || (pad & PAD_A_SELECT) )
	i++;
    error:
      VSync(0);
    }
  }

}


static DR_MODE  mode;
static SPRT_16  sprt;

InitFont(tx, ty, cx, cy)
int	tx, ty, cx, cy;
{
  extern u_long font[];
	
  u_short	tpage, clut;
	
  tpage = LoadTPage(font+0x80, 0, 1, tx, ty, 256,256);
  clut  = LoadClut(font, cx, cy);
	
  SetDrawMode(&mode, 0, 0, tpage, 0);
  SetSprt16(&sprt);
  SetSemiTrans(&sprt, 1);
  SetShadeTex(&sprt, 1);
  sprt.clut = clut;
}

	
Puts(x, y, s)
int	x, y;
char	*s;
{
  int	code, u, v;
	
  DrawPrim(&mode);
  while (*s) {
    code = *s - '0' + 48;
    v = (code%16)*16;
    u = (code/16)*16;
    setUV0(&sprt, u, v);
    setXY0(&sprt, x, y);
    DrawPrim(&sprt);
		
    s++;
    x += 16;
  }
}



int
cdload(file)

char *file;
{
  CdlFILE	fp;	/* file location & size :: ファイルの位置・サイズ情報 */
  int	mode = CdlModeSpeed;	
  char hbuf[H_SIZE];		/* buffer of EXE-header :: EXEヘッダ用バッファ */
  unsigned long offset;		/* sector location :: 絶対セクタ位置 */
  int cnt,i;

  for (i = 0; i < 10; i++) {	/* 10 times retry :: リトライは 10 回 */
    if (CdSearchFile(&fp, file) == 0) 
      continue;

    /* set target position :: ターゲットポジションを設定 */
    CdControl(CdlSetloc, (u_char *)&(fp.pos), 0);
    cnt = _read1(H_SIZE,(void *)hbuf,mode); /* read EXE-header :: EXEヘッダリード */
    if(cnt)
      continue;

    head = (struct XF_HDR *)hbuf;		

    printf("pc = %08x\n",(head->exec).pc0);
    printf("t_addr = %08x\n",(head->exec).t_addr);
    printf("t_size = %08x\n",(head->exec).t_size);

    /* 


    /* LOC情報を絶対セクタに直し、１セクタ（EXEファイルのヘッダ分）
       　インクリメントした後、再びLOC情報に直す */
    offset = CdPosToInt(&fp.pos);
    offset++;
    CdIntToPos(offset,&fp.pos);

    /* set target position :: ターゲットポジションを設定 */
    CdControlB(CdlSetloc, (u_char *)&(fp.pos), 0);
    /* read program :: プログラムをリード */
    cnt = _read1((head->exec).t_size,(void *)((head->exec).t_addr),mode);

    /* If it success, return. :: 正常終了ならばブレーク */
    if (cnt == 0)	return(0);
  }
  printf("%s: can not read\n", file);	
  return(0);
}


int
_read1(byte,sectbuf,mode)

long byte;
void *sectbuf;
int mode;
{	
  int nsector,cnt;
	
  nsector = (byte+2047) / 2048;
	
  /* read start :: リード開始 */
  CdRead(nsector, sectbuf, mode);
	
  while ((cnt = CdReadSync(1, 0)) > 0 ) {
    VSync(0);
  }

  return(cnt);
}
