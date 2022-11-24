# RKRTOS - リアルタイム OS

## 機能

- タスク管理
  - 割り込み
  - 可変優先度
- 競合管理
  - セマフォ
  - ミューテックス
- メモリ管理
  - ヒープ

## メモリマップ

| Addr   | Usage     | Var       | Value  |
| ------ | --------- | --------- | ------ |
| 0x0000 | Registors |           |        |
|  :     |           |           |        |
| 0x0040 | IO        |           |        |
|  :     |           |           |        |
| 0x0100 | OS        | running   | null   | 実行中のタスク    |
| 0x0101 |           | task_top  | 0x0042 | タスクリストの先頭 |
| 0x0102 |           | heap_top  | 0x0119 | ヒープの先頭      |
| 0x0103 |           | flag      | 0x0000 | フラグ           |
| 0x0104 | task A    | next      | 0x010B | 連結リスト        |
| 0x0105 |           | state     | 0      | タスクの状態      |
| 0x0106 |           | flag_mask | 0x0000 | フラグの待ちマスク |
| 0x0107 |           | priority  | 1      | 現在の優先度      |
| 0x0108 |           | base_pri  | 1      | 基本優先度        |
| 0x0109 |           | sp        | 0xcfff | スタックポインタ  |
| 0x010A |           | bp        | 0xcfff | ベースポインタ    |
| 0x010B | task B    | next      | 0x0112 |                  |
| 0x010C |           | state     | 0      |                  |
| 0x010D |           | flag_mask | 0x0000 |                  |
| 0x010E |           | priority  | 2      |                  |
| 0x010F |           | base_pri  | 2      |                  |
| 0x0110 |           | sp        | 0xbfff |                  |
| 0x0111 |           | bp        | 0xbfff |                  |
| 0x0112 | task C    | next      | null   | 末尾はnull       |
| 0x0113 |           | state     | 0      |                  |
| 0x0114 |           | flag_mask | 0x0000 |                  |
| 0x0115 |           | priority  | 3      |                  |
| 0x0116 |           | base_pri  | 3      |                  |
| 0x0117 |           | sp        | 0xafff |                  |
| 0x0118 |           | bp        | 0xafff |                  |
| 0x0119 | heap      | next      | null   |                  |
| 0x011a |           | size      | 0x9ee3 |                  |
| 0x011b |           | mem       |        |                  |
|  :     |           |           |        |                  |
| 0x9fff |           |           |        |                  |
| 0xa000 | stack C   | [0x1000]  |        |                  |
|  :     |           |           |        |                  |
| 0xafff |           |           |        |                  |
| 0xb000 | stack B   | [0x1000]  |        |                  |
|  :     |           |           |        |                  |
| 0xbfff |           |           |        |                  |
| 0xc000 | stack A   | [0x1000]  |        |                  |
|  :     |           |           |        |                  |
| 0xcfff |           |           |        |                  |
| 0xd000 | VRAM      | [26x300]  |        |                  |
|        |           |           |        |                  |
| 0xffff |           |           |        |                  |

## タスク管理

### タスクのセットアップ

`os_task_setup`

- 引数 a0: スタックサイズ, a1: 優先度
- 返値 a0: TCB のアドレス

1. task_top から、tcb連結リストを辿る
2. 末尾に到達したら、

### 優先度とタスクの切り替え

`os_task_next`

現時点での実装では、リストの順が優先度順になっているので、リストを先頭から走査して

1. task_top から、

ready queue と waiting flag による

### タスクの一時停止

`os_task_wait`

### タスクの再開

`os_task_ready`

- 引数 a0: task_id

### タスクの終了

`os_task_exit`

## 割り込み

## メモリ管理

### セットアップ

`os_heap_init`

### メモリ確保

`os_heap_aloc`

### メモリ解放

`os_heap_free`

## メモ

- 割り込みによる時分割実行
- ヒープの実装
- フラグの実装
