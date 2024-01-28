/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	PMD VIEWING ROUTINE
 *
 *
 *		Version 1.00	Jul,  26, 1994
 *
 *		Copyright (C) 1994  Sony Computer Entertainment
 *		All rights Reserved
 */

#include <sys/types.h>

#include <libetc.h>		/* for Control Pad
				/* PAD���g�����߂ɃC���N���[�h����K�v���� */
#include <libgte.h>		/* LIBGS uses libgte
				/* LIGS���g�����߂ɃC���N���[�h����K�v����*/
#include <libgpu.h>		/* LIBGS uses libgpu
				/* LIGS���g�����߂ɃC���N���[�h����K�v���� */
#include <libgs.h>		/* for LIBGS
				/* �O���t�B�b�N���C�u���� ���g�����߂�
				   �\���̂Ȃǂ���`����Ă��� */


#define OBJECTMAX 100		/* Max Objects
                                /* �RD�̃��f���͘_���I�ȃI�u�W�F�N�g��
                                   �������邱�̍ő吔 ���`���� */

#define TEX_ADDR   0xa0010000	/* Top Address of texture data1 (TIM FORMAT)
                                /* �e�N�X�`���f�[�^�iTIM�t�H�[�}�b�g�j
				   ���������A�h���X */

#define TIM_HEADER 0x00000010

#define MODEL_ADDR 0xa0040000	/* Top Address of modeling data (TMD FORMAT) 
                                /* ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j
				   ���������A�h���X */

#define OT_LENGTH  10		/* bit length of OT
                                /* �I�[�_�����O�e�[�u���̉𑜓x */


GsOT            Wot[2];		/* Handler of OT
                                   Uses 2 Hander for Dowble buffer
                                /* �I�[�_�����O�e�[�u���n���h��
				   �_�u���o�b�t�@�̂��߂Q�K�v */

GsOT_TAG	zsorttable[2][1<<OT_LENGTH]; /* Area of OT
                                             /* �I�[�_�����O�e�[�u������ */

GsDOBJ3		object[OBJECTMAX]; /* Array of Object Handler
                                   /* �I�u�W�F�N�g�n���h��
				      �I�u�W�F�N�g�̐������K�v */

u_long          Objnum;		/* valibable of number of Objects
                                /* ���f�����O�f�[�^�̃I�u�W�F�N�g�̐���
				   �ێ����� */

/* �I�u�W�F�N�g���W�n */
GsCOORDINATE2   DWorld;


SVECTOR         PWorld; /* work short vector for making Coordinate parmeter
                        /* ���W�n����邽�߂̃��[�e�[�V�����x�N�^�[ */

extern MATRIX GsIDMATRIX;	/* Unit Matrix /* �P�ʍs�� */

GsRVIEW2  view;			/* View Point Handler
                                /* ���_��ݒ肷�邽�߂̍\���� */
GsF_LIGHT pslt[3];		/* Flat Lighting Handler
                                /* ���s������ݒ肷�邽�߂̍\���� */
u_long padd;			/* Controler data
                                /* �R���g���[���̃f�[�^��ێ����� */

