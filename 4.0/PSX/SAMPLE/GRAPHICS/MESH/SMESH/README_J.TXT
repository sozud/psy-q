$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

	ストリップメッシュ用関数(RotMeshPrimS_?)のサンプルプログラム
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

<説明>
	ストリップメッシュデータを座標変換、透視変換しパケットをつくりＯＴに
	リンクします。ストリップメッシュデータはTMESH構造体で指定されます。


		    1-------3-------5-------7-------9
		   / \     / \     / \     / \     /
		  /   \   /   \   /   \   /   \   /
		 /     \ /     \ /     \ /     \ /
		0-------2-------4-------6-------8

			ストリップメッシュデータ


	typedef struct {
        	SVECTOR         *v;             /*shared vertices*/
        	SVECTOR         *n;             /*shared normals*/
        	SVECTOR         *u;             /*shared texture addresses*/
        	CVECTOR         *c;             /*shared colors*/
        	u_long          len;            /*mesh length(=#vertex)*/
	} TMESH;

			TMESH構造体

	上の図では
		len = 10
		v,n,u,cは上の番号順の配列


	tuto0はいろいろなモードごとに表示します。
	表示するポリゴンの数は可変です。
	F3->G3->FT3->FC3->GC3->GT3->FCT3->GCT3->T3┐
	└────────────────────┘
	表示される数字は１秒間に描画しているポリゴンの数です。
	それぞれのモードでポリゴンの数が MAX のときは
	コンパイラの種類、コンパイラオプションなど
	の条件によって６０フレーム／秒に間に合わない場合があります。

	tuto1はいろいろなモードを一度に表示します。
	表示するポリゴンの数は一定です。
	G3, F3, FT3, GT3
	GC3,FC3,FCT3,GCT3,T3

	tuto2は 
	RotMeshPrimS_F3
	RotMeshPrimR_F3 
	RotMeshPrimS_FT3
	RotMeshPrimR_FT3
	を使って正２０面体（イコサ）をつくる例です。

<操作方法>
	tuto0
		PADRup,PADRdown		....X軸まわり回転
		PADRleft,PADRright	....Y軸まわり回転
		PADLup,PADLdown		....ポリゴンモード変更
		PADLleft,PADLright	....ポリゴン数変更
		PADL1,PADR1		....Z軸方向平行移動
		PADR2,PADL2		....デプスキューイング ON/OFF
		PADstart,PADselect	....backface clip ON/OFF
		PADL1&PADR1&PADstart	....ホップストップ
		PADR2&PADL2&PADselect	....終了
	tuto1
		PADRup,PADRdown		....X軸まわり回転
		PADRleft,PADRright	....Y軸まわり回転
		PADLup,PADLdown		....Y軸方向平行移動
		PADLleft,PADLright	....X軸方向平行移動
		PADL1,PADR1		....Z軸方向平行移動
		PADR2,PADL2		....デプスキューイング ON/OFF
		PADstart,PADselect	....backface clip ON/OFF
		PADR2&PADL2&PADselect	....終了

	tuto2
		PADRup,PADRdown		....X軸まわり回転（全体）
		PADRleft,PADRright	....Y軸まわり回転（全体）
		PADL1,PADR1		....Z軸まわり回転（全体）
		PADR2,PADL2		....Z軸方向平行移動
		PADLup			....イコサ増加
		PADLdown		....イコサ減少
		PADLleft		....テクスチャつきイコサ増加
		PADLright		....テクスチャつきイコサ減少
		PADLup&PADh		....デプスキューイング ON
		PADLdown&PADh 		....デプスキューイング OFF
		PADLleft&PADh		....デプスキューイング遠ざかる 
		PADLright&PADh		....デプスキューイング近づく
		PADk			....終了
