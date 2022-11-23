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

|      |           |
| ---- | --------- |
| 0000 |           |
|      |           |
| 0100 | OS Global |
|      | :         |
| ??   | TCB_A     |
|      | :         |
| ??   | TCB_B     |
|      | :         |
| ??   | TCB_C     |
|      | :         |
| ??   | Heap      |
|      | :         |
|      |           |
|      | :         |
| efff | Stack_C   |
|      | :         |
| f7ff | Stack_B   |
|      | :         |
| ffff | Stack_A   |

## タスクのセットアップ

### TCB

- スタックの位置情報
  - Stack Top
  - Stack Base
- 優先度
  - Priority
  - Base Priority

### os_task_setup

- a0: TCB のアドレス
- a1: スタックサイズ
- a2: 優先度
