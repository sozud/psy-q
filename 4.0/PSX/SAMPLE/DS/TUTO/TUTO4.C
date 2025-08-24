/* $PSLibId: Run-time Library Release 4.0$ */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="ds/tuto/tuto4.cpe";
#endif
/*
 *			tuto4: DsControlF
 *
 *	Copyright(C) 1994 1997  Sony Computer Entertainment Inc.,
 *	All rights Reserved.
 *
 *		Version		Date		Design
 *		-----------------------------------------
 *		1.00		Jul.07,1995	suzu
 *		2.00		Apr.23,1997	makoto
 *
 *			DsControlF
 *
 *	DsControlF() is fast because it never waits for acknowledge from
 *	CD-ROM subsystem. We can apply this strategy for CD-XA repeat play.
 *	But to write a reliable code using DsControlF is a little complicate.
 *
 :	DsControlF() はサブシステムとハンドシェークをしないので高速。
 *	またオートリピートを使用しないので CD-XA の再生にも適用できる。
 *	しかしその実装はかなり繁雑になる。
 *
 *	このサンプルは libds 向きのアルゴリズムではないが、敢えてその
 *	まま移植してある。
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libds.h>

static void play( int ipos );
static void cbvsync( void );

void main( void )
{
	int max_time, min_time, time;
	int next_v = 0;
	u_long padd;
	DslLOC loc[ 100 ];
	u_char param[ 4 ], result[ 8 ];

	/* initialize */
	ResetGraph( 0 );
	PadInit( 0 );
#if defined (APD_SAVE)
	APDSaveInit(0,progname);
#elif defined (APD_LOAD)
	if ( APDLoadInit(0,progname) ) return;
#endif
	FntLoad( 960, 256 );
	SetDumpFnt( FntOpen( 32, 32, 320, 200, 0, 512 ) );

	DsInit();
	DsSetDebug( 0 );

	/* read TOC: TOC を読む */
	if( DsGetToc( loc ) == 0 ) {
		printf( "No TOC found: please use CD-DA disc...\n" );
		goto abort;
	}

	/* play between 2nd track and the las track
	 : ２トラック－最終トラックで演奏
	 */
	min_time = DsPosToInt( &loc[ 2 ] );
	max_time = DsPosToInt( &loc[ 0 ] );

	/* set mode: モード設定 */
	param[ 0 ] = DslModeDA;
	DsControl( DslSetmode, param, 0 );

	/* start the first track */
	play( 0 );

	/* hook callback: コールバック設定 */
	VSyncCallback( cbvsync );

#if defined(APD_SAVE)
	while( ( ( padd = APDSave(PadRead( 1 )) ) & PADselect ) == 0 ) {
#elif defined(APD_LOAD)
	while( ( ( padd = APDLoad( 1 ) ) & PADselect ) == 0 ) {
#else
	while( ( ( padd = PadRead( 1 ) ) & PADselect ) == 0 ) {
#endif
		balls();

		/* track jump in every 5 sec: 5 sec ごとにランダム再生 */
		if( VSync( -1 ) > next_v ) {
			time = min_time
			  + ( rand() << 5 ) % ( max_time - min_time );
			play( time );
			next_v = VSync( -1 ) + 5 * 60;
		}
		/* print status: ステータスの表示 */
		FntPrint( "CD-DA REPEAT (ControlF)\n\n" );
		FntPrint( "Use Audio DISC for this test\n\n" );
		FntPrint( "Zapping Play (%d sec)\n",
		  ( next_v - VSync( -1 ) ) / 60 );

		/* If you don't want to use VSyncCallback() interrupt,
		 * call callback function here by yourself.
		 : VSyncCallback() を使用しない時はここでポーリング
		 */
		/* cbvsync(); */

		FntFlush( -1 );
	}

abort:
	VSyncCallback( 0 );
	DsControlB( DslPause, 0, 0 );
	DsClose();
	PadStop();
#if defined (APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDLoadStop(VSync(-1));
#endif
	ResetGraph( 1 );
	StopCallback();
	return;
}

static int is_play = 0;		/* play request */
static DslLOC pos;		/* play position */
static u_char p_com = 0;	/* previous command */
static u_char result[ 8 ];	/* result */
static int errcnt = 0;		/* debug */
static int p_id = 0;		/* command id */

/*
 * register the CD play request (which will be executed at the next V-BLNK
 : Play 要求を登録する
 */
static void play( int ipos )
{
	DsIntToPos( ipos, &pos );
	is_play = 1;
}

/*
 * status flow which is called in V-BLNK.
 : Vsync の度に呼ばれるステータスマシン
 * 状態遷移図を書くとわかり易い。
 */
static void cbvsync( void )
{
	int ret;

	switch( p_com ) {
	case 0:		/* IDLE -> Nop or Setloc */
		/* FntPrint( "IDLE(%d)\n" ); */
		if( is_play )
			p_id = DsControlF( ( p_com = DslSetloc ),
			  ( u_char* )&pos );
		else {
			int f = DsSetDebug( 0 );
			p_id = DsControlF( ( p_com = DslNop ), 0 );
			DsSetDebug( f );
		}
		break;

	case DslSetloc:	/* Setloc -> Play */
#if 0		/* for debug */
		FntPrint( "(%02x:%02x:%02x):%d\n",
		  pos.minute, pos.second, pos.sector, VSync( -1 ) );
#endif
		if( ( ret = DsSync( p_id, result ) ) == DslComplete )
			p_id = DsControlF( ( p_com = DslPlay ), 0 );
		else if( ret == DslDiskError ) {
			errcnt++;
			p_com = 0;
		}
		break;

	case DslPlay:	/* Play -> IDLE */
		if( ( ret = DsSync( p_id, result ) ) == DslComplete ) {
			p_com = 0;
			is_play = 0;
		} else if( ret == DslDiskError ) {
			errcnt++;
			p_com = 0;
		}
		break;

	case DslNop:	/* Nop -> IDLE */
		if( DsSync( p_id, result ) ) {
			if( ( result[ 0 ] & ( DslStatPlay | DslStatSeek ) )
			  == 0 )
				is_play = 1;
			p_com = 0;
		}
		break;

	default:
		printf( "%s: unexpected command\n", CdComstr( p_com ) );
		p_com = 0;
	}
#if 0	/* for debug */
	FntPrint( "pos=(%02x:%02x:%02x), stat=%02x, errcnt=%d\n",
	  pos.minute, pos.second, pos.sector, result[ 0 ], errcnt );
#endif
}
