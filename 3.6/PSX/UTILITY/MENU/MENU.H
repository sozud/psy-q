/*
 * $PSLibId: Runtime Library Release 3.6$
 */
#ifndef MENU_INCLUDED
#define MENU_INCLUDED

#define	MAX_DIRDEPTH	10

/* struct MENU : ���j���[�\���� */
typedef struct {
	char 	str[81];			/* item name : ���O */
	char	root[81];			/* root : ���[�g�p�X */
	char	fname[MAX_DIRDEPTH][81];	/* file name */
	u_short	kind[MAX_DIRDEPTH];		/* & 0x7fff -> kind */
						/* & 0x8000 -> flag */
	int	depth;		/* depth of path : �t�@�C�����̊K�w�̐[�� */
	int	ndata;		/* the number of data : �f�[�^�̐� */
	u_long  daddr[16];	/* data address : �f�[�^�̃A�h���X */
	char	dname[16][81];	/* path of data file : �f�[�^�̃p�X */
	char	sentj[161];	/* the explanation in Japanese */
	char	sente[161];	/* the explanation in English */
} MENU;

/* ���j���[�A�C�e���S�̂ƁA��ʂɕ\������A�C�e���̊֌W */
typedef struct {
	int	top;	/* top of display items: �\���A�C�e���̐擪 */
	int	no;	/* current item No.: ���ݎw���Ă���A�C�e�� No.*/
	int	cnt;	/* the count of display items: �\������A�C�e���̐� */
	short	item[100];	/* display items of MENU items
				 : ���j���[�A�C�e�����̕\������A�C�e�� */
} DISP_MENU;

#endif