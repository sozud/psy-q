/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	MIMe Animation
 *
 *
 *	"nmime.c" ******** routine
 *
 *		Version 1.**	Mar,  14, 1994
 *
 *		Version ??	Aug/3/1994 S. Aoki
 *			 multi TMD handling
 *
 *		Copyright (C) 1994 by Sony Corporation
 *			All rights Reserved
 */

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>  
#define NMIMEMAIN
#include "nmime.h"  

/* TMD file �� �I�u�W�F�N�g���  */
typedef struct {
    SVECTOR *vtxtop;	/* ���_���̃X�^�[�g�|�C���^ */
    u_long vtxtotal;	/* ���_���̑��� */
    SVECTOR *nrmtop;	/* �@�����̃X�^�[�g�|�C���^ */
    u_long nrmtotal;	/* �@�����̑��� */
    u_long *prmtop;		/* �|���S�����̃X�^�[�g�|�C���^�i���g�p�j*/
    u_long prmtotal;	/* �|���S�����̑����i���g�p�j*/
    u_long scale;		/* �I�u�W�F�N�g�̃X�P�[���i���g�p�j*/
} TMDOBJECT;

/* MDF file�i�����t�@�C���j�� �I�u�W�F�N�g���  */
typedef struct {
    SVECTOR		*top;	/* �������̃X�^�[�g�|�C���^ */
    u_long		offset;	/* �������̃I�t�Z�b�g */
    u_long		total;	/* �������̑��� */
    u_long		object;	/* �Ή�����̂s�l�c�I�u�W�F�N�g�ԍ� */
} MDFOBJECT;

/* TMD file �� MIMe�v�Z�p�\����   */
typedef struct {
    u_long 		objtotal;	/* �s�l�c���̃I�u�W�F�N�g�̑��� */
    SVECTOR		*orgvtx;	/* �I���W�i�����_���̑ޔ��G���A�|�C���^ */
    SVECTOR		*orgnrm;	/* �I���W�i���@�����̑ޔ��G���A�|�C���^ */
    TMDOBJECT 	*tmdobj;	/* �I�u�W�F�N�g���z��̃|�C���^ */
} TMDDATA;

/* MDF file �� MIMe�v�Z�p�\����   */
typedef struct {
    u_long 		objtotal;	/* �l�c�e���̃I�u�W�F�N�g�̑��� */
    MDFOBJECT	*mdfvct;	/* �I�u�W�F�N�g���z��̃|�C���^ */		
    long		*mime;		/* MIMe�̏d�݌W��(����ϐ�)�z��̃|�C���^ */
} MDFDATA;

static void set_tmd_info(u_long *model, TMDDATA *tmddt);
static void original_vertex_save(TMDDATA *tmddt, SVECTOR *orgvtxbf);
static void original_normal_save(TMDDATA *tmddt, SVECTOR *orgnrmbf);
static void reset_mime_vertex(TMDDATA *tmddt);
static void reset_mime_norm(TMDDATA *tmddt);
static void set_mime_vertex(TMDDATA *tmddt, MDFDATA *vtxdt);
static void set_mime_norm(TMDDATA *tmddt, MDFDATA *nrmdt);
static void set_mdf_vertex(u_long *mdfdtvtx, MDFDATA *vtxdt);
static void set_mdf_normal(u_long *mdfdtnrm, MDFDATA *nrmdt);


TMDOBJECT tmdobj0[MIMEMODELMAX][OBJMAX]; /* MIMe�v�Z�pTMD�I�u�W�F�N�g���z�� */
MDFOBJECT mdfvtx0[MIMEMODELMAX][MIMEMAX]; /* MIMe�v�Z�p���_�����f�[�^ */
MDFOBJECT mdfnrm0[MIMEMODELMAX][MIMEMAX]; /* MIMe�v�Z�p�@�������f�[�^ */

/* MIMe�v�Z�pTMD�f�[�^ */
TMDDATA tmddt0[MIMEMODELMAX];
MDFDATA vtxdt0[MIMEMODELMAX];
MDFDATA nrmdt0[MIMEMODELMAX];

/*==MIME Function==================*/

int init_mime_data(int num, u_long *modeladdr, u_long *mdfdatavtx, u_long *mdfdatanrm, u_long *orgvtxbuf, u_long *orgnrmbuf)
{
    int i;
/*    printf("in init_mime_data\n");*/

    tmddt0[num].objtotal=1;
    tmddt0[num].orgvtx=(SVECTOR *)orgvtxbuf;
    tmddt0[num].orgnrm=(SVECTOR *)orgnrmbuf;
    tmddt0[num].tmdobj=tmdobj0[num];
    vtxdt0[num].objtotal=1;
    vtxdt0[num].mdfvct=mdfvtx0[num];
    vtxdt0[num].mime= &mimepr[num][0];
    nrmdt0[num].objtotal=1;
    nrmdt0[num].mdfvct=mdfnrm0[num];
    nrmdt0[num].mime= &mimepr[num][0];

    set_tmd_info(modeladdr, &tmddt0[num]);	/* TMD�f�[�^�̃Z�b�g */

    if (mdfdatavtx){
	/* �I���W�i�����_���̑ޔ� */
	original_vertex_save(&tmddt0[num], (SVECTOR *)orgvtxbuf);
	/* ���_�����f�[�^�̃Z�b�g */
	set_mdf_vertex(mdfdatavtx,&vtxdt0[num]);
    }
    if (mdfdatanrm){
        /* �I���W�i���@�����̑ޔ� */
	original_normal_save(&tmddt0[num], (SVECTOR *)orgnrmbuf); 
	/* �@�������f�[�^�̃Z�b�g */ 
	set_mdf_normal(mdfdatanrm,&nrmdt0[num]);
    }
#if 0
    for (i=0; i<MIMEMAX; i++){
	mimepr[num][i] = 0;
    }
#endif /* 0 */
    return vtxdt0[num].objtotal;
}

