# RKCC

RISC-K 用の C コンパイラ

## 特徴

- 型
  - 型は int = uint16 のみ
  - ポインタも 16bit
  - const, volatile
- 構造体

## 作業日誌

### 11/10

- 四則演算の実装
  - 構文木を構築
  - 再帰的に評価する

```
expr =  mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary | "%" primary)*
primary = num | "(" expr ")"
```

### 11/13

- 演算子の拡張
  - ? :, ||, &&, |, ^, &, ==, !=, <, <=, >, >=, <<, >>, \*, /, %, ++, --

### 11/15

- 変数の実装

```
prim = num | ident | "(" expr ")"
```

### 11/16

- 文の実装
- 代入の実装
- 現時点での文法

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

### 2/8

- 関数

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

### 変数リスト

### 型の実装

C 言語の型は記法が複雑なので，ここでは簡単な記法を使う．

| C           | C--                   |
| ----------- | --------------------- |
| `int a`     | `int a`               |
| `int* a`    | `ptr<Int> a`          |
| `int* a`    | `ptr<ptr<int>> a`     |
| `int a[N]`  | `array<int,N> a`      |
| `int* a[N]` | `array<ptr<int>,N> a` |

```
type = "int"
     | "ptr" "<" type ">"
     | "array" "<" type "," num ">"
```

### 関数定義の実装

```
program = func_def*
func_def = type ident "(" (type ident) % "," ")" "{" stmt* "}"
```

### 関数呼出の実装

```
prim = ("(" expr ")" | num | ident)("(" expr % "," ")")?
```

### アセンブリの生成

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

### 構造体の実装

C++スタイル

```
struct = "struct" ident "{" (type ident ";")* "}" ";"
```

### 型チェックとキャスト

### ?/?

- グローバル変数の実装

```
program  = extrnal*
external = func_def | glob_var
func_def = type ident "(" (type ident) % "," ")" "{" stmt* "}"
glob_var = type ident ("=" expr) ";"
stmt = ...
```

### トップレベル

```
プログラム
 = 外部宣言*
  = 変数宣言   = 型修飾子* 型名 変数名 ("=" (初期化子 | 代入子))? ";"
  | 関数宣言   = 型名 関数名 "("  型名        % "," ")" ";"
  | 構造体定義 = "struct" 構造体名 "{" (型名 メンバ名)        % ";" "}" ";"
  | 列挙体定義 = "enum"   列挙体名 "{" ラベル名 ("=" 定整数)? % "," "}" ";"
  | 関数定義   = 型名 関数名 "(" (型名 変数名) % "," ")" 文
```

### 今後

- 関数定義の実装

```
プログラム = 外部宣言*
外部宣言 = 変数宣言 | 関数宣言 | 型宣言
program = external_declaration*
external_declaration = type_qualifier // 変数定義
                     |
function_definition = declaration_specifier* declarator declaration* compound_stmt
```

## c 言語の文法

### 用語

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

### BNF

```
program = external_declaration*
external_declaration = function_definition | declaration
```

external declaration は、関数ブロックの外での宣言のこと。

プログラムは、トップレベルで見ると、関数定義か宣言の列になっている。

### 宣言

```
declaration_specifier = storage_class_specifier | type_specifier | type_qualifier

storage_class_specifier = "auto" | "register" | "static" | "extern" | "typedef"

type_specifier = "char" | "short" | "int" | "long" | "float" | "double" | "signed" | "unsigned"
               | struct_or_union_specifier | enum_specifier | typedef_name

type_qualifier = "const" | "volatile"
```

```
declarator = pointer? direct_declarator

pointer = "*" type_qual* pointer?

direct_declarator = (ident | "(" declarator ")") ("[" const_expr? "]" | "(" param_type_list ")" | "(" ident* ")")
```

### 関数定義

```
function_definition = declaration_specifier* declarator declaration* compound_stmt
```

### 式

下に行くほど、優先度が高い演算になります。

```
expr = assign_expr ("," assign_expr)?

assign_expr = cond_expr | unary_expr ("=" | "*=" | "/=" | "%=" | "+=" | "_=" | "<<=" | ">>=" | "&=" | "^=" | "|=") assign_expr

const_expr = cond_expr
cond_expr  = logical_or ("?" expr ":" cond_expr)?
logical_or  = logical_and ("||" logical_and)*
logical_and = bit_or  ("&&" bit_or)*
bit_or  = bit_xor ("|" bit_xor)*
bit_xor = bit_and ("^" bit_and)*
bit_and = eqality ("&" eqality)*
equality   = relational ("==" relational | "!=" relational)*
relational = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
shift = add ("<<" add | ">>" add)*
add   = mul ("+" mul | "-" mul)*
mul   = cast ("*" cast | "/" cast | "%" cast)*
cast  = ("(" type_name ")")* unary
unary   = postfix
        | ("++" | "--" ) unary
        | ("&" | "*" | "+" | "-" | "~" | "!") cast
        | "sizeof" unary
        | "sizeof" type_name
postfix = primary ("[" expr "]" | "(" assignment* ")" | "." ident | "->" ident | "++" | "--")*
primary = ident | const | string | "(" expr ")"
const = int_const | char_const | float_const | enum_const

ident = 'regexp:[a-zA-Z][a-zA-Z0-9_]*'
int_const = '\d+'
char_const = '[a-zA-Z]'
float_const = '[+-]?([0-9]*[.])?[0-9]+f'
enum_const='[a-zA-Z][a-zA-Z0-9_]*' // Same as identifier
```

### 型

```
type_name = specifier_qualifier+ abstract_declarator?

parameter_type_list = parameter_list ("," "...")?

parameter_list = parameter_declaration ("," parameter_declaration)*

parameter_declaration = declaration_specifier+ (declarator | abstract_declarator)

abstract_declarator = pointer | pointer direct_abstract_declarator | direct_abstract_declarator

direct_abstract_declarator =  "(" abstract_declarator ")"
                           | direct_abstract_declarator? "[" constant_expr? "]"
                           | direct_abstract_declarator? "(" parameter_type_list? ")"
```

### 構造体・共用体

```
struct_or_union_specifier = ("struct"|"union") (ident | "{" struct_declarator+ "}" | ident "{" struct_decl+ "}")

struct_declaration = specifier_qualifier* struct_declarator_list

specifier_qualifier = type_specifier | type_qualifier

struct_decl_list = struct_decl ("," struct_decl)*

struct_decl = declarator | declarator ":" const_expr | ":" const_expr
```

### 列挙型

```
enum_specifier = "enum" (ident | ident "{" enum_list "}" | "{" enum_list "}")
enum_list = (ident ("=" const_expr)?)*
```

```
typedef_name = ident
```

### 宣言と初期化

```
declaration =  declaration_spec+ init_declarator* ";"

init_declarator = declarator ("=" initializer)?

initializer = assign_expr | "{" initializer ("," initializer)* (",")? "}"
```

### 文

```
stmt = expr? ";"                             // expr : 式文
     | "{" decl* stmt* "}"                   // compound :複文
     | "if" "(" expr ")" stmt ("else" stmt)? // selection : 選択文
     | ident ":" stmt                        // labeled : ラベル文
     | "case" const_expr : stmt
     | "default" ":" stmt
     | "while" "(" expr ")" stmt            // iteration : 反復文
     | "do" stmt "while" ( expr ) ";"
     | "for" "(" expr? ";" expr? ";" expr? ")" stmt
     | "goto" ident ";"                     // jump : ジャンプ文
     | "continue" ";"
     | "break" ";"
     | "return" expr? ";"
```
