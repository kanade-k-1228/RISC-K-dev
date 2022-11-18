# RISC-K ISA

![](img/bit_format.png)

## 命令一覧

| operation | arg1 | arg2    | arg3    |
| --------- | ---- | ------- | ------- |
| calc      | rd   | rs1     | rs2     |
| calci     | rd   | rs      | imm     |
| load      | rd   | rs      | imm     |
| loadi     | rd   | imm/lab |         |
| store     | rs   | rs      | imm     |
| jump      | ra   | rs      | imm/lab |
| breq/brlt | rs1  | rs2     | imm/lab |

- add / addi
- sub / subi
- and / andi
- or / ori
- not

## 特殊レジスタ

| CSR |              |
| --- | ------------ |
| 0   | 割り込み許可 |
| 1   | クロック停止 |
| 2   | 割り込み     |
| 3   | 割り込み 1   |
| 4   | 割り込み 2   |
| 5   | 割り込み 3   |
| 6   | 割り込み 4   |
| 7   |              |
| 8   |              |
| 9   |              |
| A   |              |
| B   |              |
| C   |              |
| D   |              |
| E   | シリアル送信 |
| F   | シリアル受信 |

### シリアル出力
