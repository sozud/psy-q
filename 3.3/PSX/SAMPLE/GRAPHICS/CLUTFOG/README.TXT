$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$
							1995.09.29 S.C.E
	
clut によるテクスチャへの FOG 効果のサンプルプログラム

・tuto0
	フレームごとに FOG をかけた CLUT を生成し、VRAM 上にロードして
	FOG 効果を出します。

・tuto1
	あらかじめ数段階の FOG のかかった CLUT を VRAM 上に用意してお
	き、描画するポリゴンごとに適当なものを選択して FOG 効果を出し
	ます。

・tuto2
	あらかじめ数段階の FOG のかかった CLUT を VRAM 上に用意してお
	き、OT にぶらさげた DR_MOVE プリミティブで深さに応じた CLUT を 
	CLUT 領域に転送することによって、FOG 効果のかかった描画をおこ
	ないます。この方法はパケット生成時でなく描画時に CLUT を変更す
	るので、 libgs による描画にもそのまま適用することができます。


・コントロールパッドによる操作は以下の通りです。

	左十字キー	上下左右の移動
	L1, L2		前後の移動
	
	R1, R2		回転 (tuto0 を除く)
	右十字キー	回転 (tuto0 を除く)

・プログラムのコンパイル

	> psymake tuto0
	
	> psymake tuto1

	> psymake tuto2

・プログラムのロード

	> psymake load
	> run tuto0

	> psymake load
	> run tuto1

	> psymake load
	> run tuto2
	
・その他のヒント
	・tuto0 は、毎フレーム CLUT を生成するので、精度などの面で自由が
	  効きますが、一つの CLUT を使うポリゴンはすべておなじ FOG が
	  かかってしまいます。また、LoadImage を使うため、実行時間でも
	  多少不利になることがあります。
	・tuto1 は、ポリゴンプリミティブの CLUT ID を書き換えねばなら
	  ない点と、VRAM 上の CLUT 領域が増えてしまう点が不便ですが、
	  一つの CLUT を使う ポリゴンでもそれぞれの深さに応じた FOG 効
	  果をかけることができます。
	・tuto2 は、VRAM 上の CLUT 領域が増えてしまうという点は tuto1 
	  と同じです。しかしポリゴンプリミティブと直接の関係を持たずに 
	  CLUT を書き換えることができるため、FOG 解像度(FOG の段数) に
	  比べて該当ポリゴン数が多いときや、libgs を用いた描画のときに
	  有利です。

	・otz 値とフォグパラメータ p 値との関係は otz2p(), p2otz() を
	  使うと求めることができます。

-----------------------------------------------------------------------------
							1995.06.22 S.C.E
	Sample program for CLUT fog texture mapping

*tuto0
	tuto0 makes depth queued CLUT by each frame, and loads it to the
	frame buffer. This method does not need more extra CLUT memroy
	area on the frame buffer, but not fast.

*tuto1
	tuto1 makes some depth queued CLUT patterns at the first time.
	Each primitive change its CLUT entry according to its Z value
	to simulate foggy space. This method is faster than tuto0 but
	it needs more CLUT memory area.

*controller assign
	Left cross key		move up/down/right/left 
	L1,L2			foreward/backwaord
	R1,R2			rotation (except tuto0)
	Right cross key		rotation (except tuto0)

*compile
	>psymake tuto0
	>psymake tuto1
	>psymake tuto2

*load
	>psymake load
	>run tuto0

	>psymake load
	>run tuto1

	>psymake load
	>run tuto2

			Copyright (C) 1995 Sony Computer Entertainment Inc.
