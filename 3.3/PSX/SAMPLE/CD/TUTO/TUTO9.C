/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			tuto9: Load and Exec
 *			
 *	Copyright (C) 1994 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *		 Version	Date		Design
 *		-----------------------------------------	
 *		1.00		May.16,1995	suzu
 *	
 *		      Load and execute programs
 *
 *	This program reads the executable data from CD-ROM and execute
 *	it as a child process. The process returns to parent when child
 *	is over. Because child program usually starts from 0x80010000,
 *	parent should be loaded not to share the same memomry area.
 *
 :	CD-ROM 上の実行形式プログラムを読みだし、子プロセスとして起動
 *	する。子プロセス終了後は親（このプログラム）へリターンする
 *	子プロセスは大抵 0x80010000 からロードされるので親プログラムは、
 *	そのアドレスを避けてロード・実行する必要がある。
 *
 */
#include <sys/types.h>
#include <libcd.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <kernel.h>

/* file name table to execute: 実行するファイルのリスト */
static char *file[] = {
	"\\EXECMENU\\RCUBE.EXE;1", 
	"\\EXECMENU\\ANIM.EXE;1",
	"\\EXECMENU\\BALLS.EXE;1", 
	0,
};

/* on screen debug strings */
static char	cbfile[256] = "-";
static int	cbtime = 0;

static void cbvsync(void) {
	FntPrint("Exec Text: Use DTL-S2190 for this test\n");
	FntPrint("now loading %s\n", cbfile);
	FntPrint("time = %d\n", VSync(-1)-cbtime);
	FntFlush(-1);
}

int cdExec(char *file, CdlLOC *pos, int mode, int argc, char **argv);

main()
{
	int	i;
	DRAWENV	draw;
	DISPENV	disp;

	PadInit(0);
	ResetGraph(0);
	CdInit();

	SetDefDispEnv(&disp, 0, 0, 320, 240);
	SetDefDrawEnv(&draw, 0, 0, 320, 240);
	draw.isbg = 1;
	setRGB0(&draw, 0, 0, 0);
	PutDrawEnv(&draw);
	PutDispEnv(&disp);

	FntLoad(960, 256);
	SetDumpFnt(FntOpen(16, 64, 0, 0, 1, 512));
	SetDispMask(1);		

	VSyncCallback(cbvsync);

	/* initialize graphics */
	for (i = 0; file[i]; i++) {

		/* copy strings displayed in every VSync */
		strcpy(cbfile, file[i]);	
		cbtime = VSync(-1);

		/* execute: プログラムを実行する */
		cdExec(file[i], 0, 0, 0, 0);
	}
	
	CdFlush();
	ResetGraph(1);
	PadStop();
	StopCallback();
	return;
}
	

/***************************************************************************
 * 
 *CdExec	Read an executable file form CD-ROM and execute it.
 *
 * SYNOPSIS
 *	EXEC *CdExec(char *file, CdlLOC *pos, int mode, int argc, char **argv)
 *
 * ARGUMENT
 *	file	file name of the executable module
 *	pos	file position 
 *	mode	execution mode (reserved)
 *	argc	number of the argument
 *	argv	argument pointer array
 *
 * DESCRIPTION
 *	CdExec() reads an executable file from the position 'pos',
 *	and execute it as a child program. If NULL is specified in 'pos',
 *	CdExec() calculates the file position using 'file' name.
 *
 * BUGS
 *	mode, argc, argv is invalid now
 *
 * RETURN
 *	none
 *
 : CdExec	実行可能データをロードして実行
 *
 * 形式	EXEC *CdExec(char *file, CdlLOC *pos, int mode, int argc, char **argv)
 *
 * 引数		file	実行ファイル名
 *		pos	実行ファイルポジション
 *		mode	実行モード（予約）
 *		argc	引数の数 
 *		argv	引数
 *
 * 解説		位置 pos より実行可能データをリードして、これを子プロセス
 *		として実行する。pos に 0 が指定されば場合は、ファイル名
 *		file から位置を検索してそれを pos として用いる
 *
 * 備考
 *		mode, argc, argv は現在は見ていない		
 *
 * 返り値	なし
 *
 ***************************************************************************/

static void cdRead(CdlLOC *pos, u_long *buf, int nbyte);

int cdExec(char *file, CdlLOC *pos, int mode, int argc, char **argv)
{
	static u_long	headbuf[2048/4];
	static struct XF_HDR *head = (struct XF_HDR *)headbuf;

	DRAWENV	draw;
	DISPENV	disp;
	CdlFILE	fp;
	CdlLOC	p0, p1;

	if (pos) 
		p0 = *pos;
	else {
		if (CdSearchFile(&fp, file) == 0) {
			printf("%s: not found\n", file);
			return(-1);
		}
		p0 = fp.pos;
	}
	printf("exec:loc=(%02x:%02x:%02x)\n", p0.minute, p0.second, p0.sector);
	
	cdRead(&p0, (u_long *)head, 2048);
		
	head->exec.s_addr = 0;
	head->exec.s_size = 0;
	
	printf("exec:pc=%08x,t_addr=%08x,t_size=%08x\n",
	       head->exec.pc0, head->exec.t_addr, head->exec.t_size);
	
	CdIntToPos(CdPosToInt(&p0)+1, &p1);
	cdRead(&p1, (u_long *)head->exec.t_addr, head->exec.t_size);

	printf("executing...\n", file);

	GetDispEnv(&disp);		/* push DISPENV */
	GetDrawEnv(&draw);		/* push DRAWENV */
	SetDispMask(0);			/* black out */
	ResetGraph(1);			/* flush GPU */
	CdFlush();			/* flush CD-ROM */
	PadStop();			/* stop PAD */
	StopCallback();			/* stop callback */
	Exec(&head->exec,1,0);		/* execute */
	RestartCallback();		/* recover callback */
	PadInit(0);			/* recover PAD */
	PutDispEnv(&disp);		/* pop DISPENV */
	PutDrawEnv(&draw);		/* pop DRAWENV */
	SetDispMask(1);			/* recover dispmask */

	printf("...done\n", file);
	return(head->exec.t_addr);
}

static void cdRead(CdlLOC *pos, u_long *buf, int nbyte)
{
#ifdef DEBUG
	printf("cdRead(%02x:%02x:%02x, %08x, %d)\n",
	       pos->minute, pos->second, pos->sector, buf, nbyte);
#endif	
	do {
		CdControl(CdlSetloc, (u_char *)pos, 0);
		CdRead((nbyte+2047)/2048, buf, CdlModeSpeed);
	} while (CdReadSync(0, 0) < 0);
}
