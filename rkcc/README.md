# RKCC <!-- omit in toc -->

自作マイコン用の自作言語、meccyha-sugoi-gengo（仮）のコンパイラです。

> 組み込みシステムを作るために実用上十分な機能を持つこと

> コンパイラの実装が簡単になること

を目指して開発しました。

# 目次 <!-- omit in toc -->

- [1. 文法](#1-文法)
  - [1.1. 型](#11-型)
    - [1.1.1. 型指定](#111-型指定)
    - [1.1.2. 参照演算子・アドレス演算子](#112-参照演算子アドレス演算子)
    - [1.1.3. キャスト演算子](#113-キャスト演算子)
    - [1.1.4. 基本型](#114-基本型)
    - [1.1.5. ポインタ型](#115-ポインタ型)
    - [1.1.6. 配列型](#116-配列型)
    - [1.1.7. 構造体型](#117-構造体型)
    - [1.1.8. 関数型](#118-関数型)
  - [1.2. プログラム](#12-プログラム)
  - [1.3. 文](#13-文)
    - [1.3.1. 式文](#131-式文)
    - [1.3.2. 代入文](#132-代入文)
    - [1.3.3. goto label](#133-goto-label)
  - [1.4. 式](#14-式)
    - [1.4.1. 演算](#141-演算)
    - [1.4.2. 値](#142-値)
- [2. コンパイラの動作](#2-コンパイラの動作)
  - [2.1. トークン化](#21-トークン化)
  - [2.2. 抽象構文木の構築](#22-抽象構文木の構築)
  - [2.3. ASTの再帰的評価](#23-astの再帰的評価)
    - [2.3.1. 型のサイズの評価](#231-型のサイズの評価)
    - [2.3.2. 変数として評価](#232-変数として評価)
    - [2.3.3. アドレスとして評価](#233-アドレスとして評価)
    - [2.3.4. 定数値として評価](#234-定数値として評価)
  - [2.4. シンボルテーブル生成](#24-シンボルテーブル生成)
  - [2.5. グローバル](#25-グローバル)
  - [2.6. グローバル変数の配置](#26-グローバル変数の配置)
  - [2.7. ローカル](#27-ローカル)
  - [2.8. 関数のコード生成](#28-関数のコード生成)
  - [2.9. 実行環境メモ](#29-実行環境メモ)
- [3. 作業日誌](#3-作業日誌)
  - [3.1. 11/10](#31-1110)
  - [3.2. 11/13](#32-1113)
  - [3.3. 11/15](#33-1115)
  - [3.4. 11/16](#34-1116)
  - [3.5. 2/8](#35-28)
  - [3.6. 2/9](#36-29)
  - [3.7. 2/10](#37-210)
  - [3.8. 2/12](#38-212)
  - [3.9. 2/14](#39-214)

# 1. 文法

## 1.1. 型

|            | 記法                          |
| ---------- | ----------------------------- |
| 基本型     | `int`                         |
| ポインタ型 | `*int`                        |
| 配列型     | `[N]int`                      |
| 構造体型   | `{m0 : int, m1 : int}`        |
| 関数型     | `(a0 : int, a1 : int) => int` |

```
type =
 | type_prim   = ident | "(" type ")"
 | type_ptr    = "*" type
 | type_arr    = "[" expr "]" type
 | type_struct = "{" (ident ":" type) % "," "}"
 | type_func   = "(" (ident ":" type) % "," ")" "=>" type
```

ポインタ `*`、配列 `[]` をベース型の前に置きます。

### 1.1.1. 型指定

`var hoge : int`

シンボルの後に型を書きます。

### 1.1.2. 参照演算子・アドレス演算子

`a*` で `a` をポインタとみなして、`a` の指す値を得ます。

`a : *int` → `a* : int`

`a&` で `a` のアドレスを得ます。

`a : int` → `a& : *int`

### 1.1.3. キャスト演算子

`a : T`

変数に型を書いて型のキャストができます。
メモリ上でサイズが同じならキャストできます。

### 1.1.4. 基本型

基本型は 16bit 整数のみをサポートします。
符号付きか無しかについては未定です。
ハードウェアの都合で決めます。

### 1.1.5. ポインタ型

ポインタ型は 16bit のアドレスです。
これはアドレス空間が 16bit であることに由来します。

アクセス演算子を適用すると、ベース型になります。

`hoge : *int` → `hoge* : int`

アドレス演算子を適用すると、ポインタ型になります。

`hoge : int` → `hoge& : *int`

### 1.1.6. 配列型

配列はコンパイル時にベース型の N 個分のメモリを確保します。

添字演算子を適用すると、ベースの型になります。

`hoge : [N]int` → `hoge[0] : int`

多次元配列は、このように表されます。

`hoge : [N][M]int`

C言語と異なり、配列とポインタの暗黙のキャストは行いません。

配列のアドレスが欲しい場合は、アドレス演算子を使います。

`hoge : [N]int` → `hoge& : *[N]int`

配列の先頭の要素のアドレスは、このように取得します。

`hoge : [N]int` → `hoge[0]& : *int`

これらのポインタの値は一致しますが、型は異なります。

### 1.1.7. 構造体型

構造体のサイズはメンバの合計です。

メンバ演算子を適用すると、メンバの型になります。

`hoge : {a : int}` → `hoge.a : int`

### 1.1.8. 関数型

関数型は関数の持つ型です。

関数呼び出し演算子を適用すると、返り値の型になります。

`hoge : (arg : Arg) => Ret` → `hoge(arg) : Ret`

関数型の変数は定義できません。かわりに関数ポインタ型を使います。

`hoge_p : *(arg : Arg) => Ret = hoge&;` → `hoge_p*(arg) : Ret`

関数ポインタには関数のアドレスが入ってます。

## 1.2. プログラム

|          | 記法                                                    |
| -------- | ------------------------------------------------------- |
| 変数定義 | `var  hoge : int;`                                      |
| 関数定義 | `func hoge : (a : int, b : int) => int { return a+b; }` |
| 型定義   | `type hoge : {x : int, y : int};`                       |

プログラムのトップレベルには、グローバル変数定義、関数定義、型定義が並びます。

```
program  =
 | gvar_def = "var"  ident ":" type ";"
 | func_def = "func" ident ":" type compound
 | type_def = "type" ident ":" type ";"
```

## 1.3. 文

関数定義には複文 (compound satements) が続き、
その中には文 (statement) が並びます。

```
compound = "{" stmt* "}"

stmt =
空文
 | void_stmt = ";"
複文
 | compound  = "{" stmt* "}"
式文
 | expr_stmt = expr ";"
ローカル変数定義
 | lvar_def  = "var" ident ":" type ";"
代入文
 | assign    = expr ("="|"+="|"-="|"*="|"/=") expr ";"
 | uassign   = ("++"|"--") expr ";"
制御文
 | if        = "if" "(" expr ")" stmt
 | if_else   = "if" "(" expr ")" stmt "else" stmt
 | goto      = "goto" ident ";"
 | label     = ident ":"
 | return    = "return" expr ";"
繰り返し文
 | while     = "while" "(" expr ")" stmt
 | continue  = "continue" ";"
 | break     = "break" ";"
```

### 1.3.1. 式文

式を評価します。評価値は破棄されるため、実用上は副作用を実行するための文です。

### 1.3.2. 代入文

代入文が変数の値を書き換える唯一の方法です。

左辺はアドレス、右辺は値として評価できる必要があります。

`a : int = b : int`

という代入文は、実際には、

`a& : *int <= b : int`

このような動作をしています。

### 1.3.3. goto label

関数呼び出しのABIを守るため、gotoは同一の関数内である必要がある。

ラベルの前に関数名を付記することで `<func-name>_<label-name>` 、
関数外へのgotoはアセンブラがエラーを出す。

アドホックですが、gotoはそんなに使わないのでこの程度のエラー処理でいいでしょう。

```
func main : ()=>int {
  goto hoge; //     jump zero zero main_hoge
  hoge:      // main_hoge:
}
```

## 1.4. 式

### 1.4.1. 演算

```
expr = cond = logical_or ("?" expr ":" cond)?
logical_or  = logical_and ("||" logical_and)*
logical_and = bit_or  ("&&" bit_or)*
bit_or  = bit_xor ("|" bit_xor)*
bit_xor = bit_and ("^" bit_and)*
bit_and = equal ("&" equal)*
equal = relat ("==" relat | "!=" relat)*
relat = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
shift = (shift "<<" | shift ">>")? add
add   = mul ("+" mul | "-" mul)*
mul   = prim ("*" prim | "/" prim | "%" prim)*
```

### 1.4.2. 値

```
prim =
 | num
 | cast      = ("(" expr ")" | ident) (":" type)*
 | access    = ("(" expr ")" | ident) ("*")*
 | addr      = ("(" expr ")" | ident) ("&")*
 | array     = ("(" expr ")" | ident) ("[" expr "]")*
 | member    = ("(" expr ")" | ident) ("."  ident)*
 | func_call = ("(" expr ")" | ident) ("(" expr % "," ")")*
```

# 2. コンパイラの動作

## 2.1. トークン化

## 2.2. 抽象構文木の構築

抽象構文木 (Abstract Syntax Tree : AST) は構造化されたソースコードです。

|                | `Node::Type` | `childs`  |           |            |         |
| -------------- | ------------ | --------- | --------- | ---------- | ------- |
| **Type**       | TypeFunc     | arg[]     | ret       |            |         |
|                | TypeStruct   | member[]  |           |            |         |
|                | TypeArray    | base      | size      |            |         |
|                | TypePointer  | base      |           |            |         |
|                | TypePrim     | ident     |           |            |         |
| **Program**    | Program      | defs      |           |            |         |
|                | FuncDef      | name      | type      | compound   |         |
|                | GVarDef      | name      | type      |            |         |
|                | TypeDef      | name      | type      |            |         |
| **Stmt**       | Compound     | stmt[]    |           |            |         |
|                | VoidStmt     |           |           |            |         |
|                | ExprStmt     | expr      |           |            |         |
|                | LVarDef      | name      | type      |            |         |
| **AssignStmt** | Assign       | expr      | expr      |            |         |
|                | UAssign      | expr      |           |            |         |
| **CtrlStmt**   | If           | cond      | true_node |            |         |
|                | IfElse       | cond      | true_node | false_node |         |
|                | Goto         | name      |           |            |         |
|                | Label        | name      |           |            |         |
|                | Return       | expr      |           |            |         |
| **LoopStmt**   | While        | cond      | body      |            |         |
|                | DoWhile      | cond      | body      |            |         |
|                | For          | cond      | body      | init       | iterate |
|                | Continue     |           |           |            |         |
|                | Break        |           |           |            |         |
|                | Return       | expr      |           |            |         |
| **Expr**       |              |           |           |            |         |
| **TriOpr**     | Cond         | cond      | true_node | false_node |         |
| **BinOpr**     | LogicalOr    | lhs       | rhs       |            |         |
|                | LogicalAnd   |           |           |            |         |
|                | BitOr        |           |           |            |         |
|                | BitXor       |           |           |            |         |
|                | BitAnd       |           |           |            |         |
|                | EQ           |           |           |            |         |
|                | NEQ          |           |           |            |         |
|                | LT           |           |           |            |         |
|                | LEQ          |           |           |            |         |
|                | GT           |           |           |            |         |
|                | GEQ          |           |           |            |         |
|                | RShift       |           |           |            |         |
|                | LShift       |           |           |            |         |
|                | Add          |           |           |            |         |
|                | Sub          |           |           |            |         |
|                | Mul          |           |           |            |         |
|                | Div          |           |           |            |         |
|                | Mod          |           |           |            |         |
| **UnaryOpr**   | Num          | ***int*** |           |            |         |
|                | Ident        | ***std*** |           |            |         |
|                | Cast         | ident     | type      |            |         |
|                | Ref          | ident     |           |            |         |
|                | Addr         | ident     |           |            |         |
|                | Array        | ident     | expr      |            |         |
|                | Member       | ident     | ident     |            |         |
|                | FuncCall     | ident     |           |            |         |


## 2.3. ASTの再帰的評価

ASTの部分木を再帰的評価して、コード生成に必要なさまざまな情報を得ることができます。

### 2.3.1. 型のサイズの評価

変数に必要なメモリを知るために型のサイズを計算します。

- 整数型 : 1 (16bit)
- ポインタ型 : 1 (16bit)
- 配列型 : ベース型の N 倍
- 構造体型 : メンバのサイズの合計
- 関数型 : 直接評価不可

### 2.3.2. 変数として評価

### 2.3.3. アドレスとして評価

代入文の左辺を評価します。

### 2.3.4. 定数値として評価

コンパイル時に値が決まっているべき部分を評価します。

## 2.4. シンボルテーブル生成


## 2.5. グローバル

トップレベルの `func` `var` `type` で定義されたシンボルをまとめた表（map）を作ります。

| name : str | kind : Symbol::Kind | type : Node\* | label |
| ---------- | ------------------- | ------------- | ----- |
| hoge       | Func                |               |       |
| fuga       | VarGlobal           |               |       |
| piyo       | Type                |               |       |

テキスト領域に関数を、データ領域にグローバル変数を置きます。

## 2.6. グローバル変数の配置

データ領域の先頭から、型のサイズだけグローバル変数に割り当てていきます。

```
int global_top = 0x1000;
for(auto var : global_table){
  if(var.kind=VarGlobal){
    file << "@" << global_top << ":" << var.name << endl;
    global_top += type_size(var.type);
  }
}
```

## 2.7. ローカル

関数内の `var` を探索し、ローカル変数のテーブルを作ります。

| name : str | type : Node\* | create | delete |
| ---------- | ------------- | ------ | ------ |
| arg0       |               | -1     |        |
| arg1       |               | -1     |        |
| var0       |               | 1      |        |
| var1       |               | 2      |        |

一覧表ができたら、依存関係と生存期間からアドレスを決定します。

スタックのアドレスはフレームポインタからの相対アドレスです。

## 2.8. 関数のコード生成

## 2.9. 実行環境メモ

現在の位置を保持しながらコード生成をする。

| 関数 | ループ       |
| ---- | ------------ |
| main | null / for_1 |

return, break, continue に必要な情報。

# 3. 作業日誌

## 3.1. 11/10

- 四則演算の実装
  - 構文木構築
  - 再帰的評価

```
expr =  mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary | "%" primary)*
primary = num | "(" expr ")"
```

## 3.2. 11/13

- 演算子の拡張
  - ? :, ||, &&, |, ^, &, ==, !=, <, <=, >, >=, <<, >>, \*, /, %, ++, --

## 3.3. 11/15

- 変数の実装

```
prim = num | ident | "(" expr ")"
```

## 3.4. 11/16

- 文の実装
- 代入の実装

```
program = stmt*
stmt = ";"
     | "{" stmt* "}"
     | "if" "(" expr ")" stmt ("else" stmt)?
     | "while" "(" expr ")" stmt
     | "do" stmt "while" ( expr ) ";"
     | "for" "(" expr? ";" expr? ";" expr? ")" stmt
     | expr ";"
```

## 3.5. 2/8

- 関数定義

```
program = func*
func = ident "(" ident % "," ")" compound
compound = "{" stmt* "}"
```

## 3.6. 2/9

- 型

## 3.7. 2/10

- AST ノードの改良
  - child を vector にまとめメソッドでアクセスするようにした

## 3.8. 2/12

- 変数定義

## 3.9. 2/14

- 細かい文法を決めた
- ドキュメントの整備
