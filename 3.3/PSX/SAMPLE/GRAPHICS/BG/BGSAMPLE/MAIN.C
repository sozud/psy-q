/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	BG �`��T���v���v���O����
 *
 *		Version 1.10	Jan, 11, 1995
 *
 *		Copyright (C) 1994,1995 by Sony Computer Entertainment
 *			All rights Reserved
 */

#include	<sys/types.h>
#include	<libetc.h>
#include	<libgte.h>
#include	<libgpu.h>
#include	<libgs.h>

/* �I�[�_�����O�e�[�u��(OT)�̒�` */
#define	OT_LENGTH 4
GsOT WorldOT[2];
GsOT_TAG OTTags[2][1<<OT_LENGTH];

/* GPU�p�P�b�g�̈�̒�` */
#define	PACKETMAX 6000*24
PACKET	GpuPacketArea[2][PACKETMAX];

/* �e�N�X�`����� */
#define TEX_ADDR 0x80180000		/* �e�N�X�`���擪�A�h���X */
#define	TIM_HEADER 0x00000010		/* TIM�w�b�_�̒l */
GsIMAGE TexInfo;			/* TIM�f�[�^�̏�� */

/*
 *  BG���
 */
#define BGD_ADDR 0x80100000		/* BGD�f�[�^�̊i�[�A�h���X */
#define	BGD_HEADER 0x23			/* BGD�w�b�_�̒l */
#define CEL_ADDR 0x80140000		/* CEL�f�[�^�̊i�[�A�h���X */
#define	CEL_HEADER 0x22			/* CEL�w�b�_�̒l */
GsBG	BGData;				/* BG��� */
GsMAP	BGMap;				/* �}�b�v��� */

/*
 *  ���̑�...
 */
u_long padd;			/* �R���g���[���p�b�h�f�[�^ */

/*
 *  �֐��̃v���g�^�C�v�錾
 */
void drawAll();
int  moveCharacter();
void initSystem();
void initTexture();
void initBG();

/*
 *  ���C�����[�`��
 */
