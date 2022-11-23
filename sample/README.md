# テスト

### アセンブラ

- 01:関数呼び出し
- 02:スタックを使った関数呼び出し
- 03:再帰関数

### コンパイラ

## 命名規則

ラベルは、名前空間、関数名、制御文を用いて、ネスト構造がわかりやすいようにつける。

```C:
int main(){
    for(int i=10;i!=0;--i);
}
```

```rkasm:
main:
    loadi t0 10
main_for_loop:
    breq t0 zero main_for_break
    subi t0 t0 1
    jump zero zero main_for_loop
main_for_break:
```