/* ���_��MIMe�v�Z���� */
void vertex_mime(int num)
{
    reset_mime_vertex(&tmddt0[num]);	/* �I���W�i�����_���̕��A�@*/
    set_mime_vertex(&tmddt0[num],&vtxdt0[num]); /* MIMe�v�Z�����i���_�j�@*/

}

/* �@����MIMe�v�Z���� */
void normal_mime(int num)
{
    reset_mime_norm(&tmddt0[num]);		/* �I���W�i���@�����̕��A�@*/
    set_mime_norm(&tmddt0[num],&nrmdt0[num]); /* MIMe�v�Z�����i�@���j�@*/
}


/* TMD�f�[�^�̃Z�b�e�B���O */
static void set_tmd_info(u_long *model, TMDDATA *tmddt)
{
    u_long size,*dop;
    int i,n;

    dop= model;
    dop++;   			/* �t�@�C���w�b�_�̓ǂݔ�΂� */
    dop++;   			/* �t���O�̓ǂݔ�΂� */
    n = tmddt->objtotal = *dop;  	/* TMD�f�[�^�̃I�u�W�F�N�g�� */
    dop++;
    for(i = 0; i < n; i++){
	tmddt->tmdobj[i].vtxtop = (SVECTOR *)*dop++ ;	/* ���_���̃|�C���^ */	
	tmddt->tmdobj[i].vtxtotal = *dop++ ;		/* ���_���̑��� */
	tmddt->tmdobj[i].nrmtop = (SVECTOR *)*dop++ ;	/* �@�����̃|�C���^ */
	tmddt->tmdobj[i].nrmtotal = *dop++ ;		/* �@�����̑��� */
	tmddt->tmdobj[i].prmtop = (u_long *)*dop++ ;	/* �|���S�����|�C���^ */
	tmddt->tmdobj[i].prmtotal = *dop++ ;		/* �|���S�����̑��� */
	tmddt->tmdobj[i].scale  = *dop++ ;		/* �I�u�W�F�N�g�̃X�P�[�� */
    } 

#ifdef VERBOSE
    printf("objtotal = %d\n", tmddt->objtotal);
    for(i = 0; i < n; i++){
	printf("v_top%d   = %x\n", 	i, 	tmddt->tmdobj[i].vtxtop );
	printf("v_num%d   = %d\n",	i,  	tmddt->tmdobj[i].vtxtotal );
	printf("nrm_top%d = %x\n",	i,	tmddt->tmdobj[i].nrmtop );
	printf("nrm_num%d = %d\n",	i, 	tmddt->tmdobj[i].nrmtotal );
	printf("prm_top%d = %x\n",	i, 	tmddt->tmdobj[i].prmtop );
	printf("prm_num%d = %d\n",	i, 	tmddt->tmdobj[i].prmtotal );
	printf("scale%d   = %d\n",	i, 	tmddt->tmdobj[i].scale );
    } 
#endif /* VERBOSE */
}

/* ���_�����f�[�^�̃Z�b�g */
static void set_mdf_vertex(u_long *mdfdtvtx, MDFDATA *vtxdt)
{
    int i,n;
    u_long *dop2;

    dop2=mdfdtvtx;
    n = vtxdt->objtotal = *dop2++;
/*	n = vtxdt->objtotal = 5;*/
/*	printf("vtxdt->objtotal=%d\n",n); */

    for(i = 0; i < n; i++){
	vtxdt->mdfvct[i].object = *dop2++;		/* �I�u�W�F�N�g�ԍ� */ 		
	vtxdt->mdfvct[i].offset = *dop2++;		/* �������̃I�t�Z�b�g */ 	
	vtxdt->mdfvct[i].total 	= *dop2++;		/* �������̑��� */ 	
	vtxdt->mdfvct[i].top 	= (SVECTOR *)dop2; 	/* �������̃|�C���^ */
	dop2 	+= vtxdt->mdfvct[i].total*2;
    }
}