main(void)
{
	ResetCallback();
	initSystem();

	padd = 0;

	initTexture();
	initBG();

	while(1) {
		if(moveCharacter()) break;
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
void drawAll()
{
	int activeBuff;		/* �A�N�e�B�u�ȃo�b�t�@�ւ̃|�C���^ */
	int i;
	
	activeBuff = GsGetActiveBuff();
	GsSetWorkBase((PACKET *)GpuPacketArea[activeBuff]);
	GsClearOt(0, 0, &WorldOT[activeBuff]);

	/* BG��OT�ւ̓o�^ */
	GsSortBg(&BGData, &WorldOT[activeBuff], 15);

	/* ��]���Ȃ��ꍇ�͍����`��֐����g���� */
	/* GsSortFastBg(&BGData, &WorldOT[activeBuff], 15); */

	padd = PadRead(0);

	VSync(0);

	ResetGraph(1);
	GsSwapDispBuff();
	GsSortClear(0, 0, 0, &WorldOT[activeBuff]);
	GsDrawOt(&WorldOT[activeBuff]);
}

/*
 *  �R���g���[���p�b�h�ła�f�𓮂���
 */
int moveCharacter()
{
	/* �a�f�T�C�Y�̕ύX */
	if(((padd & PADRup)>0)&&(BGData.h < 480)) {
		BGData.scrolly -= 1;
		BGData.h += 2;
		BGData.my += 1;
	}
	if(((padd & PADRdown)>0)&&(BGData.h > 2)) {
		BGData.scrolly += 1;
		BGData.h -= 2;
		BGData.my -= 1;
	}
	if(((padd & PADRleft)>0)&&(BGData.w < 640)) {
		BGData.scrollx -= 1;
		BGData.w += 2;
		BGData.mx += 1;
	}
	if(((padd & PADRright)>0)&&(BGData.w > 2)) {
		BGData.scrollx += 1;
		BGData.w -= 2;
		BGData.mx -= 1;
	}

	/* �a�f��]�p�̕ύX */
	if((padd & PADl)>0) {
		BGData.rotate += ONE*4;
	}
	if((padd & PADn)>0) {
		BGData.rotate -= ONE*4;
	}

	/* �a�f�g�嗦�̕ύX */
	if(((padd & PADm)>0)&&(BGData.scalex<28000)) {
		BGData.scalex += BGData.scalex/8;
		BGData.scaley += BGData.scaley/8;
	}
	if(((padd & PADo)>0)&&(BGData.scalex>512)) {
		BGData.scalex -= BGData.scalex/8;
		BGData.scaley -= BGData.scaley/8;
	}

	/* �X�N���[�� */
	if((padd & PADLup)>0) {
		BGData.scrolly -= 2;
	}
	if((padd & PADLdown)>0) {
		BGData.scrolly += 2;
	}
	if((padd & PADLleft)>0) {
		BGData.scrollx -= 2;
	}
	if((padd & PADLright)>0) {
		BGData.scrollx += 2;
	}

	/* K�{�^���ŏI�� */
	if((padd & PADk)>0) return -1;

	return 0;
}

/*
 *  �V�X�e���̃C�j�V�����C�Y
 */
void initSystem()
{
	int i;

	/* �p�b�h�̏����� */
	PadInit(0);

	/* �p�b�h�̏����� */
	GsInitGraph(320, 240, 0, 0, 0);
	GsDefDispBuff(0, 0, 0, 240);

	/* OT�̏����� */
	for(i = 0; i < 2; i++) {
		WorldOT[i].length = OT_LENGTH;
		WorldOT[i].org = OTTags[i];
	}

	/* ��ʍ��W�n��3D�Ɠ����ɂ��� */	
	GsInit3D();
}

/*
 *  �X�v���C�g�p�^�[���̓ǂݍ���
 *  �i������TIM�f�[�^��VRAM�֓]���j
 */
void initTexture()
{
	u_long *timP;

	timP = (u_long *)TEX_ADDR;
	while(1) {
		/* TIM�f�[�^�����邩�ǂ��� */
		if(*timP != TIM_HEADER)	{
			break;
		}

		/* �w�b�_�ǂ݂Ƃ΂� */
		timP++;

		/* TIM�f�[�^�̈ʒu���𓾂� */
		GsGetTimInfo( timP, &TexInfo );

		/* PIXEL�f�[�^��VRAM�֓]�� */
		timP += TexInfo.pw * TexInfo.ph/2+3+1;	/* �|�C���^��i�߂� */
		LoadImage((RECT *)&TexInfo.px, TexInfo.pixel);
		
		/* CLUT�������VRAM�֓]�� */
		if((TexInfo.pmode>>3)&0x01) {
			LoadImage( (RECT *)&TexInfo.cx, TexInfo.clut );
			timP += TexInfo.cw*TexInfo.ch/2+3;	/* �|�C���^��i�߂� */
		}
	}
}

/*
 *  BG�̏�����
 */
void initBG()
{
        u_char *cel;
        u_char *bgd;
	u_char celflag;
	int ncell;
	u_char bgdflag;

        cel = (u_char *)CEL_ADDR;        
	cel += 3;
	celflag = *cel++;
	ncell = *(u_short *)cel;	
	cel += 4;

       	bgd = (u_char *)BGD_ADDR;
	bgd += 3;
	bgdflag = *bgd++;

	BGMap.ncellw = *bgd++;
	BGMap.ncellh = *bgd++;
	BGMap.cellw = *bgd++;
	BGMap.cellh = *bgd++;
	BGMap.base = (GsCELL *)cel;
	BGMap.index = (u_short *)bgd;

	BGData.attribute = ((TexInfo.pmode&0x03)<<24);
	BGData.x = 0;
	BGData.y = 0;
	BGData.scrollx = BGData.scrolly = 0;
	BGData.r = BGData.g = BGData.b = 128;
	BGData.map = &BGMap;
	BGData.mx = 320/2;
	BGData.my = 224/2;
	BGData.scalex = BGData.scaley = ONE;
	BGData.rotate = 0;
	BGData.w = 320;
	BGData.h = 224;

	cel += ncell*8;
	if(celflag&0xc0 == 0x80) {
		cel += ncell;		/* Skip ATTR (8bit) */
	}
	if(celflag&0xc0 == 0xc0) {
		cel += ncell*2;		/* Skip ATTR (16bit) */
	}

	bgd += BGMap.ncellw*BGMap.ncellh*2;
	if(bgdflag&0xc0 == 0x80) {
		bgd += BGMap.ncellw*BGMap.ncellh;
	}
	if(bgdflag&0xc0 == 0xc0) {
		bgd += BGMap.ncellw*BGMap.ncellh*2;
	}
}