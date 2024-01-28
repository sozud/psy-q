$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
PS-X Demonstration Program "janken"
==================================

・janken は、階層構造を持った手のモデリングデータを用いて「じゃんけん」の
  アニメーションを表示します。
  複数の TODデータを使ってアニメーションを表示するサンプルプログラムです。

  また、プログラム内で次のような処理を行っています。
	視点の移動
	TODデータの切り替え
	フレームごとの TODデータの読み取りと描画

・実行方法
	psymake
	resetps 1
	run \....\patchx.cpe
	psymake load
	run main

・ファイルの説明
	makefile.mak	メイクファイル

	main.c		メインルーチン
	te.h			TMD ID テーブル

	tmd\te.tmd		モデリングデータ

	tod\appear.tod	TODデータ ( 手が歩いて登場する部分 )
	tod\jaanken.tod	TODデータ ( 「じゃ～んけ～ん」と握り拳を振る部分 )
	tod\n_c.tod		TODデータ ( 「ぽい」の部分。定位置から「ちょき」を出す。 )
	tod\c_n.tod		TODデータ ( 「ちょき」から次の手を出すため定位置に戻る部分。 )
	tod\n_g.tod		TODデータ ( 「ぽい」の部分。定位置から「ぐー」を出す。 )
	tod\g_n.tod		TODデータ ( 「ぐー」から次の手を出すため定位置に戻る部分。 )
	tod\n_p.tod		TODデータ ( 「ぽい」の部分。定位置から「ぱー」を出す。 )
	tod\p_n.tod		TODデータ ( 「ぱー」から次の手を出すため定位置に戻る部分。 )
	tod\disapp.tod	TODデータ ( 手がジャンプして退場する部分。 )

・コントロールパッドの操作は以下の通り。

	十字キー(左)：	視点が X軸のプラス方向に移動
	十字キー(右)：	視点が X軸のマイナス方向に移動
	十字キー(下)：	視点が Y軸のプラス方向に移動
	十字キー(上)：	視点が Y軸のマイナス方向に移動
	L1ボタン：		視点が Z軸のプラス方向に移動
	L2ボタン：		視点が Z軸のマイナス方向に移動
	○ボタン：		「ぐー」のアニメーションの再生
	△ボタン：		「ちょき」のアニメーションの再生
	□ボタン：		「ぱー」のアニメーションの再生
	×ボタン：		「ぐー」、「ちょき」、「ぱー」のいずれかのアニメーションを
				ランダムに再生
	STARTボタン：	視点を初期位置に戻す
	SELECTボタン：	終了


		Copyright (C) 1994,1995 by Sony Computer Entertainment
		All Rights Reserved
