# rkasm.rs

「アセンブリの１行と１機械語命令が対応する」という原則を守りながら、わかりやすい書き方ができるようにします。

## レジスタ演算

```
rd = rs1 + rs2 ; add
rd = rs1 - rs2 ; sub
rd = rs1 & rs2 ; and
rd = rs1 | rs2 ; or
rd = rs1 ^ rs2 ; xor
rd =     ~ rs2 ; not

rd = < rs1    ; sl  - shift left
rd = < rs1 <  ; rl  - rotate left
rd = : rs1 >  ; srs - shift right (signed)
rd =   rs1 >  ; sru - shift right (unsigned)
rd = > rs1 >  ; rr  - rotate right

rd = rs1 == rs2 ; eq
rd = rs1 != rs2 ; neq
rd = rs1 <: rs2 ; lts - less than (signed)
rd = rs1 <  rs2 ; ltu - less than (unsigned)

nop            ; * nop
rd =       rs2 ; * mov
rd =     - rs2 ; * mns
```

## 即値演算

```
rd = rs1 + 0x1234 ; addi
rd = rs1 - 0x1234 ; subi
rd = rs1 & 0x1234 ; andi
rd = rs1 | 0x1234 ; ori
rd = rs1 ^ 0x1234 ; xori

rd = rs1 == 0x1234 ; eqi
rd = rs1 != 0x1234 ; neqi
rd = rs1 <: 0x1234 ; ltsi - less than (signed)
rd = rs1 <  0x1234 ; ltui - less than (unsigned)

rd =       0x1234 ; * loadi
```

## ロード

```
rd = [rs1.0x1234] ; load
rd = [rs1       ] ; *
rd = [    0x1234] ; *
rd = [     label] ; *
```

`.` は相対アドレスを意味します。
ラベルを用いることで、次のような構造体的なアクセスができます。

```
@0x1234 list_head

; struct LinkList
.0 next
.2 member

t0 = list_head
rd = [t0.member]
```

これを、次のような１行で書き展開することは考えましたが、やめました。

```
rd = [list_head.member]
```

１行１命令の対応が取れなくなり、バイナリとアセンブラを比べながら読むのが面倒になるからです。

## ストア

```
[rs1.0x1234] = rs2 ; store
```

## 制御命令

```
if rs2 goto rs1 + 0x1234
if rs2 goto rom_lab
goto rom_lab
call func
ret
iret
```

## ラベル

即値フィールドには、数値リテラルかラベルを置くことができます。

### 自動ラベル

```
rom_label:
```

次の命令のアドレスを指します。

### 固定ラベル

```
#0x1234 def_label
@0x1234 ram_label
.0x4    rel_label
```

固定ラベルは固定値を指します。
ラベルの種類はアセンブラにラベルの意味をサジェストするために使われ、命令の引数に適合しないものが入ると warn を出しますが error になることはありません。

## 逆アセンブリ

アセンブラが文字列とパターンマッチしてバイナリを出力するものだとすれば、逆アセンブリはその逆、バイナリにパターンマッチして文字列を出力するものです。
ただし、ラベルの情報はバイナリ上では捨てられているので復元できません。

## 命令記述 DSL


