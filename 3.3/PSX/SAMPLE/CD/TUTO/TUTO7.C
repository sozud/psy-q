/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto7: simple CdRead
 *
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Aug.05,1994	suzu
 *		1.10		Dec,27,1994	suzu
 *		1.20		Mar,12,1995	suzu
 *	
 *				CdRead
 *	This program reads the data recorded in DTS-2190 CD-ROM disc.
 *	The program reads the same data 2 times, and verifis.
 *
 :	DTS-2190 �̃f�B�X�N�Ɋi�[����Ă��� PSX.EXE ���Q��ǂ݂�����
 *	�f�[�^���r����B
 *	�ϋv�e�X�g�����˂Ė����񂭂肩�����B
 *	
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

void read_test(char *file);
void notfound(char *file);
main() {
	PadInit(0);
	while ((PadRead(1)&PADselect) == 0)
		read_test("\\DATA\\MOV.STR;1");
	
	PadStop();
	StopCallback();
	return;
}


/* maximus mector size: �ǂݏo���Z�N�^�T�C�Y */
#define MAXSECTOR	256		
void read_test(char *file)
{
	static u_long	sectbuf[2][MAXSECTOR*2048/4];
	static int	errcnt = 0;
	RECT	rect;
	DRAWENV	draw;
	DISPENV	disp;
	int	i, j, cnt, retry = 0;
	CdlFILE	fp, *s;		
	int	mode = CdlModeSpeed;	
	int	nsector;
	
	/* initialize graphics and controller */
	ResetGraph(0);
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 64, 0, 0, 1, 512));

	/* initialize CD subsytem: CD �T�u�V�X�e���������� */
	CdInit();
	CdSetDebug(0);
	
	/* initialize drawing/display environment: ��ʃv�����g�̂��߂̏��� */
	PutDrawEnv(SetDefDrawEnv(&draw, 0, 0, 320, 240));
	PutDispEnv(SetDefDispEnv(&disp, 0, 0, 320, 240));
	setRECT(&rect, 0, 0, 320, 240);
	ClearImage(&rect, 60, 120, 120);
	SetDispMask(1);
	
	/* clear reading buffer: ���[�h�o�b�t�@����U�N���A */
	for (i = 0; i < sizeof(sectbuf[0])/4; i++) 
		sectbuf[0][i] = sectbuf[1][i] = 0;
	
	
	/* start reading: �o�b�N�O���E���h���[�h */
	for (i = 0; i < 2; i++) {
		if  (CdSearchFile(&fp, file) == 0) {
			notfound(file);
			goto abort;
		}
		    
		/* get file position: �t�@�C���̈ʒu���m�� */
		if ((nsector = (fp.size+2047)/2048) > MAXSECTOR)
			nsector = MAXSECTOR;
		nsector = MAXSECTOR;	/* for debug */
		
		/* start reading: ���[�h�J�n */
		CdControl(CdlSetloc, (u_char *)&fp.pos, 0);
		CdRead(nsector, sectbuf[i], mode);

		/* Since CdRead() runs in background. the program can
		 * do another task in foreground.  The current reading 
		 * status can be monitored in CdReadSync().
		 * In this sample, VSync(0) is simply called in foreground.
		 : ���[�h�̗��Œʏ�̏����͎��s�ł���B
		 * �����ł́ARead ���I������܂Ŏc��̃Z�N�^�����Ď�����
		 */
		while ((cnt = CdReadSync(1, 0)) > 0 ) {
			FntPrint("\t\t SIMPLE CDREAD\n\n");
			FntPrint("Use DTL-S2190 DISC FOR THIS TEST\n\n");
			FntPrint("file:%s\n\n", file);
			FntPrint("reading ...%d Sectors(err=%d)\n",
				 cnt, errcnt);
			VSync(0);
			FntFlush(-1);
		}

		/* check retur value: �Ԃ�l�����̏ꍇ�̓G���[ */
		if (cnt != 0) {
			FntPrint("Read ERROR in %d\n\n", i);
			errcnt++;
			break;
		}
	}
	       
	/* compare: ��̃f�[�^���r���� */
	for (i = 0; i < sizeof(sectbuf[0])/4; i++) 
		if (sectbuf[0][i] != sectbuf[1][i]) {
			printf("verify ERROR at (%08x:%08x)\n\n",
				 &sectbuf[0][i], &sectbuf[1][i]);
			errcnt++;
			break;
		}
	FntPrint("verify done(err=%d)\n", errcnt);
	
    abort:
	CdFlush();
	ResetGraph(1);
	return;
}

void notfound(char *file)
{
	int n = 60*4;
	while (n--) {
		FntPrint("\n\n%s: not found\n", file);
		FntFlush(-1);
		VSync(0);
	}
	printf("%s: not found\n", file);
}
