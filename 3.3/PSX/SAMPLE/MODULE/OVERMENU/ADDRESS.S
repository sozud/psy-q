; assembler file : address.s

; アセンブラファイル address.s
; グローバル変数 LoadAddress を定義している。

	opt	c+

balls	group

	xdef	LoadAddress

	section	.rdata		; define as read-only data.
				; リードオンリデータとして定義

LoadAddress	dw	group(balls)
; load address for child process. it is refer in parent(MENU.C).
; 子プロセスのロードアドレスです。親(menu.c)で参照します。

