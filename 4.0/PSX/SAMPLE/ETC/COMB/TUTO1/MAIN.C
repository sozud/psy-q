/* $PSLibId: Run-time Library Release 4.0$ */
/*
 * File:main.c
 *
 * 同期通信サンプル
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <kernel.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libsn.h>
#include <libcomb.h>
#include "balls.h"

#define	PIH		320
#define	PIV		240
#define	OTLEN		16
#define	BUFFSIZE	256

static int	side;
static long	ot[2][OTLEN];
static DISPENV	disp[2];
static DRAWENV	draw[2];
static unsigned long fr, fw;
static char recbuf[BUFFSIZE];
static char senbuf[BUFFSIZE];
static long sencnt, reccnt;
static long hcnt = 0;
static long master = 0;
/*************************************************************************/
static int sync_read_write( int mode, char* recbuf, char* senbuf, long size );
static int sync_vsync_trger( int side );
/*************************************************************************/
void main(void)
{
	/* システム環境初期化 :initialize system resource */
	SetDispMask(0);
	ResetGraph(0);
	SetGraphDebug(0);
	ResetCallback();
	PadInit(0);
	ExitCriticalSection();

	/* 描画環境設定 :initialize environment for double buffer */
	SetDefDrawEnv(&draw[0], 0,   0, PIH, PIV);
	SetDefDrawEnv(&draw[1], 0, PIV, PIH, PIV);
	SetDefDispEnv(&disp[0], 0, PIV, PIH, PIV);
	SetDefDispEnv(&disp[1], 0,   0, PIH, PIV);
	draw[0].isbg = draw[1].isbg = 1;
	setRGB0( &draw[0], 0, 0, 64 );
	setRGB0( &draw[1], 0, 0, 64 );
	PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

	/* init balls module */
	_make_balls_data();

	/* font system */
	FntLoad(960, 256);		/* load basic font pattern */
	SetDumpFnt(FntOpen(16, 16, 256, 200, 0, 512));

	/* 対戦ドライバ初期化 */
	AddCOMB();
	_comb_control( 1,3, 2073600 );	/* 通信速度設定 */
	_comb_control( 1,4, 4 );	/* 通信単位文字数設定 */
	fw = open("sio:", O_WRONLY);
	fr = open("sio:", O_RDONLY);

	/* 相手局の電源オン待ち */
	while((_comb_control( 0,0,0 )&COMB_DSR)!=0 );

	 /* 相手局と同期をとる(master,slaveの調停) */
	if(_comb_control( 3,1,0 )!=0) {
		_comb_control( 3,0,1 );
		_comb_control( 5,0,0 );
		master = 0;
	} else {
		_comb_control( 3,0,1 );
		while(_comb_control( 3,1,0 )==0 );
		_comb_control( 5,0,0 );
		master = 1;
	}
	VSync(0);
	_comb_control( 3,0,0 );
	VSync(0);
	SetDispMask(1);

	side = 0;
	sencnt = reccnt = 0;

	while(1) {
		ClearOTag( ot[side], OTLEN );

		/* データの送受信 */
		sync_read_write( master, recbuf, senbuf, BUFFSIZE );
		reccnt++; sencnt++;

		_draw_balls_data( side, ot[side] );
		hcnt = VSync(0);

		/* 定期的に相手局と垂直同期を合わせる */
		if( sync_vsync_trger( side )) continue;

		side ^= 1;
		PutDispEnv(&disp[side]);
		PutDrawEnv(&draw[side]);
		DrawOTag( ot[side^1] );
		FntPrint("\nREMOTE CONTROLLER\n\n");
		FntPrint("SYNCRONOUS READ\n");
		FntPrint("SYNCRONOUS WRITE\n\n");
		FntPrint("SEND     %d\nRECEIVE  %d\n", sencnt, reccnt );
		FntPrint("MODE     %s\n", master ? "MASTER":"SLAVE" );
		FntPrint("HSYNC    %d\n", hcnt );
		FntFlush(-1);
	}
}

int sync_read_write( int mode, char* recbuf, char* senbuf, long size )
{
	if( mode ) {
		/* マスター側の処理 */
		write( fw, senbuf, BUFFSIZE );
		read( fr, recbuf, BUFFSIZE );
	} else {
		/* スレーブ側の処理 */
		read( fr, recbuf, BUFFSIZE );
		write( fw, senbuf, BUFFSIZE );
	}
	return(0);
}

/* ３分おきに相手局と垂直同期を合わせる */
#define	_REFRESH_INTERVAL	(3*60*60)
int sync_vsync_trger( int side )
{
	RECT	rect;
	static long refreshcount = _REFRESH_INTERVAL;

	if( refreshcount ) {
		refreshcount--;
		return(0);
	}

	ResetGraph(1);
	_comb_control(3,0,1);
	if( _comb_control( 3,1,0 )==0) {
		while(_comb_control( 3,1,0 )==0);
	}

	SetDispMask(0);
	_comb_control( 5,0,0 );
	setRECT( &rect, 0, 0, PIH, 480 );
	ClearImage(&rect,0,0,0);
	refreshcount = _REFRESH_INTERVAL;
	_comb_control(3,0,0);
	VSync(0);
	SetDispMask(1);

	return(1);
}
