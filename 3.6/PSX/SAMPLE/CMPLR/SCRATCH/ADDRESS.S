
	opt	c+

text	group				; main text group
bss	group	bss			; main data group
spad	group	obj($1f800000)

	xdef	SpadStock

	section	.rdata

SpadStock	dw	grouporg(spad)
; grouporg()�̓O���[�v��ORG�A�h���X��Ԃ�����֐��B
; �uDTL-D2130 Psy-Q Programmer's Guide�vP.93 �Q�ƁB