$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

回転マトリクスからオイラー角を求める例

	    Copyright (C) 1994-1996, Sony Computer Entertainment
			    All right reserved

・動作

このサンプルでは、RotMatrix() の逆をおこなう方法の一例として、関数 
InvRotMatrixZYX() を示します。


画面左側の四角形は、パッドの操作によって回転します。
画面右側の四角形は、左と同じ回転マトリクスから一旦オイラー角を求め、
	再度回転マトリクスを計算して回転します。


		パッド操作
		    ↓
		原オイラー角
		    ↓
		回転マトリクス →   (オイラー角計算)   → 推定オイラー角
		    ↓              InvRotMatrixZYX()        ↓
		    ↓                                       ↓
		左側四角形描画                         回転マトリクス
		                                             ↓
		                                       右側四角形描画


このサンプルでは、回転の順序が Z,Y,X 軸まわりの順 (RotMatrix() と同順) 
であるとしています。

一般に、ある回転マトリクスに対するオイラー角は複数存在するため、かならずし
も原オイラー角と同じ推定オイラー角が求められるわけではありません。


・プログラムのコンパイル

	> psymake all

・プログラムの実行

	> psymake load
	> run tuto0

・パッド操作
	左十字キー上下	X 軸回転
	左十字キー左右	Y 軸回転
	L1, L2		Z 軸回転

