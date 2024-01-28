/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	����BG�`��T���v��
 *		(GsSortFixBG32()�g�p)
 *
 *		Version 1.00	Mar, 7, 1995
 *
 *		Copyright (C) 1995 by Sony Computer Entertainment
 *			All rights Reserved
 */

#include	<sys/types.h>
#include	<libetc.h>
#include	<libgte.h>
#include	<libgpu.h>
#include	<libgs.h>

/* GPU Info. */
#define	OT_LENGTH 4
GsOT WorldOT[2];
GsOT_TAG OTTags[2][1<<OT_LENGTH];

/* �e�N�X�`���i�p�^�[���j��� */
#define TEX_ADDR 0x80180000	/* �e�N�X�`���擪�A�h���X */
#define	TIM_HEADER 0x00000010	/* TIM�w�b�_�̒l */
GsIMAGE TimInfo;

/*
 *  BG���
 */
#define N_BG 8			/* �p�ӂ���BG�̖��� */
GsBG	BGData[N_BG];		/* BG��� */
GsMAP	BGMap[N_BG];		/* �}�b�v��� */
#define CEL_ADDR  0x80140000	/* �Z���f�[�^�擪�A�h���X�i�e�ʋ��ʁj */

/* GsSortFixBg32()�̂��߂̍�Ɨ̈�̊m�ہi�W�ʕ��j */
#define BGWSIZE (((320/32+1)*(240/32+1+1)*6+4)*2+2)
u_long BGPacket[BGWSIZE*N_BG];

/* �e�ʂ̍�Ɨ̈�ւ̃|�C���^ */
u_long *BGWork[N_BG];

/* 4�ʕ��̃}�b�v�f�[�^(BGD�t�@�C��) */
#define BGD0_ADDR 0x80100000	/* �}�b�v#0�擪�A�h���X */
#define BGD1_ADDR 0x80101000	/* �}�b�v#1�擪�A�h���X */
#define BGD2_ADDR 0x80102000	/* �}�b�v#2�擪�A�h���X */
#define BGD3_ADDR 0x80103000	/* �}�b�v#3�擪�A�h���X */
unsigned long *BgdAddr[N_BG] = {
	(unsigned long *)BGD0_ADDR,
	(unsigned long *)BGD1_ADDR,
	(unsigned long *)BGD2_ADDR,
	(unsigned long *)BGD3_ADDR
};

/* BG�̊e�ʂ��ǂ̃}�b�v���g�����̃e�[�u�� */
int MapIndex[8] = {
	0, 1, 2, 3, 1, 2, 3, 1	
};
int NumBG;			/* �\������BG�ʐ� */

/*
 *  ���̑�...
 */
u_long PadData;			/* �R���g���[���p�b�h�f�[�^ */
u_long OldPad;			/* �O�̃t���[���̃f�[�^ */

/*
 *  ���C�����[�`��
 */
main(void)
{
	/* �V�X�e���̏����� */
	ResetCallback();
	initSystem();

	/* ���̑��̏����� */
	PadData = 0;
	NumBG = 1;
	initTexture();
	initBG();
	GsInitVcount();

	while(1) {
		if(moveBG()) break;
		drawAll();
	}

	PadStop();
	ResetGraph(3);
	StopCallback();
	return 0;
}

/*
 *  �`��
 */
drawAll()
{
	int activeBuff;
	int i;
	
	/* �_�u���o�b�t�@�̂ǂ��炪�A�N�e�B�u���𓾂� */
	activeBuff = GsGetActiveBuff();

	/* OT�̃N���A */
	GsClearOt(0, 0, &WorldOT[activeBuff]);

	/* BG�̕`�� */
	for(i = 0; i < NumBG; i++) {	
		GsSortFixBg32(BGData+i, BGWork[i], &WorldOT[activeBuff], N_BG-i);
	}

	/* �p�b�h�̓ǂݍ��� */
	OldPad = PadData;
	PadData = PadRead(1);

	/* V-Sync ���� */
	VSync(0);

	ResetGraph(1);
	GsSwapDispBuff();

	GsSortClear(0, 0, 0, &WorldOT[activeBuff]);
	GsDrawOt(&WorldOT[activeBuff]);
}

/*
 *  �a�f�𓮂���
 */
moveBG()
{
	int i;

	/* ���� = BG�\���������炷 */
	if(((PadData&PADLup)>0)&&
		((OldPad&PADLup)==0)&&
		(NumBG < N_BG)) NumBG++;

	/* ����� = BG�\���������炷 */
	if(((PadData&PADLdown)>0)&&
		((OldPad&PADLdown)==0)&&
		(NumBG > 1)) NumBG--;

	/* BG�\���ʒu */
	BGData[0].scrolly += 1;
	BGData[1].scrollx -= 1;
	BGData[1].scrolly -= 1;
	BGData[3].scrollx += 1;
	BGData[4].scrollx -= 1;
	BGData[4].scrolly += 2;
	BGData[5].scrollx += 2;
	BGData[6].scrolly -= 1;

	/* K�{�^���ŏI�� */
	if((PadData & PADk)>0) return -1;

	return 0;
}

/*
 *  �C�j�V�����C�Y
 */
initSystem()
{
	int i;
		
	/* �p�b�h�̏����� */
	PadInit(0);
	
	/* GPU�̏����� */
	GsInitGraph(320,240,0,0,0);
	GsDefDispBuff(0,0,0,240);

	/* OT�̈�̏����� */
	WorldOT[0].length=OT_LENGTH;
	WorldOT[0].org=OTTags[0];
	WorldOT[1].length=OT_LENGTH;
	WorldOT[1].org=OTTags[1];
	
	/* 3D���̏����� */
	GsInit3D();
}

/*
 *  �Z���p�^�[��(Texture pattern)�̓ǂݍ���
 *  �i���炩���߃�������ɒu���ꂽTIM�f�[�^��VRAM�֓]���j
 */
initTexture()
{
	RECT rect;
	u_long *timP;
	int i;

	/* TIM�f�[�^�̐擪�A�h���X */	
	timP = (u_long *)TEX_ADDR;

	/* �w�b�_�ǂ݂Ƃ΂� */
	timP++;

	/* TIM�f�[�^�̈ʒu���𓾂� */
	GsGetTimInfo( timP, &TimInfo );

	/* PIXEL�f�[�^��VRAM�֓]�� */
	timP += TimInfo.pw * TimInfo.ph/2+3+1;
	LoadImage((RECT *)&TimInfo.px, TimInfo.pixel);
		
	/* CLUT�������VRAM�֓]�� */
	if((TimInfo.pmode>>3)&0x01) {
		LoadImage( (RECT *)&TimInfo.cx, TimInfo.clut );
		timP += TimInfo.cw*TimInfo.ch/2+3;
	}
}

/*
 *  BG�̏�����
 */
initBG()
{
	int i;
	GsCELL *cellP;
        u_char *cel;
        u_char *bgd;
	int ncell;

	/* CEL�f�[�^����Z�����𓾂� */
        cel = (u_char *)CEL_ADDR;        
	cel += 4;
	ncell = *(u_short *)cel;	
	cel += 4;

	/* BG�̊e�ʂ̃}�b�v��BG�\���̂������� */
	for(i = 0; i < N_BG; i++) {

		/* BGD�f�[�^����}�b�v���𓾂� */
	       	bgd = (u_char *)BgdAddr[MapIndex[i]];
		bgd += 4;

		BGMap[i].ncellw = *bgd++;		/* MAP�̑傫�� */
		BGMap[i].ncellh = *bgd++;
		BGMap[i].cellw = *bgd++;		/* �Z���̑傫�� */
		BGMap[i].cellh = *bgd++;
		BGMap[i].index = (unsigned short *)bgd; /* �}�b�v�{�� */
		BGMap[i].base = (GsCELL *)cel;		/* �Z���z��ւ̃|�C���^ */
		
		/* ��Ɨ̈�(Primitive�G���A)�̊m�ہ������� */
		BGWork[i] = BGPacket+i*BGWSIZE;
		GsInitFixBg32(&BGData[i], BGWork[i]);

		/* ���̑�(attribute��) */
		BGData[i].attribute = (0<<24)|GsROTOFF;
		BGData[i].scrollx = BGData[i].scrolly = 0;
		BGData[i].r = BGData[i].g = BGData[i].b = 128;
		BGData[i].map = &(BGMap[i]);
	}
}