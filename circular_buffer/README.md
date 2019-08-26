# circular_buffer

---------------------

## About

- 標準ライブラリのみを利用
- [Boost.Circular Buffer](https://www.boost.org/doc/libs/1_70_0/doc/html/circular_buffer.html) のような実装
- ヒープ上に固定サイズのバッファを確保する(リサイズはできない)



## Compatibility

Supports C++ 14 or higher.

| Compiler | Version          | Remarks       |
| -------- | ---------------- | ------------- |
| gcc      | 4.9.0 or higher. |               |
| clang    | 3.5.0 or higher. |               |
| msvc     |                  | 16.1.6 で確認 |



## Files

| Name              | Description  |
| ----------------- | ------------ |
| circular_buffer.h | 環状バッファ |



## Namespace hierarchy

| 1    | 2         | 3    |
| ---- | --------- | ---- |
| root | container |      |
|      |           | pmr  |

`container::circular_buffer<T, Allocator>`

`container::pmr::circular_buffer<T>`



## References

1. [Circular buffer](https://en.wikipedia.org/wiki/Circular_buffer)

