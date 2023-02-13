# RKCC

自作マイコン用の高級言語です。

## 言語設計の方針

基本的には C 言語を踏襲しますが、以下の方針に従って改変しています。

- 組み込みシステムを作るために実用上十分な機能
- コンパイラの実装がわかりやすく、簡単になること

## 型システム

C 言語の型システムはわかりにくく、それは非本質的な難しさです。
TypeScript や Go を参考に、わかりやすい
C をベースにしつつ、型の記法は Go や TS を参考にしています。

- [RKCC](#rkcc)
  - [言語設計の方針](#言語設計の方針)
  - [型システム](#型システム)
- [文法](#文法)
  - [文法の書き方](#文法の書き方)
  - [型](#型)
  - [ポインタとアクセス](#ポインタとアクセス)
  - [定義](#定義)
  - [文](#文)
  - [式](#式)
    - [演算](#演算)
    - [値](#値)
  - [補足](#補足)
    - [++の問題](#の問題)
- [トークン化](#トークン化)
- [抽象構文木の構築](#抽象構文木の構築)
- [コード生成](#コード生成)
  - [シンボルテーブル生成](#シンボルテーブル生成)
  - [型のサイズ決定](#型のサイズ決定)
  - [グローバル変数のコード生成](#グローバル変数のコード生成)
  - [関数のシンボルテーブル生成](#関数のシンボルテーブル生成)
  - [実行環境メモ](#実行環境メモ)
  - [関数のコード生成](#関数のコード生成)
- [今後追加したい機能](#今後追加したい機能)
  - [型チェック](#型チェック)
- [作業日誌](#作業日誌)
  - [11/10](#1110)
  - [11/13](#1113)
  - [11/15](#1115)
  - [11/16](#1116)
  - [2/8](#28)
  - [2/9](#29)
  - [2/10](#210)
  - [2/12](#212)
  - [2/13](#213)
  - [配列](#配列)
- [c 言語の文法](#c-言語の文法)
  - [用語](#用語)
  - [BNF](#bnf)
  - [宣言](#宣言)
  - [関数定義](#関数定義)
  - [式](#式-1)
  - [型](#型-1)
  - [構造体・共用体](#構造体共用体)
  - [列挙型](#列挙型)
  - [宣言と初期化](#宣言と初期化)
  - [文](#文-1)

# 文法

## 文法の書き方

ebnf っぽい雰囲気で書きます。
`%` は配列を意味します。

`a % b = (a (b a)*)? `

## 型

型の書きかたは C 言語とは異なります。
TypeScript と Go を参考に作りました。

```
type =
 | type_prim   = ident | "(" type ")"
 | type_ptr    = "*" type
 | type_arr    = "[" expr "]" type
 | type_struct = "{" (ident ":" type) % "," "}"
 | type_func   = "(" (ident ":" type) % "," ")" "=>" type
```

|            | 記法                          |
| ---------- | ----------------------------- |
| 基本型     | `int`                         |
| ポインタ型 | `*int`                        |
| 配列型     | `[N]int`                      |
| 構造体型   | `{m0 : int, m1 : int}`        |
| 関数型     | `(a0 : int, a1 : int) => int` |

基本型は 16bit 整数のみをサポートします。
符号付きか無しかについては未定です。
ハードウェアの都合で決めます。

ポインタ型は 16bit のアドレスです。
これはメモリ空間が 16bit であることに由来します。

配列は 16bit のアドレスです。
配列はコンパイル時に N 個分のメモリを確保します。

ポインタ・配列は、ベースの型の前に書きます。
英語の語順と一緒です。

- `*int` : pointer to int
- `[N]int` : array of int
- `[N]*int` : array of pointer to int

関数型は特殊です。
関数型の変数は定義できません。
そのかわりに、関数ポインタ型を使います。

`*(a:int)=>int`

## ポインタとアクセス

`*a` でポインタ `a` の指す先の値 `mem[a]` を得ます。
コンパイル時に `load` 命令になります。

`&a` で変数のアドレスを得ます。
変数のアドレスはコンパイル時に決定するので、
コンパイル時にアドレスの数値に置換します。

## 定義

```
program  =
 | var_def  = "var"  ident ":" type ";"
 | func_def = "func" ident ":" type compound
 | type_def = "type" ident ":" type ";"
```

プログラムのトップレベルには、グローバル変数定義、関数定義、型定義が並びます。

|          | 記法                                                    |
| -------- | ------------------------------------------------------- |
| 変数定義 | `var  hoge : int;`                                      |
| 関数定義 | `func hoge : (a : int, b : int) => int { return a+b; }` |
| 型定義   | `type hoge : {x : int, y : int};`                       |

変数の型は、関数型以外である必要があります。

## 文

関数定義には複文 (compound satements) が続き、
その中には文 (statement) が並びます。

```
compound = "{" stmt* "}"

stmt =
空文
 | void_stmt = ";"
式文
 | expr_stmt = expr ";"
複文
 | compound  = "{" stmt* "}"
変数定義
 | var_def   = "var" ident ":" type ";"
代入文
 | assign    = expr ("="|"+="|"-="|"*="|"/=") expr ";"
 | uassign   = ("++"|"--") expr ";"
制御文
 | if        = "if" "(" expr ")" stmt ("else" stmt)?
 | goto      = "goto" ident ";"
 | label     = ident ":"
繰り返し文
 | while     = "while" "(" expr ")" stmt
 | for       = "for" "(" expr? ";" expr? ";" expr? ")" stmt
 | continue  = "continue" ";"
 | break     = "break" ";"
 | return    = "return" expr ";"
```

## 式

### 演算

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

### 値

```
prim =
 | num
 | ("(" expr ")" | ident) (
    | "*"
    | "&"
    | "[" expr "]"
    | "."  ident
    | "->" ident
    | "(" expr % "," ")"
   )
```

## 補足

### ++の問題

C 言語の ++ には

```c++:
#include <iostream>

struct Int {
  int i;
  Int(int i) : i(i) {}
  int preinc() {
    i += 1;
    return i;
  }
  int postinc() {
    int ret = i;
    i += 1;
    return ret;
  }
};

int main() {
  Int i(0);
  std::cout << i.preinc() + i.preinc() << std::endl; // 3
  int j = 0;
  std::cout << ++j + ++j << std::endl; // 4
  return 0;
}
```

# トークン化

# 抽象構文木の構築

| `Node::Type` | `childs` |           |            |         |
| ------------ | -------- | --------- | ---------- | ------- |
| **Type**     |          |           |            |         |
| TypeFunc     | arg[]    | ret       |            |         |
| TypeStruct   | member[] |           |            |         |
| TypeArray    | base     | size      |            |         |
| TypePointer  | base     |           |            |         |
| TypePrim     | ident    |           |            |         |
| **Program**  |          |           |            |         |
| Program      | defs     |           |            |         |
| FuncDef      | name     | type      | compound   |         |
| VarDef       | name     | type      | expr       |         |
| TypeDef      | name     | type      |            |         |
| **Stmt**     |          |           |            |         |
| Compound     | stmt[]   |           |            |         |
| VoidStmt     |          |           |            |         |
| ExprStmt     | expr     |           |            |         |
| If           | cond     | true_node |            |         |
| IfElse       | cond     | true_node | false_node |         |
| While        | cond     | body      |            |         |
| DoWhile      | cond     | body      |            |         |
| For          | cond     | body      | init       | iterate |
| Continue     |          |           |            |         |
| Break        |          |           |            |         |
| Return       | expr     |           |            |         |
| **Expr**     |          |           |            |         |
| 三項演算     | cond     | true_node | false_node |         |
| 二項演算     | lhs      | rhs       |            |         |
| 単項演算     | child    |           |            |         |

# コード生成

入力するファイルはひとつです。

## シンボルテーブル生成

トップレベルの `func` `var` `type` で定義されたシンボルをまとめた表（map）を作ります。

| name : str | kind : Symbol::Kind | type : Node\* | label |
| ---------- | ------------------- | ------------- | ----- |
| hoge       | Func                |               |       |
| fuga       | VarGlobal           |               |       |
| piyo       | Type                |               |       |

これはグローバルのスコープです。

## 型のサイズ決定

変数に必要なメモリを知るために型のサイズを計算します。

型のノードを再帰的に評価すれば求まります。

- 整数型 : 1 (16bit)
- ポインタ型 : 1 (16bit)
- 配列型 : ベース型の N 倍
- 構造体型 : メンバのサイズの合計
- 関数型 : 関数ポインタ型

## グローバル変数のコード生成

グローバル変数の開始アドレスから、順番に割り当てます。

型のサイズが必要

## 関数のシンボルテーブル生成

関数ごとにローカル変数のテーブルを作ります。

関数内の `var` を探索します。

| name : str | type : Node\* | create | delete |
| ---------- | ------------- | ------ | ------ |
| arg0       |               | -1     |        |
| arg1       |               | -1     |        |
| var0       |               | 1      |        |
| var1       |               | 2      |        |

一覧表ができたら、依存関係と生存期間からアドレスを決定します。

スタックのアドレスはフレームポインタからの相対アドレスです。

## 実行環境メモ

現在の位置（関数 main 内のループ for_1 内の for_2 内みたいな）を保持しながらコード生成をする。

## 関数のコード生成

ident のノードを評価する際に、自分の関数の

# 今後追加したい機能

## 型チェック

# 作業日誌

## 11/10

- 四則演算の実装
  - 構文木を構築
  - 再帰的に評価する

```
expr =  mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary | "%" primary)*
primary = num | "(" expr ")"
```

## 11/13

- 演算子の拡張
  - ? :, ||, &&, |, ^, &, ==, !=, <, <=, >, >=, <<, >>, \*, /, %, ++, --

## 11/15

- 変数の実装

```
prim = num | ident | "(" expr ")"
```

## 11/16

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
expr = assign
assign = cond ("=" assign)?
cond = logical_or ("?" expr ":" cond)?
logical_or  = logical_and ("||" logical_and)*
logical_and = bit_or  ("&&" bit_or)*
bit_or  = bit_xor ("|" bit_xor)*
bit_xor = bit_and ("^" bit_and)*
bit_and = equal ("&" equal)*
equal = relat ("==" relat | "!=" relat)*
relat = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
shift = (shift "<<" | shift ">>")? add
add   = mul ("+" mul | "-" mul)*
mul   = unary ("*" unary | "/" unary | "%" unary)*
unary = ("++"|"--")? post
post  = prim ("++"|"--")?
prim  = num | ident | "(" expr ")"
```

## 2/8

- 関数定義

```
program = func*
func = ident "(" ident % "," ")" compound
compound = "{" stmt* "}"
stmt = ";" | expr ";" | compound
     | if | while | do_while | for
     | continue | break
     | return
if       = "if" "(" expr ")" stmt ("else" stmt)?
while    = "while" "(" expr ")" stmt
do_while = "do" stmt "while" ( expr ) ";"
for      = "for" "(" expr? ";" expr? ";" expr? ")" stmt
continue = "continue" ";"
break    = "break" ";"
return   = "return" expr ";"
```

## 2/9

- 型

```
program = func*
func = type ident "(" type ident % "," ")" compound
```

## 2/10

- AST の子供を一つの vector にまとめた
- 型演算

## 2/12

- 変数定義

## 2/13

## 配列

- 配列の定義は、コンパイル時にメモリの確保をする

`int a[N]`

を読み込んだ時に、コンパイラはメモリリソースカウンタを `sizeof(int)*N` 進める。

- 配列は先頭へのポインタである

`*a == a[0]`

- 配列アクセスはメモリへの相対アクセスである

`a[n] == *(a + n * sizeof(int))`

# c 言語の文法

## 用語

- declaration : 宣言
  - `int func(int,int);`
- declarator : 宣言子
- definition : 定義
  - `int func(int a, int b){}`
- qualifier : 修飾子
  - `const`
- specifier : 指定子
- statement : 文
- expression : 式

## BNF

```
program = external_declaration\*
external_declaration = function_definition | declaration
```

external declaration は、関数ブロックの外での宣言のこと。

プログラムは、トップレベルで見ると、関数定義か宣言の列になっている。

## 宣言

```
declaration_specifier = storage_class_specifier | type_specifier | type_qualifier

storage_class_specifier = "auto" | "register" | "static" | "extern" | "typedef"

type_specifier = "char" | "short" | "int" | "long" | "float" | "double" | "signed" | "unsigned"
| struct_or_union_specifier | enum_specifier | typedef_name

type_qualifier = "const" | "volatile"
```

```
declarator = pointer? direct_declarator

pointer = "_" type_qual_ pointer?

direct_declarator = (ident | "(" declarator ")") ("[" const_expr? "]" | "(" param_type_list ")" | "(" ident\* ")")
```

## 関数定義

```

function_definition = declaration_specifier* declarator declaration* compound_stmt

```

## 式

下に行くほど、優先度が高い演算になります。

```
expr = assign_expr ("," assign_expr)?

assign*expr = cond_expr | unary_expr ("=" | "\*=" | "/=" | "%=" | "+=" | "*=" | "<<=" | ">>=" | "&=" | "^=" | "|=") assign_expr

const*expr = cond_expr
cond_expr = logical_or ("?" expr ":" cond_expr)?
logical_or = logical_and ("||" logical_and)*
logical*and = bit_or ("&&" bit_or)*
bit*or = bit_xor ("|" bit_xor)*
bit*xor = bit_and ("^" bit_and)*
bit*and = eqality ("&" eqality)*
equality = relational ("==" relational | "!=" relational)_
relational = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)_
shift = add ("<<" add | ">>" add)_
add = mul ("+" mul | "-" mul)_
mul = cast ("_" cast | "/" cast | "%" cast)_
cast = ("(" type*name ")")* unary
unary = postfix
| ("++" | "--" ) unary
| ("&" | "_" | "+" | "-" | "~" | "!") cast
| "sizeof" unary
| "sizeof" type_name
postfix = primary ("[" expr "]" | "(" assignment_ ")" | "." ident | "->" ident | "++" | "--")\*
primary = ident | const | string | "(" expr ")"
const = int_const | char_const | float_const | enum_const

ident = 'regexp:[a-zA-Z][a-za-z0-9_]_'
int_const = '\d+'
char_const = '[a-zA-Z]'
float_const = '[+-]?([0-9]_[.])?[0-9]+f'
enum*const='[a-zA-Z]a-zA-Z0-9*]\*' // Same as identifier
```

## 型

```
type_name = specifier_qualifier+ abstract_declarator?

parameter_type_list = parameter_list ("," "...")?

parameter_list = parameter_declaration ("," parameter_declaration)\*

parameter_declaration = declaration_specifier+ (declarator | abstract_declarator)

abstract_declarator = pointer | pointer direct_abstract_declarator | direct_abstract_declarator

direct_abstract_declarator = "(" abstract_declarator ")"
| direct_abstract_declarator? "[" constant_expr? "]"
| direct_abstract_declarator? "(" parameter_type_list? ")"
```

## 構造体・共用体

```
struct_or_union_specifier = ("struct"|"union") (ident | "{" struct_declarator+ "}" | ident "{" struct_decl+ "}")

struct_declaration = specifier_qualifier\* struct_declarator_list

specifier_qualifier = type_specifier | type_qualifier

struct_decl_list = struct_decl ("," struct_decl)\*

struct_decl = declarator | declarator ":" const_expr | ":" const_expr
```

## 列挙型

```
enum_specifier = "enum" (ident | ident "{" enum_list "}" | "{" enum_list "}")
enum_list = (ident ("=" const_expr)?)\*
```

```
typedef_name = ident
```

## 宣言と初期化

```
declaration = declaration_spec+ init_declarator\* ";"

init_declarator = declarator ("=" initializer)?

initializer = assign_expr | "{" initializer ("," initializer)\* (",")? "}"
```

## 文

```
stmt = expr? ";" // expr : 式文
| "{" decl* stmt* "}" // compound :複文
| "if" "(" expr ")" stmt ("else" stmt)? // selection : 選択文
| ident ":" stmt // labeled : ラベル文
| "case" const_expr : stmt
| "default" ":" stmt
| "while" "(" expr ")" stmt // iteration : 反復文
| "do" stmt "while" ( expr ) ";"
| "for" "(" expr? ";" expr? ";" expr? ")" stmt
| "goto" ident ";" // jump : ジャンプ文
| "continue" ";"
| "break" ";"
| "return" expr? ";"
```
