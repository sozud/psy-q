$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

							1996.01.27
	
		Mixture of lowlevel and high level functions
	
	    Copyright (C) 1994-1996, Sony Computer Entertainment
			    All right reserved
	

	ここには、高レベルライブラリ libgs と低レベルライブラリ libgte 
	を同時に使用したサンプルが収められています。いずれのサンプルも
	libgs で処理された３Ｄオブジェクトと libgte で左上端点が計算さ
	れた複数個のスプライトが同時に表示されます。

	libgs の関する詳細は、	

		doc/jp/gs/{abstract.txt,function.txt}

	を参照下さい。libgte に関する詳細は、
	
		doc/jp/gte/{abstract.txt,function.txt}

	を参照下さい。
	
	============================================================
	プログラム	内容				
	------------------------------------------------------------
 	tuto0		libgs の管理するＯＴに低レベルプリミティブを 
			AddPrim() 関数を使用して接続して表示する
	tuto1		低レベルのＯＴに libgs の管理する DOBJ オブジェ
			クトを接続して表示する
	------------------------------------------------------------



