/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="ds/tuto/tuto5.cpe";
#endif
/*
 *			tuto5: CD-XA repeat
 *
 *	Copyright(C) 1994 1997  Sony Computer Entertainment Inc.,
 *	All rights Reserved.
 *
 *		 Version	Date		Design
 *		-----------------------------------------
 *		1.00		Jul.30,1994	suzu
 *		1.10		Dec,27,1994	suzu
 *		2.00		Feb,02,1995	suzu
 *		3.00		Apr.23,1997	makoto
 *
 *			    Repeat Play
 *		auto repeat play among 2 point of CD-XA.
 *
 *	Since we cannot use report mode callback when playing CD-XA
 *	audio, we have to use DslGetloc command to monitor current
 *	position on CD-ROM. If CdControl is slow, CdControlF is
 *	possible.
 *
 :			    リピート再生
 *		CD-XA トラックの任意の２点間をリピートする。
 *
 *	CD-XA 再生時はレポートモードのコールバックが使用できない。
 *	そのため DslGetloc で場所を探す必要がある。CdControl() が重い
 *	場合は、CdControlF() を使用する。
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libds.h>

int dsRepeat( int startp, int endp );
int dsRepeatXA( int startp, int endp );
int dsGetPos( void );
int dsGetRepTime( void );

char title[] = "    SELECT MENU    ";
static char* menu[] = { "Play", "Pause", 0 };

static void print_gage( int startp, int endp, int curp );
static void notfound( char* file );

void main( void )
{
	char* file = "\\XA\\MULTI8.XA;1";

	DslFILE fp;
	DslLOC loc;
	DslFILTER filter;
	int padd, n, id;
	int startp, endp;

	/* initialize graphics and controller */
	ResetGraph( 0 );
	PadInit( 0 );
#if defined (APD_SAVE)
	APDSaveInit(0,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0,progname) ) return;
#endif
	menuInit( 0, 80, 88, 256 );
	SetDumpFnt( FntOpen( 16, 16, 320, 200, 0, 512 ) );

	/* initialize CD subsytem: CD サブシステムを初期化 */
	DsInit();
	DsSetDebug( 0 );

	/* get file position: ファイルの位置を確定 */
	if( DsSearchFile( &fp, file ) == 0 ) {
		notfound( file );
		goto abort;
	}

	/* XA チャンネルの設定 */
	filter.file = 1;
	filter.chan = 0;
	DsCommand( DslSetfilter, ( u_char* )&filter, 0, -1 );

	/* kick auto loop: 自動ループスタート */
	startp = DsPosToInt( &fp.pos );
	endp = startp + fp.size / 2048;
	dsRepeatXA( startp, endp );

#if defined(APD_SAVE)
	while( ( ( padd = APDSave(PadRead( 1 )) ) & PADselect ) == 0 ) {
#elif defined(APD_LOAD)
	while( ( ( padd = APDLoad( 1 ) ) & PADselect ) == 0 ) {
#else
	while( ( ( padd = PadRead( 1 ) ) & PADselect ) == 0 ) {
#endif
		balls();

		id = menuUpdate( title, menu, padd );

		if( DsSystemStatus() == DslReady ) {
			switch( id ) {
			case 0:
				DsCommand( DslReadS, 0, 0, -1 );
				break;
			case 1:
				DsCommand( DslPause, 0, 0, -1 );
				break;
			}
		}

		/* print status:  ステータスの表示 */
		DsIntToPos( dsGetPos(), &loc );
		FntPrint( "\t\t XA-AUDIO Repeat\n\n" );
		FntPrint( "Use DTL-S2310 for this test.\n\n" );
		FntPrint( "position=(%02x:%02x:%02X)\n", loc.minute,
		  loc.second, loc.sector );
		FntPrint( "%s...\n", DsComstr( DsLastCom() ) );
		print_gage( startp, endp, DsPosToInt( &loc ) );

		FntFlush( -1 );
	}
	DsCommand( DslStop, 0, 0, -1 );

abort:
	DsControlB( DslPause, 0, 0 );
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

static void print_gage( int startp, int endp, int curp )
{
	int i = 0, rate;

	rate = 32 * ( curp - startp ) / ( endp - startp );

	FntPrint( "~c444" );
	while( i++ < rate )
		FntPrint( "*" );
	FntPrint( ( VSync( -1 ) >> 4 ) & 0x01 ? "~c888*~c444" : "*" );
	while( i++ < 32 )
		FntPrint( "*" );
}

static void notfound( char* file )
{
	int n = 60 * 4;

	while( n-- ) {
		balls();
		FntPrint( "\n\n%s: not found (waiting)\n", file );
		FntFlush( -1 );
	}
	printf( "%s: not found\n", file );
}
