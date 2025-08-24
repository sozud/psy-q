$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$

              メモリカードサンプル  Memory Card Manager

			 				1997.05 (C)SCEI

[概要]

	スロット１（向かって左）のメモリカードに対して、メニュー選択に
	より各種アクセスを行うサンプルプログラムです。

	1. FILE LIST
	カードに保存されているファイル一覧を表示します。

	2. INFO
	カードの有無、フォーマット情報などを表示します。

	3. FORMAT
	カードをフォーマットします。

	4. UNFORMAT (QUICK)
	フォーマット済みのカードを未フォーマット状態に戻します。
	ＦＡＴ部分のみを初期化します。

	5. UNFORMAT (QUICK)
	フォーマット済みのカードを未フォーマット状態に戻します。
	全セクタをアンフォーマットします。

	6. CREATE 15 FILES
	容量 1ブロックの空ファイルを１５個セーブします。

	7. EASY FORMAT TEST
	カードがフォーマットされいるか否かを調べます。

	8. MAKE CARD DATA
	「HAND」というファイル名で、サンプルデータをセーブします。

	9 EXIT
	プログラムを終了します。


[作成方法]

	psymake を実行

	>psymake [RETURN]

[実行方法]

	>resetps [RETURN]
	>run card [RETURN]	※patchjが必要なボードはこの間に
				  patchjを実行


