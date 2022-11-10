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

## 疑似命令(未実装)

- mov x1 x2 = store x1 zero x2
- push x = store x zero, addi sp 1
- pop x = subi sp 1, load x zero
