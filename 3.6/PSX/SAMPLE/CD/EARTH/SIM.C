/* $PSLibId: Runtime Library Release 3.6$ */
/*
 *			�I��������STR �f�[�^�r���[�A
 *
 */	
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>	

#define PPW		1
#define STRADDR		(CDSECTOR *)0xa0300000	
#define USRSIZE		(512-8)		

typedef struct {
	u_short	id;			/* always 0x0x0160 */
	u_short	type;			
	u_short	secCount;	
	u_short	nSectors;
	u_long	frameCount;
	u_long	frameSize;
	
	u_short	width;
	u_short	height;
	
	u_long	headm;
	u_long	headv;
	
	u_char	reserved[4];
	
	u_long	data[USRSIZE];		/* ���[�U�f�[�^ */
} CDSECTOR;				/* CD-ROM STR �\���� */

CDSECTOR	*Sector;		/* ���݂̃Z�N�^�̈ʒu */

StGetNextS(addr, sector)
u_long		**addr;
CDSECTOR	**sector;
{
	static int	first = 1;
	static u_long data[20*USRSIZE];	/* �~�j�w�b�_����菜�����f�[�^ */
	
	int		i, j, len;
	u_long		*sp, *dp;

	if (first) {
		first = 0;
		Sector = STRADDR;
	}
#ifdef DEBUG
	printf("%d: %d sect,(%d,%d)\n",
	       Sector->frameCount, Sector->nSectors,
	       Sector->width, Sector->height);
#endif

	/* ���^�[���l��ݒ� */
	*addr   = data;
	*sector = Sector;
	
	/* �~�j�w�b�_����菜�������[�U�f�[�^���쐬���� */
	dp  = data;
	len = Sector->nSectors;
	while (len--) {

		/* �Ӗ��̂���w�b�_�����m���߂�B*/
		if (Sector->id != 0x0160) {
			Sector = STRADDR;
			return(-1);
		}
		/* �f�[�^���R�s�[ */
		for (sp = Sector->data, i = 0; i < USRSIZE; i++) 
			*dp++ = *sp++;
		Sector++;
	}

	/* �f�[�^�w�b�_���R�s�[ */
	(*sector)->headm = data[0];
	(*sector)->headv = data[1];
	return(0);
}


StFreeRingS(next)
u_long	*next;
{
	/* do nothing */
}