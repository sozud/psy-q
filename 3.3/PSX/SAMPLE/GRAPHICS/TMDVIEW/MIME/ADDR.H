/*
 * $PSLibId: Runtime Library Version 3.3$
 */
/*
 *	mime
 *
 *	"addr.h" ���������蓖��
 *
 *		Copyright (C) 1995 by Sony Computer Entertainment
 *		All rights Reserved
 */

/* ���f�����O�f�[�^�iTMD�t�H�[�}�b�g�j ���������A�h���X */
#define MODEL_ADDR (u_long *)0x800d0000

/* �I���W�i���̒��_�f�[�^���ޔ������A�h���X */
#define ORGVTXBUF (u_long *)0x80050000	

/* �I���W�i���̖@���f�[�^���ޔ������A�h���X */	
#define ORGNRMBUF (u_long *)0x80060000	

/* ���_�����f�[�^�iMDF�t�H�[�}�b�g�j ���������A�h���X */
#define MDFDATAVTX (u_long *)0x80090000	

/* �@�������f�[�^�iMDF�t�H�[�}�b�g�j ���������A�h���X */
#define MDFDATANRM (u_long *)0x800b0000	

/* �p�P�b�g�f�[�^��u���A�h���X */
#define PACKETBUF  (u_long *)0x80100000