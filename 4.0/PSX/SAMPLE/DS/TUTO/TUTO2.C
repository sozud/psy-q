/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="ds/tuto/tuto2.cpe";
#endif
/*
 *			tuto2: CD-DA repeat
 *
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------
 *		1.00		Sep.12,1994	suzu
 *		1.10		Oct,24,1994	suzu
 *		2.00		Feb,02,1995	suzu
 *		3.00		Apr.22,1997	makoto
 *
 *			     Repeat Play
 *	Auto repeat play among 2 points of CD audio disc using ReportMode.
 *
 :			    リピート再生
 *		D-DA トラックの任意の２点間をリピートする。
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

char* title = "    SELECT MENU    ";
static char* menu[] = { "Play", "Pause", "Forward", "Backward", 0 };

static void print_gage( int startp, int endp, int curp );

void main( void )
{
	DslLOC loc, toc[ 100 ];
	int n, padd;
	int startp, endp;

	/* initialize environment */
	ResetGraph( 0 );
	PadInit( 0 );
#if defined (APD_SAVE)
	APDSaveInit(0,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0,progname) ) return;
#endif

	menuInit( 0, 80, 88, 256 );
	SetDumpFnt( FntOpen( 32, 32, 320, 200, 0, 512 ) );

	DsInit();
	DsSetDebug( 0 );

	/* read TOC: TOC を読む */
	if( DsGetToc( toc ) <= 3 ) {
		printf( "Too few TOC: please use CD-DA disc...\n" );
		goto abort;
	}

	/* start playing: 読みだし（再生）開始 */
	startp = DsPosToInt( &toc[ 2 ] );
	endp = DsPosToInt( &toc[ 3 ] );
	dsRepeat( startp, endp );

#if defined(APD_SAVE)
	while( ( ( padd = APDSave(PadRead( 1 )) ) & PADselect ) == 0 ) {
#elif defined(APD_LOAD)
	while( ( ( padd = APDLoad( 1 ) ) & PADselect ) == 0 ) {
#else
	while( ( ( padd = PadRead( 1 ) ) & PADselect ) == 0 ) {
#endif
		balls();

		switch( menuUpdate( title, menu, padd ) ) {
		case 0:
			DsCommand( DslPlay, 0, 0, -1 );
			break;
		case 1:
			DsCommand( DslPause, 0, 0, -1 );
			break;
		case 2:
			DsCommand( DslForward, 0, 0, -1 );
			break;
		case 3:
			DsCommand( DslBackward, 0, 0, -1 );
			break;
		}

		DsIntToPos( dsGetPos(), &loc );
		FntPrint( "CD-DA REPEAT (report mode)\n\n" );
		FntPrint( "Use Audio DISC for this test\n\n" );
		FntPrint( "position=(%02x:%02x:%02X)\n",
		  loc.minute, loc.second, loc.sector );
		FntPrint( "%s..\n", DsComstr( DsLastCom() ) );
		print_gage( startp, endp, DsPosToInt( &loc ) );

		FntFlush( -1 );
	}

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
