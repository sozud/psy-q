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
 *		     ���s�[�g�Đ����C�u�����T���v��
 *	    CD-DA/XA �g���b�N�̔C�ӂ̂Q�_�Ԃ��I�[�g���s�[�g����B
 *
 *   CD-DA:
 *	���|�[�g���[�h���g�p���č����ȃI�[�g���s�[�g���s�Ȃ��B
 *
 *   CD-XA:	
 *	ADPCM �Đ����̓��|�[�g���[�h�̃R�[���o�b�N���g�p�ł��Ȃ��B
 *	���̂��� CdlGetloc �ŏꏊ��T���K�v������BCdControl() ���d��
 *	�ꍇ�́ACdControlF() ���g�p����B
 *
 *   �֐�:
 *	cdRepeat(int startp, int endp)		CD-DA �̃��[�v
 *	cdRepeatXA(int startp, int endp)	CD-XA �̃��[�v	
 *	cdGetPos()				���݈ʒu�̊l��
 *	cdGetRepTime()				���[�v�񐔂̊l��
 *
 *	* cdRepeatXA() �́A������VSyncCallback() ���g�p����̂ŁA���̃\�[�X
 *	  �R�[�h�����̂܂܎g�p����ꍇ�͂����ӂ��������B
 *
 *	* cdRepeatXA() �͔{�� XA �p�ł��B
 */
	
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#define	XA_MODE	(CdlModeSpeed|CdlModeRT)	/* XA ���[�h */
#define	XA_FREQ	32/*15*/			/* �|�[�����O�Ԋu */

static int	StartPos, EndPos;		/* �J�n�ʒu�E�I���ʒu */
static int	CurPos;				/* ���݈ʒu */
static int	RepTime;			/* �J��Ԃ��� */

static void cbready(int intr, u_char *result);	/* �R�[���o�b�N */
static void cbvsync(void);			/* �R�[���o�b�N */
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
 *	cdRepeat() �Ŏg�p����R�[���o�b�N
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
 *	cdRepeatXA() �Ŏg�p����R�[���o�b�N
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
