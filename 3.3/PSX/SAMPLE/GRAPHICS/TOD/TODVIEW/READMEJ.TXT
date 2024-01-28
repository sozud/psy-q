$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
PS-X Demonstration Program "todview"
==================================

・todview は、1つの TODファイルに記述されたアニメーションを表示する
  サンプルプログラムです。使用される TODファイル名と TMDファイル名は、
  メイクファイルに書かれています。

  また、プログラム内で次のような処理を行っています。
	視点の移動
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

	te.tmd		モデリングデータ
	jaanken.tod		TODデータ

・コントロールパッドの操作は以下の通り。

	十字キー(左)：	視点が X軸のプラス方向に移動
	十字キー(右)：	視点が X軸のマイナス方向に移動
	十字キー(下)：	視点が Y軸のプラス方向に移動
	十字キー(上)：	視点が Y軸のマイナス方向に移動
	L1ボタン：		視点が Z軸のプラス方向に移動
	L2ボタン：		視点が Z軸のマイナス方向に移動
	○ボタン, ×ボタン, □ボタン, △ボタン：	アニメーションの再生
	STARTボタン：	視点を初期位置に戻す
	SELECTボタン：	終了


		Copyright (C) 1994,1995 by Sony Computer Entertainment
		All Rights Reserved
