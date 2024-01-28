$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$
							95.10. 5
漢字フォントデータの配布


【概要】

　プレイステーションのアプリケーション開発ツールとして、以下の漢字フォント
データを配布します。

	書体　：　ゴシック体
	サイズ：　11、13、15 ドット
	種類　：　非漢字	got1[135]j0b
		　第一水準	got1[135]j1b
		　第二水準	got1[135]j2b
		　縦書き	got1[135]tat
		　半角		got1[135]hlf
		　外字		got15gai

  以上のデータファイルは、fontdata\*.h に置いてあります。

 【使用方法】

０．漢字フォントデータのフォーマットについて

  漢字フォントデータの構造およびそのファイル構造に関しましては、
  fontdata\format.txt に記述してありますので、参照してください。

  また、半角データの文字配列は、fontdata\hankaku.txt に記述してあります。

１．プレイステーション本体の内臓の漢字フォンとデータを使用する場合

　既にプレイステーション本体には、15 ドットの非漢字と第一水準と外字の
  フォントデータが内蔵されています。内蔵されているデータは、別途ご案内
  しましたアクセス関数 (Krom2RawAdd()) でご利用下さい。(\psx\doc\jp\api
  を参照してください。)
  
  また、内蔵フォントの漢字コードを一覧するためのツールを用意しましたので、
  ご利用ください。（ sjiscode\codeview.cpe ）

  ＊注意＊
  このツールでは、海外版 PlayStation 用に追加された外字をアクセスするため
  の関数 Krom2RawAdd2() を使用しています。Krom2RawAdd2() 関数の仕様は、
  Krom2RawAdd() と同じですが、非漢字/漢字第一水準/外字の全てのコードに
  対応しています。


２．必要な漢字のフォントデータだけを切り出して使用する場合

　今回配布しますフォントデータにつきましては、全フォントデータから必要な
  文字のデータだけを切り出し、切り出したデータを各アプリケーションで利用
  される形態を想定しています。
  
  従いまして、kanjidiv に必要なフォントデータを切り出すツールと、切り出した
  データのビューアを添付しました。ただし、縦書きと半角のデータはサンプルの
  対象となっていません。

３．フォントサイズ毎に一括して漢字データファイルを使用する場合

  さらに参考のため、kanjifnt に漢字データファイルを、サイズ毎にまとめて
  扱う場合のサンプルも添付しました。


【配布物】

	readme.txt	このファイル

	fontdata\	フォントデータ

		got1[135]j0b.h	非漢字データ
		got1[135]j1b.h	第一水準データ
		got1[135]j2b.h	第二水準データ
		got1[135]tat.h	縦書きデータ
		got1[135]hlf.h	半角データ
		got15gai.h	外字データ
		format.txt	フォントデータファイルおよび
						漢字データの構造の説明
		hankaku.txt	半角データの文字配列
		offset.h	コード変換用テーブル

	sjiscode\	内蔵フォント漢字コード一覧
		codeview.c	Shift-JIS Code の一覧ツール
		codeview.txt	codeview の説明
		makefile.mak	ツールの makefile (psymake 用)
		makefile	ツールの makefile (NEWS 用)

	kanjidiv\	文字毎のデータ切り出しサンプル

		kanjidiv.txt	切り出しコマンドの説明
		kanjidiv.exe	文字毎のデータ切り出しコマンド
		test.knj	入力データサンプル     (-> kanji.out)
		kanji.out	切り出しデータサンプル (<- test.knj)

		viewer.txt	ビューアの説明
		viewer.c	ビューアのソースファイル
		makefile.mak	ビューアの makefile (psymake 用)
		makefile	ビューアの makefile (NEWS 用)


	kanjifnt\	サイズ毎に一括利用するサンプル

		kanjifnt.txt	サンプルプログラムの説明
		kanji.h		フォントのサイズを指定
		main.c		メインルーチン
		trans.c		漢字コード変換およびアドレス変換
		makefile.mak	サンプルの makefile (psymake 用)
		makefile	サンプルの makefile (NEWS 用)

以上
