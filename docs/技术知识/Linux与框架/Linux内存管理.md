---
title: Linux 内存管理
---

### 

| 接口           | 特点                   | 适用场景                           |
| :------------- | :--------------------- | :--------------------------------- |
| malloc         | 用户态堆，小内存复用   | 通用，大多数应用                   |
| calloc         | malloc + 自动清零      | 数组、结构体需要初始化为0          |
| realloc        | 调整已有内存大小       | 动态扩容（如vector）               |
| mmap (匿名)    | 直接向内核要页，页对齐 | 大块内存（超过128KB）、共享内存    |
| alloca         | 栈上分配，自动释放     | 临时小对象（KB级），函数返回即失效 |
| posix_memalign | 指定对齐字节           | SIMD指令、DMA缓冲区需要特定对齐    |

------

### 

| 接口               | 物理连续性            | 大小限制                   | 适用场景                                    |
| :----------------- | :-------------------- | :------------------------- | :------------------------------------------ |
| kmalloc            | 物理连续              | 不超过几MB（通常4MB）      | 通用小对象，驱动中90%场景                   |
| kcalloc            | 物理连续              | 不超过几MB                 | kmalloc + 自动清零                          |
| krealloc           | 物理连续              | 不超过几MB                 | 调整已分配的内存大小                        |
| kfree              | —                     | —                          | 释放 kmalloc/kcalloc/krealloc               |
| vmalloc            | 虚拟连续，物理不连续  | 大（GB级）                 | 大内存（超过1MB），如加载固件、帧缓冲       |
| vfree              | —                     | —                          | 释放 vmalloc                                |
| get_free_pages     | 物理连续，页对齐      | 2的order次方页（最大几MB） | 需要物理连续且页对齐（部分DMA）             |
| free_pages         | —                     | —                          | 释放 get_free_pages                         |
| alloc_pages        | 物理连续              | 2的order次方页             | 返回 struct page*，更底层操作               |
| __get_free_page    | 物理连续              | 1页（通常4KB）             | 只申请一页内存                              |
| kmem_cache_alloc   | 物理连续              | 固定大小                   | 高频分配固定结构体（如 inode、task_struct） |
| kmem_cache_free    | —                     | —                          | 释放 kmem_cache_alloc                       |
| dma_alloc_coherent | 物理连续 + 缓存一致性 | 中等                       | 硬件DMA，设备和CPU共享内存                  |
| dma_free_coherent  | —                     | —                          | 释放 dma_alloc_coherent                     |
| mempool_alloc      | 依赖底层（kmalloc等） | 取决于池                   | 关键路径，保证分配永不失败                  |
