$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

			libpress チュートリアル
							1994.07.13. suzu (SCE)

	このディレクトリには、低レベルグラフィックライブラリ libpress の
	レベルの簡単なチュートリアルプログラムが入っています。

	---------------------------------------------------------------------
	ファイル	|内容
	----------------+----------------------------------------------------
	tuto1		|単純な VLC デコードと MDEC オンメモリデコードの実験
	tuto2		|LoadImage() と、DecDCTout() のインターリーブ
	tuto3		|単純な動画の再生（フレームダブルバッファ付き）
	tuto4		|コールバックの実現
	tuto5		|LoadImage() をバックグラウンドで実行
	tuto6		|バックグラウンド動画の再生
	---------------------------------------------------------------------

	各プログラムは、以下のテストパターンを使用します。

	---------------------------------------------------------------------
	ファイル	|内容
	----------------+----------------------------------------------------
	siro.bs		|RAW ビットストリーム (256x170)
	msiro/*.bs	|RAW ビットストリーム（アニメーションパターン）
	---------------------------------------------------------------------

