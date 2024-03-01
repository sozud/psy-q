/*
 * $PSLibId: Runtime Library Release 3.6$
 */
/*				60frame
 *
 *         Copyright (C) 1995 by Sony Computer Entertainment
 *			All rights Reserved
 *
 *	 Version	Date		Design
 *	--------------------------------------------------
 *	1.00		Nov,11,1995	sachiko
 *
 *			60frame or 30frame
 :			60�t���[����30�t���[���̈Ⴂ
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#define OT_LENGTH	12		/* OT�̉𑜓x */
#define	OT_SIZE		(1<<OT_LENGTH)	/* OT�̑傫�� */
#define	SCR_Z		1000	/* ���_����X�N���[���܂ł̋��� */

#define OBJECTMAX 100		/* Max Objects
                                 : �RD�̃��f���͘_���I�ȃI�u�W�F�N�g��
                                   �������邱�̍ő吔 ���`���� */

#define PACKETMAX  10000		/* Max GPU packets */
#define PACKETMAX2 (PACKETMAX*24)	/* size of PACKETMAX (byte)
                                    	   paket size may be 24 byte(6 word) */
#define	FRAME_30	0
#define	FRAME_60	1

/* Top Address of texture data (TIM FORMAT)
 : �e�N�X�`���f�[�^�iTIM�t�H�[�}�b�g�j���������A�h���X */
#define TEX_ADDR1	0x80010000
#define TEX_ADDR2	0x80020000
#define TEX_ADDR3	0x80030000
#define TEX_ADDR4	0x80040000
#define TEX_ADDR5	0x80050000
#define TEX_ADDR6	0x80060000
#define TEX_ADDR7	0x80070000

/* Top Address of modeling data (TMD FORMAT) 
 : ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j���������A�h���X */
#define MODEL_ADDR	0x80080000

typedef struct {
	GsOT		gsot;
	GsOT_TAG	ot[OT_SIZE];
	PACKET		packet[PACKETMAX2];
} DB;

static void init_system(DB *db);
static void init_coordinate(GsCOORDINATE2 *objWorld,SVECTOR *rotate);
static void init_view();
static void init_light();
static void init_model(GsCOORDINATE2 *objWorld,GsDOBJ2 *object,int *objcnt);
static void init_texture();
static void load_texture(u_long addr);
static int  pad_read(GsCOORDINATE2 *objWorld,SVECTOR *rotate,int *frame);
static void move_object(GsCOORDINATE2 *objWorld,SVECTOR *rotate);
static void draw_object(DB *db,GsDOBJ2 *object,int objcnt,int frame);
static void opening(GsCOORDINATE2 *objWorld,SVECTOR *rotate
			,DB *db,GsDOBJ2 *object,int objcnt);

extern MATRIX	GsIDMATRIX;

main()
{
	GsCOORDINATE2	objWorld;	/* �I�u�W�F�N�g���W�n */
	int		objcnt;
	DB		db[2];		/* packet double buffer: */
	int		idx;		/* current db index
					 : ���݂� db �̃C���f�b�N�X */
	SVECTOR		rotate;		/* �I�u�W�F�N�g�̉�]�x�N�g�� */
	GsDOBJ2		object[OBJECTMAX];	/* Array of Object Handler
						 : �I�u�W�F�N�g�n���h��
						   �I�u�W�F�N�g�̐������K�v */
	static int	frame=FRAME_60;
	/*MATRIX		fnt;*/

	/* initialize: �����ݒ� */
	init_system(db);
	init_coordinate(&objWorld,&rotate);
	init_view();
	init_light();
	init_model(&objWorld,object,&objcnt);
	init_texture();

	FntLoad(960,256);
	SetDumpFnt(FntOpen(32,32,640,240,0,30));

	opening(&objWorld,&rotate,db,object,objcnt);

	while ( pad_read(&objWorld,&rotate,&frame)==NULL ) {

		/* move objects
		 : �I�u�W�F�N�g�̉�]�E���s�ړ����v�Z���� */
		move_object(&objWorld,&rotate);

		/* Get double buffer index
		 : �_�u���o�b�t�@�̂ǂ��炩�𓾂� */
		idx = GsGetActiveBuff();

		/* draw objects
		 : �I�u�W�F�N�g�̕`�� */
		draw_object(&db[idx],object,objcnt,frame);

	}
	PadStop();
	ResetGraph(3);
	StopCallback();
	return 0;
}

static void init_system(DB *db)
{
	/* initialize callback: �R�[���o�b�N�̏����� */
	ResetCallback();

	GsInitVcount();

	/* initialize controller: �R���g���[���̏�����*/
	PadInit(0);

	/* initialize GPU: GPU �̏����� */
	GsInitGraph(640,240,GsINTER|GsOFSGPU,0,0);
	GsDefDispBuff(0,0,0,240);

	/* initialize OT: OT �̏����� */
	db[0].gsot.length = db[1].gsot.length = OT_LENGTH;
	db[0].gsot.point  = db[1].gsot.point  = 0;
	db[0].gsot.offset = db[1].gsot.offset = 0;
	db[0].gsot.org    = db[0].ot;
	db[1].gsot.org    = db[1].ot;

	/* initialize 3D system: 3D�V�X�e���̏����� */
	GsInit3D();
}

static void init_coordinate(GsCOORDINATE2 *objWorld,SVECTOR *rotate)
{

	memset(objWorld,0,sizeof(GsCOORDINATE2));
	memset(rotate,0,sizeof(SVECTOR));
	/* setting hierarchy of coordinate: ��Ԑe�ɂȂ���W�n�̎w�� */
	GsInitCoordinate2(WORLD,objWorld);

	/*objWorld->coord.t[2] = -500;*/
}

static void init_view()
{
	GsRVIEW2	view;

	/* ���e�ʂ̈ʒu�ݒ� */
	GsSetProjection(SCR_Z);

	/* ���_�ʒu�̐ݒ� */
	view.vpx = 0; view.vpy = 0; view.vpz = 1000;
	view.vrx = 0; view.vry = 0; view.vrz = 0;
	view.rz  = 0;
	view.super = WORLD;
	GsSetRefView2(&view);

	/* �y�N���b�v�l��ݒ� */
	GsSetNearClip(100);
}

static void init_light()
{
	GsF_LIGHT	light;

	/* ���� #0 �̐ݒ� */
	light.vx = 20;   light.vy = -100;  light.vz = -100;
	light.r  = 0xd0; light.g  = 0xd0; light.b  = 0xd0;
	GsSetFlatLight(0,&light);

	/* �A���r�G���g�i���ӌ��j�̐ݒ� */
	GsSetAmbient(ONE/2,ONE/2,ONE/2);

	/* �������[�h�̐ݒ�i�ʏ����/FOG�Ȃ��j */
	GsSetLightMode(0);
}

/* Read modeling data (TMD FORMAT): ���f�����O�f�[�^�̓ǂݍ��� */
static void init_model(GsCOORDINATE2 *objWorld,GsDOBJ2 *object,int *objcnt)
{
	u_long	*dop;
	GsDOBJ2	*objp;		/* handler of object
				 : ���f�����O�f�[�^�n���h�� */
	int i;

	/* Top Address of MODELING DATA(TMD FORMAT)
	 : ���f�����O�f�[�^���i�[����Ă���A�h���X */
	dop=(u_long *)MODEL_ADDR;
	dop++;	/* hedder skip */

	/* Mapping real address 
	 : ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j�����A�h���X�Ƀ}�b�v���� */
	GsMapModelingData(dop);

	dop++;
	*objcnt = *dop;		/* Get number of Objects
				 : �I�u�W�F�N�g����TMD�̃w�b�_���瓾�� */

	dop++;			/* Adjusting for GsLinkObject4
				 : GsLinkObject4�Ń����N���邽�߂�TMD��
				   �I�u�W�F�N�g�̐擪�ɂ����Ă��� */

	/* Link ObjectHandler and TMD FORMAT MODELING DATA
	 : TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
	for(i=0;i<*objcnt;i++)
		GsLinkObject4((u_long)dop,&object[i],i);

	objp = object;
	i = *objcnt;
	while ( i-- ) {
		/* Set Coordinate of Object Handler
		 : �f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
		objp->coord2 =  objWorld;

		objp->attribute = 0;
		objp++;
	}
}

static void init_texture()
{
	load_texture(TEX_ADDR1);
	load_texture(TEX_ADDR2);
	load_texture(TEX_ADDR3);
	load_texture(TEX_ADDR4);
	load_texture(TEX_ADDR5);
	load_texture(TEX_ADDR6);
	load_texture(TEX_ADDR7);
}

/* Load texture to VRAM
 : �e�N�X�`���f�[�^��VRAM�Ƀ��[�h���� */
static void load_texture(u_long addr)
{
	RECT	rect;
	GsIMAGE	tim;

	/* Get texture information of TIM FORMAT
	 : TIM�f�[�^�̃w�b�_����e�N�X�`���̃f�[�^�^�C�v�̏��𓾂� */  
	GsGetTimInfo((u_long *)(addr+4),&tim);

	/* XY point of image data on VRAM : �e�N�X�`�������VRAM�ł�XY���W */
	rect.x = tim.px;
	rect.y = tim.py;

	/* Width and Height of image: �e�N�X�`���̕��ƍ��� */
	rect.w = tim.pw;
	rect.h = tim.ph;

	/* Load texture to VRAM: VRAM�Ƀe�N�X�`�������[�h���� */
	LoadImage(&rect,tim.pixel);

	/* Exist Color Lookup Table: �J���[���b�N�A�b�v�e�[�u�������݂��� */
	if((tim.pmode>>3)&0x01)
	{
		/* XY point of CLUT data on VRAM
		 : �N���b�g�����VRAM�ł�XY���W */
		rect.x = tim.cx;
		rect.y = tim.cy;

		/* Width and Height of CLUT: �N���b�g�̕��ƍ��� */
		rect.w = tim.cw;
		rect.h = tim.ch;

		/* Load CULT data to VRAM: VRAM�ɃN���b�g�����[�h���� */
		LoadImage(&rect,tim.clut);
	}
}

static int pad_read(GsCOORDINATE2 *objWorld,SVECTOR *rotate,int *frame)
{
	u_long		padd;
	static int	rot_speed[2]={4*ONE/360,2*ONE/360};
	static int	tra_speed[2]={20,10};
	static u_long	padd_old;

	padd = PadRead(0);

	/* end: �I�� */
	if ( padd & PADselect )
		return(-1);

	/* change speed: �`�摬�x�̐ؑւ� */
	if ( padd & PADstart ) {
		if ( padd != padd_old )
			*frame ^= 1;
	}

	/* rotate Y: Y����] */
	if ( padd & PADRleft  ) rotate->vy -= rot_speed[*frame];
	if ( padd & PADRright ) rotate->vy += rot_speed[*frame];

	/* rotate X: X����] */
	if ( padd & PADRup    ) rotate->vx += rot_speed[*frame];
	if ( padd & PADRdown  ) rotate->vx -= rot_speed[*frame];

	/* translate Z: Z���ɂ����Ĉړ� */
	if ( padd & PADL1 ) {
		if ( objWorld->coord.t[2] < 500 )
			objWorld->coord.t[2] += tra_speed[*frame];
	}
	if ( padd & PADL2 ) objWorld->coord.t[2] -= tra_speed[*frame];

	/* translate Y: Y���ɂ����Ĉړ� */
	if ( padd & PADLdown ) objWorld->coord.t[1] += tra_speed[*frame];
	if ( padd & PADLup   ) objWorld->coord.t[1] -= tra_speed[*frame];

	/* translate X: X���ɂ����Ĉړ� */
	if ( padd & PADLleft  ) objWorld->coord.t[0] += tra_speed[*frame];
	if ( padd & PADLright ) objWorld->coord.t[0] -= tra_speed[*frame];

	rotate->vy += rot_speed[*frame]/2;

	padd_old = padd;

	return(0);
}

static void opening(GsCOORDINATE2 *objWorld,SVECTOR *rotate,DB *db,
			GsDOBJ2 *object,int objcnt)
{
	int	idx;
	int	i;

	objWorld->coord.t[0] = -930;
	objWorld->coord.t[1] = 60;
	objWorld->coord.t[2] = -6050;
	rotate->vy = +110;

	for ( i=0; i<470; i++ ) {

		objWorld->coord.t[0] += 2;
		objWorld->coord.t[2] += (1+i*.05);

		move_object(objWorld,rotate);
		idx = GsGetActiveBuff();
		draw_object(&db[idx],object,objcnt,FRAME_60);

	}
}


static void move_object(GsCOORDINATE2 *objWorld,SVECTOR *rotate)
{
	MATRIX		tmpmat;
	SVECTOR		tmpvec;

	/* start from unit matrix: �P�ʍs�񂩂�o������ */
	tmpmat = GsIDMATRIX;

	/* Set Translate: ���s�ړ����Z�b�g���� */
	tmpmat.t[0] = objWorld->coord.t[0];
	tmpmat.t[1] = objWorld->coord.t[1];
	tmpmat.t[2] = objWorld->coord.t[2];

	/* set rotate: ��]�x�N�g�����Z�b�g���� */
	tmpvec = *rotate;

	/* rotate matrix: �}�g���b�N�X�ɉ�]�x�N�g������p������ */
	RotMatrix(&tmpvec,&tmpmat);

	/* set Matrix to coordinate: ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
	objWorld->coord = tmpmat;

	/* clear flag because of changing parameter:
	 : �Čv�Z�̂��߂̃t���O���N���A���� */
	objWorld->flg = 0;
}

static void draw_object(DB *db,GsDOBJ2 *object,int objcnt,int frame)
{
	GsDOBJ2		*op;
	MATRIX		tmpmat;

	/* Set top address of Packet Area for output of GPU PACKETS */
	GsSetWorkBase(db->packet);

	/* Clear OT for using buffer
	 : �I�[�_�����O�e�[�u�����N���A���� */
	GsClearOt(0,0,&db->gsot);

	op = object;
	while ( objcnt-- ) {

		/* Perspective Translate Object and Set OT
		 : �I�u�W�F�N�g�𓧎��ϊ��� OT �ɓo�^���� */
		GsGetLw(op->coord2,&tmpmat);
		GsSetLightMatrix(&tmpmat);
		GsGetLs(op->coord2,&tmpmat);
		GsSetLsMatrix(&tmpmat);
		GsSortObject4(op,&db->gsot,14-OT_LENGTH,getScratchAddr(0));

		op++;
	}

	DrawSync(0);
	switch (frame) {
	case FRAME_30:		/* 30 frame */
		VSync(2);
		break;
	case FRAME_60:		/* 60 frame */
		VSync(0);
		break;
	}

	FntPrint("speed = %d frame/sec\n",30*(frame+1));

	FntFlush(-1);

	/* Swap double buffer: �_�u���o�b�t�@��ؑւ��� */
	GsSwapDispBuff();

	/* Set SCREEN CLESR PACKET to top of OT
	 : ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
	GsSortClear(0x0,0x0,0x0,&db->gsot);

	/* Drawing OT
	 : OT �ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
	GsDrawOt(&db->gsot);
}