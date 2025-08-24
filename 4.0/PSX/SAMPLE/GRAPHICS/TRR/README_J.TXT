$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

	TransRot... sample program

	このプログラムは隣接したポリゴンが異なる座標系に属するとき
	に発生するスキマを解消するための関数TransRot...のサンプルです。
	ここでは、９つの正方形が辺の長さだけずらした座標系に置かれていると
	します。全体を含む座標系（ワールド）が回転していないとき、隣接する
	頂点の座標は一致するので９つの正方形の間にスキマは発生しません。
	しかし、ワールドが回転すると通常のRotTransPersなどでは、
	平行移動量を回転したものが別個に足されるため計算誤差により
	頂点が一致せずスキマが発生する場合があります。
	TransRot...関数は最初に平行移動量を足しておいて次に回転を
	くわえるためスキマが発生しません。

	
	Lupボタン...	RotTransPersによるスキマあり
	Lleftボタン...	TransRotPersによるスキマなし
	Ldownボタン...	TransRotPers3によるスキマなし
	Lrightボタン...	TransRot_32によるスキマなし（32bit空間まで計算可）

