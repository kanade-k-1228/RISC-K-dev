# RKCC

RISC-K 用の C コンパイラ

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
  - ? :, ||, &&, |, ^, &, ==, !=, <, <=, >, >=, <<, >>, *, /, %, ++, --

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
stmt = expr ";"
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

## BNF

```
program = ext_declaration*
ext_declaration = funct_def | declaration
```

external declaration は、関数ブロックの外での宣言のこと。

プログラムは、トップレベルで見ると、関数定義か宣言のどちらか。

```
funct_def = dec_spec* declarator declaration* compound_stmt
```

### 宣言

```
dec_spec = storage_class_spec | type_spec | type_qual

storage_class_spec = "auto" | "register" | "static" | "extern" | "typedef"

type_spec = "char" | "short" | "int" | "long" | "float" | "double" | "signed" | "unsigned"
          | struct_or_union_spec | enum_spec | typedef_name

type_qual = "const" | "volatile"

```

```
declarator = pointer? direct_declarator

pointer = "*" type_qual* pointer?

direct_declarator = ident
                  | "(" declarator ")"
                  | direct_declarator "[" const_expr? "]"
                  | direct_declarator "(" param_type_list ")"
                  | direct_declarator "(" ident* ")"
```


### 構造体・共用体

```
<struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>

<struct-or-union> ::= struct
                    | union

<struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>

<specifier-qualifier> ::= <type-specifier>
                        | <type-qualifier>

<struct-declarator-list> ::= <struct-declarator>
                           | <struct-declarator-list> , <struct-declarator>

<struct-declarator> ::= <declarator>
                      | <declarator> : <constant-expr>
                      | : <constant-expr>
```


### 式

```
expr = assign_expr ("," assign_expr)?

assign_expr = cond_expr | unary_expr assign_opr assign_expr

assign_opr = "=" | "*=" | "/=" | "%=" | "+=" | "_=" | "<<=" | ">>=" | "&=" | "^=" | "|="

unary_opr = "&" | "*" | "+" | "-" | "~" | "!"
```

### 定式

下に行くほど、優先度が高い演算になります。

```
const_expr = cond_expr
cond_expr  = logical_or_expr ("?" expr ":" cond_expr)?
logical_or  = logical_xor ("||" logical_xor)*
logical_xor = logical_and ("^^" logical_and)*
logical_and = bit_or  ("&&" bit_or)*
bit_or  = bit_xor ("|" bit_xor)*
bit_xor = bit_and ("^" bit_and)*
bit_and = eqal ("&" eqal)*
equal      = relational ("==" relational | "!=" relational)*
relational = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
shift = add ("<<" add | ">>" add)*
add   = mul ("+" mul | "-" mul)*
mul   = unary ("*" unary | "/" unary | "%" unary)*
unary   = ("++"|"--")? post
post    = prim ("++"|"--")?
primary = num | "(" expr ")"

```

#### 論理 AND

```
<logical_and_expr> ::= <inclusive_or_expr>
                           | <logical_and_expr> && <inclusive_or_expr>
```

#### ビット OR

```
<inclusive_or_expr> ::= <exclusive_or_expr>
                            | <inclusive_or_expr> | <exclusive_or_expr>
```

#### ビット XOR

```
<exclusive_or_expr> ::= <and_expr>
                            | <exclusive_or_expr> ^ <and_expr>
```

#### ビット AND

```
<and_expr> ::= <equality_expr>
                   | <and_expr> & <equality_expr>
```

#### 比較演算

```
<equality_expr> ::= <relational_expr>
                        | <equality_expr> == <relational_expr>
                        | <equality_expr> != <relational_expr>

<relational_expr> ::= <shift_expr>
                          | <relational_expr> < <shift_expr>
                          | <relational_expr> > <shift_expr>
                          | <relational_expr> <= <shift_expr>
                          | <relational_expr> >= <shift_expr>
```

#### シフト演算

```
<shift_expr> ::= <additive_expr>
                     | <shift_expr> << <additive_expr>
                     | <shift_expr> >> <additive_expr>
```

#### 四則演算

```
<additive_expr> ::= <multiplicative_expr>
                        | <additive_expr> + <multiplicative_expr>
                        | <additive_expr> _ <multiplicative_expr>

<multiplicative_expr> ::= <cast_expr>
                              | <multiplicative_expr> * <cast_expr>
                              | <multiplicative_expr> / <cast_expr>
                              | <multiplicative_expr> % <cast_expr>
```

#### キャスト

```
<cast_expr> ::= <unary_expr>
                    | ( <type_name> ) <cast_expr>
```

#### 単項演算

```
<unary_expr> ::= <postfix_expr>
                     | ++ <unary_expr>
                     | __ <unary_expr>
                     | <unary_operator> <cast_expr>
                     | sizeof <unary_expr>
                     | sizeof <type_name>
```

#### 後置演算

```
<postfix_expr> ::= <primary_expr>
                       | <postfix_expr> [ <expr> ]
                       | <postfix_expr> ( {<assignment_expr>}* )
                       | <postfix_expr> . <identifier>
                       | <postfix_expr> _> <identifier>
                       | <postfix_expr> ++
                       | <postfix_expr> __
```

#### 最小単位

```
prim_expr = ident | const | str | "(" expr ")"
```

#### 定数値

```
const = int_const | char_const | float_const | enum_const
```


### 型

```
<type_name> ::= {<specifier_qualifier>}+ {<abstract_declarator>}?

<parameter_type_list> ::= <parameter_list>
                        | <parameter_list> , ...

<parameter_list> ::= <parameter_declaration>
                   | <parameter_list> , <parameter_declaration>

<parameter_declaration> ::= {<declaration_specifier>}+ <declarator>
                          | {<declaration_specifier>}+ <abstract_declarator>
                          | {<declaration_specifier>}+

<abstract_declarator> ::= <pointer>
                        | <pointer> <direct_abstract_declarator>
                        | <direct_abstract_declarator>

<direct_abstract_declarator> ::=  ( <abstract_declarator> )
                               | {<direct_abstract_declarator>}? [ {<constant_expr>}? ]
                               | {<direct_abstract_declarator>}? ( {<parameter_type_list>}? )
```

```
<enum_specifier> ::= enum <identifier> { <enumerator_list> }
                   | enum { <enumerator_list> }
                   | enum <identifier>

<enumerator_list> ::= <enumerator>
                    | <enumerator_list> , <enumerator>

<enumerator> ::= <identifier>
               | <identifier> = <constant_expr>
```

```
<typedef_name> ::= <identifier>

<declaration> ::=  {<declaration_specifier>}+ {<init_declarator>}* ;

<init_declarator> ::= <declarator>
                    | <declarator> = <initializer>

<initializer> ::= <assignment_expr>
                | { <initializer_list> }
                | { <initializer
                list> , }

init_list = initializer | init_list "," initializer
```

### 文

```
stmt = expr? ";"                             // expr 式文
     | "{" decl* stmt* "}"                   // compound 複文
     | "if" "(" expr ")" stmt ("else" stmt)? // selection 選択文
     | ident ":" stmt                        // labeled ラベル文
     | "case" const_expr : stmt 
     | "default" ":" stmt
     | "while" "(" expr ")" stmt            // iteration 反復文
     | "do" stmt "while" ( expr ) ";"
     | "for" "(" expr? ";" expr? ";" expr? ")" stmt
     | "goto" ident ";"                     // jump ジャンプ文
     | "continue" ";"
     | "break" ";"
     | "return" expr? ";"
```
