$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

ＳＩＯのサンプル： tuto2


・デバッグステーションとＰＣをH3050で接続してＰＣからの入力を
  エコーバックするプログラムです。標準入出力のドライバをもち
　いずに、BIOSを用いて、受信割り込みによるコールバックを実現
  しています。

・実行方法
        psymake
        resetps
        run tuto2

・ファイルの説明
        makefile.mak    メイクファイル
        main.c          メインルーチン


・コントロールパッドの操作は以下の通り。

        SELECTボタン：  終了

                Copyright (C) 1997 by Sony Computer Entertainment
                All Rights Reserved

