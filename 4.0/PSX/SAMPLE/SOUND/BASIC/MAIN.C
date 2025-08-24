/* ----------------------------------------------------------------
 *   sound sample...basic 
 * ----------------------------------------------------------------*/
/*
 * $PSLibId: Run-time Library Release 4.0$
 */
#if defined(APD_SAVE) || defined(APD_LOAD)
	static char	*progname="sound/basic/basic.cpe";
#endif
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libsnd.h> 
#ifdef __psx__
#include <libsn.h> 
#else
#define pollhost()
#endif

#define NOTICK

int   end_flag;

short seq1;  /* SEQ data id */
short sep1;  /* SEP data id */
short vab1;  /* VAB data id */

char seq_table[SS_SEQ_TABSIZ * 2 * 3];

/* ----------------------------------------------------------------
 *    For controller @@@
 *    コントローラ設定
 * ----------------------------------------------------------------*/
void
padToCallSnd (unsigned long padd)
{
	static u_long opadd = 0;     /* a previous pad data */
	static int h_flag = 0;
	static int rl_flag = 0;
	static int rr_flag = 0;

	if (opadd == padd)
		return;
	
        if (padd & PADh) {
		if (h_flag == 0) {
               		SsSeqPlay (seq1, SSPLAY_PLAY, 3);
			h_flag = 1;
		}
	}

        if (padd & PADRleft) { 
		if (rl_flag == 0) {
			SsSepPlay (sep1, 2, SSPLAY_PLAY, 2);
			SsSeqSetDecrescendo (seq1, 50, 200);
			rl_flag = 1;
		}
        }

        if (padd & PADRright)  {
		if (rr_flag == 0) {
			SsSepPlay (sep1, 1, SSPLAY_PLAY, 1);
			rr_flag = 1;
		}
	}

        if (padd & PADRdown) {
		SsSepStop (sep1,2);
		SsSeqStop (seq1);
		h_flag = 0;
		rl_flag = 0;
        }

        if (padd & PADk)
                end_flag = 1;
	
	opadd = padd;
}

main (void)
{
        unsigned long padd;

	end_flag = 0;
	ResetCallback();

/* ----------------------------------------------------------------
 *    Initialize Control pad @@@
 *    コントロールパッド の初期化
 * ---------------------------------------------------------------- */

#if defined(APD_SAVE)
	APDSaveInit(0,progname);
#elif defined(APD_LOAD)
	if ( APDLoadInit(0,progname) ) return 0;
#endif
#ifndef APD_LOAD
    	PadInit (0);
#endif

/* ----------------------------------------------------------------
 *    Initialize Graphics system @@@
 *    グラフィックスの初期化 
 * ---------------------------------------------------------------- */

	ResetGraph(0);

/* ----------------------------------------------------------------
 *    Initialize Sound system @@@
 *    サウンドの初期化 
 * ---------------------------------------------------------------- */

	SsInit ();

/* ----------------------------------------------------------------
 *    Set Table size of data attributes @@@
 *    データ属性テーブル領域の設定
 * ---------------------------------------------------------------- */

	SsSetTableSize (seq_table, 2, 3);

/* ----------------------------------------------------------------
 *    Set Tick  @@@
 *    Tick の設定
 * ---------------------------------------------------------------- */

#ifdef NOTICK
	SsSetTickMode (SS_NOTICK);
#else 
	SsSetTickMode (SS_TICK60);
#endif

/* ----------------------------------------------------------------
 *    Open & transfer VAB data @@@
 *    VAB データのオープン，転送
 * ---------------------------------------------------------------- */

        vab1 = SsVabOpenHead ((unsigned char*)0x80030000L, -1);
	if( vab1 == -1 ) {
	  printf( "VAB headder open failed\n" );
	  exit(0);
	}
        vab1 = SsVabTransBody ((unsigned char*)0x80032a20L, vab1);
	if( vab1 == -1 ) {
	  printf( "VAB body open failed\n" );
	  exit(0);
	}
	SsVabTransCompleted (SS_WAIT_COMPLETED);

/* ----------------------------------------------------------------
 *    Start Sound System @@@
 *    サウンドシステムの開始
 * ---------------------------------------------------------------- */

#ifndef NOTICK
	SsStart ();

#endif

/* ----------------------------------------------------------------
 *    Open SEQ/SEP datas @@@
 *    SEQ/SEP データのオープン
 * ---------------------------------------------------------------- */

	seq1 = SsSeqOpen ((unsigned long *)0x80015000, vab1); 
	sep1 = SsSepOpen ((unsigned long *)0x80010000, vab1, 3); 

	SsSetMVol (127, 127);
	SsSeqSetVol (seq1, 127, 127);
	SsSepSetVol (sep1, 1, 80, 80);
	SsSepSetVol (sep1, 2, 127, 127);
	SsSepSetVol (sep1, 3, 127, 127);
	while (!end_flag) {
#ifdef APD_LOAD
		APDSetCnt(VSync(0));
#else
		VSync (0);
#endif
#if 0
		pollhost ();
#endif
#ifdef NOTICK
		SsSeqCalledTbyT ();
#endif
#if defined(APD_SAVE)
		padd = APDSave(PadRead(0));
#elif defined(APD_LOAD)
		padd = APDLoad(0);
#else
	    	padd = PadRead (0);
#endif
		padToCallSnd (padd);
	}

/* ----------------------------------------------------------------
 *    Close SEQ/SEP,VAB datas @@@
 *    SEQ/SEP および VAB データのクローズ
 * ---------------------------------------------------------------- */

        SsSeqClose (seq1);
        SsSepClose (sep1);
        SsVabClose (vab1);

/* ----------------------------------------------------------------
 *    Stop & Quit Sound System @@@
 *    サウンドシステムの停止，終了
 * ---------------------------------------------------------------- */

        SsEnd ();
	SsQuit ();

#ifndef APD_LOAD
        PadStop ();
#endif
#if defined(APD_SAVE)
	APDSaveStop();
#elif defined(APD_LOAD)
	APDLoadStop(VSync(-1));
#endif
	ResetGraph(3);
	StopCallback();
	return 0; 
}

/* ----------------------------------------------------------------
 *    End of File
 * ---------------------------------------------------------------- */
