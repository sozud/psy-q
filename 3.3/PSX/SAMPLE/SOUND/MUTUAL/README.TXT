$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

┏━━━━━━━━━━━━━━━━━┓
┃  ♪サウンドサンプルプログラム♪  ┃ 
┗━━━━━━━━━━━━━━━━━┛       


CD からデータを読み込み、２つの曲を演奏します。さまざまな状態を main.c
内の #define で変更できます。

==========================================================================
[!!!! 注意 !!!!]
本サンプルプログラムは本来

	\DATA\MUTUAL.VH
	\DATA\MUTUAL.VB
	\DATA\MUTUAL1.SEQ
	\DATA\MUTUAL2.SEQ

のデータを CD から読み込むことによって動作させることを目的としています。
このディレクトリにある各ファイルを CD-ROM ファイルとして作成し、ご利用
下さい。
==========================================================================

コンパイル時に main.c 内で #define するオプションにつきましては、
main.c の先頭部分のコメントをお読みください。

コントロールパッドの操作は以下の通りです。

		■			■
		■ ball 減		■ ball 増

		↑サウンド 1 load	○ サウンド 2 load
	      ←  → サウンド 1 play  ○  ○ サウンド 2 play
	      ↑		      ↑
	      サウンド 1 stop	      サウンド 2 stop

			select ボタン ： 終了

【プログラムのロード】

	> psymake
	> psymake load (main.c 内で #defin WITH_CDROM した場合、
			#define START_PLAY しなかった場合は必要ない)
	> dbugpsx main /r50 /e

	を行い、デバッガ内で実行させて下さい。

	                Copyright (C) 1994 by Sony Computer Entertainment Inc.
