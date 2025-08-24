$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

LOWLEVEL	GsDOBJ2型オブジェクトを使ったTMDビューワー

				1995 (C) Sony Computer Entertainment Inc.

tuto0: TMDデータを解析してポリゴンタイプに応じて、GsSortObject4J()の
       下位関数群を呼び出す

【起動方法】

	> psymake
	> psymake load
	> run tuto0

【操作方法】

	左十字キー	オブジェクトの並行移動
	右十字キー	オブジェクトの回転
	R1R2キー	拡大縮小

【説明】

	このプログラムは、GsDOBJ2型オブジェクトからGPUパケットを生
	成しオーダリングテーブルに登録する関数GsSortObject4()の一
	部機能に相当する部分を実装したサンプルプログラムです。
	GsSortObject4()のフラット３角ポリゴンの処理部分に相当する
	関数がSortTMDFlat()です。

	この関数内でTMDデータを解析し、ポリゴンタイプに対応する
	GsSortObject4()の下位関数群(GsTMDfastF3L(),
	GsTMDfastTF3L())を呼び出しています。このサンプルプログラム
	では、フラット３角ポリゴンのみをサポートするように書かれて
	いますので、cube3.tmdなどフラット３角のTMDデータのみ表示可
	能です。



tuto1: TMDデータを解析して、GPUパケットを生成し、オーダリングテー
       ブルに登録する。

【起動方法】

	> psymake tuto1
	> psymake load2
	> run tuto1

	注）このサンプルプログラムはDM(Direct Mapping)を使用してい
	ますのでDMPSXが必要です。

【操作方法】

	左十字キー	オブジェクトの並行移動
	右十字キー	オブジェクトの回転
	R1R2キー	拡大縮小


【説明】

	このプログラムは、GsSortObject4()の下位関数群
	(GsTMDfastG4L(), GsTMDfastTG4L() など)に相当する部分をGTE
	インライン関数を用いて実装したサンプルプログラムです。

	このサンプルプログラムでは、グーロー４角ポリゴンのみ実装さ
	れていますのでcube4g.tmdなどグーロー４角のTMDデータのみ表
	示可能です。



tuto2: TMDデータを解析して、GPUパケットを生成しオーダリングテーブ
       ルに登録する。(スキマ問題対応版)

【起動方法】

	> psymake tuto2
	> psymake load2
	> run tuto2

	注）このサンプルプログラムはDM(Direct Mapping)を使用してい
	ますのでDMPSXが必要です。

【操作方法】

	左十字キー	オブジェクトの並行移動
	右十字キー	オブジェクトの回転
	R1R2キー	拡大縮小


【説明】

	このプログラムは、GsSortObject4()の下位関数群
	(GsTMDfastG4L(), GsTMDfastTG4L() など)に相当する部分をGTE
	インライン関数を用いて実装したサンプルプログラムです。ただ
	し、通常の下位関数群では、座標変換と透視変換にRotTransPers
	関数を使用していますが、このサンプルプログラムでは、スキマ
	問題に対応するためTransRotPers関数に対応する処理を付加して
	あります。

	このサンプルプログラムでは、グーロー４角ポリゴンのみ実装さ
	れていますのでcube4g.tmdなどグーロー４角のTMDデータのみ表
	示可能です。


tuto3: TMDデータを解析して、GPUパケットを生成しオーダリングテーブ
       ルに登録する。(mipmap版)

【起動方法】

	> psymake tuto3
	> psymake load3
	> run tuto3

	注）このサンプルプログラムはDM(Direct Mapping)を使用してい
	ますのでDMPSXが必要です。

【操作方法】

	左十字キー	オブジェクトの並行移動
	右十字キー	オブジェクトの回転
	R1R2キー	拡大縮小


【説明】

	このプログラムは、GsSortObject4()の下位関数群
	(GsTMDfastG4L(), GsTMDfastTG4L() など)に相当する部分をGTE
	インライン関数を用いて実装したサンプルプログラムです。ただ
	し、下位関数の演算時に、Z値に応じてテクスチャUV座標を付け
	替えて描画の高速化をはかる、mipmapを実装するサンプルプログ
	ラムです。ここでは、オリジナルのテクスチャを座標(0,32)に置
	き、1/4のテクスチャを(0,16)に、1/16のテクスチャを(0,8)に置
	いてZ値に応じて付け替えるようにしています。

	このサンプルプログラムでは、グーロー４角ポリゴンのみ実装さ
	れています。


						以上

