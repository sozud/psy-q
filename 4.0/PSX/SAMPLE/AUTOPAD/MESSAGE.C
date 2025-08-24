/*
 * $PSLibId: Run-time Library Release 4.0$
 */
/*				balls
 *
 *		画面にメッセージを表示するプログラム
 *
 *	 Version	Date		Design
 *	-----------------------------------------
 *	1.00		Mar,31,1997	sachiko
 */


#include <sys/types.h>
#include <libetc.h>
#include <kernel.h>
#include <libgte.h>
#include <libgpu.h>

main()
{
	DRAWENV		draw;		/* drawing environment : 描画環境 */
	DISPENV		disp;		/* display environment : 表示環境 */
	RECT		rect;
	int		fd;
	char		buf[256];
#ifdef CARD
	static char	*msg1=
			"スロット１と２にメモリカードを、セットしてください。\n";
#elif defined(MOUSE)
	static char	*msg1=
			"スロット１に、マウスをセットしてください。\n";
#endif
	static char	*msg2=
			"用意ができたら、コントローラを押してください。\n";

/*
 *	イニシャライズ
 */
	PadInit(0);
	ResetGraph(0);
/*------ 画面 ------*/
	SetDefDrawEnv(&draw, 0,   0, 320, 240);
	SetDefDispEnv(&disp, 0,   0, 320, 240);
	PutDrawEnv(&draw); 
/*------- フォント -----*/
	KanjiFntOpen(20, 70, 256, 200, 704, 0, 768, 256, 0, 512);
	FntLoad(960, 256);	
	SetDumpFnt(FntOpen(16, 16, 256, 200, 0, 512));	

	SetDispMask(1);
/*
 *	描画
 */
/*--------- 背景クリア --------*/
	setRECT(&rect, 0, 0, 320, 240);
	ClearImage(&rect, 0, 60, 40);
/*------ メッセージを表示 ------*/
	KanjiFntPrint("%s",msg1);
	KanjiFntPrint("%s",msg2);
	KanjiFntFlush(-1);

	PutDispEnv(&disp); 

/*
 *	コントローラ入力待ち
 */
	while ( PadRead(1) == 0 );
/*
	プログラム終了判定のためのダミーファイル出力
*/
	if ( PCinit() != 0 ) {
		printf("PCinit() error!\n");
		return(-1);
	}
	if ( (fd=PCcreat("c:\\tmp\\APDtmp.txt",0)) == -1 ) {
		printf("%s cannot create.\n","c:\\tmp\\APDtmp.txt");
		return(-1);
	}
	strcpy(buf,"This is temporary file for AutoPad.\n");
	PCwrite(fd,buf,strlen(buf));
	PCclose(fd);
/*
 *	終了処理
 */
	PadStop();
	ResetGraph(3);
	return 0;
}
