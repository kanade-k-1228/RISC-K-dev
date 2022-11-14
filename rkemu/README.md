# RKEMU

RISC-Kのエミュレータです。

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

コアダンプを吐き出すPCの一覧表。

第二引数に入れることができる。
