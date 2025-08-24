$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

							1996.01.27
	
				  rgb24
	
	    Copyright (C) 1994-1996, Sony Computer Entertainment
			    All right reserved
	

	これは、フレームバッファを 24bit モードで表示するためのサンプ
	ルプログラムです。
	24bit モードの場合 RGB それぞれに 8bit (256色）、合計で 3byte 
	が割り当てらるため、実際の表示ピクセルとフレームバッファの x 
	アドレスは一致しません。 
	また 24bit モードでの描画はできませんが、フレームバッファ間、
	フレームバッファ・メインメモリ間の転送は可能になります。

	詳細は、

		doc/jp/gpu/{abstract.txt,function.txt}

	を参照下さい。

