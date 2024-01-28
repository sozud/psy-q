$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$

┏━━━━━━━━━━━━━━━━━┓
┃  ♪サウンドサンプルプログラム♪  ┃ 
┗━━━━━━━━━━━━━━━━━┛       


・main：Key On された効果音にたいして、ピッチベンドやオートパンニングや
	オートボリュームなどのエフェクトをかけます。
	

	コントロールパッドの操作は以下の通りです。
                H ボタン(START) ： キーオン
                K ボタン(SELECT) ： キーオフ
		A ボタン(↑ボタン) ： 動かせるタイルの UP
                B ボタン(↓ボタン) ： 動かせるタイルの DOWN
                N ボタン(R1ボタン) ： タイルを左に動かす
                L ボタン(L1ボタン) ： タイルを右に動かす
                H ボタン(START) & K(SELECT) ボタン
                         ： 終了

	タイルを移動させることにより、画面をみながらオートパンニングや
	オートボリュームやピッチベンドをかけることができます。

      ＊今回のサンプルでは、オートパンニングおよびオートボリュームは動
	作しませんので、御了承下さい。

	main.c の TONENO を 9 にすることにより、vibrato のかかった
	効果音になります。


【プログラムのロード】

	> psymake
	> psymake load
	> dbugpsx main /r50 /e

	を行なって下さい。


                  Copyright (C) 1994 by Sony Computer Entertainment Inc.
