/*
 * $PSLibId: Runtime Library Release 3.6$
 */
#ifndef MENU_INCLUDED
#define MENU_INCLUDED

#define	MAX_DIRDEPTH	10

/* struct MENU : メニュー構造体 */
typedef struct {
	char 	str[81];			/* item name : 名前 */
	char	root[81];			/* root : ルートパス */
	char	fname[MAX_DIRDEPTH][81];	/* file name */
	u_short	kind[MAX_DIRDEPTH];		/* & 0x7fff -> kind */
						/* & 0x8000 -> flag */
	int	depth;		/* depth of path : ファイル名の階層の深さ */
	int	ndata;		/* the number of data : データの数 */
	u_long  daddr[16];	/* data address : データのアドレス */
	char	dname[16][81];	/* path of data file : データのパス */
	char	sentj[161];	/* the explanation in Japanese */
	char	sente[161];	/* the explanation in English */
} MENU;

/* メニューアイテム全体と、画面に表示するアイテムの関係 */
typedef struct {
	int	top;	/* top of display items: 表示アイテムの先頭 */
	int	no;	/* current item No.: 現在指しているアイテム No.*/
	int	cnt;	/* the count of display items: 表示するアイテムの数 */
	short	item[100];	/* display items of MENU items
				 : メニューアイテム中の表示するアイテム */
} DISP_MENU;

#endif
