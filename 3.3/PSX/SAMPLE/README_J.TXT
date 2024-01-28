$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

PS-X ライブラリサンプル構成                     95. 10. 16

psx\kanji\
├─fontdata
├─kanjidiv
└─kanjifnt

psx\sample\
├─cd
│  ├─earth
│  ├─movie
│  ├─str3d
│  └─tuto
├─cmplr
│  └─scratch
├─etc
│  ├─card
│  │  ├─doc
│  │  ├─lib
│  │  ├─makecard
│  │  └─max
│  ├─cman
│  ├─comb
│  └─mouse
├─graphics
│  ├─2d
│  ├─arot
│  ├─balls
│  ├─bg
│  │  ├─bgsample
│  │  └─fix32
│  ├─clutfog
│  ├─data
│  ├─diffuse
│  ├─divide
│  │  ├─active
│  │  └─clip
│  ├─faster
│  │  ├─matsu
│  │  ├─take
│  │  └─ume
│  ├─gsgpu
│  ├─mat
│  ├─mesh
│  │  ├─qmesh
│  │  ├─rmesh
│  │  └─smesh
│  ├─oden
│  ├─phong
│  ├─pmd
│  ├─ppm
│  ├─rgb24
│  ├─shadow
│  ├─tmdview
│  │  ├─data
│  │  ├─lowlevel
│  │  ├─mime
│  │  ├─rcube
│  │  ├─shuttle
│  │  ├─tmdview3
│  │  ├─tmdview4
│  │  └─tmdview5
│  ├─tod
│  │  ├─janken
│  │  │  ├─tmd
│  │  │  └─tod
│  │  └─todview
│  ├─trr
│  ├─tuto
│  ├─zimen
│  └─walk
├─math
│  └─tree
├─module
│  ├─execmenu
│  │  ├─anim
│  │  ├─balls
│  │  └─rcube
│  └─overmenu
├─press
│  └─tuto
│      └─msiro
└─sound
    ├─balls
    ├─basic
    ├─cdvol
    ├─mutual
    ├─simple
    ├─stream
    ├─tuto
    └─xse

psx\utility\
├─cdexec
├─menu
└─screen

-------------------------------------------------------------------------------

PS-X ライブラリサンプル一覧                     95. 10. 16
（ライブラリ順）

[kanji] フォントデータ関係

.\fontdata: フォントデータ
        サイズと種類毎のフォントデータ。
                11/13/15ドット
                非漢字/第１水準/第２水準/縦書き/半角
        コード変換テーブル。

.\kanjidiv: 文字データ切り出しコマンド
        文字単位でフォントデータからイメージデータを切り出す
        コマンドと、切り出したイメージデータのビューア。

.\kanjifnt: フォントデータ一括利用
        フォントデータをサイズ単位でまとめて利用するサンプル。


[sample] サンプルプログラム

.\cmplr\scratch: スクラッチパッドエリア使用サンプル
        スクラッチパッドエリアにデータを置き、３種類のアクセス方法で
        処理速度の違いを見る。

.\math\tree: 数学ライブラリ使用サンプル
        三角関数を用いて樹木曲線を描く。

.\module\execmenu: EXECサンプル
        メニューからBALLS,RCUBE,ANIMを起動する。
          LoadExec() により起動
          メモリの空き領域に読み込んだ後 Exec() で起動
        の２種類の例を挙げている。
.\module\overmenu: オーバレイサンプル
        メニューからBALLS,RCUBE,ANIMを起動する。
        音楽を鳴らしたまま、３つのBINファイルを読み代えて実行する。

.\etc\card: メモリーカード駆動サンプル
        makecard: メモリーカード用アイコン生成プログラム
                メモリーカードにじゃんけんアイコン付きファイルを生成する。
        max: メモリーカード操作画面
                ＯＳＤの操作画面のソース公開
.\etc\cman: メモリーカードアクセスサンプル
        メモリーカードのファイルの状態監視、フォーマット、ファイルの生成
        などのユーティリティ
