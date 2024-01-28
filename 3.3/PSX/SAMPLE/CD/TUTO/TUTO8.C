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
 :			      分割リード
 *	 一つのファイルを複数回に分割してバックグラウンドでリードする
 *
 */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>

void read_test(char *file);
void notfound(char *file);

/* for long term test: 耐久試験 */
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
 : CdRead() は一回に NSECTOR 読み込み、これは NREAD 回バックグラウンドで
 * 実行される。合計では、最大 MAXSECTOR が読み込まれることになる。
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
	 : ファイルの先頭位置、セクタ数、バッファアドレスは、以下の配列
	 * を経由して cdReadChain() 関数に渡される。cdReadChain() はこれを
	 * 使用してデータを CD-ROM より読み込む
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
	
	/* initialize: 初期化 */
	ResetGraph(0);
	CdInit();
	CdSetDebug(0);
	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 64, 0, 0, 1, 512));
	
	/* initialize drawing environment: 画面プリントのための準備 */
	PutDrawEnv(SetDefDrawEnv(&draw, 0, 0, 320, 240));
	PutDispEnv(SetDefDispEnv(&disp, 0, 0, 320, 240));
	setRECT(&rect, 0, 0, 320, 240);
	ClearImage(&rect, 60, 120, 120);
	SetDispMask(1);
	
	
	/* clear destination buffer: リードバッファを一旦クリア */
	for (i = 0; i < sizeof(sectbuf[0])/4; i++) {
		sectbuf[0][i] = 0;
		sectbuf[1][i] = 1;
	}

	/* get the file postion and size ; ファイルの位置とサイズを検索 */
	if (CdSearchFile(&fp, file) == 0) {
		notfound(file);
		goto abort;
	}
	if ((nsector = (fp.size+2047)/2048) > MAXSECTOR)
		nsector = MAXSECTOR;
	
	/* read in one time: まとめて読む */
	CdControl(CdlSetloc, (u_char *)&fp.pos, 0);
	CdRead(nsector, sectbuf[0], mode);
	
	/* wait for end of reading: 読み込み終了を待つ */
	while ((cnt = CdReadSync(1, 0)) > 0) {
		FntPrint("\t\tCONTINUOUS CDREAD\n\n");
		FntPrint("Use DTL-S2190 DISC FOR THIS TEST\n\n");
		FntPrint("file:%s\n\n", file);
		FntPrint("Burst Read ...%d Sectors\n", cnt);
		VSync(0);
		FntFlush(-1);
	}

	/* get the file postion and size ; ファイルの位置とサイズを検索 */
	if (CdSearchFile(&fp, file) == 0) {
		FntPrint("%s: cannot open\n", file);
		goto abort;
	}
	if ((nsector = (fp.size+2047)/2048) > MAXSECTOR)
		nsector = MAXSECTOR;
		
	/* make tables for CdReadChain(): リードテーブルを作成する */
	ipos = CdPosToInt(&fp.pos);
	for (i = j = 0; i < nsector; i += NSECTOR, ipos += NSECTOR, j++) {
		CdIntToPos(ipos, &postbl[j]);
		scttbl[j] = NSECTOR;
		buftbl[j] = &sectbuf[1][i*2048/4];
	}
	
	/* start chained CdRead: チェーンリード */
	cdReadChain(postbl, scttbl, buftbl, nsector/NSECTOR);
	
	/* wait for end of reading: 読み込み終了を待つ */
	while ((cnt = cdReadChainSync()) > 0) {
		FntPrint("\t\t CONTINUOUS CDREAD\n\n");
		FntPrint("Use DTL-S2190 DISC FOR THIS TEST\n\n");
		FntPrint("file:%s\n\n", file);
		FntPrint("Divided Read....%d blocks\n", cnt);
		VSync(0);
		FntFlush(-1);
	}

	/* compare: 比較する */
	for (i = 0; i < nsector*2048/4; i++) 
		if (sectbuf[0][i] != sectbuf[1][i]) {
			printf("verify ERROR at (%08x:%08x)\n\n",
				 &sectbuf[0][i], &sectbuf[1][i]);
			errcnt++;
			break;
		}
	FntPrint("verify done(err=%d)\n", errcnt);
	
	/* end: 終了 */
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

