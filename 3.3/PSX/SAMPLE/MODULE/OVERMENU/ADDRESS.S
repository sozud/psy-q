; assembler file : address.s

; �A�Z���u���t�@�C�� address.s
; �O���[�o���ϐ� LoadAddress ���`���Ă���B

	opt	c+

balls	group

	xdef	LoadAddress

	section	.rdata		; define as read-only data.
				; ���[�h�I�����f�[�^�Ƃ��Ē�`

LoadAddress	dw	group(balls)
; load address for child process. it is refer in parent(MENU.C).
; �q�v���Z�X�̃��[�h�A�h���X�ł��B�e(menu.c)�ŎQ�Ƃ��܂��B
