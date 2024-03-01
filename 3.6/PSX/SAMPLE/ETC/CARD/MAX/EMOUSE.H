/* $PSLibId: Runtime Library Release 3.6$ */
#ifndef _EMOUSE_H_
#define _EMOUSE_H_

/*****************************************************************
 *
 * file: emouse.h
 *
 * 	Copyright (C) 1994,1995 by Sony Computer Entertainment Inc.
 *				          All Rights Reserved.
 *
 *	Sony Computer Entertainment Inc. Development Department
 *
 *****************************************************************/

/*
 * �}�E�X�J�[�\���\���t�b�N
 *	flag:	0	�}�E�X�J�[�\���̕\���^��\���ɏ]��
 *		1	�}�E�X�J�[�\���̋����\��
 */
extern void EMouseDisp( int flag, DB *cdb) ;

/*
 * �}�E�X�J�[�\���ύX
 *	type:	�}�E�X�J�[�\���^�C�v
 */
extern void EMouseType(int type) ;

/*
 * �}�E�X�J�[�\���\��
 */
extern void EMouseON(void) ;

/*
 * �}�E�X�J�[�\����\��
 */
extern void EMouseOff(void) ;

/*
 * �}�E�X�J�[�\���ړ�
 *	x,y:	�ړ���
 */
extern void EMouseMove( int x, int y) ;

/*
 * �}�E�X�J�[�\���̂w�ʒu
 */
extern int EMouseX( void) ;

/*
 * �}�E�X�J�[�\���̂x�ʒu
 */
extern int EMouseY( void) ;

/*
 * �p�b�h���̃|�[�����O
 *	ch:	���肷��@PADbuf�̃`���l���ԍ�
 *	dx,dy:	�}�E�X�ړ���
 *	bt:	�{�^�����
 *
 *	return:		-1	�f�[�^�擾���s
 *			���4�r�b�g�F�[�����
 *                                   0x10:�}�E�X
 *		                     0x20:16�{�^���A�i���O
 *			             0x40:16�{�^��
 *			             0x80:�}���`�^�b�v
 */
extern int EMousePadStat( int ch, int *dx, int *dy, u_long *bt) ;

/*
 * �}�E�X�J�[�\���̏󋵂��擾����
 *	x,y:	�}�E�X���W
 *	bt:	�{�^�����
 *
 *	return:		-1	�f�[�^�擾���s
 *			���4�r�b�g�F�[�����
 *                                   0x10:�}�E�X
 *		                     0x20:16�{�^���A�i���O
 *			             0x40:16�{�^��
 *			             0x80:�}���`�^�b�v
 */
extern int EMouseStat( int *x, int *y, u_long *bt) ;

/*
 * �}�E�X�̕\������
 */
extern int EMouseInitDisp( void ) ;


/*
 * �}�E�X���x�ݒ�
 */
/*extern void EMouseSense( float sensx, float sensy) ;*/

/*
 * �}�E�X�ړ��̈�ݒ�
 *	x,y:	��`�̈�̍���ʒu
 *	dx,dy:	��`�̈�̃T�C�Y
 */
extern void EMouseRange( int x, int y, int dx, int dy) ;

/*
 * �}�E�X���W���[��������
 */
extern int EMouseInit( void) ;

/*
 * �}�E�X�ړ��ɂƂ��Ȃ��A�J�����g�O���[�v���ړ�����
 */
extern int EMouseMoveMenu( int mx, int my, SPRTX **sprt, u_long padd, u_long *padret) ;

/*
 *�T���E���h�{�^���̉������ݏ󋵕\��
 *      RM4��id=61..65���g�p
 *      id:0..�\�����邉��
 */
extern void EMouse_dispreverb( int id );

/*
 *�E�F�C�g�t���ŃR�}���h�A�C�R���̌�����ς��邽�߂̃t�b�N
 */
extern void EMouseRevCardEXECON( int count) ;


/*
 *�J�[�h�̈ړ��͈͌���
 */
extern void EMouseSetFree();
extern void EMouseSetCardHome();
extern void EMouseSetCardConfirm();
extern void EMouseSetCardIcon();

#endif /* _EMOUSE_H_ */
/* DON'T ADD STUFF AFTER THIS */