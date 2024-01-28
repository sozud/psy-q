/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	spaceshuttle: sample program
 *
 *	"tuto0.c" main routine
 *
 *		Version 1.00	Mar,  31, 1994
 *
 *		Copyright (C) 1994  Sony Computer Entertainment
 *		All rights Reserved
 */

#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>	/* �O���t�B�b�N���C�u���� ���g�����߂� �\���̂Ȃǂ�
			   ��`����Ă��� */

#define OBJECTMAX 100	/* �RD�̃��f���͘_���I�ȃI�u�W�F�N�g�ɕ�������
			   ���̍ő吔 ���`���� */

#define TEX_ADDR   0x80010000	/* �e�N�X�`���f�[�^�iTIM�t�H�[�}�b�g�j
				   ���������A�h���X */

#define MODEL_ADDR 0x80040000	/* ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j
				   ���������A�h���X */

#define OT_LENGTH  10		/* �I�[�_�����O�e�[�u���̉𑜓x */


GsOT            Wot[2];		/* �I�[�_�����O�e�[�u���n���h��
				   �_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* �I�[�_�����O�e�[�u������ */

GsDOBJ5		object[OBJECTMAX]; /* �I�u�W�F�N�g�n���h��
				      �I�u�W�F�N�g�̐������K�v */

unsigned long   Objnum;		/* ���f�����O�f�[�^�̃I�u�W�F�N�g�̐���
				   �ێ����� */


GsCOORDINATE2   DSpaceShattle,DSpaceHatchL,DSpaceHatchR,DSatt;
/* �I�u�W�F�N�g���Ƃ̍��W�n */

SVECTOR         PWorld,PSpaceShattle,PSpaceHatchL,PSpaceHatchR,PSatt;
/* �I�u�W�F�N�g���Ƃ̍��W�n�����
   ���߂̌��f�[�^ */

GsRVIEW2  view;			/* ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* ���s������ݒ肷�邽�߂̍\���� */
unsigned long padd;		/* �R���g���[���̃f�[�^��ێ����� */

u_long          preset_p[0x10000];

