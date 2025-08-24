$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
		DTL-H2500 Psy-Q Driver's Kit

本パッケージは、Psy-Q プログラマーツール DTL-H2500 において Psy-Q ツール
を使用するためのドライバキットです。

1.インストール
  まず、DTL-H2500 の取扱説明書に書いてある「ボードの取り付け」の手順にし
たがってボードを正しく接続してください。
以下の手順にしたがって DTL-H2500 の初期化を行います。
  DOS> cd \psyq\flash25
  DOS> flash.bat
これでインストールは終了です。

2.DTL-H2500 用ドライバ h25bios の組み込み
  DTL-H2500 で Psy-Q ツールを利用するには h25bios.com というドライバが組
み込まれている必要があります。これは DTL-H2000(ISAボード) の dexbios.com
に相当するものです。
MS-DOS ではプロンプト状態、Windows 3.1 or 95 では DOS 窓を開き、
  DOS> cd \psyq
  DOS> h25bios
とします。正常に組み込まれると、
  Psy-Q BIOS for Sony DTL-H2500.   Version 1.41
  Installed OK - card address: DF000  Interrupt: 3  Video: NTSC
というように出力されます。(ただし Interrupt は PC の環境に依存)
これで DTL-H2000 と同様に resetps, run, pqbload, dbugpsx などが使用可能
となります。

3.DTL-H2500 付属のフロッピーディスクのユーティリティ
  付属のフロッピーディスクに入っているツールは Psy-Q ツールとは同時にご
利用できません。付属のフロッピーディスクのユーティリティを使う場合は付属
ディスクの \h2500\util\flash.bat を実行して別途 DTL-H2500 の初期化を行っ
てください。再度、Psy-Qツールを使う場合は \psyq\flash25\flash.bat を実行
して、Psy-Q ツール用の初期化を行ってください。なお、Psy-Q ツールを使用す
る場合、このユーティリティは必要ありません。

4.トラブルシューティング
  h25bios.com を実行時に "Cannot locate DTL-H2500 PCI card" というメッ
セージが出力された場合には DTL-H2500 が正常に認識されておりません。PC の
BIOS 設定でシャドウをすべて無効(Disable)に設定し直して、もう一度
flash.bat の実行からやり直してください。

5.PAL 対応
  DTL-H2500 は PAL/NTSC どちらでも使用可能となっています。
デフォルトは NTSC となっています。Psy-Q ツールにおいてこれを PAL に切り
替えるには、以下の2つの方法があります。
 5-1) h25bios インストール時
   h25bios を実行する時に、/pオプションを付ける。
 5-2) resetpsを実行時
   resetpsを実行するときに、/pオプションを付ける。

h2500.img(SN) ver1.0
-----------------------------------------------------------------------

Copyright (C) 1996,1997 by Sony Computer Entertainment Inc.
		All Rights Reserved.
