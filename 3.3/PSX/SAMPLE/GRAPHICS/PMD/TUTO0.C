/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*			    tuto0
 *			
 *	        PMD �r���[�A�v���g�^�C�v (�֐��e�[�u���^)
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	----------------------------------------------------	
 *	1.00		Jul, 7,1994	oka
 *	2.00		Jul,21,1994	suzu	
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>	

#define TEX_ADDR	0x801c0000	
#define PMD_ADDR	0x801a0000	
#define OTLENGTH	8		
#define OTSIZE		(1<<OTLENGTH)
#define SCR_Z		1024

static int pad_read(void);
static void RotPMD(u_long *ot, int id);
static void loadTIM(u_long *addr);

main()
{
	u_long	otbuf[2][1<<OTLENGTH]; 	/* �I�[�_�����O�e�[�u������ */
	int     id;			/* �_�u���o�b�t�@ ID */

	db_init(640, 480, SCR_Z, 60, 120, 120);	/* �_�u���o�b�t�@������ */
	loadTIM((u_long *)TEX_ADDR);		/* �e�N�X�`�����[�h */
	SetDispMask(1);				/* �\���J�n */

	while (pad_read() == 0) {
		id = id? 0: 1;			/* �o�b�t�@�X���b�v */
		ClearOTagR(otbuf[id], OTSIZE);	/* �n�s�N���A */
		RotPMD(otbuf[id], id);		/* PMD ��o�^ */
		db_swap(otbuf[id]+OTSIZE-1);	/* �n�s�`�� */
	}
	PadStop();				/* �I�� */
	StopCallback();
	return(0);
}

/*
 * GTE �}�g���N�X��ݒ肷��
 */
static int pad_read(void)
{
	static SVECTOR	ang   = {512, 512, 512};	/* rotate angle */
	static VECTOR	vec   = {0,     0, SCR_Z*5};	
	static MATRIX	m;				/* matrix */
	static int	scale = ONE;
	
	VECTOR	svec;
	int 	padd = PadRead(1);
	
	if (padd & PADk)	return(-1);
	if (padd & PADRup)	ang.vx += 32;
	if (padd & PADRdown)	ang.vx -= 32;
	if (padd & PADRleft) 	ang.vy += 32;
	if (padd & PADRright)	ang.vy -= 32;
	if (padd & PADL1)	vec.vz += 32;
	if (padd & PADL2) 	vec.vz -= 32;
	if (padd & PADR1)	scale  -= ONE/256;
	if (padd & PADR2)	scale  += ONE/256;

	ang.vz += 4;
	ang.vy += 4;
	
	setVector(&svec, scale, scale, scale);
	RotMatrix(&ang, &m);	
	TransMatrix(&m, &vec);	
	ScaleMatrix(&m, &svec);
	SetRotMatrix(&m);
	SetTransMatrix(&m);

	return(0);
}		

/*
 * TIM �����[�h����
 */	
static void loadTIM(u_long *addr)
{
	TIM_IMAGE	image;		/* TIM �w�b�_ */
	
	OpenTIM(addr);			/* TIM ���I�[�v�� */
	while (ReadTIM(&image)) {
		if (image.caddr)	/* CLUT�i��������΁j�����[�h */
			LoadImage(image.crect, image.caddr);
		if (image.paddr) 	/* �p�^�[�������[�h */
			LoadImage(image.prect, image.paddr);
	}
}


/*
 * PMD �`��֐�
 *	�v���~�e�B�u�̌^�Ƌ��L���_�^�Ɨ����_�ɉ����ĂP�U�̊֐���
 *	����B
 */	
	
/* independent vertex type (8 type) */
static void (*PMD_func[])() = {
	RotPMD_FT3,	RotPMD_FT4,	RotPMD_GT3,	RotPMD_GT4,
	RotPMD_F3,	RotPMD_F4,	RotPMD_G3,	RotPMD_G4,
};
/* shared vertex type (8 type) */
static void (*PMD_SV_func[])() = {
	RotPMD_SV_FT3,	RotPMD_SV_FT4,	RotPMD_SV_GT3,	RotPMD_SV_GT4,
	RotPMD_SV_F3,	RotPMD_SV_F4,	RotPMD_SV_G3,	RotPMD_SV_G4,
};

/*
 * PMD ��`�悷��
 */	
static void RotPMD(u_long *ot, int id)
{
	int     i,j;
	long	*PMDTOP;	/*PMD file PRIMITIVE Gp top address*/
	long	*PMDWC;		/*PMD file word counter*/
	long	POINTER;	/*PMD file POINTER*/
	long	*PTOP;		/*PMD file PRIMITIVE Gp Block top address*/
	long	ID;		/*PMD file ID*/
	long	*PRIMTOP;	/*PMD file PRIMITIVE Gp top address*/
	long	NOBJ;		/*PMD file NOBJ*/
	long	NPTR;		/*PMD file NPTR*/
	long	TYPE_NPACKET;	/*PMD file PRIMITIVE Gp header*/
	short	TYPE;		/*PMD file PRIMITIVE Gp type*/
	short	type;		/*PMD file PRIMITIVE Gp local type*/
	long	*SVTOP;		/*PMD file Vertex top address*/
	long	BACKC;		/*PMD file Back clip ON/OFF flag*/

	PMDWC = PMDTOP = (long *)PMD_ADDR;

	ID    = *PMDWC;			PMDWC++;
	PTOP  = PMDTOP + ((*PMDWC)>>2);	PMDWC++;
	SVTOP = PMDTOP + ((*PMDWC)>>2);	PMDWC++;
	NOBJ= *PMDWC;			PMDWC++;

	for(i = 0;i < NOBJ; i++) {
		NPTR= *PMDWC;
		PMDWC++;
		for(j = 0;j < NPTR; j++){
			POINTER= *PMDWC;
			PRIMTOP= PMDTOP+(POINTER>>2);
			TYPE_NPACKET= *(PRIMTOP);	
			TYPE= TYPE_NPACKET>>16;
			type= TYPE&0x000f;
			BACKC= (TYPE&0x0020)>>5;
			if (type < 0x8)
				(*PMD_func[type])(PRIMTOP,
						     ot, OTLENGTH, id, BACKC);
			else if (type < 0x10)
				(*PMD_func[type])(PRIMTOP, SVTOP,
						     ot, OTLENGTH, id, BACKC);
			PMDWC++;
			}
		}
}





