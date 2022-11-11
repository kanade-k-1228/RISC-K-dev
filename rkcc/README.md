# RKCC

RISC-K 用の C コンパイラ

## BNFの解読

```
<translation-unit> ::= {<external-declaration>}*
<external-declaration> ::= <function-definition>
                         | <declaration>
```

翻訳単位とは、外部宣言の列である。

外部宣言とは、関数定義と、宣言である。

```
<function-definition> ::= {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>
```

関数定義とは、宣言指定子、

```
<declaration-specifier> ::= <storage-class-specifier>
                          | <type-specifier>
                          | <type-qualifier>

<storage-class-specifier> ::= auto
                            | register
                            | static
                            | extern
                            | typedef

<type-specifier> ::= void
                   | char
                   | short
                   | int
                   | long
                   | float
                   | double
                   | signed
                   | unsigned
                   | <struct-or-union-specifier>
                   | <enum-specifier>
                   | <typedef-name>

```

宣言指定子とは、ストレージ指定子または型指定子または型？？

ストレージ指定子は、

型指定子は、要する型の名前である。見慣れた名前が並んでる。


### 構造体・共用体

```
<struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>

<struct-or-union> ::= struct
                    | union

<struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>
```

union（共用体）というのは、同じメモリに異なる型の変数を入れることができる

構造体が直積、共用体が直和に対応しており、構造体と共用体が対になる概念。

バグの温床になるのでできるだけ使わないほうがいいです。
どうしてもメモリを節約したいときに、きちんと排他的であることを確認して使いましょう。

// 式

```BNF:
<expression> ::= <assignment-expression>
               | <expression> , <assignment-expression>

<assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>

<assignment-operator> ::= =
                        | *=
                        | /=
                        | %=
                        | +=
                        | -=
                        | <<=
                        | >>=
                        | &=
                        | ^=
                        | |=


// 式
expression ::= assignment
            | expression , assignment

// 代入式
assignment ::= 

// 代入演算子
assignment-operator ::= 

unary ::= postfix 
        | "++" unary 
        | "--" unary 
        | unary-ope cast 
        | sizeof unary 
        | sizeof typename

// 一番基本の要素
primary ::= ident
        | const
        | str
        | "(" expression ")"

postfix ::= primary
            | postfix "[" expression "]" 
            | postfix "(" { assigment }* ")"
            | postfix "." ident
            | postfix "->" ident
            | postfix ++
            | postfix --

```
