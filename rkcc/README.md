# RKCC

自作マイコン用の高級言語です。

- [RKCC](#rkcc)
- [特徴](#特徴)
  - [言語設計の方針](#言語設計の方針)
  - [型システム](#型システム)
- [文法](#文法)
  - [トップレベル](#トップレベル)
  - [型](#型)
  - [文](#文)
  - [型システム](#型システム-1)
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
- [実装すること](#実装すること)
  - [変数リスト d](#変数リスト-d)
  - [関数定義](#関数定義)
  - [関数呼出](#関数呼出)
  - [アセンブリの生成](#アセンブリの生成)
  - [構造体](#構造体)
  - [型チェックとキャスト](#型チェックとキャスト)
  - [グローバル変数](#グローバル変数)
  - [列挙体](#列挙体)
  - [トップレベル](#トップレベル-1)
- [c 言語の文法](#c-言語の文法)
  - [用語](#用語)
  - [BNF](#bnf)
  - [宣言](#宣言)
  - [関数定義](#関数定義-1)
  - [式](#式)
  - [型](#型-1)
  - [構造体・共用体](#構造体共用体)
  - [列挙型](#列挙型)
  - [宣言と初期化](#宣言と初期化)
  - [文](#文-1)
- [AST](#ast)
- [コード生成](#コード生成)
  - [シンボルテーブル生成](#シンボルテーブル生成)
  - [型のサイズ決定](#型のサイズ決定)
  - [グローバル変数のコード生成](#グローバル変数のコード生成)
  - [関数のシンボルテーブル生成](#関数のシンボルテーブル生成)
  - [関数のコード生成](#関数のコード生成)

# 特徴
## 言語設計の方針
基本的にはＣ言語を踏襲しますが、以下の方針に従って改変しています。
- 組み込みシステムを作るために実用上十分な機能
- コンパイラの実装がわかりやすく、簡単になること
## 型システム
C言語の型システムはわかりにくく、それは非本質的な難しさです。
TypeScriptやGoを参考に、わかりやすい
Cをベースにしつつ、型の記法はGoやTSを参考にしています。

# 文法

## トップレベル

```
program  = ( func_def | var_def | type_def )*
func_def = "func" ident ":" type compound
var_def  = "var"  ident ":" type ("=" expr)? ";"
type_def = "type" ident ":" type ";"
```

トップレベルには、グローバル変数定義、関数定義、型定義が並ぶ。

|            | 記法                                                    |
| ---------- | ------------------------------------------------------- |
| ※変数宣言  | `extern hoge : int;`                                    |
| 変数定義   | `var hoge : int;`                                       |
| 変数初期化 | `var hoge : int = 10;`                                  |
| ※関数宣言  | `extern hoge : (int, int) => int;`                      |
| 関数定義   | `func hoge : (a : int, b : int) => int { return a+b; }` |
| 型定義     | `type position : {x : int, y : int};`                   |

## 型

```
type =
 | type_struct = "{" ident ":" type % "," "}"
 | type_func   = "(" ident ":" type % "," ")" "=>" type
 | type_ptr    = "*" type
 | type_arr    = "[" expr "]" type
 | type_prim   = "(" type ")" | ident
```

型は構造体型、関数型、ポインタ型、配列型からなる。

|            | TypedC                        |
| ---------- | ----------------------------- |
| 基本型     | `int`                         |
| ポインタ型 | `*int`                        |
| 配列型     | `[N]int`                      |
| 構造体型   | `{m0 : int, m1 : int}`        |
| 関数型     | `(a0 : int, a1 : int) => int` |


## 文

関数定義には複文(compound satements)が続き、その中には文(statement)が並ぶ。

```
compound = "{" stmt* "}"

stmt =
 | ";"
 | expr_stmt = expr ";"
 | assign    = expr ("="|"+="|"*="|"/=") expr ";"
 | uassign   = ("++"|"--") expr ";"
 | var_def   = "var" ident ":" type ("=" expr)? ";"
 | compound  = "{" stmt* "}"
 | if        = "if" "(" expr ")" stmt ("else" stmt)?
 | while     = "while" "(" expr ")" stmt
 | do_while  = "do" stmt "while" ( expr ) ";"
 | for       = "for" "(" expr? ";" expr? ";" expr? ")" stmt
 | continue  = "continue" ";"
 | break     = "break" ";"
 | return    = "return" expr ";"
```

文は空文、式文、代入文、変数定義、制御文からなる。
空文な何もしない文で、NOPにコンパイルされる。
式文は式を評価する文で、評価値は破棄される。
代入文は左辺のアドレスに対して、右辺の評価値を代入する。
変数定義は、コンパイラの内部のシンボルテーブルに変数を追加する。初期化式を評価しその値を代入する。代入文と同じ。


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
mul   = unary ("*" unary | "/" unary | "%" unary)*
unary = ("++"|"--")? post
post  = prim ("++"|"--")?
prim  = num | ident | "(" expr ")"
```

## 型システム


```
type =
 | type_struct = "{" ident ":" type % "," "}"
 | type_func   = "(" ident ":" type % "," ")" "=>" type
 | type_prim   = "(" type ")" | ident
 | type_single = type ( "*" | "[" expr "]" )*

```

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

# 実装すること

## 変数リスト d

## 関数定義

```
program = func_def*
func_def = type ident "(" (type ident) % "," ")" "{" stmt* "}"
```

## 関数呼出

```
prim = ("(" expr ")" | num | ident)("(" expr % "," ")")?
```

## アセンブリの生成

関数が実装できたので，次のようなコードが生成できる

```
int main(){
int a = 42;
return 0;
}
```

変数は fp からの相対アドレスとして

```
main:
loadi t0 42
loadi a0 0
jump zero ra zero
```

環境メモを更新しながら、抽象構文木を深さ優先探索する

- 実行場所（スコープ
  - main 関数内の
  - id:for_1 の内
- 有効なローカル変数一覧
  - アドレス
    - レジスタ
    - スタックポインタに対する相対アドレス
- グローバル変数一覧

## 構造体

C++スタイル

```
struct = "struct" ident "{" (type ident ";")\* "}" ";"
```

## 型チェックとキャスト

## グローバル変数

```
program = extrnal*
external = func_def | glob_var
func_def = type ident "(" (type ident) % "," ")" "{" stmt* "}"
glob_var = type ident ("=" expr) ";"
stmt = ...
```

## 列挙体

## トップレベル

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

# AST

抽象構文木のノード一覧

| `Node::Type` | `childs` |           |            |         |
| ------------ | -------- | --------- | ---------- | ------- |
| Program      |          |           |            |         |
| **Type**     |          |           |            |         |
| TypeFunc     | arg[]    | ret       |            |         |
| TypeStruct   | member[] |           |            |         |
| TypeArray    | base     | size      |            |         |
| TypePointer  | base     |           |            |         |
| TypePrim     | ident    |           |            |         |
| -            |          |           |            |         |
| Func         | name     | type      | expr       |         |
| Compound     | stmt[]   | -         | -          | -       |
| **stmt**     |          |           |            |         |
| VarDef       | name     | type      | stmt?      |         |
| Stmt         | expr?    | -         | -          | -       |
| If           | cond     | true_node | -          | -       |
| IfElse       | cond     | true_node | false_node | -       |
| While        | cond     | body      | -          | -       |
| DoWhile      | cond     | body      | -          | -       |
| For          | cond     | body      | init       | iterate |
| Continue     | -        | -         | -          | -       |
| Break        | -        | -         | -          | -       |
| Return       | expr     | -         | -          | -       |
| 式 : expr    |          |           |            |         |
| 三項演算     | cond     | true_node | false_node | -       |
| 二項演算     | lhs      | rhs       | -          | -       |
| 単項演算     | child    | -         | -          | -       |

# コード生成

入力するファイルはひとつです。

## シンボルテーブル生成

トップレベルの `func` `var` `type` で定義されたシンボルをまとめた表（map）を作ります。

| name : str | kind : Symbol::Kind | type : Node* | label |
| ---------- | ------------------- | ------------ | ----- |
| hoge       | Func                |              |       |
| fuga       | VarGlobal           |              |       |
| piyo       | Type                |              |       |

これはグローバルのスコープです。

## 型のサイズ決定

変数に必要なメモリを知るために型のサイズを計算します。

型のノードを再帰的に評価すれば求まります。

- 整数型 : 1 (16bit)
- ポインタ型 : 1 (16bit)
- 配列型 : ベース型のN倍
- 構造体型 : メンバのサイズの合計
- 関数型 : 関数ポインタ型

## グローバル変数のコード生成

グローバル変数の開始アドレスから、順番に割り当てます。

型のサイズが必要

## 関数のシンボルテーブル生成

関数ごとにローカル変数のテーブルを作ります。

関数内の `var` を探索します。

| name : str | type : Node* | create | delete |
| ---------- | ------------ | ------ | ------ |
| arg0       |              | -1     |        |
| arg1       |              | -1     |        |
| var0       |              | 1      |        |
| var1       |              | 2      |        |

一覧表ができたら、依存関係と生存期間からアドレスを決定します。

スタックのアドレスはフレームポインタからの相対アドレスです。

## 関数のコード生成

identのノードを評価する際に、自分の関数の
