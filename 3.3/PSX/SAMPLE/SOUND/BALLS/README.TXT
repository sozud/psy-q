$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$

┏━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃  ♪サウンド＆グラフィックスサンプルプログラム♪  ┃ 
┗━━━━━━━━━━━━━━━━━━━━━━━━━┛       

・main：ball が壁にぶつかったときに効果音を鳴らします。
	バックグラウンドでは曲が流れています。
	sound と graphics を組み合わせた使用例です。

	コントロールパッドの操作は以下の通りです。
                A ボタン(↑ボタン) ： ホイッスル(低)
		B ボタン(↓ボタン) ： ホイッスル(高)
                C ボタン(←ボタン) ： バスドラム
                D ボタン(→ボタン) ： ハンドクラップ(拍手)
                E ボタン(△ボタン) ： タム(高)
                F ボタン(×ボタン) ： タム(低)
                G ボタン(□ボタン) ： コンガ(高)
                P ボタン(○ボタン) ： コンガ(低)
                N ボタン(L1ボタン) ： 「ごめんね」
                L ボタン(R1ボタン) ： スネア
                K ボタン(SELECT) ： 終了

	上記のボタンを押している間、ボールが上下に動きます。
	壁にボールがぶつかったときにそれぞれの効果音が鳴ります。

【プログラムのロード】

	> psymake
	> psymake load
	> dbugpsx main /r50 /e

	を行なって下さい。


		Copyright (C) 1994 by Sony Computer Entertainment Inc.
