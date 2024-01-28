$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$
┏━━━━━━━━━━━━━━━━━┓
┃  ♪サウンドサンプルプログラム♪  ┃
┗━━━━━━━━━━━━━━━━━┛

main：SEQ および SEP データの演奏をします。

  SEP である sample.sep は、

	brahms.seq
	sonata_loop.seq （曲中繰り返しデータが含まれています）
	fuga.seq

  を 連結したものです。


        コントロールパッドの操作は以下の通りです。
                H ボタン (START):       SEQ プレイ
                G ボタン (△ボタン):    SEP Track 2 プレイ
                                        SEQ デクレッシェンド
                P ボタン (○ボタン):    SEP Track 3 プレイ
                                        SEP Track 2 デクレッシェンド
                F ボタン (×ボタン):    SEQ，SEP Track 2 ストップ
                K ボタン (SELECT):      クローズ (演奏終了)


【プログラムのロード・実行】
	
	> psymake
	> psymake load
	> dbugpsx main /r50 /e

	を行なって下さい。


                 Copyright (C) 1994 by Sony Computer Entertainment Inc.
