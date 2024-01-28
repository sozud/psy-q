/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			repeat: CD-DA/XA repeat
 *
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Sep.12,1994	suzu
 *		1.10		Oct,24,1994	suzu
 *		2.00		Feb,02,1995	suzu
 *
 *			    Repeat Play
 *	CD-DA:	CD-DA track is repeated by using Auto Report Mode.
 *	CD-XA:	CD-XA track is repeated by using CdlGetloc primitive
 *		command. If CdControl() is slow, CdControlF() is
 *		available.
 *
 *	cdRepeat(int startp, int endp)		auto loop for CD-DA
 *	cdRepeatXA(int startp, int endp)	auto loop for CD-XA
 *	cdGetPos()				returns currentpostion
 *	cdGetRepTime()				returns repeated times
 *
 *	* cdRepeatXA() uses VSyncCallback() in it. The sample code may be
 *	  changed if you use VSyncCallback() for another purpose.
 *	* cdRepeatXA() is only for double-speed CD-XA.
 :
 *		     リピート再生ライブラリサンプル
 *	    CD-DA/XA トラックの任意の２点間をオートリピートする。
 *
 *   CD-DA:
 *	レポートモードを使用して高速なオートリピートを行なう。
 *
 *   CD-XA:	
 *	ADPCM 再生時はレポートモードのコールバックが使用できない。
 *	そのため CdlGetloc で場所を探す必要がある。CdControl() が重い
 *	場合は、CdControlF() を使用する。
 *
 *   関数:
 *	cdRepeat(int startp, int endp)		CD-DA のループ
 *	cdRepeatXA(int startp, int endp)	CD-XA のループ	
 *	cdGetPos()				現在位置の獲得
 *	cdGetRepTime()				ループ回数の獲得
 *
 *	* cdRepeatXA() は、内部でVSyncCallback() を使用するので、このソース
 *	  コードをそのまま使用する場合はご注意ください。
 *
 *	* cdRepeatXA() は倍速 XA 用です。
 */
	
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#define	XA_MODE	(CdlModeSpeed|CdlModeRT)	/* XA モード */
#define	XA_FREQ	32/*15*/			/* ポーリング間隔 */

static int	StartPos, EndPos;		/* 開始位置・終了位置 */
static int	CurPos;				/* 現在位置 */
static int	RepTime;			/* 繰り返し回数 */

static void cbready(int intr, u_char *result);	/* コールバック */
static void cbvsync(void);			/* コールバック */
static cdplay(u_char com);

int cdRepeat(int startp, int endp)
{
	u_char	param[4];
	
	StartPos = startp;
	EndPos   = endp;
	CurPos   = StartPos;
	RepTime  = 0;
	
	param[0] = CdlModeRept|CdlModeDA;
	CdControlB(CdlSetmode, param, 0);
	
	CdReadyCallback(cbready);
	cdplay(CdlPlay);
}

int cdRepeatXA(int startp, int endp)
{
	u_char	param[4];
	
	StartPos = startp;
	EndPos   = endp;
	CurPos   = StartPos;
	RepTime  = 0;
	
	param[0] = CdlModeSpeed|CdlModeRT;
	CdControlB(CdlSetmode, param, 0);
	
	VSyncCallback(cbvsync);
	cdplay(CdlReadS);
}

int cdGetPos()
{
	return(CurPos);
}

int cdGetRepTime()
{
	return(RepTime);
}

/*
 *	cdRepeat() で使用するコールバック
 */
static void cbready(int intr, u_char *result)
{
	CdlLOC	pos;
	if (intr == CdlDataReady) {
		if ((result[4]&0x80) == 0) {
			pos.minute = result[3];
			pos.second = result[4];
			pos.sector = 0;
			CurPos = CdPosToInt(&pos);
		}
		if (CurPos > EndPos || CurPos < StartPos) 
			cdplay(CdlPlay);
	}
	else {
		/*printf("cdRepeat: error:%s\n", CdIntstr(intr));*/
		while (cdplay(CdlPlay) != 0);
	}
}	


/*
 *	cdRepeatXA() で使用するコールバック
 *
 */
#define MAX_ERROR	8
static void cbvsync(void)
{
	u_char		result[8];
	int		cnt, ret;
	
	if (VSync(-1)%XA_FREQ)	return;
	
	if ((ret = CdSync(1, result)) == CdlDiskError) {
		/*printf("cdRepeatXA: DiskError\n");*/
		cdplay(CdlReadS);
	}
	else if (ret == CdlComplete) {
		if (CurPos > EndPos || CurPos < StartPos) 
			cdplay(CdlReadS);
		else {
			if (CdLastCom() == CdlGetlocP &&
			    (cnt = CdPosToInt((CdlLOC *)&result[5])) > 0)
				CurPos = cnt;
			CdControlF(CdlGetlocP, 0);
		}
	}
}


static cdplay(u_char com)
{
	CdlLOC	loc;
	
	CdIntToPos(StartPos, &loc);
	if (CdControl(com, (u_char *)&loc, 0) != 1)
		return(-1);
	
	CurPos = StartPos;
	RepTime++;
	return(0);
}

