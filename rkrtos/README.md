# RKRTOS - リアルタイム OS

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

## タスクの中断と再開

## 作業日誌

### 今後の予定

- コンテキストスイッチ
- 割り込み
- タスク優先度
- セマフォ
- ミューテックス
- ヒープ
  - os_malloc:
  - os_free:
