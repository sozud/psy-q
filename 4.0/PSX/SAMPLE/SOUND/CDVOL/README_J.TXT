$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

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