.\etc\comb: 対戦ケーブルサンプル
        対戦ケーブルで接続された２台のマシンでそれぞれのボールを
        操作するサンプル
.\etc\mouse: マウスコントロールサンプル
        マウスによるカーソル移動とボタンを押した場合の処理のサンプル

.\graphics\arot: 回転角内挿プログラム <libgte>
        回転角(0,0,0)の状態と任意の回転角(a,b,c)の状態を一本の主軸のまわりの
        回転で内挿するプログラム。これを変形すると任意の回転角どうしの内挿も
        主軸のまわりにできます。
.\graphics\diffuse: ディフュージョン <libgte> => 見直し 
        16x16 のボールか、４角形ポリゴンをワールド座標で原点から
        拡散させる。GTE の性能デモ

.\graphics\divide: ポリゴン自動分割 <libgte>
        clip: テクスチャの歪みを防ぐための Divide 関数の使用例。
              readme.txt には RCpoly 関数の使用例もあります。
        active: DM(Direct Mapping)を使った active sub - divideのサンプル
            すき間問題・ソーティングZを 重心でなく 最大値でする
            というトピックも盛り込まれています。

.\graphics\mesh: メッシュ <libgte>
        smesh: strip mesh
        rmesh: round mesh
        qmesh: ２次元 mesh

.\graphics\phong: phong シェーディング <libgte>

