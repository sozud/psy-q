/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="ds/tuto/tuto9.cpe";
#endif
/*
 *			tuto9: Load and Exec
 *
 *	Copyright(C) 1994 1997  Sony Computer Entertainment Inc.,
 *	All rights Reserved.
 *
 *		 Version	Date		Design
 *		-----------------------------------------
 *		1.00		May.16,1995	suzu
 *		2.00		Apr.25,1997	makoto
 *
 *		      Load and execute programs
 *
 *	This program reads the executable data from CD-ROM and execute
 *	it as a child process. The process returns to parent when child
 *	is over. Because child program usually starts from 0x80010000,
 *	parent should be loaded not to share the same memomry area.
 *
 :	CD-ROM 上の実行形式プログラムを読みだし、子プロセスとして起動
 *	する。子プロセス終了後は親（このプログラム）へリターンする
 *	子プロセスは大抵 0x80010000 からロードされるので親プログラムは、
 *	そのアドレスを避けてロード・実行する必要がある。
 */

#include <sys/types.h>
#include <libds.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <kernel.h>

/* menu title */
char title[] = "    Exec test: Use DTL-S2320    ";

/* file name table to execute: 実行するファイルのリスト */
static char* file[] = {
#if defined(APD_SAVE) || defined(APD_LOAD)
	"\\EXECMENU\\RCUBEL.EXE;1",
	"\\EXECMENU\\ANIML.EXE;1",
	"\\EXECMENU\\BALLSL.EXE;1",
#else
	"\\EXECMENU\\RCUBE.EXE;1",
	"\\EXECMENU\\ANIM.EXE;1",
	"\\EXECMENU\\BALLS.EXE;1",
#endif
	0,
};

/* callback parameters */
static int is_exec = 0;				/* indicate executing */
static int menu_id = -1;			/* menu ID */

static void cbvsync( void );
int dsExec( char* file, int mode );

static void cbvsync( void )
{
	/* change drawing contents according to dsExec execution.
	 : dsExec() を実行中かどうかで画面を切替える
	 */
	if( is_exec == 0 ) {
		bgUpdate( 96 );
#if defined(APD_SAVE)
		menu_id = menuUpdate( title, file, APDSave(PadRead( 1 )) );
#elif defined(APD_LOAD)
		menu_id = menuUpdate( title, file, APDLoad( 1 ) );
#else
		menu_id = menuUpdate( title, file, PadRead( 1 ) );
#endif
	} else {
		bgUpdate( 48 );
		FntPrint( "loading...\n" );
		FntFlush( -1 );
	}
}

/*
 * Read 中に画面が停止するのは好ましくないので、読み込み中にコールバッ
 * ク内で簡単な描画やサウンドの再生を行なう
 */
void main( void )
{
	int id;
	DRAWENV draw;
	DISPENV disp;
	u_long padd;

	ResetGraph( 0 );
	PadInit( 0 );
#if defined (APD_SAVE)
	APDSaveInit(0x80550000,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0x80550000,progname) ) return 0;
#endif

	DsInit();
	DsSetDebug( 0 );

	menuInit( 0, 32, 88, 512 );	/* initialize menu */

	/* draw background and menu system in the VSync callback
	 : VSync のコールバック内で描画を起動する
	 */
	VSyncCallback( cbvsync );

	/* play background music by SPU
	 : バックグラウンドで BGM を再生する
	 */
	sndInit();
	SetDispMask( 1 );		/* start display */

	/* initialize graphics */
#if defined(APD_SAVE)
	while( ( ( padd = APDSave(PadRead( 1 )) ) & PADselect ) == 0 ) {
#elif defined(APD_LOAD)
	while( ( ( padd = APDLoad( 1 ) ) & PADselect ) == 0 ) {
#else
	while( ( ( padd = PadRead( 1 ) ) & PADselect ) == 0 ) {
#endif

#ifdef APD_LOAD
		APDSetCnt(VSync( 0 ));
#else
		VSync( 0 );
#endif
		if( ( id = menu_id ) != -1 ) {
			/* execute: プログラムを実行する */
			is_exec = 1;	/* notify to callback */
			dsExec( file[ id ], 0 );
			is_exec = 0;	/* notify to callback */

			/* clear PAD */
			do {
#ifdef APD_LOAD
				APDSetCnt(VSync( 0 ));
#else
				VSync( 0 );
#endif
#if defined(APD_SAVE)
			} while( APDSave(PadRead( 1 )) );
#elif defined(APD_LOAD)
			} while( APDLoad( 1 ) );
#else
			} while( PadRead( 1 ) );
#endif
		}
	}
	printf( "ending...\n" );
	sndEnd();
	DsClose();
	ResetGraph( 1 );
	PadStop();
#if defined (APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDLoadStop(VSync(-1));
#endif
	StopCallback();
	return;
}

/***************************************************************************
 *
 *dsExec	Read an executable file form CD-ROM and execute it.
 *
 * SYNOPSIS
 *	EXEC *dsExec(char *file, int mode)
 *
 * ARGUMENT
 *	file	file name of the executable module
 *	mode	execution mode (reserved)
 *
 * DESCRIPTION
 *	dsExec() reads an executable file named 'file' and execute it.
 *
 * BUGS
 *	"mode" is invalid now
 *
 * RETURN
 *	none
 *
 : dsExec	実行可能データをロードして実行
 *
 * 形式	EXEC *dsExec(char *file, int mode)
 *
 * 引数		file	実行ファイル名
 *		mode	実行モード（予約）
 *
 * 解説		位置 file より実行可能データをリードして、これを子プロセス
 *		として実行する。
 *
 * 備考
 *		mode は現在は見ていない
 *
 * 返り値	なし
 *
 ***************************************************************************/

int dsExec( char* file, int mode )
{
	static u_long headbuf[ 2048 / 4 ];
	static struct XF_HDR* head = ( struct XF_HDR* )headbuf;

	DRAWENV draw;
	DISPENV disp;
	DslFILE fp;
	DslLOC p0, p1;

	/* read header of EXE file */
	printf( "loading %s...\n", file );
	if( DsReadFile( file, ( u_long* )head, 2048 ) == 0 ) {
		printf( "%s: cannot open\n", file );
		return 0;
	}
	while( DsReadSync( 0 ) );

	head->exec.s_addr = 0;
	head->exec.s_size = 0;

	/* load contents of EXE file to t_addr */
	DsReadFile( 0, ( u_long* )head->exec.t_addr, 0 );
	while( DsReadSync( 0 ) );
	printf( "executing %s at %08x...\n", file, head->exec.t_addr );

	/* execute */
	GetDispEnv( &disp );		/* push DISPENV */
	GetDrawEnv( &draw );		/* push DRAWENV */
	SetDispMask( 0 );		/* black out */
	DsClose();			/* flush CD-ROM */
	PadStop();			/* stop PAD */
	ResetGraph( 1 );		/* flush GPU */
	StopCallback();			/* stop callback */
	Exec( &head->exec, 1, 0 );	/* execute */
	RestartCallback();		/* recover callback */
	PadInit( 0 );			/* recover PAD */
	DsInit();			/* recover CD */
	PutDispEnv( &disp );		/* pop DISPENV */
	PutDrawEnv( &draw );		/* pop DRAWENV */
	SetDispMask( 1 );		/* recover dispmask */

	return head->exec.t_addr;
}
