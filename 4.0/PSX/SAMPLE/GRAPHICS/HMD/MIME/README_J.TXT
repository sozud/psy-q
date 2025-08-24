$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
$PSDocID$

HMD でのマイムサンプルプログラム

                             Copyright(C) 1997 Sony Computer Entertainment Inc.
                             All rights reserved.



・プログラムのコンパイル

	> psymake all

・プログラムの実行
	モデル #0 (トリケラトプスによる Joint-Axes-MIMe サンプル)
	> psymake load0
	> run tuto0

	モデル #1 (ぷよでによる Vertex/Normal-MIMe サンプル)
	> psymake load1
	> run tuto0

	tuto2 の実行 (トリケラトプスによる Joint-RPY-MIMe サンプル)
	> psymake load2
	> run tut0

・このサンプルの使い方

	* A パッド: 
		L1/L2/R1/R2→←↑↓: MIMe パラメータ操作
			L1	MIMe 動作 1
			L2	MIMe 動作 2
			R1	MIMe 動作 3
			R2	MIMe 動作 4
			←	MIMe 動作 5
			→	MIMe 動作 6
			↑	MIMe 動作 7
			↓	MIMe 動作 8
			画面にそれぞれの MIMe 係数(重み)が表示されます

			(モデル「ぷよで」には MIMe 動作は 1-4 しかありません)

		○△□×: オブジェクトの回転
		select, start: オブジェクトの Z 軸平行移動


・説明
	HMD データフォーマットを用いたマイムのサンプルです。

	トリケラトプスのモデルによるサンプルでは、関節の曲げを多重内挿
	します (Joint-MIMe)。内挿方法には、Axes-MIMe と RPY-MIMe の二
	種類があります。トリケラトプスは関節構造をもったワンスキンモデ
	ルです。

	ぷよでのモデルによるサンプルでは、従来型である頂点と法線の多重
	内挿をします (Vertex/Normal-MIMe)。ぷよでは関節構造を持ちませ
	ん。

・マイムデータを含む HMD データを扱うプログラム作成時の注意
	マイムプリミティブを持つ HMD を扱う場合には、通常の HMD の初期
	化の他、関数 GsMap*MIMe() による初期化が必要です。GsMap*MIMe() 
	関数を呼び出すタイミングは、GsScanUnit() 後、GsSortUnit() 前で
	す。

	また、マイムパラメータ(内挿の重み係数)を HMD 内でなくプログラ
	ム側で持つ場合は、パラメータを置くアドレスを指定する必要があり
	ます。指定場所は GsMap*MIMe() 関数の返り値によってわかります。

	このサンプルでは、これらの初期化を tuto0.c 中の model_init() 
	内でおこなっています。


【追記】
	本サンプルで使用しているモデルデータ「trirpy.hmd」「triaxes.hmd」
	は、米国 Viewpoint DataLabs により作成され、米国 Newtek	Incorporated
	により配布されているデータを元にして作成されました。


								以上
