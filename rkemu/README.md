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

## ブレークポイントファイル（.bkp）

コアダンプを吐き出す PC の一覧表。

第二引数に入れることができる。

## 出力装置

デバッグ用に、シリアル出力みたいな出力装置があります。

- COUT : 送信する文字コード
- SEND : 送信指令、送信完了すると 0 になる

```C:
mem.at(COUT) = 'h';
mem.at(SEND) = 1;
mem.at(COUT) = 'i';
mem.at(SEND) = 1;
// > hi
```

こんなかんじ。

```C:
void print(char *buf){
  while(*buf != 0){
    mem.at(COUT) = *buf;
    mem.at(SEND) = 1;
  }
  return;
}
```
