$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

	２次元メッシュ用関数(RotMeshPrimQ_T)のサンプルプログラム
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<説明>
	２次元メッシュデータを座標変換、透視変換しパケットをつくりＯＴに
	リンクします。２次元メッシュデータはQMESH構造体で指定されます。


		0---1---2---3---4---5---6---7---8
		|   |   |   |   |   |   |   |   |
		9---10--11--12--13--14--15--16--17
		|   |   |   |   |   |   |   |   |
		18--19--20--21--22--23--24--25--26
		|   |   |   |   |   |   |   |   |
		27--28--29--30--31--32--33--34--35

			２次元メッシュデータ


	typedef struct {
       		SVECTOR         *v;             /*shared vertices*/
        	SVECTOR         *n;             /*shared normals*/
        	SVECTOR         *u;             /*shared texture addresses*/
        	CVECTOR         *c;             /*shared colors*/
        	u_long          lenv;           /*mesh length_V(=#vertex_V)*/
        	u_long          lenh;           /*mesh length_H(=#vertex_H)*/
	} QMESH;

			QMESH構造体

	上の図では
		lenv = 4
		lenh = 9
		v,n,u,cは上の番号順の配列

	サンプルプログラムtuto0では
		lenh = 42
		lenv = 43
	サンプルプログラムtuto1では
		lenh = 42
		lenv = 46
	です。
	３点同時計算関数を使っているためlenhは３の倍数でなければなりません。
	RotMeshPrimQ_Tはテクスチャだけのモードであるので光源計算は
	デプスキューイング以外行ないません。従ってQMESHのn,cは無視されます。

	RotMeshPrimQ_Tはあるクリップ領域の外側にあるポリゴンを描画しない
	スクリーンクリップを行なえます。
	tuto0はクリップ領域をスクリーンより少し小さく設定して
	クリップの様子をわかりやすくしています。
	tuto1はクリップ領域をスクリーンと一致させています。

	表示される数字は１フレーム分の計算時間で、単位はＨです。

<操作方法>
	tuto0
		PADRup,PADRdown		....Z軸まわり回転
		PADRleft,PADRright	....Y軸まわり回転
		PADL1,PADR1		....Z軸方向平行移動
		PADLleft,PADLright	....X軸方向平行移動
		PADR2,PADL2		....backface clip ON/OFF
		PADLup,PADLdown		....デプスキューイングON/OFF
		PADselect		....終了
	tuto1
		PADRleft,PADRright	....Y軸まわり回転
		PADL1,PADR1		....Z軸方向平行移動
		PADR2,PADL2		....backface clip ON/OFF
		PADRup,PADRdown		....デプスキューイングON/OFF
		PADLleft,PADLright	....X軸方向平行移動
		PADLup,PADLdown		....Y軸方向平行移動

