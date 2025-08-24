$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

		DMPSXによる高速化サンプルプログラム
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<説明>
	tuto0.cはDMPSXを使ってプログラムを高速化する例を示したものです。
	libgteの低レベル関数を使って書いた場合とそれをDMPSXによって
	書き直しさらに最適化した場合のスピードの比較が行なえます。
	
	プログラムはそのままでDMPSX版になっています。先頭にある
		#define DMPSX_MACRO
	をコメントアウトすることによりlibgte低レベル関数版にすることが
	できます。

	画面に表示される数字は計算と描画の合計時間をＨ単位で表したものです。

<操作方法>
	PADRup/down	.....	Ｘ軸まわり回転
	PADRleft/right	.....	Ｙ軸まわり回転
	PADm/o		.....	Ｚ軸方向移動
	PADl/n		.....	Ｚ軸まわり回転
	PADLup/down	.....	フォグＯＮ／ＯＦＦ
	PADLleft/right	.....	フォグニアーの移動
	PADk		.....	終了
