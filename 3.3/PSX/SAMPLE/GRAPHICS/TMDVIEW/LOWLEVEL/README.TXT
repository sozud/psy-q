$PSDocId: Document Version 1.0 for Runtime Library Version 3.3$

tuto0.c

このプログラムは GsTMDfast...()を用いた オブジェクト表示プログラムです。
GsSortObject ...()よりも 低いレベルのインタフェースにでプログラムされています。

そのため TMDデータの解析する部分も SortTMDFlat()のなかにインプリされています。
解析の部分は フラットポリゴンのみインプリされているため このプログラムのまま
では cube3.tmdなど フラット３角形のみ表示可能です。

