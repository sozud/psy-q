/*
 * $PSLibId: Run-time Library Release 4.3$
 */
/*				menu
 *
 *			Simple Menu Database
 *
 *		Copyright (C) 1997 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	-----------------------------------------
 *	0.10		Jul,15,1997	suzu
 */

/*
 * program list: メニューから起動するプログラム（関数）リスト
 */
void	tuto0(void),tuto1(void);

/*
 * menu title: メニューのタイトル
 */
static char *_title = "relation of GS and GPU";
	
/*
 * program list: 起動するプログラムのリスト
 *=============================================================
 *	colum	contents		 : 内容
 *------------------------------------------------------------
 *	 0-8	program name		 : プログラム名		
 *	 9-13	execututtion button name : 実行ボタン名	
 *	14-17	help button name	 : ヘルプボタン名
 *	18-	description		 : 詳細
 *------------------------------------------------------------
 */
static char *_list[] = {
	" tuto0   demo help : primitivis on libgs     ",
	" tuto1   demo help : objects on libgpu       ",
	0,
};

/*
 * Cursor stop position; number indicates cursor width
 : カーソルが停止する位置。数字はそこの位置のカーソルの幅を表す
 */
static char *_attr[] = {
	"         4    4 ",
	"         4    4 ",
	0,
};

/*
 * Detail message; shown in the bottom line according to cursor position
 : ユーザメッセージ。カーソルの位置に応じて、メニューの最下段に表示される
 */
static char *_msg[] = {
	" append lowlevel primitives on the GsOT ",
	" append complex TMD objects on simple OT ",
	0,
};

/*
 * function name to be executed
 : 実行プログラム（関数）名
 */
static void (*_func[])() = {
	tuto0,
	tuto1,
};

/*
 * help message; shown when HELP button is selected
 : ヘルプメッセージ。HELP が押された時に表示される
 */
static char *_help[] = {
	"tuto0: Usage",
	"   Up       rotate balls -x ",
	"   Down     rotate balls +x ",
	"   Right    rotate balls -y ",
	"   Left     rotate balls +y ",
	"   -------",	
	"   L1       move balls far   ",
	"   L2       move balls near ",
	"   -------",	
	"   Triangle rotate TMD -x  ",
	"   Cross    rotate TMD +x  ",
	"   Circle   rotate TMD -y  ",
	"   Square   rotate TMD +y  ",
	"   -------",	
	"   R1       move TMD far   ",
	"   R2       move TMD near ",
	"   -------",	
	"   SELECT   exit",
	0,
	
	"tuto1: Usage",
	"   Up       rotate balls -x ",
	"   Down     rotate balls +x ",
	"   Right    rotate balls -y ",
	"   Left     rotate balls +y ",
	"   -------",	
	"   L1       move balls far   ",
	"   L2       move balls near ",
	"   -------",	
	"   Triangle rotate TMD -x  ",
	"   Cross    rotate TMD +x  ",
	"   Circle   rotate TMD -y  ",
	"   Square   rotate TMD +y  ",
	"   -------",	
	"   R1       move TMD far   ",
	"   R2       move TMD near ",
	"   -------",	
	"   SELECT   exit",
	0,
};

/* (for GUI debug) 
void tuto0(void) {printf("tuto0...\n");}
void tuto1(void) {printf("tuto1...\n");}
*/

