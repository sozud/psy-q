/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="ds/tuto/tuto8.cpe";
#endif
/*
 *			tuto8: multi DsRead
 *
 *	Copyright(C) 1994 1997  Sony Computer Entertainment Inc.,
 *	All rights Reserved.
 *
 *		 Version	Date		Design
 *		-----------------------------------------
 *		1.00		Oct.16,1994	suzu
 *		1.20		Mar,12,1995	suzu
 *		2.00		Jul,20,1995	suzu
 *		3.00		Apr.25,1997	makoto
 *
 *			multi file DsRead
 *		read many files from CD-ROM in background
 :			      分割リード
 *	 一つのファイルを複数回に分割してバックグラウンドでリードする
 *
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libds.h>

void bgFlush( void );
void bgUpdate( u_char col );

void sndInit( void );
void sndMute( int mode );
void sndEnd( void );

static void read_test( void );
static void notfound( void );
static void cbvsync( void );

/* for long term test: 耐久試験 */

char file[] = "\\DATA\\MOV.STR;1";

void main( void )
{
	/* initialize: 初期化 */
	ResetGraph( 0 );
	PadInit( 0 );
#if defined (APD_SAVE)
	APDSaveInit(0,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0,progname) ) return;
#endif
	FntLoad( 960, 256 );
	SetDumpFnt( FntOpen( 16, 64, 0, 0, 1, 512 ) );

	DsInit();
	DsSetDebug( 0 );

	sndInit();

	VSyncCallback( cbvsync );
	SetDispMask( 1 );

#if defined(APD_SAVE)
	while( ( APDSave(PadRead( 1 ) ) & PADselect ) == 0 )
#elif defined(APD_LOAD)
	while( ( APDLoad( 1 ) & PADselect ) == 0 )
#else
	while( ( PadRead( 1 ) & PADselect ) == 0 )
#endif
		read_test();

	/* ending: 終了処理 */
	PadStop();
#if defined (APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDSetCnt(VSync(0));
	APDLoadStop(VSync(-1));
#endif
	DsClose();
	StopCallback();
	return;
}

/* DsRead() reads NSECTOR sectors each, and it is invoked NREAD times
 * in the background, so MAXSECTOR sectors is read at last.
 : DsRead() は一回に NSECTOR 読み込み、これは NREAD 回バックグラウンドで
 * 実行される。合計では、最大 MAXSECTOR が読み込まれることになる。
 */
#define MAXSECTOR	256
#define NSECTOR		32
#define NREAD		( MAXSECTOR / NSECTOR )

int dsReadChainSync( void );
int dsReadChain( DslLOC* _postbl, int* _scttbl, u_long** _buftbl, int _ntbl );

#define StatREADIDLE	0
#define StatREADBURST	1
#define StatREADCHAIN	2

static int read_stat = StatREADIDLE;