/************* MAIN START ******************************************/
main()
{
  int     i;
  GsDOBJ3 *op;			/* pointer of Object Handler
                                /* �I�u�W�F�N�g�n���h���ւ̃|�C���^ */
  int     outbuf_idx;		/* double buffer index */
  MATRIX  tmpls;
  
  ResetCallback();
  init_all();
  
  while(1)
    {
      if(obj_interactive()==0)
          return 0;	/* interactive parameter get
	                        /* �p�b�h�f�[�^���瓮���̃p�����[�^������ */
      GsSetRefView2(&view);	/* Calculate World-Screen Matrix
	                        /* ���[���h�X�N���[���}�g���b�N�X�v�Z */
      outbuf_idx=GsGetActiveBuff();/* Get double buffer index
	                           /* �_�u���o�b�t�@�̂ǂ��炩�𓾂� */

      GsClearOt(0,0,&Wot[outbuf_idx]); /* Clear OT for using buffer
	                               /* �I�[�_�����O�e�[�u�����N���A���� */

      for(i=0,op=object;i<Objnum;i++)
	{
	  /* Calculate Local-World Matrix
	  /* ���[���h�^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLw(op->coord2,&tmpls);
	  
	  /* Set LWMATRIX to GTE Lighting Registers
	  /* ���C�g�}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLightMatrix(&tmpls);
	  
	  /* Calculate Local-Screen Matrix
	  /* �X�N���[���^���[�J���}�g���b�N�X���v�Z���� */
	  GsGetLs(op->coord2,&tmpls);

	  /* Set LSAMTRIX to GTE Registers
	  /* �X�N���[���^���[�J���}�g���b�N�X��GTE�ɃZ�b�g���� */
	  GsSetLsMatrix(&tmpls);
	  
	  /* Perspective Translate Object and Set OT
	  /* �I�u�W�F�N�g�𓧎��ϊ����I�[�_�����O�e�[�u���ɓo�^���� */
	  GsSortObject3(op,&Wot[outbuf_idx],14-OT_LENGTH);
	  op++;
	}

      VSync(0);			/* Wait VSYNC /* V�u�����N��҂� */
      padd=PadRead(1);		/* Readint Control Pad data
	                        /* �p�b�h�̃f�[�^��ǂݍ��� */
      GsSwapDispBuff();		/* Swap double buffer
	                        /* �_�u���o�b�t�@��ؑւ��� */
      
      /* Set SCREEN CLESR PACKET to top of OT
      /* ��ʂ̃N���A���I�[�_�����O�e�[�u���̍ŏ��ɓo�^���� */
      GsSortClear(0x0,0x0,0x0,&Wot[outbuf_idx]);
      
      /* Drawing OT
      /* �I�[�_�����O�e�[�u���ɓo�^����Ă���p�P�b�g�̕`����J�n���� */
      GsDrawOt(&Wot[outbuf_idx]);
    }
}


obj_interactive()
{
  SVECTOR v1;
  MATRIX  tmp1;
  
  /* Rotate Y 
  /* �I�u�W�F�N�g��Y����]������ */
  if((padd & PADRleft)>0) PWorld.vy -=5*ONE/360;

  /* Rotate Y
  /* �I�u�W�F�N�g��Y����]������ */
  if((padd & PADRright)>0) PWorld.vy +=5*ONE/360;

  /* Rotate X
  /* �I�u�W�F�N�g��X����]������ */
  if((padd & PADRup)>0) PWorld.vx+=5*ONE/360;

  /* Rotate X
  /* �I�u�W�F�N�g��X����]������ */
  if((padd & PADRdown)>0) PWorld.vx-=5*ONE/360;
  
  /* Translate Z
  /* �I�u�W�F�N�g��Z���ɂ����ē����� */
  if((padd & PADm)>0) DWorld.coord.t[2]-=100;
  
  /* Translate Z
  /* �I�u�W�F�N�g��Z���ɂ����ē����� */
  if((padd & PADl)>0) DWorld.coord.t[2]+=100;

  /* Translate X
  /* �I�u�W�F�N�g��X���ɂ����ē����� */
  if((padd & PADLleft)>0) DWorld.coord.t[0] +=10;

  /* Translate X
  /* �I�u�W�F�N�g��X���ɂ����ē����� */
  if((padd & PADLright)>0) DWorld.coord.t[0] -=10;

  /* Translate Y
  /* �I�u�W�F�N�g��Y���ɂ����ē����� */
  if((padd & PADLdown)>0) DWorld.coord.t[1]+=10;

  /* Translate Y
  /* �I�u�W�F�N�g��Y���ɂ����ē����� */
  if((padd & PADLup)>0) DWorld.coord.t[1]-=10;

  /* exit program
  /* �v���O�������I�����ă��j�^�ɖ߂� */
  if((padd & PADk)>0) {PadStop();ResetGraph(3);StopCallback();return 0;}
  
  /* Calculate Matrix from Object Parameter and Set Coordinate
  /* �I�u�W�F�N�g�̃p�����[�^����}�g���b�N�X���v�Z�����W�n�ɃZ�b�g */
  set_coordinate(&PWorld,&DWorld);
  
  /* Clear flag of Coordinate for recalculation
  /* �Čv�Z�̂��߂̃t���O���N���A���� */
  DWorld.flg = 0;
  return 1;
}

