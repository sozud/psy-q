$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
�������[�J�[�h�T���v���v���O����

�y�T�v�z

�@���̃T���v���v���O�����́A�v���C�X�e�[�V�����ɓ�������܂��������[�J�[
�h�Ǘ��c�[���𔲂��o�������̂ł��B(�k�Č����d�l�̉�ʃf�U�C���ł��B)
�������[�J�[�h�A�N�Z�X�����̍쐬�̎Q�l�ɂ��Ē������߂ɁA�\�[�X�t�@�C��
�̈ꕔ���T���v���v���O�����Ƃ��Č��J���܂����̂ŁA�����p���������B

  ��ʃC���[�W�ƃ������[�J�[�h�̑�����@�́A�v���C�X�e�[�V�����Ɠ�����
�Ȃ��Ă��܂��B

�@���j���[�V�X�e���ȂǁA�������[�J�[�h���암�ȊO�ɂ��܂��Ă̓\�[�X��
���J�ł��܂���̂ŁA���C�u�����[�Ƃ��ēY�t���܂����B���C�u�����[�̏ڍׂ�
���܂��ẮA�Y�t���܂����h�L�������g�����Q�Ƃ��������B

�@����ɁA�������[�J�[�h�Ǘ���ʂŎg�p�����A�C�R���𐶐�����c�[����
��������Ă��܂��̂ŁA�����Ă����p���������B


�y�z�z���z

	�z�z���ꂽ�t�@�C���̍\���͈ȉ��̒ʂ�ł��B
	�i����̃t�@�C�����d�v�ł��B�j

	readme.txt		���̃h�L�������g�ł��B

	doc\
		superx.txt	super-x library �h�L�������g
		turtle.txt	turtle graphics library �h�L�������g
		malloc.txt	malloc �֘A�h�L�������g
	lib\
		superx.lib	super-x library
		turtle.lib	turtle graphics library
	max\
		cardio.h	�������[�J�[�h����
	�@�@��	card.c			�V
		makefile.mak	psymake �p makefile
		main.lnk	(for psylink)
		menu.lnk	(for PCMENU)
		makefile	NEWS �p makefile
		psxload.h	super-x library �p header
		spritex.h		�V
		menux.h			�V
		turtle.h	turtle library �p header
		memeng.h	�C���[�W�f�[�^�t�@�C��
		memjpn.h		�V
		pad.h			�V
		mouse.h			�V
		pencila.h		�V
		libps.h		psx library header
		const.h		���j���[�V�X�e���p header
		maxtypes.h		�V
		maxstat.h		�V
		maxproto.h		�V
		menuset.h		�V
		menuset2.h		�V
		vsync.h			�V
		emouse.h			�V
		maxconf.h	�p��^���{��؂�ւ��p header

		max.c		���C�����[�`��
		vsync.c		��{�`�惋�[�`��
		pad.c		�R���g���[���p�b�h����
		emouse.c	�R���g���[���p�b�h/�}�E�X����
		maxinit.c	���j���[�V�X�e�������ݒ�
		drawmax.c	���j���[�C���[�W�`�惋�[�`��
		menuset.c	���j���[�V�X�e���n���h���[
		
	makecard\
		makefile.mak	psymake �p makefile
		makefile.lnk		�V
		hand1.h		����񂯂�@���[
		hand2.h		����񂯂�@���傫
		hand3.h		����񂯂�@�ρ[
	�@�@��	makecard.c	�A�C�R�������v���O����


�y�g�p���@�z

�@�P�D�������[�J�[�h�Ǘ���ʂ̎��s

�@max\ �Ɉړ����ăT���v���v���O�������R���p�C�����Ă��������B
�R���p�C���̊��ݒ�̓f�t�H���g���g�p���Ă��܂����A�ύX���K�v�ȏꍇ��
makefile.mak ����� main.lnk �����������Ă��������B

	cd max
	psymake

�@�R���p�C�����I�����܂�����A�v���O�������N�����Ă��������B

	run main

�@���b�̃u���b�N�A�E�g�̌�A�������[�J�[�h�Ǘ���ʂ�����܂��B�������[
�J�[�h����������ĂȂ��ꍇ�ł��A�Ǘ���ʂ͌���܂��B�v���O�����̋N����́A
�������[�J�[�h�𐏎����E�\�ł����A�A�C�R���̏��������ɑ������Ԃ�������
�܂��̂ŁA���������������B

�@�������[�J�[�h�Ǘ���ʂ̃L�[�A�T�C���́A���̒ʂ�ł��B

	���\���L�[	���j���[�A�A�C�R���̈ړ�
	�E�\���L�[	����{�^���i�S�Ƃ��j
	START �L�[	�������{�^��
	SELECT �L�[	�T���v���I��
	
	���S��		DELETE �̎������i�����j

�@������@�́A�n�߂Ƀ��j���[���瑀���I�����܂��B���\���L�[�ŃJ�[�\����
�㉺�ɓ������ACOPY/COPY ALL/DELETE ���E�\���L�[�̉��ꂩ�������đI�����܂��B
�@���삪�I�������ƁACard1 �̃A�C�R���̏�ɃJ�[�\�����ړ����܂��B���\���L�[
�̏㉺�L�[�ŁA�J�[�\�����ړ������A�C�R����I�����܂��BCard2 ���ɃJ�[�\����
�ړ����������ꍇ�́A���E�L�[�̉E�L�[�������܂��B����̑I���ɖ߂肽���ꍇ��
SELECT �L�[�� START �L�[�������܂��B
�@�A�C�R�����I�������Ƒ���m�F���j���[������܂��̂ŁAYES/NO ��I������
��������s���܂��B���s���́A�A�C�R�������얈�ɈقȂ�A�j���[�V�����������܂�
�̂ł��y���݂��������B
�@DELETE ���s�̏ꍇ�̂݁A���S�L�[�𓯎��ɉ������ƂŁA��������������
�A�C�R�����������܂��B

�@�}�E�X�̃L�[�A�T�C���́A���̒ʂ�ł��B

	���{�^��	����{�^��
	�E�{�^��	������{�^��

	���E����	DELETE �̎����	

�@�}�E�X�̏ꍇ���A������@�͓����ł��B���߂ɑ�������肵�Ă���A�A�C�R����
�I�����Ă��������B

�@�R���g���[������у}�E�X�́A�P�A�Q�ǂ���ɑ������Ă��g�p�ł��܂����A������
��������Ă���ꍇ�ɂ́A�P���������������܂��B


�@�Q�D�A�C�R�������̃T���v���v���O�����̎��s

�@makecard �Ɉړ����ē��l�ɃR���p�C�����s���Ă��������B

	cd makecard
	psymake
	run makecard

�@makecard ���s���͉����\������܂��񂪁Arun �R�}���h�ŏI�����m�F�ł��܂��B

	Cannot connect to target

�ƕ\�������ꍇ�́A�܂� makecard �����s���ł��̂ł��΂炭���҂����������B

	No file to process

�ƕ\�������ꍇ�͏I�����Ă��܂��B


�y�������[�J�[�h�A�N�Z�X�֐��ɂ��āz

�@���̃T���v���v���O�����ł́A�������[�J�[�h�A�N�Z�X�Ɋւ��鏈���͂��ׂ�
max\card.c �ɋL�q����Ă��܂��B�������[�J�[�h�A�N�Z�X�Ɋւ���h�L�������g��
�������܂���ł������A�����̓��e�ɂ��܂��Ă� card.c �̃R�����g���Q�l�ɂ���
���������B