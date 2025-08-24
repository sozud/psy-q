$PSDocId: Document Release 1.0 for Run-time Library Release 4.0$
							1997.04.25
	
				AutoPaD
	
	    Copyright (C) 1997 Sony Computer Entertainment
			    All right reserved


	このディレクトリには、パッドデータの自動読み込みによる、サンプルの
	自動実行化のための関数のソースと、バッチファイルが入っています。

【自動実行の手順】

	１．サンプルの make

		> apdmake.bat all save
		> apdmake.bat all load

	２．パッドデータの作成

		それぞれのサンプルのディレクトリで、パッドデータ作成用のプ
		ログラム（XXXXｓ.cpe （XXXはディレクトリ名））を実行して、
		パッドデータを作成する。

		（例） graphics/tuto の場合

			> run tuto1s.cpe

		実行後、tuto1.pad というパッドデータファイルができる。

	ここまでの作業は、通常、配布される CD-ROM に含まれているので、
	パッドデータを変更したいとき以外は、行う必要はありません。

	３．サンプルの自動実行

		> sample.bat all

		全てのサンプルが実行される。
		sample.bat のオプションについては、usage を参照のこと。

	４．実行後、/psx/sample/autopad/logfile.txt に記述したファイルに、
	    実行結果が出力される。


【自動実行のためのプログラミング】

	パッドデータをファイルから読み込んで自動実行させるためには、ソース
	に以下の手続きが必要になります。

	１．パッドデータ作成用プログラムの場合

		(1) プログラム開始直後

		    int APDSaveInit(long *addr,char *progname)
			addr    ... パッドデータを保存しておくメモリアドレ
				    スの指定。
			            0にすると、デフォルトの 0x80500000 に
				    設定される。
			progname... プログラム名。パッドファイルは、この
				    プログラム名.pad の名前で出力される。
			             （ディレクトリは無視されて、カレントに
				    できる）
			戻り値  ...  常に０

		(2) パッドの読み込み

		    u_long APDSave(long padd)
			padd   ... PadRead() で読み込んだパッドの値
			戻り値 ... padd

		(3) プログラム終了時

		    int APDSaveStop()
			戻り値 ... 成功:0, 失敗:-1

	２．パッドデータ読み込み用プログラムの場合

		(1) プログラム起動直後

		    int APDLoadInit(long *addr,char *progname)
			addr    ... パッドデータをロードするメモリアドレス
				    の指定。
			            0にすると、デフォルトの 0x80500000 に
				    設定される。
			progname... プログラム名。パッドファイルは、この
				    プログラム名.pad を読み込む。
			             （ディレクトリは無視されて、カレントで
				    探す）
			戻り値 ... 成功:0, 失敗:-1

		(2) パッドの読み込み

		    u_long APDLoad(int id)
			id     ... コントローラ番号。現在は意味を持たない。
			戻り値 ... パッドデータ

		(3) カウントしたい値を設定する

		    long APDSetCnt(long count)
			count ... カウントしたい値
			          通常は、フレーム毎に VSync(n) (n>=0) の戻
			          り値を与えるのが良いでしょう。

		(4) プログラム終了時

		    int APDLoadStop(long frame_cnt)
			frrame_cnt ... 総フレーム数
				       VSync(-1) の戻り値を与えるのが簡単
				       ですが、実際のフレーム数とは異なる
				       場合もあります。
			戻り値 ... 成功:0, 失敗:-1

		プログラム実行後に出力される結果ファイルには、
		(3)の count の、累計、最小値、最大値、(4)の frame_cnt の値、
		countの累計/frame_cnt の値、(1)の progname が記述されます。