/* Initialize routine
/* ���������[�`���Q */
init_all()
{
  ResetGraph(0);		/* Reset GPU /* GPU���Z�b�g */
  PadInit(0);			/* Reset Controller /* �R���g���[�������� */
  padd=0;			/*/* �R���g���[���l������ */
  
  GsInitGraph(640,480,GsINTER|GsOFSGPU,1,0);
  /* rezolution set , interrace mode
  /* �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
    
  GsDefDispBuff(0,0,0,0);	/* Double buffer setting
                                /* �_�u���o�b�t�@�w�� */
#if 0    
  GsInitGraph(320,240,GsNONINTER|GsOFSGPU,0,0);
  /* rezolution set , non interrace mode
  /* �𑜓x�ݒ�i�C���^�[���[�X���[�h�j */
  GsDefDispBuff(0,0,0,240);	/* Double buffer setting
                                /* �_�u���o�b�t�@�w�� */
#endif
  
  GsInit3D();			/* Init 3D system /* �RD�V�X�e�������� */
  
  Wot[0].length=OT_LENGTH;	/* Set bit length of OT handler
                                /* �I�[�_�����O�e�[�u���n���h���ɉ𑜓x�ݒ� */

  Wot[0].org=zsorttable[0];	/* Set Top address of OT Area to OT handler
                                /* �I�[�_�����O�e�[�u���n���h����
				   �I�[�_�����O�e�[�u���̎��̐ݒ� */
    
  /* same setting for anoter OT handler
  /*�_�u���o�b�t�@�̂��߂�������ɂ������ݒ� */
  Wot[1].length=OT_LENGTH;
  Wot[1].org=zsorttable[1];
  
  coord_init();			/* Init coordinate /* ���W��` */
  model_init();			/* Reading modeling data
                                /* ���f�����O�f�[�^�ǂݍ��� */  
  view_init();			/* Setting view point /* ���_�ݒ� */
  light_init();			/* Setting Flat Light /* ���s�����ݒ� */
  
  initTexture1();	        /* texture load of TEX_ADDR */
}

view_init()			/* Setting view point /* ���_�ݒ� */
{
  /*---- Set projection,view ----*/
  GsSetProjection(1000);	/* Set projection /* �v���W�F�N�V�����ݒ� */
  
  /* Setting view point location /* ���_�p�����[�^�ݒ� */
  view.vpx = 0; view.vpy = 0; view.vpz = 2000;
  
  /* Setting focus point location /* �����_�p�����[�^�ݒ� */
  view.vrx = 0; view.vry = 0; view.vrz = 0;
  
  /* Setting bank of SCREEN /* ���_�̔P��p�����[�^�ݒ� */
  view.rz=0;

  /* Setting parent of viewing coordinate /* ���_���W�p�����[�^�ݒ� */
  view.super = WORLD;
  
  /* Calculate World-Screen Matrix from viewing paramter
  /* ���_�p�����[�^���Q���王�_��ݒ肷��
     ���[���h�X�N���[���}�g���b�N�X���v�Z���� */
  GsSetRefView2(&view);
  
  GsSetNearClip(100);           /* Set Near Clip /* �j�A�N���b�v�ݒ� */
}


light_init()			/* init Flat light /* ���s�����ݒ� */
{
  /* Setting Light ID 0 /* ���C�gID�O �ݒ� */
  /* Setting direction vector of Light0
  /* ���s���������p�����[�^�ݒ� */
  pslt[0].vx = 20; pslt[0].vy= -100; pslt[0].vz= -100;
  
  /* Setting color of Light0
  /* ���s�����F�p�����[�^�ݒ� */
  pslt[0].r=0xd0; pslt[0].g=0xd0; pslt[0].b=0xd0;
  
  /* Set Light0 from parameters
  /* �����p�����[�^��������ݒ� */
  GsSetFlatLight(0,&pslt[0]);

  
  /* Setting Light ID 1 /* ���C�gID�P �ݒ� */
  pslt[1].vx = 20; pslt[1].vy= -50; pslt[1].vz= 100;
  pslt[1].r=0x80; pslt[1].g=0x80; pslt[1].b=0x80;
  GsSetFlatLight(1,&pslt[1]);
  
  /* Setting Light ID 2 /* ���C�gID�Q �ݒ� */
  pslt[2].vx = -20; pslt[2].vy= 20; pslt[2].vz= -100;
  pslt[2].r=0x60; pslt[2].g=0x60; pslt[2].b=0x60;
  GsSetFlatLight(2,&pslt[2]);
  
  /* Setting Ambient /* �A���r�G���g�ݒ� */
  GsSetAmbient(0,0,0);

  /* Setting default light mode /* �����v�Z�̃f�t�H���g�̕����ݒ� */
  GsSetLightMode(0);
}

