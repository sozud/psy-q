$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
PS-X Demonstration Program "bgfix32"
=====================================

・bgfix32はlibgsのＢＧ面の高速描画関数であるGsSortFixBg32を使用して、
  スプライトエディタで作成したＢＧデータの内容を表示するプログラムです。

・実行方法
	psymake
	resetps
	psymake load
	run main


・ファイルの説明
	makefile.mak	メイクファイル

	main.c		メインルーチン

	sample.tim	テクスチャデータ
	sample.cel	BGセルデータ
	sample0.bgd	BGマップデータ
	sample1.bgd	  〃
	sample2.bgd	  〃
	sample3.bgd	  〃

・コントロールパッドの操作は以下の通り。

	上矢印キー：	ＢＧ面数増加
	下矢印キー：	ＢＧ面数減少

	SELECTボタン：	終了



		Copyright (C) 1994,1995 by Sony Computer Entertainment
		All Rights Reserved
