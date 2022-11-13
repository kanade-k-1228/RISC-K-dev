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
expr       = equality ("=" assign)?
equality   = relational ("==" relational | "!=" relational)*
relational = add ("<" add | "<=" add | ">" add | ">=" add)*
add        = mul ("+" mul | "-" mul)*
mul        = unary ("*" unary | "/" unary | "%" unary)*
unary      = ("+"|"-")? primary
primary    = num | "(" expr ")"
```

- 文と変数の実装
  - 変数はスタックに追加する

```
program = stmt*
stmt = expr ";"
expr =  mul ("+" mul | "-" mul)*
mul = primary ("*" primary | "/" primary | "%" primary)*
primary = num | ident | "(" expr ")"
```


## 予定

- 演算
  - 論理演算
  - ビット演算
  - 比較演算
  - シフト演算
  - 単項演算
- 文
  - stmt = expr ";"
- 変数
  - ident
- 代入
  - assign = equality "=" assign
