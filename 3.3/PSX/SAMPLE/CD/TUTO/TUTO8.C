/*			tuto8: multi CdRead
 *			
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		Oct.16,1994	suzu
 *		1.20		Mar,12,1995	suzu
 *		2.00		Jul,20,1995	suzu
 *	
 *			multi file CdRead
 *		read many files from CD-ROM in background
 :			      �������[�h
 *	 ��̃t�@�C���𕡐���ɕ������ăo�b�N�O���E���h�Ń��[�h����
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

void read_test(char *file);
void notfound(char *file);

/* for long term test: �ϋv���� */
main() {
	PadInit(0);
	
	while ((PadRead(1)&PADselect) == 0)
		read_test("\\DATA\\MOV.STR;1");
	
	PadStop();
	StopCallback();
	return;
}


/* CdRead() reads NSECTOR sectors each, and it is invoked NREAD times
 * in the background, so MAXSECTOR sectors is read at last.
 : CdRead() �͈��� NSECTOR �ǂݍ��݁A����� NREAD ��o�b�N�O���E���h��
 * ���s�����B���v�ł́A�ő� MAXSECTOR ���ǂݍ��܂�邱�ƂɂȂ�B
 */
#define MAXSECTOR	256	
#define NSECTOR		32	
#define NREAD		(MAXSECTOR/NSECTOR)	

int cdReadChainSync();
int cdReadChain(CdlLOC *_postbl, int *_scttbl, u_long **_buftbl, int _ntbl);

void read_test(char *file)
{
	static u_long	sectbuf[2][MAXSECTOR*2048/4];
	static int	errcnt = 0;
	
	/* The positions, sector numbers and destination buffer pointers
	 * are indicated through following queue arrays.
	 * cdReadChain() reads the data from CD-ROM according to this
	 * instruction queue.
	 : �t�@�C���̐擪�ʒu�A�Z�N�^���A�o�b�t�@�A�h���X�́A�ȉ��̔z��
	 * ���o�R���� cdReadChain() �֐��ɓn�����BcdReadChain() �͂����
	 * �g�p���ăf�[�^�� CD-ROM ���ǂݍ���
	 */
	CdlLOC	postbl[NREAD];	/* table for the position on CD-ROM */
	int	scttbl[NREAD];	/* table for sector number to be read */
	u_long	*buftbl[NREAD];	/* table for destination buffer */
	
	RECT	rect;
	DRAWENV	draw;
	DISPENV	disp;
	int	i, j, cnt, ipos;
	CdlLOC	pos;
	CdlFILE	fp;	
	int	mode = CdlModeSpeed;	
	int	nsector;
	
	/* initialize: ������ */
	ResetGraph(0);
	CdInit();
	CdSetDebug(0);
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 64, 0, 0, 1, 512));
	
	/* initialize drawing environment: ��ʃv�����g�̂��߂̏��� */
	PutDrawEnv(SetDefDrawEnv(&draw, 0, 0, 320, 240));
	PutDispEnv(SetDefDispEnv(&disp, 0, 0, 320, 240));
	setRECT(&rect, 0, 0, 320, 240);
	ClearImage(&rect, 60, 120, 120);
	SetDispMask(1);
	
	
	/* clear destination buffer: ���[�h�o�b�t�@����U�N���A */
	for (i = 0; i < sizeof(sectbuf[0])/4; i++) {
		sectbuf[0][i] = 0;
		sectbuf[1][i] = 1;
	}

	/* get the file postion and size ; �t�@�C���̈ʒu�ƃT�C�Y������ */
	if (CdSearchFile(&fp, file) == 0) {
		notfound(file);
		goto abort;
	}
	if ((nsector = (fp.size+2047)/2048) > MAXSECTOR)
		nsector = MAXSECTOR;
	
	/* read in one time: �܂Ƃ߂ēǂ� */
	CdControl(CdlSetloc, (u_char *)&fp.pos, 0);
	CdRead(nsector, sectbuf[0], mode);
	
	/* wait for end of reading: �ǂݍ��ݏI����҂� */
	while ((cnt = CdReadSync(1, 0)) > 0) {
		FntPrint("\t\tCONTINUOUS CDREAD\n\n");
		FntPrint("Use DTL-S2190 DISC FOR THIS TEST\n\n");
		FntPrint("file:%s\n\n", file);
		FntPrint("Burst Read ...%d Sectors\n", cnt);
		VSync(0);
		FntFlush(-1);
	}

	/* get the file postion and size ; �t�@�C���̈ʒu�ƃT�C�Y������ */
	if (CdSearchFile(&fp, file) == 0) {
		FntPrint("%s: cannot open\n", file);
		goto abort;
	}
	if ((nsector = (fp.size+2047)/2048) > MAXSECTOR)
		nsector = MAXSECTOR;
		
	/* make tables for CdReadChain(): ���[�h�e�[�u�����쐬���� */
	ipos = CdPosToInt(&fp.pos);
	for (i = j = 0; i < nsector; i += NSECTOR, ipos += NSECTOR, j++) {
		CdIntToPos(ipos, &postbl[j]);
		scttbl[j] = NSECTOR;
		buftbl[j] = &sectbuf[1][i*2048/4];
	}
	
	/* start chained CdRead: �`�F�[�����[�h */
	cdReadChain(postbl, scttbl, buftbl, nsector/NSECTOR);
	
	/* wait for end of reading: �ǂݍ��ݏI����҂� */
	while ((cnt = cdReadChainSync()) > 0) {
		FntPrint("\t\t CONTINUOUS CDREAD\n\n");
		FntPrint("Use DTL-S2190 DISC FOR THIS TEST\n\n");
		FntPrint("file:%s\n\n", file);
		FntPrint("Divided Read....%d blocks\n", cnt);
		VSync(0);
		FntFlush(-1);
	}

	/* compare: ��r���� */
	for (i = 0; i < nsector*2048/4; i++) 
		if (sectbuf[0][i] != sectbuf[1][i]) {
			printf("verify ERROR at (%08x:%08x)\n\n",
				 &sectbuf[0][i], &sectbuf[1][i]);
			errcnt++;
			break;
		}
	FntPrint("verify done(err=%d)\n", errcnt);
	
	/* end: �I�� */
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
