$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

このサンプルは ４角形ポリゴンを Z値 および ポリゴンの大きさに 応じて
適当な分割数で 分割するものです。

分割時に ポリゴンのすき間を埋める処理を 行なっています。

また DM（Direct Mapping) を使って コーディングされていますので
コンパイルに当たっては DMをインストールしておく必要があります。

予め bload earth8.tim 80100000 を行なって テクスチャをロードしておいてから
プログラムをはしらせて下さい。

このプログラムは 速度優先でつくってあるので 歪みを減らしたい場合は
分割を行なうポイントを より遠方に設定して下さい。


						以上
