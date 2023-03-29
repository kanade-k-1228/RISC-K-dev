# RISC-K ISA

## 命令一覧

| operation | arg1                     | arg2         | arg3                  |
| --------- | ------------------------ | ------------ | --------------------- |
| calc      | rd  (Destination)        | rs1 (Source) | rs2 (Source)          |
| calci     | rd  (Destination)        | rs1 (Source) | imm/const (Source)    |
| load      | rd  (Destination)        | rs1 (Offset) | imm/var (RAM Address) |
| store     | rs2 (Source)             | rs1 (Offset) | imm/var (RAM Address) |
| jmpif     | rs2 (Condition)          | rs1 (Offset) | imm/lab (ROM Address) |
| fcall     | rd  (Set Return Address) | rs1 (Offset) | imm/lab (ROM Address) |

- add / addi
- sub / subi
- and / andi
- or / ori
- xor / xori
- not
- srs (Shift Right Signed)
- sru (Shift Right Unsigned)
- sl (Shift Left)
- eq / eqi (Equal)
- lts / ltsi (Less Than Signed)
- ltu / ltui (Less Than Unsigned)
- lcast (Cast to boolean)

## レジスタ

| addr | name |           |
| ---- | ---- | --------- |
| 0x00 | zero | Read Only |
| 0x01 | pc   | Read Only |
| 0x02 | ira  | Read Only |
| 0x03 | csr  |           |
| 0x04 | ra   |           |
| 0x05 | sp   |           |
| 0x06 | fp   |           |
| 0x07 | cout |           |
| 0x08 | s0   |           |
| 0x09 | s1   |           |
| 0x0A | s2   |           |
| 0x0B | s3   |           |
| 0x0C | t0   |           |
| 0x0D | t1   |           |
| 0x0E | t2   |           |
| 0x0F | t3   |           |

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
