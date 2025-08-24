$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

							1996.01.27
	
				  2D
	
	    Copyright (C) 1994-1996, Sony Computer Entertainment
			    All right reserved
	

	これはメッシュ構造を利用して曲面に２次元画像をマッピングするサ
	ンプルです。PlayStation のテクスチャマッピング機能は１次変換
	（線形変換）のみですが、 ポリゴンを細かいメッシュに分割してメッ
	シュ上の各点（代表点）のみを正しく計算することでさまざまな曲面
	にマッピングを行なうことができます。こうしたポリゴン分割の考え
	方は透視変換の際に歪まないテクスチャマッピングを行なう場合にも
	応用できます。

	詳細は、

		doc/jp/gpu/{abstract.txt,function.txt}
		doc/jp/gte/{abstract.txt,function.txt}

	を参照下さい。

