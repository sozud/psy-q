$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

97/4/25


		Psy-Q ツール 97/4/25版


以下のツールが含まれています。
各ツールに関する解説は、ドキュメント(\PSX\DOC)の方を参照して下さい。

ASMPSX.EXE	R3000 アセンブラ（Psy-Q シンタックス）（32bit版）
ASPSX.EXE	R3000 アセンブラ（MIPS 互換アセンブラ）（32bit版）
BV.EXE		VGAユーテリィティ
CCPSX.EXE	Ｃコンパイラ（32bit版）
DBUGPSX.EXE	R3000 デバッガ
DEBUGDOC.TXT	デバッガマニュアル（英語）
DELAY.COM	ディレイユーテリィティ
DEXBIOS.COM	H2000 用 Psy-Q TSR BIOS
DUMPOBJ.EXE	OBJファイルダンプツール
DUMPSYM.EXE	SYMファイルダンプツール
H25BIOS.COM	H2500 用 Psy-Q TSR BIOS
KANJI.COM	漢字コード変換ユーティリィティ
MAIN.C		メッセージ出力サンプルプログラム（ソース）
MAIN.CPE	メッセージ出力サンプルプログラム（実行ファイル）
MESS1.COM	メッセージハンドラ（H2000用）
NEWDEX.CPE	カーネルパッチモジュール
PQALOAD.COM	COFF形式の実行ファイルローダー
PQBLOAD.COM	バイナリ形式ファイルローダー
PQGOPSX.COM	実行ユーテリィティ（指定番地から実行）
PREFSECT.EXE	グループ情報書き換えユーティリティ（PREFSMPL\README.TXT参照）
PSYLIB.EXE	Psy-Q ライブラリアン（32bit版）
PSYLINK.EXE	Psy-Q リンカ（32bit版）
PSYMAKE.COM	Psy-Q Make ユーティリティ
PSYQ.CB		Brief マクロ（ソース）
PSYQ.CM		Brief マクロ（コンパイル済）
PSYQ.INI	定義ファイルのサンプル
RESETPS.EXE	ターゲットボードブートプログラム
RUN.EXE		実行ファイルローダー
SNPATCHJ.CPE	H2000用 パッチモジュール（SNSystems版）
SNPATCHW.CPE	H2000用 パッチモジュール（SNSystems版）
SNPATCH.DOC	H2000用 パッチモジュール ドキュメント（英語）
TESTMESS.COM	メッセージ出力をPCのstdoutからにするユーティリティ

FLASH25\	H2500用ドライバキット（詳しくは FLASH25\README.TXT 参照）
PREFSMPL\	PREFSECT用サンプル（詳しくは PREFSMPL\README.TXT 参照）
DOS\		コンパイラ、アセンブラ、リンカ の16bit版



＜前回リリース(96/12/25版)からの変更点＞

今回より、以下のツールが32bit版に変更されました。
Windows95のDOS窓上で、今までと同様に使用できます。

ASMPSX.EXE
ASPSX.EXE
CCPSX.EXE
PSYLIB.EXE
PSYLINK.EXE

また、16bit版は以下のように名前を変更して \PSYQ\DOS に移動しました。
DOS環境で開発されている方はこちらを御使用下さい。

ASMPSXD.EXE
ASPSXD.EXE
CCPSXD.EXE
PSYLIBD.EXE
PSYLINKD.EXE


今回変更されたファイル：
README_J.TXT
ASMPSX.EXE
ASPSX.EXE
CCPSX.EXE
PSYLIB.EXE
PSYLINK.EXE
PSYMAKE.EXE
DBUGPSX.EXE
DBG_READ.TXT
PSYQ.INI

今回新たに追加されたファイル：
DOS\ASMPSXD.EXE
DOS\ASPSXD.EXE
DOS\CCPSXD.EXE
DOS\PSYLIBD.EXE
DOS\PSYLINKD.EXE


以上