static void read_test( void )
{
	static u_long sectbuf[ 2 ][ MAXSECTOR * 2048 / 4 ];
	static int errcnt = 0;

	/* The positions, sector numbers and destination buffer pointers
	 * are indicated through following queue arrays.
	 * dsReadChain() reads the data from CD-ROM according to this
	 * instruction queue.
	 : ファイルの先頭位置、セクタ数、バッファアドレスは、以下の配列
	 * を経由して dsReadChain() 関数に渡される。dsReadChain() はこれを
	 * 使用してデータを CD-ROM より読み込む
	 */
	DslLOC postbl[ NREAD ];		/* table for the position on CD-ROM */
	int scttbl[ NREAD ];		/* table for sector number */
	u_long* buftbl[ NREAD ];	/* table for destination buffer */

	int i, j, cnt, ipos;
	DslLOC pos;
	DslFILE fp;
	int nsector;

	/* notify to callback : コールバックに状態を知らせる */
	read_stat = StatREADIDLE;

	/* clear destination buffer: リードバッファを一旦クリア */
	for( i = 0; i < sizeof( sectbuf[ 0 ] ) / 4; i++ ) {
		sectbuf[ 0 ][ i ] = 0;
		sectbuf[ 1 ][ i ] = 1;
	}

	/* get the file postion and size ; ファイルの位置とサイズを検索 */
	if( DsSearchFile( &fp, file ) == 0 ) {
		notfound();
		return;
	}

	if( ( nsector = ( fp.size + 2047 ) / 2048 ) > MAXSECTOR )
		nsector = MAXSECTOR;

	/* notify to callback : コールバックに状態を知らせる */
	read_stat = StatREADBURST;

	/* read in one time: まとめて読む */
	DsRead( &fp.pos, nsector, sectbuf[ 0 ], DslModeSpeed );

	/* wait for end of reading: 読み込み終了を待つ */
	while( ( cnt = DsReadSync( 0 ) ) > 0 );

	/* notify to callback : コールバックに状態を知らせる */
	read_stat = StatREADCHAIN;

	/* get the file postion and size ; ファイルの位置とサイズを検索 */
	if( DsSearchFile( &fp, file ) == 0 ) {
		FntPrint( "%s: cannot open\n", file );
		return;
	}
	if( ( nsector = ( fp.size + 2047 ) / 2048 ) > MAXSECTOR )
		nsector = MAXSECTOR;

	/* make tables for dsReadChain(): リードテーブルを作成する */
	ipos = DsPosToInt( &fp.pos );
	for( i = j = 0; i < nsector; i += NSECTOR, ipos += NSECTOR, j++ ) {
		DsIntToPos( ipos, &postbl[ j ] );
		scttbl[ j ] = NSECTOR;
		buftbl[ j ] = &sectbuf[ 1 ][ i * 2048 / 4 ];
	}

	/* start chained DsRead: チェーンリード */
	dsReadChain( postbl, scttbl, buftbl, nsector / NSECTOR );

	/* wait for end of reading: 読み込み終了を待つ */
	while( ( cnt = dsReadChainSync() ) > 0 );

	/* notify to callback : コールバックに状態を知らせる */
	read_stat = StatREADIDLE;

	/* compare: 比較する */
	for( i = 0; i < nsector * 2048 / 4; i++ ) {
		if( sectbuf[ 0 ][ i ] != sectbuf[ 1 ][ i ] ) {
			printf( "verify ERROR at (%08x:%08x)\n\n",
			  &sectbuf[ 0 ][ i ], &sectbuf[ 1 ][ i ] );
			errcnt++;
			break;
		}
	}
	FntPrint( "verify done(err=%d)\n", errcnt );
}

static void notfound( void )
{
	int n = 60 * 4;
	while( n-- ) {
		FntPrint( "\n\n%s: not found\n", file );
		FntFlush( -1 );
#ifdef APD_LOAD
		APDSetCnt(VSync( 0 ));
#else
		VSync( 0 );
#endif
	}
	printf( "%s: not found\n", file );
}

/*
 *	VSync Callback:
 *	Drawing function is automatically called by V-BLNK interrupt
 *	to avoid flicker of the background animation.
 *	Note that callback should return immediately becausse all
 *	other callbacks are suspended during that period.
 *	callback returns.
 *
 :	描画関数は垂直同期割り込みで自動的に起動され背景画面の更新が乱
 *	れるのを防ぐ。コールバック処理中は他のコールバックが全て待たさ
 *	れるので処理を終了したら直ちにリターンすること
 */
static void cbvsync( void )
{
	FntPrint( "\t\t CONTINUOUS DsRead\n\n" );
	FntPrint( "Use DTL-S2310 DISC FOR THIS TEST\n\n" );
	FntPrint( "file:%s\n\n", file );
	switch( read_stat ) {
	case StatREADIDLE:
		FntPrint( "idle...\n" );
		break;

	case StatREADBURST:
		FntPrint( "Burst Read....%d sectors\n", DsReadSync( 0 ) );
		break;

	case StatREADCHAIN:
		FntPrint( "DIvided Read ...%d blocks\n", dsReadChainSync() );
		break;
	}
	bgUpdate( 96 );
	FntFlush( -1 );
}
