$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

	FLAT TEXTURE 4角形の完全透視変換サンプルプログラム
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<説明>
	四角形のピクセルごとに透視変換を行ない、歪まないテクスチャ
	マッピングを行なうサンプルです。
	現在ではjimenサンプルの
	
		draw_general_square

	という関数を使ったほうが高速にテクスチャマッピングできます。

<操作方法>
	PADRup/down:	Ｚ軸まわりの回転
	PADRleft/right:	Ｙ軸まわりの回転
	PADl/PADn:	Ｚ軸方向の移動
	PADLup/down:	Ｙ軸方向の移動
	PADLleft/right:	Ｘ軸方向の移動
	PADk:		終了