/* �@�������f�[�^�̃Z�b�g */
static void set_mdf_normal(u_long *mdfdtnrm, MDFDATA *nrmdt)
{
    int i,n;
    u_long  *dop2;


    dop2= mdfdtnrm;
    n = nrmdt->objtotal = *dop2++;
    for(i = 0; i < n; i++){
	nrmdt->mdfvct[i].object = *dop2++;		/* �I�u�W�F�N�g�ԍ� */ 	
	nrmdt->mdfvct[i].offset = *dop2++;		/* �������̃I�t�Z�b�g */ 	
	nrmdt->mdfvct[i].total 	= *dop2++;		/* �������̑��� */ 	 
	nrmdt->mdfvct[i].top 	= (SVECTOR *)dop2; 	/* �������̃|�C���^ */
	dop2 	+= nrmdt->mdfvct[i].total*2;
    }
}


/* �I���W�i�����_���̑ޔ��@*/
static void original_vertex_save(TMDDATA *tmddt, SVECTOR *orgvtxbf)
{
    SVECTOR *otp,*bsp,*dfp;
    int i,j,n,m;

    bsp= tmddt->orgvtx = orgvtxbf;
    m = tmddt->objtotal;
    n=0;
    for( j = 0; j < m; j++){
  	otp = tmddt->tmdobj[j].vtxtop;
	bsp += n;
	n = tmddt->tmdobj[j].vtxtotal;
        for( i = 0; i < n; i++) *(bsp+i) = *(otp+i);
    }
}

/* �I���W�i���@�����̑ޔ��@*/
static void original_normal_save(TMDDATA *tmddt, SVECTOR *orgnrmbf)
{
    SVECTOR *otp,*bsp,*bspb,*dfp;
    int i,j,n,m;

    bsp= tmddt->orgnrm = orgnrmbf;
    m = tmddt->objtotal;
    n=0;
    for( j = 0; j < m; j++){
  	otp = tmddt->tmdobj[j].nrmtop;
	bsp  += n;
	n = tmddt->tmdobj[j].nrmtotal;
        for( i = 0; i < n; i++) *(bsp+i) = *(otp+i);
    }
}

/* �I���W�i�����_���̕��A�@*/
static void reset_mime_vertex(TMDDATA *tmddt)
{
    SVECTOR *otp,*bsp;
    int i,j,n,m;

    bsp= tmddt->orgvtx;
    m = tmddt->objtotal;
    n=0;
    for( j = 0; j < m; j++){
  	otp = tmddt->tmdobj[j].vtxtop;
	bsp += n;
	n = tmddt->tmdobj[j].vtxtotal;
        for( i = 0; i < n; i++) *(otp+i) = *(bsp+i);
    }
}

/* �I���W�i���@�����̕��A�@*/
static void reset_mime_norm(TMDDATA *tmddt)
{
    SVECTOR *otp,*bsp;
    int i,j,n,m;

    bsp= tmddt->orgnrm;
    m = tmddt->objtotal;
    n=0;
    for( j = 0; j < m; j++){
  	otp = tmddt->tmdobj[j].nrmtop;
	bsp += n;
	n = tmddt->tmdobj[j].nrmtotal;
        for( i = 0; i < n; i++) *(otp+i) = *(bsp+i);
    }
}

/* MIMe�v�Z�����i���_�j�@*/
static void set_mime_vertex(TMDDATA *tmddt, MDFDATA *vtxdt)
{
    SVECTOR *otp,*bsp,*dfp;
    int i,n;

    n = vtxdt->objtotal;

    for( i = 0; i < n; i++){
  	otp = tmddt->tmdobj[vtxdt->mdfvct[i].object].vtxtop+vtxdt->mdfvct[i].offset;
	dfp = vtxdt->mdfvct[i].top;
	if( vtxdt->mime[i] !=0 ) gteMIMefunc(otp,dfp, vtxdt->mdfvct[i].total,vtxdt->mime[i]);  
    }
}


/* MIMe�v�Z�����i�@���j�@*/
static void set_mime_norm(TMDDATA *tmddt, MDFDATA *nrmdt)
{
    SVECTOR *otp,*bsp,*dfp;
    VECTOR tmp;
    int i,n;

    n = nrmdt->objtotal;
    for( i = 0; i < n; i++){
  	otp = tmddt->tmdobj[nrmdt->mdfvct[i].object].nrmtop+nrmdt->mdfvct[i].offset;
	dfp = nrmdt->mdfvct[i].top;
	if( nrmdt->mime[i] !=0 ) 
	    {
	 	gteMIMefunc(otp,dfp, nrmdt->mdfvct[i].total,nrmdt->mime[i]);  
		tmp.vx = (otp+i)->vx;
 		tmp.vy = (otp+i)->vy;
		tmp.vz = (otp+i)->vz;
		VectorNormal(&tmp,&tmp);	/* ���K�������@*/
		(otp+i)->vx = tmp.vx;
 		(otp+i)->vy = tmp.vy;
		(otp+i)->vz = tmp.vz;
	    }
    }
}


/* ���_���̕��A�݂̂����Ȃ��C���^�t�F�[�X�֐� */
void reset_mime_vdf(int num)
{
    reset_mime_vertex(&tmddt0[num]);
}

/* ���_���̕��A�݂̂����Ȃ��C���^�t�F�[�X�֐� */
void reset_mime_ndf(int num)
{
    reset_mime_norm(&tmddt0[num]);
}