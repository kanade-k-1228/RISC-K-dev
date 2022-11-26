# RKEMU

RISC-K のエミュレータです。

こういうふうに、命令を手書きしてもシュミレーションできる

```C:
int main() {
  CPU cpu;
  cpu.loadi(T0, 0x1111);
  cpu.loadi(T1, 0x2222);
  cpu.add(T0, T0, T1);
  cpu.dump();
  return 0;
}
```

## デバッグ設定ファイル（.dump）

```
0x0000 d i0
```

- d : コアダンプを吐き出す
- i: 割り込み

## 出力装置

デバッグ用に、シリアル出力みたいな出力装置があります。

`COUT` : シリアル出力制御レジスタ

0でない値が入ると、出力します。

```C:
mem.at(COUT) = 'h';
mem.at(COUT) = 'i';
// > hi
```

こんなかんじ。

```C:
void print(char *buf){
  while(*buf != 0){
    mem.at(COUT) = *buf;
  }
  return;
}
```

https://catalog.lib.kyushu-u.ac.jp/opac_download_md/1441262/isee530.pdf