/************* MAIN START ******************************************/
main()
{
  int     i;
  GsDOBJ5 *op;			/* �I�u�W�F�N�g�n���h���ւ̃|�C���^ */
  int     outbuf_idx;
  MATRIX  tmpls;
  
  ResetCallback();
  init_all();

  GsInitVcount();
  while(1)
    {
      if(obj_interactive()==0)
        return 0;	/* �p�b�h�f�[�^���瓮���̃p�����[�^������ */
      GsSetRefView2(&view);	/* ���[���h�X�N���[���}�g���b�N�X�v�Z */
      outbuf_idx=GsGetActiveBuff();/* �_�u���o�b�t�@�̂ǂ��炩�𓾂� */
      
      GsClearOt(0,0,&Wot[outbuf_idx]); /* �I�[�_�����O�e�[�u�����N���A���� */
      
      for(i=0,op=object;i<Objnum;i++)
	{
	  /* �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLs(op->coord2,&tmpls);
	  /* ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLightMatrix(&tmpls);
	  /* �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLsMatrix(&tmpls);
	  /* �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
	  GsSortObject5(op,&Wot[outbuf_idx],14-OT_LENGTH,getScratchAddr(0));
  	  op++;
	}
      padd=PadRead(0);		/* �p�b�h�̃f�[�^��ǂݍ��� */
      VSync(0);			/* V�u�����N��҂� */
      ResetGraph(1);		/* GPU�����Z�b�g���� */
      GsSwapDispBuff();		/* �_�u���o�b�t�@��ؑւ��� */
      /* ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
      GsSortClear(0x0,0x0,0x0,&Wot[outbuf_idx]);
      /* �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
      GsDrawOt(&Wot[outbuf_idx]);
    }
}

obj_interactive()
{
  SVECTOR v1;
  MATRIX  tmp1;
  
  /* �V���g����Y����]������ */
  if((padd & PADRleft)>0) PSpaceShattle.vy +=5*ONE/360;
  /* �V���g����Y����]������ */
  if((padd & PADRright)>0) PSpaceShattle.vy -=5*ONE/360;
  /* �V���g����X����]������ */
  if((padd & PADRup)>0) PSpaceShattle.vx+=5*ONE/360;
  /* �V���g����X����]������ */
  if((padd & PADRdown)>0) PSpaceShattle.vx-=5*ONE/360;
  
  /* �V���g����Z���ɂ����ē����� */
  if((padd & PADh)>0)      DSpaceShattle.coord.t[2]+=100;
  
  /* �V���g����Z���ɂ����ē����� */
  /* if((padd & PADk)>0)      DSpaceShattle.coord.t[2]-=100; */
  
  /* exit program
  /* �v���O�������I�����ă��j�^�ɖ߂� */
  if((padd & PADk)>0) {PadStop();ResetGraph(3);StopCallback();return 0;}
  
  
  /* �V���g����X���ɂ����ē����� */
  
  if((padd & PADLleft)>0) DSpaceShattle.coord.t[0] -=10;
  /* �V���g����X���ɂ����ē����� */
  if((padd & PADLright)>0) DSpaceShattle.coord.t[0] +=10;
  /* �V���g����Y���ɂ����ē����� */
  if((padd & PADLdown)>0) DSpaceShattle.coord.t[1]+=10;
  /* �V���g����Y���ɂ����ē����� */
  if((padd & PADLup)>0) DSpaceShattle.coord.t[1]-=10;
  
  if((padd & PADl)>0)
    {				/* �n�b�`���J�� */
      object[3].attribute &= 0x7fffffff;	/* �q���𑶍݂����� */
      /* �E�n�b�`��Z���ɉ�]�����邽�߂̃p�����[�^���Z�b�g���� */
      PSpaceHatchR.vz -= 2*ONE/360;
      /* ���n�b�`��Z���ɉ�]�����邽�߂̃p�����[�^���Z�b�g���� */      
      PSpaceHatchL.vz += 2*ONE/360;
      /* �E�n�b�`��Z���ɉ�]�����邽�߂Ƀp�����[�^����}�g���b�N�X���v�Z��
	 �E�n�b�`�̍��W�n�ɃZ�b�g���� */
      set_coordinate(&PSpaceHatchR,&DSpaceHatchR);
      /* ���n�b�`��Z���ɉ�]�����邽�߂Ƀp�����[�^����}�g���b�N�X���v�Z��
	 ���n�b�`�̍��W�n�ɃZ�b�g���� */
      set_coordinate(&PSpaceHatchL,&DSpaceHatchL);
    }
  if((padd & PADm)>0)
    {				/* �n�b�`���Ƃ��� */
      PSpaceHatchR.vz += 2*ONE/360;
      PSpaceHatchL.vz -= 2*ONE/360;
      set_coordinate(&PSpaceHatchR,&DSpaceHatchR);
      set_coordinate(&PSpaceHatchL,&DSpaceHatchL);
    }

  if((padd & PADn)>0 && (object[3].attribute & 0x80000000) == 0)
    {				/* �q���𑗏o���� */
      DSatt.coord.t[1] -= 10;	/* �q����X���ɉ����ē������p�����[�^�Z�b�g */
      /* �q����Y���ɉ����ĉ�]�����邽�߂̃p�����[�^�Z�b�g */
      PSatt.vy += 2*ONE/360;
      /* �p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
      set_coordinate(&PSatt,&DSatt);
    }
  if((padd & PADo)>0 && (object[3].attribute & 0x80000000) == 0)
    {				/* �q����������� */
      DSatt.coord.t[1] += 10;
      PSatt.vy -= 2*ONE/360;
      set_coordinate(&PSatt,&DSatt);
    }
  /* �V���g���̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
  set_coordinate(&PSpaceShattle,&DSpaceShattle);
  return 1;
}


init_all()			/* ���������[�`���Q */
{
  ResetGraph(0);		/* GPU���Z�b�g */
  PadInit(0);			/* �R���g���[�������� */
  padd=0;			/* �R���g���[���l������ */
  GsInitGraph(640,480,2,1,0);	/* �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
  GsDefDispBuff(0,0,0,0);	/* �_�u���o�b�t�@�w�� */

/*  GsInitGraph(640,240,1,1,0);	/* �𑜓x�ݒ�i�m���C���^�[���[�X���[�h�j */
/*  GsDefDispBuff(0,0,0,240);	/* �_�u���o�b�t�@�w�� */

  GsInit3D();			/* �RD�V�X�e�������� */
  
  Wot[0].length=OT_LENGTH;	/* �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */
  Wot[0].org=zsorttable[0];	/* �I�[�_�����O�e�[�u���n���h����
				   �I�[�_�����O�e�[�u���̎��̐ݒ� */
  /* �_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
  Wot[1].length=OT_LENGTH;
  Wot[1].org=zsorttable[1];
  coord_init();			/* ���W��` */
  model_init();			/* ���f�����O�f�[�^�ǂݍ��� */
  view_init();			/* ���_�ݒ� */
  light_init();			/* ���s�����ݒ� */
  /*
  texture_init(TEX_ADDR);
  */
}

view_init()			/* ���_�ݒ� */
{
  /*---- Set projection,view ----*/
  GsSetProjection(1000);	/* �v���W�F�N�V�����ݒ� */
  
  /* ���_�p�����[�^�ݒ� */
  view.vpx = 0; view.vpy = 0; view.vpz = -2000;
  /* �����_�p�����[�^�ݒ� */
  view.vrx = 0; view.vry = 0; view.vrz = 0;
  /* ���_�̔P��p�����[�^�ݒ� */
  view.rz=0;
  view.super = WORLD;		/* ���_���W�p�����[�^�ݒ� */
  /* view.super = &DSatt;	/* ���_���W�p�����[�^�ݒ� */
  
  /* ���_�p�����[�^���Q���王�_��ݒ肷��
     ���[���h�X�N���[���}�g���b�N�X���v�Z���� */
  GsSetRefView2(&view);
}


light_init()			/* ���s�����ݒ� */
{
  /* ���C�gID�O �ݒ� */
  /* ���s���������p�����[�^�ݒ� */
  pslt[0].vx = 100; pslt[0].vy= 100; pslt[0].vz= 100;
  /* ���s�����F�p�����[�^�ݒ� */
  pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
  /* �����p�����[�^��������ݒ� */
  GsSetFlatLight(0,&pslt[0]);
  
  /* ���C�gID�P �ݒ� */
  pslt[1].vx = 20; pslt[1].vy= -50; pslt[1].vz= -100;
  pslt[1].r=0x80; pslt[1].g=0x80; pslt[1].b=0x80;
  GsSetFlatLight(1,&pslt[1]);
  
  /* ���C�gID�Q �ݒ� */
  pslt[2].vx = -20; pslt[2].vy= 20; pslt[2].vz= 100;
  pslt[2].r=0x60; pslt[2].g=0x60; pslt[2].b=0x60;
  GsSetFlatLight(2,&pslt[2]);
  
  /* �A���r�G���g�ݒ� */
  GsSetAmbient(0,0,0);
  
  /* �����v�Z�̃f�t�H���g�̕����ݒ� */
  GsSetLightMode(0);
}

coord_init()			/* ���W�n�ݒ� */
{
  /* ���W�̊K�w��` */
  /* SpaceShattle���W�n�̓��[���h�ɒ��ɂԂ牺���� */
  GsInitCoordinate2(WORLD,&DSpaceShattle);
  /* �n�b�`�E�̍��W�n�̓X�y�[�X�V���g���ɂԂ牺���� */
  GsInitCoordinate2(&DSpaceShattle,&DSpaceHatchL);
  /* �n�b�`���̍��W�n�̓X�y�[�X�V���g���ɂԂ牺���� */
  GsInitCoordinate2(&DSpaceShattle,&DSpaceHatchR);
  /* �q���̍��W�n�̓X�y�[�X�V���g���ɂԂ牺���� */
  GsInitCoordinate2(&DSpaceShattle,&DSatt);
  
  /* �n�b�`�̌��_���X�y�[�X�V���g���̂ւ�Ɏ����Ă��� */
  
  DSpaceHatchL.coord.t[0] =  356;
  DSpaceHatchR.coord.t[0] = -356;
  DSpaceHatchL.coord.t[1] = 34;
  DSpaceHatchR.coord.t[1] = 34;
  
  /* �q���̌��_���X�y�[�X�V���g���̌��_����Y���ɂQ�O���炷 */
  DSatt.coord.t[1] = 20;
  
  /* �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
  PWorld.vx=PWorld.vy=PWorld.vz=0;
  PSatt = PSpaceHatchR = PSpaceHatchL = PSpaceShattle = PWorld;
  /* �X�y�[�X�V���g���̌��_�����[���h��Z = 4000�ɐݒ� */
  DSpaceShattle.coord.t[2] = 4000;
}

set_coordinate(pos,coor)	/* �}�g���b�N�X�v�Z���[�N����}�g���b�N�X
				   ���쐬�����W�n�ɃZ�b�g���� */
SVECTOR *pos;			/* �}�g���b�N�X�v�Z���[�N */
GsCOORDINATE2 *coor;		/* ���W�n */
{
  MATRIX tmp1;
  SVECTOR v1;
  
  tmp1   = GsIDMATRIX;		/* �P�ʍs�񂩂�o������ */
  /* ���s�ړ����Z�b�g���� */
  tmp1.t[0] = coor->coord.t[0];
  tmp1.t[1] = coor->coord.t[1];
  tmp1.t[2] = coor->coord.t[2];
  /* �}�g���b�N�X���[�N�ɃZ�b�g����Ă��郍�[�e�[�V������
     ���[�N�̃x�N�^�[�ɃZ�b�g */
  v1 = *pos;
  /* �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ */
  RotMatrix(&v1,&tmp1);
  /* ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
  coor->coord = tmp1;
  /* �������̂��߂̃L���b�V�����t���b�V������ */
  coor->flg = 0;
}

texture_init(addr)		/* �e�N�X�`���f�[�^��VRAM�Ƀ��[�h���� */
unsigned long addr;
{
  RECT rect1;
  GsIMAGE tim1;
  
  GsGetTimInfo((unsigned long *)(addr+4),&tim1); /* TIM�f�[�^�̃w�b�_����e�N�X�`����
					     �f�[�^�^�C�v�̏��𓾂� */
  rect1.x=tim1.px;		/* �e�N�X�`�������VRAM�ł�X���W */
  rect1.y=tim1.py;		/* �e�N�X�`�������VRAM�ł�Y���W */
  rect1.w=tim1.pw;		/* �e�N�X�`���� */
  rect1.h=tim1.ph;		/* �e�N�X�`������ */
  LoadImage(&rect1,tim1.pixel);	/* VRAM�Ƀe�N�X�`�������[�h���� */
  
  if((tim1.pmode>>3)&0x01)	/* �J���[���b�N�A�b�v�e�[�u�������݂��� */
    {
      rect1.x=tim1.cx;		/* �N���b�g�����VRAM�ł�X���W */
      rect1.y=tim1.cy;		/* �N���b�g�����VRAM�ł�Y���W */
      rect1.w=tim1.cw;		/* �N���b�g�̕� */
      rect1.h=tim1.ch;		/* �N���b�g�̍��� */
      LoadImage(&rect1,tim1.clut); /* VRAM�ɃN���b�g�����[�h���� */
    }
}

model_init()
{				/* ���f�����O�f�[�^�̓ǂݍ��� */
  unsigned long *dop;
  GsDOBJ5 *objp;		/* ���f�����O�f�[�^�n���h�� */
  int i;
  u_long *oppp;
  
  dop=(unsigned long *)MODEL_ADDR;	/* ���f�����O�f�[�^���i�[����Ă���A�h���X */
  dop++;			/* ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j
				   hedder skip */
  
  GsMapModelingData(dop);	/* ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j��
				   ���A�h���X�Ƀ}�b�v����*/
  dop++;
  Objnum = *dop;		/* �I�u�W�F�N�g����TMD�̃w�b�_���瓾�� */
  dop++;			/* GsLinkObject2�Ń����N���邽�߂�TMD��
				   �I�u�W�F�N�g�̐擪�ɂ����Ă��� */
  for(i=0;i<Objnum;i++)		/* TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
    GsLinkObject5((unsigned long)dop,&object[i],i);
  
  oppp = preset_p;
  for(i=0,objp=object;i<Objnum;i++)
    {				/* �f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
      objp->coord2 =  &DSpaceShattle;
				/* �f�t�H���g�̃I�u�W�F�N�g�̃A�g���r���[�g
				   �̐ݒ�i�\�����Ȃ��j */
      objp->attribute = GsDOFF;
      oppp = GsPresetObject(objp,oppp);
      objp++;
    }
  
  object[0].attribute &= ~GsDOFF;
  
  object[0].attribute &= ~GsDOFF;	/* �\���I�� */
  object[0].coord2    = &DSpaceShattle;	/* �X�y�[�X�V���g���̍��W�ɐݒ� */
  object[1].attribute &= ~GsDOFF;	/* �\���I�� */
  object[1].attribute |= GsALON;	/* ������ */
  object[1].coord2    = &DSpaceHatchR;  /* �n�b�`�E�̍��W�ɐݒ� */
  object[2].attribute &= ~GsDOFF;	/* �\���I�� */
  object[2].attribute |= GsALON;	/* ������ */
  object[2].coord2    = &DSpaceHatchL;  /* �n�b�`���̍��W�ɐݒ� */
  
  object[3].coord2    = &DSatt;	/* �q���̍��W�ɐݒ� */
}