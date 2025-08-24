$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

	ラウンドメッシュ用関数(RotMeshPrimR_?)のサンプルプログラム
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<説明>
	ラウンドメッシュデータを座標変換、透視変換しパケットをつくりＯＴに
	リンクします。ラウンドメッシュデータはTMESH構造体で指定されます。


		    2-------3
		   / \     / \
		  /   \   /   \
		 /     \ /     \
		1-------0-------4
			 \     /
			  \   /
			   \ /
			    5

		ラウンドメッシュデータ


	typedef struct {
        	SVECTOR         *v;             /*shared vertices*/
        	SVECTOR         *n;             /*shared normals*/
        	SVECTOR         *u;             /*shared texture addresses*/
        	CVECTOR         *c;             /*shared colors*/
        	u_long          len;            /*mesh length(=#vertex)*/
	} TMESH;

			TMESH構造体

	上の図では
		len = 6
		v,n,u,cは上の番号順の配列



	tuto0はいろいろなモードを一度に表示します。
	表示するポリゴンの数は一定です。
	G3, F3, FT3, GT3
	GC3,FC3,FCT3,GCT3,T3


<操作方法>
	tuto0
		PADRup,PADRdown		....X軸まわり回転
		PADRleft,PADRright	....Y軸まわり回転
		PADLup,PADLdown		....Y軸方向平行移動
		PADLleft,PADLright	....X軸方向平行移動
		PADL1,PADR1		....Z軸方向平行移動
		PADR2,PADL2		....Z軸まわり回転 & 
					    デプスキューイング ON/OFF
		PADstart,PADselect	....backface clip ON/OFF
		PADR2&PADL2&PADselect	....終了


