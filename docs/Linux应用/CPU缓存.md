---
title: CPU 缓存原理与多核一致性
---

# CPU 缓存原理与多核一致性

CPU Cache 用于减少处理器等待主内存的时间，利用时间局部性和空间局部性保存近期使用的数据与指令。它是处理器的硬件机制；Linux 负责内存属性、调度及必要的维护操作。CPU Cache、缓存地址转换的 TLB、Linux 文件页缓存是不同层面的缓存。

## 缓存行与命中

缓存通常以缓存行（cache line）为填充和一致性管理单位。访问普通可缓存内存时，若目标行已在某级缓存且状态、权限满足访问要求，即为该级命中；L1 未命中还可能命中 L2、末级缓存，或从其他核心取得数据，不一定访问 DRAM。

64 字节是常见行大小，但不是所有处理器的固定值。例如 [Cortex-A78 的 L2](https://support.arm.com/documentation/101430/0102/Functional-description/L2-memory-system/About-the-L2-memory-system) 使用 64 字节行。假设某缓存行大小为 64 字节，访问地址 `0x1004` 所属行覆盖 `0x1000`～`0x103f`，按行填充可使后续相邻访问受益。填充可能来自下一级缓存或一致性互连，不能表述成“每次读取都会从内存重新加载 64 字节”。跨行访问可能涉及两行；设备内存、不可缓存映射和特殊指令另有规则。

缓存还维护地址标签（tag）、有效性和一致性状态。组相联缓存允许一组中保存多个候选行；容量有限或地址冲突都可能引起替换。常见未命中包括首次访问、工作集超过容量及映射冲突，硬件预取也会影响观测结果。

## 缓存层级与共享范围

| 层级 | 常见组织 | 平台条件 |
| --- | --- | --- |
| L1 | 最接近执行核心，常拆为指令缓存 L1I 和数据缓存 L1D | 通常属于物理核心，同核 SMT 线程可能共享 |
| L2 | 容量通常比 L1 大、访问延迟更高 | 可以每核私有，也可以由多个核心共享 |
| L3 / LLC | 为较大工作集提供末级缓存，常由一组核心共享 | 可能没有 L3，也可能按簇、芯粒或插槽划分，不保证所有核心共用 |

缓存层数、容量、包含关系和共享范围由具体微架构决定。不能仅凭 Linux 的逻辑 CPU 编号推断物理核心和缓存关系。Linux 中的查看方式及字段解释见 [Arm 缓存层级说明](https://learn.arm.com/learning-paths/servers-and-cloud-computing/memory-subsystem/cache-hierarchy/)。

## 多核缓存一致性

同一物理内存行可以存在于多个核心的缓存中。对参与硬件一致性的普通共享内存，一致性协议协调各副本和写入权限，避免核心继续把失效副本当成有效数据使用。常见协议包括 MESI 及其扩展；协议不是由用户程序逐次广播实现的。

MESI 的稳定状态可以简化为：

| 状态 | 含义 |
| --- | --- |
| Modified | 本缓存持有已修改副本，主内存可能尚未更新 |
| Exclusive | 本缓存独占未修改副本 |
| Shared | 可有多个有效的只读共享副本 |
| Invalid | 本副本无效，不能用来满足正常读取 |

例如 A、B 先共享同一行，A 要写入时需获得写权限，使其他共享副本失效；B 后续读取必须重新取得有效数据。若最新数据仍在 A 的脏行中，可以通过一致性机制提供数据，并非必须先写回 DRAM 再读取。真实协议还存在瞬态、确认消息和不同互连实现；上述状态只是说明模型。[Cortex-A34 文档](https://support.arm.com/documentation/100246/0001/Functional-Description/Cache-Behavior-and-Cache-Protection/Coherency-between-data-caches-with-the-MOESI-protocol?lang=en) 给出了使用 MOESI 的处理器实例。

写回（write-back）缓存可先在缓存内修改脏行，之后在替换或维护等条件下写回；写穿（write-through）策略则向下层传播写入。缓存一致性不要求每次修改都立刻写到主内存，也不表示所有核心在同一时刻完成观察。

## 一致性与线程同步

缓存一致性主要约束同一位置的副本与写入传播；内存顺序讨论多个访问之间允许的先后关系。编译器优化、处理器重排和存储缓冲等都使“某一行一致”不足以保证完整程序正确。Linux 内核中的相关规则见 [内存屏障文档](https://docs.kernel.org/core-api/wrappers/memory-barriers.html)。

用户态 C/C++ 程序仍需用互斥量或语言原子操作建立同步关系。多个线程直接对普通 `counter++` 并发写入存在数据竞争；`volatile` 不能提供原子性或线程间同步。内核的 `READ_ONCE`、`WRITE_ONCE` 和 `smp_*` 接口也不能直接替代用户态语言原子接口。

下面是 C++11、标准线程库示例，Linux 下可用 `c++ -std=c++11 -pthread cache-demo.cpp -o cache-demo` 编译。主线程仅在 `join()` 后读计数，每次操作只是独立计数，因此采用 relaxed 原子加法；它不发布其他数据，不能据此推导任意业务状态已同步。

```cpp
#include <atomic>
#include <iostream>
#include <thread>

int main()
{
    std::atomic<unsigned> counter{0};
    auto increment = [&counter] {
        for (unsigned i = 0; i < 10000; ++i) {
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    };
    std::thread worker(increment);
    increment();
    worker.join();
    const unsigned total = counter.load(std::memory_order_relaxed);
    std::cout << total << '\n';
    return total == 20000 ? 0 : 1;
}
```

该示例说明正确计数，不用于测量缓存行传递或证明某平台的协议；语言层面的数据竞争与同步规则见 [C++ 工作草案](https://eel.is/c++draft/intro.races)。完整线程生命周期与同步接口见 [线程控制](./线程控制.md)。

## 伪共享

不同线程即使修改不同变量，只要变量位于同一缓存行，仍可能反复争夺该行写权限，形成伪共享。它通常是性能问题；若变量各自独立且程序本身没有数据竞争，伪共享不会自动使程序产生数据竞争。

可采用每线程或每核计数后汇总、减少共享写入，或按实际行大小调整对齐与布局。只给结构体起始地址对齐，不一定能让所有成员分别占行；填充还会增加内存占用和工作集。[Linux 伪共享文档](https://docs.kernel.org/kernel-hacking/false-sharing.html) 说明了共享行争用与 `perf c2c` 等分析工具，具体 PMU 支持因平台而异。

## Linux 下查看缓存拓扑

以下只读命令适用于提供 cacheinfo sysfs 接口的 Linux，依赖 shell 与 `cat`；虚拟机和部分平台可能不提供这些属性。`indexN` 是目录索引，层级必须读取 `level`。

```bash
for cache_dir in /sys/devices/system/cpu/cpu0/cache/index*; do
    [ -d "$cache_dir" ] || continue
    printf '%s\n' "$cache_dir"
    for field in level type size coherency_line_size shared_cpu_list; do
        if [ -r "$cache_dir/$field" ]; then
            printf '  %s: ' "$field"
            cat "$cache_dir/$field"
        fi
    done
done
```

`type` 区分 Data、Instruction、Unified，`coherency_line_size` 表示一致性行大小，`shared_cpu_list` 列出共享该缓存的逻辑 CPU。检查其他核心时替换 `cpu0`，不要假设异构核心全部相同。字段定义见 [Linux 6.6 cacheinfo ABI](https://github.com/torvalds/linux/blob/v6.6/Documentation/ABI/testing/sysfs-devices-system-cpu)。

## DMA 与缓存维护边界

CPU 核之间具备一致性，不代表所有 DMA 设备也参与同一一致性域。Linux 驱动须遵循 DMA API：检查 `dma_map_*` 的映射结果，使用正确方向，复用流式映射时按 CPU/设备所有权切换调用 `dma_sync_*`，结束后解除映射。即使使用一致性 DMA 内存，描述符字段的访问顺序仍可能需要屏障，详见 [DMA API 指南](https://docs.kernel.org/core-api/dma-api-howto.html)。

普通用户态线程共享内存不需要自行逐次刷缓存。文件的 `fsync`、`msync` 和文件页缓存管理不等于 CPU Cache 清理；TLB 失效则处理地址转换，相关区别见 [Linux 内存管理](./Linux内存管理.md) 和 [内核 Cache/TLB 接口](https://docs.kernel.org/core-api/cachetlb.html)。
