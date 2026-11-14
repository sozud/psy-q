/*
 * $PSLibId: Run-time Library Release 4.3$
 */
/*
 *		Copyright (C) 1994-1997 by Sony Corporation
 *			All rights Reserved
 *		
 *		"control.h"
 */

#define CTLMAX 	16	/* Maximum number of control waveform channels :
			   制御波形の最大チャンネル数 */

#define CTLTIME 32	/* Maximum length of transfer function waveform :
			   伝達関数波形の最大長さ */

/* Control waveform-generating program structure :
   制御波形生成プログラム用構造体 */
typedef struct {
	int  *cnv;	/* Transfer function waveform data pointer :
			   伝達関数波形データのポインタ */

	int num;	/* Transfer function waveform data length :
			   伝達関数波形データの長さ */

	int in;		/* Input : 入力 */
	int out;	/* Output : 出力 */

} CTLFUNC;

extern void	init_cntrlarry(int cnvwave[], int number,
			CTLFUNC ctlfc[CTLMAX], int cntrlarry[CTLMAX][CTLTIME]);
extern void	set_cntrl(u_long frm,
			CTLFUNC ctlfc[CTLMAX], int cntrlarry[CTLMAX][CTLTIME]);
