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

```
expr   = cond
cond   = l_or  ("?" expr ":" expr)?
l_or   = l_xor ("||" l_xor)*
l_xor  = l_and ("^^" l_and)*
l_and  = b_or  ("&&" b_or)*
b_or   = b_xor ("|" b_xor)*
b_xor  = b_and ("^" b_and)*
b_and  = eqal  ("&" eqal)*
equal  = relate ("==" relate | "!=" relate)
relate = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)
shift  = add ("<<" add | ">>" add)*
add    = mul ("+" mul | "-" mul)*
mul    = unary ("*" unary | "/" unary | "%" unary)*
unary  = ("++"|"--")? post
post   = prim ("++"|"--")?
prim   = num | "(" expr ")"
```
## 予定

- 文・変数・代入の実装
  - 変数はスタックに追加する

```
program = stmt*
stmt = expr ";"
assign = equal "=" assign
primary = num | ident | "(" expr ")"
```
