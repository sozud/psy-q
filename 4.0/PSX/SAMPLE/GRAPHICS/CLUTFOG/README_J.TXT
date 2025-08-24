$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
							1995.07.05 S.C.E
	
clut によるテクスチャへのフォグ(デプスキューイング) 効果のサンプルプログラム

・tuto0
	フレームごとにフォグをかけた CLUT を生成し、VRAM 上にロードして
	フォグ効果を出します。

・tuto1
	あらかじめ数段階のフォグのかかった CLUT を VRAM 上に用意してお
	き、描画するポリゴンごとに適当なものを選択してフォグ効果を出し
	ます。

・tuto2
	あらかじめ数段階のフォグのかかった CLUT を VRAM 上に用意してお
	き、OT にぶらさげた DR_MOVE プリミティブで深さに応じた CLUT を 
	CLUT 領域に転送することによって、フォグ効果のかかった描画をおこ
	ないます。

・tuto3
	描画するポリゴンごとに対応する深さの CLUT を作成し、 DR_LOAD
	プリミティブによってポリゴン描画の直前に VRAM へ転送し、フォグ
	効果を出します。

doc/jp/text/gte/abstract.txt に、デプスキューイング効果のための全般的
な解説がありますので、そちらもご覧ください。

・コントロールパッドによる操作は以下の通りです。

	左十字キー	上下左右の移動
	L1, L2		前後の移動
	
	R1, R2		回転 (tuto0 を除く)
	右十字キー	回転 (tuto0 を除く)

・プログラムのコンパイル

	> psymake tuto0
	
	> psymake tuto1

	> psymake tuto2

	> psymake tuto3

・プログラムのロード

	> psymake load
	> run tuto0

	> psymake load
	> run tuto1

	> psymake load
	> run tuto2
	
	> psymake load
	> run tuto3

			Copyright (C) 1995-1996 Sony Computer Entertainment Inc.
