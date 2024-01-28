/* ----------------------------------------------------------------
 *   sound sample...basic 
 * ----------------------------------------------------------------*/
/*
 * $PSLibId: Runtime Library Version 3.3$
 */
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
 *    �R���g���[���ݒ�
 * ----------------------------------------------------------------*/
void
padToCallSnd (unsigned long padd)
{
        static int key_pushed_h      = 0;
        static int key_pushed_j      = 0;
        static int key_pushed_i      = 0;
        static int key_pushed_l      = 0;
        static int key_pushed_n      = 0; 
	static int key_pushed_Rup    = 0;
        static int key_pushed_Rdown  = 0;
        static int key_pushed_Rleft  = 0;
        static int key_pushed_Rright = 0;
	static int key_pushed_Lup    = 0;
        static int key_pushed_Ldown  = 0;
        static int key_pushed_Lright = 0;
        static int key_pushed_Lleft  = 0;

        if (padd & PADh) {
                key_pushed_h = 1;
                key_pushed_Rdown = 0;
        	if (key_pushed_h == 1) {
               		SsSeqPlay (seq1, SSPLAY_PLAY, 3);
			key_pushed_h == 0;
        	}
	}
	key_pushed_h == 0;

        if (padd & PADRleft) {
                key_pushed_Rleft = 1;
                key_pushed_Rright = 0;
       		if (key_pushed_Rleft == 1) {
			SsSepPlay (sep1, 2, SSPLAY_PLAY, 2);
			SsSeqSetDecrescendo (seq1, 50, 200);
               		key_pushed_Rleft = 0;
               	}
	}

        if (padd & PADRright) { 
                key_pushed_Rright = 1;
                key_pushed_Rleft = 0;
          	if (key_pushed_Rright == 1) {
			SsSepPlay (sep1, 1, SSPLAY_PLAY, 1);
                	key_pushed_Rright = 0;
		}
	}

        if (padd & PADRdown) {
                key_pushed_Rdown = 1;
                key_pushed_h = 0;
        	if (key_pushed_Rdown == 1) {
			SsSepStop (sep1,2);
			SsSeqStop (seq1);
        	}
	}

        if (padd & PADk)
                end_flag = 1;
}

main (void)
{
        unsigned long padd;

	end_flag = 0;
	ResetCallback();

/* ----------------------------------------------------------------
 *    Initialize Control pad @@@
 *    �R���g���[���p�b�h �̏�����
 * ---------------------------------------------------------------- */

    	PadInit (0);

/* ----------------------------------------------------------------
 *    Initialize Graphics system @@@
 *    �O���t�B�b�N�X�̏����� 
 * ---------------------------------------------------------------- */

	ResetGraph(0);

/* ----------------------------------------------------------------
 *    Initialize Sound system @@@
 *    �T�E���h�̏����� 
 * ---------------------------------------------------------------- */

	SsInit ();

/* ----------------------------------------------------------------
 *    Set Table size of data attributes @@@
 *    �f�[�^�����e�[�u���̈�̐ݒ�
 * ---------------------------------------------------------------- */

	SsSetTableSize (seq_table, 2, 3);

/* ----------------------------------------------------------------
 *    Set Tick  @@@
 *    Tick �̐ݒ�
 * ---------------------------------------------------------------- */

#ifdef NOTICK
	SsSetTickMode (SS_NOTICK);
#else 
	SsSetTickMode (SS_TICK60);
#endif

/* ----------------------------------------------------------------
 *    Open & transfer VAB data @@@
 *    VAB �f�[�^�̃I�[�v���C�]��
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
 *    �T�E���h�V�X�e���̊J�n
 * ---------------------------------------------------------------- */

#ifndef NOTICK
	SsStart ();

#endif

/* ----------------------------------------------------------------
 *    Open SEQ/SEP datas @@@
 *    SEQ/SEP �f�[�^�̃I�[�v��
 * ---------------------------------------------------------------- */

	seq1 = SsSeqOpen ((unsigned long *)0x80015000, vab1); 
	sep1 = SsSepOpen ((unsigned long *)0x80010000, vab1, 3); 

	SsSetMVol (127, 127);
	SsSeqSetVol (seq1, 127, 127);
	SsSepSetVol (sep1, 1, 80, 80);
	SsSepSetVol (sep1, 2, 127, 127);
	SsSepSetVol (sep1, 3, 127, 127);
	while (!end_flag) {
		VSync (0);
		pollhost ();
#ifdef NOTICK
		SsSeqCalledTbyT ();
#endif
	    	padd = PadRead (0);
		padToCallSnd (padd);
	}

/* ----------------------------------------------------------------
 *    Close SEQ/SEP,VAB datas @@@
 *    SEQ/SEP ����� VAB �f�[�^�̃N���[�Y
 * ---------------------------------------------------------------- */

        SsSeqClose (seq1);
        SsSepClose (sep1);
        SsVabClose (vab1);

/* ----------------------------------------------------------------
 *    Stop & Quit Sound System @@@
 *    �T�E���h�V�X�e���̒�~�C�I��
 * ---------------------------------------------------------------- */

        SsEnd ();
	SsQuit ();

        PadStop ();
	ResetGraph(3);
	StopCallback();
	return 0; 
}

/* ----------------------------------------------------------------
 *    End of File
 * ---------------------------------------------------------------- */