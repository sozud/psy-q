$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$

Divide �֐��T���v���v���O����

�Elibgte �� Divide* �֐��̃T���v���v���O�����ł��B
	Divide �֐��̓|���S�������ɂ���ăe�N�X�`���̘c�݂�}�����܂��B
	���̃T���v���ł̓t���b�g�e�N�X�`���l�p�` POLY_FT4 �� DivideFT4() 
	�ɂ���ĕ������Ă��܂��B


�E�R���g���[���p�b�h�ɂ�鑀��͈ȉ��̒ʂ�ł��B

	���\���L�[	�㉺���E�̈ړ�
	L1, L2		�O��̈ړ�
	
	R1, R2		��]
	�E�\���L�[	��]

	START + ���\���L�[	�������̕ύX
	SELECT + ���\���L�[	picture area �̕ύX

�E�v���O�����̃R���p�C��

	> psymake all

�E�v���O�����̃��[�h

	> psymake load
	> run tuto0

	�����̐[���̂̏����l�� 2 (4x4 ����), picture area �̏����l�� 
	540(��) x 170(����) �ł��B


�E���̑�

	�ʏ� picture area �͉�ʂ����ς��ɐݒ肵�Ă������܂����A���̃T
	���v���ł́A�����̗l�q���킩��₷�����邽�߂ɉ�ʂ���������
	�Ȃ�悤�ɐݒ肵�Ă���܂��B�C���W�P�[�^�̔����ň͂܂ꂽ�̈�
	�� picture area �ł��BDivide �֐��ł́A���̗̈悩��͂ݏo����
	�����͂���ȏ㕪������܂���B

	Divide �֐��́A���������|���S�����֐��Ăяo�����Ɏw�肵�� OT 
	�G���g���ɒ��ړo�^���܂��B���Ȃ킿������� otz �Ɋւ��Ȃ���
	�ׂē����G���g���ɓo�^����܂��B

	Divide �֐��́A�����O�Ɉ�x�����@���N���b�v�̔���������Ȃ���
	���B���̂��߁A���_�ɐڋ߂����傫�ȃ|���S���Ȃǂł́A�����O�ɑS
	�̂��N���b�v����Ă��܂��A���҂����������`�悳��Ȃ��ꍇ������
	�܂��B����������ɂ́ADivide �֐��̑���� RCpoly �֐���p
	���Ă��������BRCpoly �̎g�����͈ȉ����Q�l�ɂ��Ă��������B

u_long *DivideFT4(v0,v1,v2,v3,uv0,uv1,uv2,uv3,rgbc,s,ot,divp)
SVECTOR *v0,*v1,*v2,*v3;
u_long *uv0,*uv1,*uv2,*uv3;
CVECTOR *rgbc;
POLY_FT4 *s;
u_long *ot;
DIVPOLYGON4 *divp;  
{
    int i;
    u_short sz;
    long p,flag;
    RVECTOR *r0,*r1,*r2,*r3;
    
    r0= &divp->r0;
    r1= &divp->r1;
    r2= &divp->r2;
    r3= &divp->r3;
    divp->cr[0].r0 = r0;		
    divp->cr[0].r1 = r1;		
    divp->cr[0].r2 = r2;		
    divp->cr[0].r3 = r3;		
    r0->v = *v0;
    r1->v = *v1;
    r2->v = *v2;
    r3->v = *v3;

    if(RotAverageNclip4
       (
	&r0->v,&r1->v,&r2->v,&r3->v,
	&r0->sxy,&r1->sxy,&r2->sxy,&r3->sxy,
	&p,&sz,&flag)>0 )
	{
	    ReadSZfifo4(&r0->sz,&r1->sz,&r2->sz,&r3->sz);
	    divp->ot = ot;
	    divp->rgbc = *rgbc;
	    divp->clut = (*uv0)>>16;
	    divp->tpage = (*uv1)>>16;
	    *(u_long *)(r0->uv) = *uv0;
	    *(u_long *)(r1->uv) = *uv1;
	    *(u_long *)(r2->uv) = *uv2;
	    *(u_long *)(r3->uv) = *uv3;
	    s =(POLY_FT4 *)RCpolyFT4(s,divp);
	}
    return((u_long *)s);
}

			Copyright (C) 1995 Sony Computer Entertainment Inc.