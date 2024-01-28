$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$


	グラフィックスと動画とのコンビネーション



【機能】
	動画を BG もしくは テクスチャ領域に転送して BGないしテクスチャとして
	利用するプログラム

	TMDVIEWとANIMをコンビさせたプログラムである。TMDVIEWの方がメインで
	ANIMの方が サブルーチンとして呼び出されている。

【ファイル構成】

	cdadd.c	tuto0.c
	cube3.tmd pat4.tim pat8.tim pat16.tim
	makefile.mak

【起動法】
	psymake	
	psymake load

【操作法】
	パッドの左十字キーで 平行移動
	パッドの右十字キーで 回転

【プログラムの説明】
	tuto0.cで BGをデファインすると 動画がBGに張られます
	tuto0.cで TEXTURE16をデファインすると 動画がテクスチャとして
	張られます。

	ver3.2 以降のlibpressでは DecDCTvlcSize()が使えるようになります。
	これは VLCのデコードを任意のサイズで中断できるもので movieの
	負荷を分散しコントロールすることができます。

	DecDCTvlcSize()で与えるサイズを大きくすると アニメーションは滑らか
	になりますが ３Dのグラフィックスはコマ落ちします。
	サイズを小さくすると アニメーションはコマ落ちしますが ３Dの部分は
	滑らかに動きます。 ケースバイケースで使い分けて下さい。

