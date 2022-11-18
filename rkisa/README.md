# RISC-K ISA

![](img/bit_format.png)

## 命令一覧

| operation | arg1 | arg2    | arg3        |
| --------- | ---- | ------- | ----------- |
| calc      | rd   | rs1     | rs2         |
| calci     | rd   | rs      | imm (12bit) |
| load      | rd   | rs      | imm         |
| loadi     | rd   | imm/lab |             |
| store     | rs   | rs      | imm         |
| jump      | ra   | rs      | imm/lab     |
| breq/brlt | rs1  | rs2     | imm/lab     |

- add / addi
- sub / subi
- and / andi
- or / ori
- not
- rrot
- lrot

## 特殊レジスタ

| CSR |       |              |
| --- | ----- | ------------ |
| 0   | IEN   | 割り込み許可 |
| 1   | INTR  | 割り込み     |
| 2   | INTR0 | 割り込み 0   |
| 3   | INTR1 | 割り込み 1   |
| 4   | INTR2 | 割り込み 2   |
| 5   | INTR3 | 割り込み 3   |
| 6   | SRCV  | シリアル受信 |
| 7   | SSEND | シリアル送信 |
| 8   | CSTOP | クロック停止 |
| 9   |       |              |
| A   |       |              |
| B   |       |              |
| C   | -     |              |
| D   | -     |              |
| E   | -     |              |
| F   | -     |              |

即値演算は 12bit

### シリアル出力
