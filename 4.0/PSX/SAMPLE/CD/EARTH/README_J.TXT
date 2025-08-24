$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

							1996.02.08 
			テクスチャアニメーション
		
		Copyright (C) 1994, Sony Computer Entertainment
			    All right reserved
	

	ここには、動画をテクスチャパターンとして使用するサンプルが収め
	られています。 ここでは、メインメモリに１フレーム分の解凍画像
	バッファを持たずにテクスチャパターンを転送する方法がとられてい
	ます。

	動画再生は、cdadd.c の
	
		animInit();
		animPoll();

	によって実行されます。animPoll() は内部で DecDCTvlc() をコール
	しており処理時間がかかります。そのためにコールバック内でこれを
	呼び出すのはお避け下さい。

	また、動画転送に使用する LoadImage() を他の描画処理に優先して
	行なうために、 BreakDraw() 関数を使用しています。現在 
	BreakDraw() は DrawOTag() による描画処理は中断できますが、他の
	LoadImage()/StoreImage() を中断することができません。アプリケー
	ションで他の LoadImage() をバックグラウンドで使用する場合には
	ご注意下さい。

	詳細は、

		doc/jp/format/pmd.txt
		doc/jp/press/{abstract,function}.txt
		doc/jp/cd/{abstract,function}.txt
	
	を参照下さい。関連するサンプルプログラムとしては、

		sample/graphics/pmd
		sample/cd/movie
		sample/cd/str3d

	を参照下さい。
