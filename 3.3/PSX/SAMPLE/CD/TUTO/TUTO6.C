/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto6: multi channel CD-XA
 *
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Jul.30,1994	suzu
 *		1.10		Dec,27,1994	suzu
 *		1.20		Jun,02,1995	suzu
 *
 *		     interleaved audio/data channel 
 *	We can record 8 interleaved data/audio channels in double speed
 *	CD-ROM disc. This program decodes the disc which contains following
 *	data/audio channels:
 *
 *		0ch-6ch		audio (CD-XA) channel
 *		7ch		data channel
 *
 *	This program reads data on 7th channel with playing one of
 *	7 audio channel, and we can select the audio channel to play.
 *		
 :		    �}���`�`�����l�� CD-XA �e�X�g�v���O����
 *
 *	CD �{���Đ����ɂ́A�ő� 8ch �܂ŉ����E�f�[�^���C���^�[���[�u����
 *	�L�^���邱�Ƃ��ł���B�����ł́A�ȉ��̃f�[�^���L�^����Ă���f�B
 *	�X�N��z�肵�ĉ����E�f�[�^�̍Đ����s�Ȃ��B
 *
 *		0ch-6ch		�I�[�f�B�I(CD-XA)�`�����l��
 *		7ch		�f�[�^�`�����l��
 *
 *	7ch �ڂ̃f�[�^��ǂݍ��݂Ȃ��� 0-6ch �̂����ꂩ��̃`�����l����
 *	�I�[�f�B�I���Đ�����B
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

#define SCTSIZE	2048
#define RNGSIZE	8
#define RNGMASK	7

static u_long	sector[RNGSIZE][SCTSIZE/4];
static int	rid = 0, errflag = 0;
static void	cbdataready();
static void	notfound(char *file);

main()
{
	u_char		param[4], result[8];
	/* char		*file = "\\DATA\\XA7V.XA;1"; */
	char		*file = "\\XA\\MULTI8.XA;1"; /* lib3.3 */
	CdlFILE		fp;
	CdlFILTER	filter;
	CdlLOC		loc;
	int		padd, opadd, ret, i, j;
	
	/* initialize graphics and controller */
	ResetGraph(0);
	PadInit(0);		
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 16, 320, 200, 0, 512));
	
	CdInit();
	CdSetDebug(0);
	
	/* get file position: �t�@�C���̈ʒu���m�� */
	if (CdSearchFile(&fp, file) == 0) {
		notfound(file);
		goto abort;
	}

	/* kick auto loop: �������[�v�X�^�[�g */
	cdRepeatXA(CdPosToInt(&fp.pos), CdPosToInt(&fp.pos) + fp.size/2048);
	
	/* set mode */
	param[0] = CdlModeSpeed|CdlModeRT|CdlModeDA|CdlModeSF;
	CdControlB(CdlSetmode, param, 0);

	/* set playing channel: ADPCM �Đ��`���l���ݒ� */
	filter.file = 1;
	filter.chan = 0;
	CdControlB(CdlSetfilter, (u_char *)&filter, result);
	
	/* hook callback: �R�[���o�b�N���t�b�N���� */
	CdReadyCallback(cbdataready);
		     
	param[0] = CdlModeSpeed|CdlModeRT|CdlModeDA|CdlModeSF;
	while (((padd = PadRead(1))&PADselect) == 0) {
		
		balls();	
		
		switch (CdSync(1, 0)) {
			
		    case CdlComplete:
		
			/* select channel: �Đ��`���l���I�� */
			if (opadd == 0 && padd&(PADLright|PADLleft)) {
				filter.chan = (padd&PADLright)?
					(filter.chan+1)%8:(filter.chan+7)%8;

				CdControlB(CdlSetfilter,
					   (u_char *)&filter, result);
			}
			break;
			
		    case CdlDiskError:
			CdControl(CdlReadS, 0, 0);
			break;
		}
		
		/* check repeated count: �ʒu�̊l�� */
		CdIntToPos(cdGetPos(), &loc);
		
		/* print status; �X�e�[�^�X�̕\�� */
		FntPrint("\tMulti-Channl XA-AUDIO \n\n");
		FntPrint("\Use DTL-S2190 for this test\n\n");
		FntPrint("position=(%02x:%02x:%02X)\n\n",
			loc.minute, loc.second, loc.sector);
		
		/* display data on 7th channel: �ǂݍ��񂾃f�[�^��\�� */
		for (i = 0; i < RNGSIZE; i++) {
			for (j = 0; j < 3; j++)
				FntPrint("%08x", sector[i][j]);
			FntPrint("\n");
		}
		
		/* display current channel */
		FntPrint("\nchannel=");
		for (i = 0; i < 8; i++)
			FntPrint("~c%s%d",
				 i==filter.chan? "888":"444", i);
		
		
		FntFlush(-1);
		opadd = padd;
	}
	
      abort:
	CdControl(CdlStop, 0, 0);
	CdFlush();
	ResetGraph(1);
	PadStop();
	StopCallback();
	return;
}


/*
 * Callback which is called when CD-ROM data is ready.
 * data on CD-ROM can be read with playing the CD-XA.
 * reading data is stored in the ring buffer.
 : CD-XA  �Đ����Ȃ���C���^�[���[�u�f�[�^��ǂݏo��
 * �C���^�[���[�u�f�[�^�͂����ł̓����O�o�b�t�@�Ɋi�[�����
 */
static void cbdataready(int intr, u_char *result)
{
	if (intr == CdlDataReady) {
		CdGetSector(sector[rid], 2048/4);
		rid = ((rid+1)&RNGMASK);
	}
	else 
		errflag++;
}	

static void notfound(char *file)
{
	int n = 60*4;
	while (n--) {
		balls();
		FntPrint("\n\n%s: not found (waiting)\n", file);
		FntFlush(-1);
	}
	printf("%s: not found\n", file);
}
