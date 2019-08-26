# stack_resource

---------------------

## About

- 標準ライブラリのみを利用
- スタック領域を利用するアロケータ



## Compatibility

Supports C++ 17 or higher.

| Compiler | Version                          | Remarks                                                  |
| -------- | -------------------------------- | -------------------------------------------------------- |
| gcc      | 9.1.0 or higher.                 |                                                          |
| clang    | 3.9.1 or higher. ※`experimental` | 9.0.0 でも`experimental`<br />最適化されていない点に注意 |
| msvc     |                                  | 16.1.6 で確認                                            |



## Files

| Name             | Description                      |
| ---------------- | -------------------------------- |
| alignment.h      | アライメント関連のユーティリティ |
| stack_resource.h | スタックを利用する多相アロケータ |



## Namespace hierarchy

| 1    | 2         | 3    |
| ---- | --------- | ---- |
| root | alignment |      |
|      | container | pmr  |



## Note

- v1 

  動的アロケータでよく見られる実装。

  - Pros

    -  

  - Cons

    - 生アドレスへのオフセット記憶領域が必要

    - 0 <= gap < alignment になるため alignment が大きくなると実効サイズが予想し難い

      ※ 静的に確保するため問題が顕著に表れる

      ```
      addr:    raw                   [aligned]           next
      size: ---@---------|------------@---------~--------@---
               |<- gap ->|<- offset ->|<- request size ->|
      
      offset == pointer size
      actual size = gap + offset + request size
      ```

    

- v2

  参考文献1 の Howard Hinnant の手法。

  - Pros

    - コンテナ専用アロケータとなるので v1 に見られるオフセット記憶領域は不要

  - Cons

    - arena-alignment で揃えられることから、msvc では実効サイズに僅かな影響が出る。

      msvc の std::vector\<int\> は 8 => 4 bytes アライメントと二段階でアロケートを行う。

      このため arena-alignment = 8 とせざるを得ない。

    - [cpprefjp - C++日本語リファレンス - std::max_align_t](https://cpprefjp.github.io/reference/cstddef/max_align_t.html)

      > `alignas`指示子により、`std::max_align_t`のアライメントより大きいサイズのアライメントを指定できるかどうかは、実装依存である。

      alignas と std::max_align_t の因果関係に触れられているソースが上記でしか見つけられず。

      確認できる環境では特に見受けられなかったが、実装制限になるため要調査。
    
      ---
    
      [cppreference.com - alignas specifier](https://en.cppreference.com/w/cpp/language/alignas) / [std::max_align_t](https://en.cppreference.com/w/cpp/types/max_align_t) には特に明記なし。
    
      C++14以前の動的アロケーションの`overalign`については散見される。
    
      [Stack Overflow - alignas keyword not respected](https://stackoverflow.com/questions/35365624/alignas-keyword-not-respected)
      [Stack Overflow - Dynamic aligned memory allocation in C++11](https://stackoverflow.com/questions/6973995/dynamic-aligned-memory-allocation-in-c11/16510895#16510895)
      
      [expr.new](http://eel.is/c++draft/expr.new)
      [basic.align](http://eel.is/c++draft/basic.align)
      
      
  
  arena-alignment を大きく設定(例えば16bytes)し arena-alignment 以下の任意のアライメントで
  
  アロケートすると、動的アロケータ(new など)の挙動に近くなる。
  
  

## References

1. [stack_alloc - Howard Hinnant](https://howardhinnant.github.io/stack_alloc.html)
2. [cpprefjp - C++日本語リファレンス - std::pmr::memory_resource](https://cpprefjp.github.io/reference/memory_resource/memory_resource.html)