.\graphics\pmd: ぴぃえむでぃ <libgte>
        pmdview (tmdview の PMD 版）
        tuto0   PMD ファイルを解析して表示 
        tuto1   TMD ファイルを解析して表示 
        tuto2   PMD を階層座標系で表示
        tuto3   PMD で光源計算を行なうもの
.\graphics\ppm: ゆがまないマッピング (perfect perspective mapping) <libgte>
.\graphics\shadow: シャドウ <libgte>
        三角形から三角形へのドロップシャドーを計算するプログラムです。
        ちゃんとクリップしているので階段へも影が落とせます。

.\graphics\trr: TransRot系関数のサンプル <libgte>
        スキマを解消するための関数 TransRot... のサンプル。
.\graphics\walk: ポリゴンに沿って歩くプログラムのサンプル <libgte>
        ポリゴン上に物体を拘束するプログラムのサンプルです。
        ポリゴンでできた物体１の上を他の物体２が歩き回らせることができます。
        物体１はどんな形でもかまいません。物体２は物体１の法線に合わせて
        向きを変えます。

.\graphics\2d: にでぃ <libgpu>
        曲面のテクスチャマッピング
        ３次元上の曲面に 512x256 のテクスチャパターンをマッピングする。
        select を押すと粉々になるおまけつき。   
.\graphics\balls: ボールズ <libgpu>
        16x16 のスプライトを多数表示する。
        FntPrint, KanjiFntPrint, VSyncCallback(),VSync() の使い方も示す。

.\graphics\rgb24: あーるじぃびぃ２４ <libgpu>
        24bit モードのデモ
        StoreImage(), LoadImage(), MoveImage() の例を示す。

.\graphics\tuto: チュートリアル <libgpu>
        段階的チュートリアル
        tuto0   とにかく画面上にスプライトを表示してみる。
        tuto1   ＯＴを使用した描画のテスト
        tuto2   回転する１枚の板を GTE を使って描画する
        tuto3   回転する立方体を描画する 
        tuto4   光源のある立方体の描画
        tuto5   複数の 3D オブジェクトの描画
        tuto6   1D スクロール BG の実験
        tuto7   デプスキューイング効果を用いた立方体の描画
        tuto8   セルタイプの BG の実現 
        tuto9   ３Ｄセルタイプの BG の実現
        tuto10  特殊効果の例（モザイク）

.\graphics\bg: libgsのBG描画関数のサンプル <libgs>
        スプライトエディタで作ったTIM/CEL/BGDファイルがそのまま読めます。
        bgsample: BG 面の描画関数で BG データを表示
        fix32:    BG 面の高速描画関数で BG データを表示
.\graphics\gsgpu: じいえすじいぴいゆう <libgpu,libgs>
        libgs と libgte を同時につかったサンプル
        tuto0: libgs と AddPrim() を使用
        tuto1: DrawOTag()で libgs のパケットを描画する
.\graphics\mat: まっちゃん
        立て看板スプライト＋スプライトアニメーション
        まっちゃん(Matchang) のアニメーションが 3D に埋め込まれたもの。
.\graphics\oden: オーデン <libgs>
        ３光源をインターラクティブに動かし また 光源色を変化させ
        リアルタイムでライティングの計算を行なうサンプル。

.\graphics\tmdview: TMDビュー
        TMD 及び PMDデータを用いたサンプル群

        data: サンプルで使うデータの置かれているディレクトリ
        tmdview3: GsDOBJ3を用いたPMDデータの 最もシンプルな表示プログラム
        tmdview4: GsDOBJ2を用いたTMDデータの 最もシンプルな表示プログラム
        tmdview5: GsDOBJ5を用いたTMDデータの 表示プログラム
                tuto0: GsDOBJ5を用いたTMDデータの 最もシンプルな表示プログラム
                tuto1: スプリット画面のサンプル
                tuto2: オブジェクトの使い回し および 複数OT
                tuto3: GsDOBJ5のアトリビュートによる自動分割
                tuto4: マルチスクリーン座標系
                tuto5: GsSortObject5Jを使って書き換えたサンプル
        rcube: 回るキューブ
                3D の各種エフェクトのサンプル
        shuttle: シャトル
                スペースシャトルのモデルを使った 階層座標系のサンプル。
                シャトルのハッチ および シャトル内の衛星 を シャトル本体の
                子座標にしてハッチ開閉等のアニメーションを行なう。
        mime: ぷよで(MIMe Interactive Animation)
                GsDOBJ5を用いた"ＭＩＭｅ"の Sample Program 
                L1,L2,R1,R2ボタンで４つの MIMe パラメータを制御。
                データは十字形の"ぷよで"、法線 MIMe も行なっている。
        lowlevel: 下位関数を使用
            GsTMDfast...()を使った 低い切口のサンプル。

.\graphics\tod: tod によるアニメーション
        janken: 複数の tod のインタラクティブなアニメーション
        todview: シンプルなアニメーション

.\graphics\clutfog: clut を使った fog のサンプル
        tuto0: フレームごとに clut を生成して vram に転送する方式です
        tuto1: 数個の clut を vram に置いておき、fog の具合によって
               適宜切替えて使う方式です。
        tuto2: DR_MOVE を使って CLUT を変更する方式。
               libgs による描画にもそのまま適用できます。

.\graphics\zimen: 地面
        無限平面を表示するプログラム群
        tuto0: 動的プリミティブ分割(dmpsx使用)
        tuto1: 基本的な視野クリップ
        tuto2: 高さなしメッシュ型地面
        tuto3: 高さなしラップラウンド無視メッシュ型地面
        tuto4: 動的分割を使用したメッシュ地面
        tuto5: CLUT FOGを使った地面のサンプル（Gs版）

.\cd\movie: ムービー <libcd>
        ストリーミングを使った動画のサンプル群
        tuto0: 最もシンプルなストリーミングを使った動画のサンプル
        tuto1: １６の倍数でない大きさの動画を再生するサンプル
        tuto2: オンメモリストリーミングサンプル
.\cd\str3d: コンビ <libcd,libstr>
        ストリーミング動画と３Dモデルの表示とを組み合わせたサンプル
.\cd\earth: アース
        球面に動画を張り付ける例
.\cd\tuto: ＣＤチュートリアル <libcd>
        CD-ROM 段階的チュートリアル
        tuto0   最も単純なCD-Player
        tuto1   最も単純なCD-Player （コールバック使用）        
        tuto2   最も簡単な CdRead のテスト（バックグラウンド）
        tuto3   分割リード（バックグラウンド）
        tuto4   CD-DA トラックの任意の２点間をリピートする。
        tuto5   CD-XA トラックの任意の２点間をリピートする。
        tuto6   マルチチャンネルADPCM テストプログラム
        tuto7   プログラムのロードと実行

.\press\tuto: チュートリアル <libpress>
        tuto1 - tuto6: MDEC 段階的チュートリアル

.\sound\balls: サウンドとグラフィックの組み合わせその１
        サウンドとグラフィックの組み合わせ例です。

        ボタンを押している間、ボールが上下に動きます。
        そのボールが壁にぶつかったときに、それぞれ効果音が鳴ります。
        バックグラウンドでは SEQ が流れています。
.\sound\basic: 基本演奏その１
        SEQ/SEP データアクセス関数の使用例です。

        ここでは、SEQ と SEP を同時に鳴らすことができます。
        SEP データは、SEQ データ 3 曲を連結したものです。
.\sound\cdvol: SPU デコードデータの読み込みのサンプル <libspu>
        CD で演奏されている曲を SPU から「SPU デコードデータ」として
        バックグラウンドで読み込み、そのボリューム状況をグラフ表示
        （ピークレベル表示付）させる。
.\sound\tuto: 基本サウンドライブラリの基本的な利用法のサンプル <libspu>
        tuto1: ピッチ指定/キーオン/キーオフ
                コントロールパッドの操作によってさまざまな音程で発音する
        tuto2: ミュート
                発音、ミュート ON、ミュート OFF を行う
        tuto3: SPU 割り込み
                ピアノ音のデータの中間に割り込みを設定し、ピアノ音を
                発音して割り込みが起きた時にサイン波を発音する
        tuto4: ノイズ音源
                サイン波とノイズをピッチを変化させて発音する
        tuto5: 波形データの分割転送
                分割転送を行い、転送後発音する、というのを 2 つの
                ボイスで交互に行う
        tuto6: リバーブ
                ピアノ音を発音させ、それに 9 種類のリバーブを指定する

.\sound\stream: SPU ストリーミング用サンプルプログラム <libspu>
        tuto1: バックグラウンドが balls です
        tuto2: バックグラウンドが movie です

        基本サウンドライブラリに含まれる SPU ストリーミングライブラリを使用し
        たサンプルです。
        7 チャンネル (ボイス数 14) の SPU ストリームをコントロールパッドの操作
        によって再生/停止を行います。SPU ストリーミングの状態も画面に表示しま
        す。

.\sound\mutual: 波形データの分割転送のサンプル <libsnd>
        任意のタイミングで CD から波形データを分割してメインメモリに
        読み込み、さらにそのデータをサウンドバッファに順次分割転送し、
        そのデータを使用して 2 つの曲を演奏する。

.\sound\simple: 基本演奏その２
        SEQ データアクセス関数の使用例です。SEQ を無限回演奏します。

        画面には、現在のテンポやボリュームやステータス（play 中，
        pause 中など）が表示されます。

        波形データはヘッダ部とボディ部に分割し、分割転送を行なっています。

.\sound\xse: オートエフェクト
        サウンドユーティリティ関数の使用例です。

        Key On された効果音に対して、ピッチベンドやオートパンニングや
        オートボリュームなどのエフェクトをかけます。
        
        タイルを移動させることにより、画面をみながらオートパンニングや
        オートボリュームやピッチベンドをかけることができます。

[utility]

.\cdexec: CD-ROM/Emulatorからの起動ユーティリティ
        H2000において、patchxした状態でCD-ROM/Emulatorから起動させる場合に
        使う。「resetps 0」の代替モジュール。

.\menu: 実行ファイルを読み込む、サンプルプログラムビューア。
        サンプルの実行ファイルをメニュー画面から起動する。
        このビューアで実行できるプログラムは none2.obj をリンクした、EXE 
        ファイルで、menu.lst にあらかじめ登録しておく必要がある。

.\screen: スクリーン
        フレームバッファビューア。
        ディスプレイモード、表示環境パラメータの説明用デモ

-------------------------------------------------------------------------------
