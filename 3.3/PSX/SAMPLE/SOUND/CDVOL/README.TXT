$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
$PSLibId: Runtime Library Version 3.3$

----------------------------------------------------------------
【 ＣＤピークレベルメータ 】

  CD で演奏されている曲を SPU から読み込み、そのボリューム状況を表示さ
  せます。

  これは SpuReadDecodeData() と SpuSetTransferCallback() のためのサン
  プルです。

・ !!!! 注意 !!!!

  本サンプルプログラムはお手持ちの CD/DA ディスク (一般に売られている
  音楽 CD) を CD ドライブ (DTL-H2010) に入れておかないと正常動作致しま
  せん。

・ コントロールパッドのアサイン

  コントロールパッドの操作はありません。

・プログラムのロード

	> psymake

	(音楽 CD を CD ドライブにセット)

	> dbugpsx main /r50 /e

	を行い、デバッガ内で実行させて下さい。

                Copyright (C) 1995 by Sony Computer Entertainment Inc.

----------------------------------------------------------------

[CD peak level meter]

  Display CD volume status by reading CD/DA data from SPU.  This is
  the sample for SpuReadDecodeData() and SpuSetTransferCallback().

  - !!!! Notice !!!! -
	This sample program works with CD/DA disc (anything you have)
	inserted in DTL-H2010.

o Control Pad Assignment:

  Not assigned.

o How to run:

  > psymake

  (Insert CD/DA disc in DTL-H2010)

  > dbugpsx main /r50 /e

                Copyright (C) 1995 by Sony Computer Entertainment Inc.

----------------------------------------------------------------
