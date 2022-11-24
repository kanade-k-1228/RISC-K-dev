# RKASM

RISC-K のアセンブラです。

## 処理の流れ

1. プリプロセッサ
   - コメントの削除
   - 空行の削除
2. アセンブラ
   - 命令
3. リンカ
   - ラベルの位置を書き換える

## 命令ラベル

アセンブラ中の、`:` で終わる行は、命令ラベルの定義。

## 変数ラベル（未実装）

アセンブラ中の、`@` で始まる行は、変数ラベルの定義。

`@0xffff hoge`

とすることで、RAM アドレスに別名をつけることができる。

`load t0 zero hoge` のように、アドレスを人間にわかりやすく扱える。

## 疑似命令（未実装）

- mov x1 x2 = store x1 zero x2
- push x = store x zero, addi sp 1
- pop x = subi sp 1, load x zero
- call
- ret