coord_init()			/* Setting coordinate /* ���W�n�ݒ� */
{
  /* Setting hierarchy of Coordinate /* ���W�̒�` */
  GsInitCoordinate2(WORLD,&DWorld);
  
  /* Init work vector
  /* �}�g���b�N�X�v�Z���[�N�̃��[�e�[�V�����x�N�^�[������ */
  PWorld.vx=PWorld.vy=PWorld.vz=0;
  
  /* the org point of DWold is set to Z = -40000
  /* �I�u�W�F�N�g�̌��_�����[���h��Z = -4000�ɐݒ� */
  DWorld.coord.t[2] = -4000;
}


/* Set coordinte parameter from work vector
/* ���[�e�V�����x�N�^����}�g���b�N�X���쐬�����W�n�ɃZ�b�g���� */
set_coordinate(pos,coor)
SVECTOR *pos;			/* work vector /* ���[�e�V�����x�N�^ */
GsCOORDINATE2 *coor;		/* Coordinate /* ���W�n */
{
  MATRIX tmp1;
  SVECTOR v1;
  
  tmp1   = GsIDMATRIX;		/* start from unit matrix
				/* �P�ʍs�񂩂�o������ */
    
  /* Set translation /* ���s�ړ����Z�b�g���� */
  tmp1.t[0] = coor->coord.t[0];
  tmp1.t[1] = coor->coord.t[1];
  tmp1.t[2] = coor->coord.t[2];
  
  /*/* �}�g���b�N�X���[�N�ɃZ�b�g����Ă��郍�[�e�[�V������
     ���[�N�̃x�N�^�[�ɃZ�b�g */
  v1 = *pos;
  
  /* Rotate Matrix
  /* �}�g���b�N�X�Ƀ��[�e�[�V�����x�N�^����p������ */
  RotMatrix(&v1,&tmp1);
  
  /* Set Matrix to Coordinate
  /* ���߂��}�g���b�N�X�����W�n�ɃZ�b�g���� */
  coor->coord = tmp1;

  /* Clear flag becase of changing parameter
  /* �}�g���b�N�X�L���b�V�����t���b�V������ */
  coor->flg = 0;
}


initTexture1()
{
        RECT rect1;
        GsIMAGE tim1;
        u_long *tex_addr;
        int i;

        tex_addr = (u_long *)TEX_ADDR;          /* Top of TIM data address */
        while(1) {
                if(*tex_addr != TIM_HEADER) {
                        break;
                }
                tex_addr++;     /* Skip TIM file header (1word) */
                GsGetTimInfo(tex_addr, &tim1);
                tex_addr += tim1.pw*tim1.ph/2+3+1;    /* Next Texture address*/
                rect1.x=tim1.px;
                rect1.y=tim1.py;
                rect1.w=tim1.pw;
                rect1.h=tim1.ph;
                printf("XY:(%d,%d) WH:(%d,%d)\n",tim1.px,tim1.py,
                                                tim1.pw,tim1.ph);
                LoadImage(&rect1,tim1.pixel);
                DrawSync(0);
                if((tim1.pmode>>3)&0x01) {      /* if clut exist */
                        rect1.x=tim1.cx;
                        rect1.y=tim1.cy;
                        rect1.w=tim1.cw;
                        rect1.h=tim1.ch;
                        LoadImage(&rect1,tim1.clut);
                        DrawSync(0);
                        tex_addr += tim1.cw*tim1.ch/2+3;
                printf("CXY:(%d,%d) CWH:(%d,%d)\n",tim1.cx,tim1.cy,
                                                tim1.cw,tim1.ch);
                }
        }
}


/* Read modeling data (TMD FORMAT) /* ���f�����O�f�[�^�̓ǂݍ��� */
model_init()
{
  u_long *dop;
  GsDOBJ3 *objp;		/* handler of object
				/* ���f�����O�f�[�^�n���h�� */
  int i;
  
  dop=(u_long *)MODEL_ADDR;	/* Top Address of MODELING DATA(TMD FORMAT)
				/* ���f�����O�f�[�^���i�[����Ă���A�h���X */

  /* Link ObjectHandler and TMD FORMAT MODELING DATA
  /* TMD�f�[�^�ƃI�u�W�F�N�g�n���h����ڑ����� */
  Objnum = GsLinkObject3((unsigned long)dop,object);

  for(i=0,objp=object;i<Objnum;i++)
    {	
      /* Set Coordinate of Object Handler
      /*�f�t�H���g�̃I�u�W�F�N�g�̍��W�n�̐ݒ� */
      objp->coord2 =  &DWorld;
      
      objp->attribute = 0;	/* init attribute */
      objp++;
    }

}