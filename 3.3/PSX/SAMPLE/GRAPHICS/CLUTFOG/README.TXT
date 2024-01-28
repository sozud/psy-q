$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$
							1995.09.29 S.C.E
	
clut �ɂ��e�N�X�`���ւ� FOG ���ʂ̃T���v���v���O����

�Etuto0
	�t���[�����Ƃ� FOG �������� CLUT �𐶐����AVRAM ��Ƀ��[�h����
	FOG ���ʂ��o���܂��B

�Etuto1
	���炩���ߐ��i�K�� FOG �̂������� CLUT �� VRAM ��ɗp�ӂ��Ă�
	���A�`�悷��|���S�����ƂɓK���Ȃ��̂�I������ FOG ���ʂ��o��
	�܂��B

�Etuto2
	���炩���ߐ��i�K�� FOG �̂������� CLUT �� VRAM ��ɗp�ӂ��Ă�
	���AOT �ɂԂ炳���� DR_MOVE �v���~�e�B�u�Ő[���ɉ����� CLUT �� 
	CLUT �̈�ɓ]�����邱�Ƃɂ���āAFOG ���ʂ̂��������`�������
	�Ȃ��܂��B���̕��@�̓p�P�b�g�������łȂ��`�掞�� CLUT ��ύX��
	��̂ŁA libgs �ɂ��`��ɂ����̂܂ܓK�p���邱�Ƃ��ł��܂��B


�E�R���g���[���p�b�h�ɂ�鑀��͈ȉ��̒ʂ�ł��B

	���\���L�[	�㉺���E�̈ړ�
	L1, L2		�O��̈ړ�
	
	R1, R2		��] (tuto0 ������)
	�E�\���L�[	��] (tuto0 ������)

�E�v���O�����̃R���p�C��

	> psymake tuto0
	
	> psymake tuto1

	> psymake tuto2

�E�v���O�����̃��[�h

	> psymake load
	> run tuto0

	> psymake load
	> run tuto1

	> psymake load
	> run tuto2
	
�E���̑��̃q���g
	�Etuto0 �́A���t���[�� CLUT �𐶐�����̂ŁA���x�Ȃǂ̖ʂŎ��R��
	  �����܂����A��� CLUT ���g���|���S���͂��ׂĂ��Ȃ� FOG ��
	  �������Ă��܂��܂��B�܂��ALoadImage ���g�����߁A���s���Ԃł�
	  �����s���ɂȂ邱�Ƃ�����܂��B
	�Etuto1 �́A�|���S���v���~�e�B�u�� CLUT ID �����������˂΂Ȃ�
	  �Ȃ��_�ƁAVRAM ��� CLUT �̈悪�����Ă��܂��_���s�ւł����A
	  ��� CLUT ���g�� �|���S���ł����ꂼ��̐[���ɉ����� FOG ��
	  �ʂ������邱�Ƃ��ł��܂��B
	�Etuto2 �́AVRAM ��� CLUT �̈悪�����Ă��܂��Ƃ����_�� tuto1 
	  �Ɠ����ł��B�������|���S���v���~�e�B�u�ƒ��ڂ̊֌W���������� 
	  CLUT �����������邱�Ƃ��ł��邽�߁AFOG �𑜓x(FOG �̒i��) ��
	  ��ׂĊY���|���S�����������Ƃ���Alibgs ��p�����`��̂Ƃ���
	  �L���ł��B

	�Eotz �l�ƃt�H�O�p�����[�^ p �l�Ƃ̊֌W�� otz2p(), p2otz() ��
	  �g���Ƌ��߂邱�Ƃ��ł��܂��B

-----------------------------------------------------------------------------
							1995.06.22 S.C.E
	Sample program for CLUT fog texture mapping

*tuto0
	tuto0 makes depth queued CLUT by each frame, and loads it to the
	frame buffer. This method does not need more extra CLUT memroy
	area on the frame buffer, but not fast.

*tuto1
	tuto1 makes some depth queued CLUT patterns at the first time.
	Each primitive change its CLUT entry according to its Z value
	to simulate foggy space. This method is faster than tuto0 but
	it needs more CLUT memory area.

*controller assign
	Left cross key		move up/down/right/left 
	L1,L2			foreward/backwaord
	R1,R2			rotation (except tuto0)
	Right cross key		rotation (except tuto0)

*compile
	>psymake tuto0
	>psymake tuto1
	>psymake tuto2

*load
	>psymake load
	>run tuto0

	>psymake load
	>run tuto1

	>psymake load
	>run tuto2

			Copyright (C) 1995 Sony Computer Entertainment Inc.