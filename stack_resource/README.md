stack_resource

---------------------

[stack_alloc](https://howardhinnant.github.io/stack_alloc.html).

[memory_resource](https://cpprefjp.github.io/reference/memory_resource/memory_resource.html).



- 標準ライブラリのみを利用
- スタックを利用するアロケータ



Supports C++ 17 or higher.

| Compiler | Version                          | Remarks                  |
| -------- | -------------------------------- | ------------------------ |
| gcc      | 9.1.0 or higher.                 |                          |
| clang    | 3.9.1 or higher. ※`experimental` | 9.0.0 でも`experimental` |
| msvc     |                                  | 16.1.6 で確認            |

※ `experimental`の clang 実装は最適化されていない



Files

| Name             | Description                      |
| ---------------- | -------------------------------- |
| alignment.h      | アライメント関連のユーティリティ |
| stack_resource.h | スタックを利用する多相アロケータ |



Namespace hierarchy

| 1    | 2         | 3    |
| ---- | --------- | ---- |
| root | alignment |      |
|      | container | pmr  |

