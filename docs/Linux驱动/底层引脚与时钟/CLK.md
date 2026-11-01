# CLK

## enable

```C
struct clk {   /* 用户句柄 */
	struct clk_core	*core;    /* 真正的时钟节点 */
	const char *dev_id;       /* clocks = <&clks 156>; */
	const char *con_id;       /* clock-names = "i2c"; */
	unsigned long min_rate;
	unsigned long max_rate;
	struct hlist_node clks_node;
};
```

### clk_core

```C
struct clk_ops {
	unsigned long (*get_rate)(struct clk *clk);
	unsigned long (*round_rate)(struct clk *clk, unsigned long rate);
	int (*set_rate)(struct clk *clk, unsigned long rate);
	int (*enable)(struct clk *clk);
	int (*disable)(struct clk *clk);
    、、、
};
```

### CLK 使能过程

```C
clk_enable(clk)
    clk_enable_lock()          // 拿递归spinlock，关本地中断
        → 保护临界区开始
    clk_core_enable(core)
        core->enable_count++   // 软件引用计数
    clk_enable_unlock()        // 释放递归spinlock
```



```C
static int clk_core_enable(struct clk_core *core)
{
	int ret = 0;

	lockdep_assert_held(&enable_lock);

	if (!core) return 0;

	if (WARN_ON(core->prepare_count == 0))
		return -ESHUTDOWN;

	if (core->enable_count == 0) {
		ret = clk_core_enable(core->parent);  /* 递归开父时钟 */

		if (ret) return ret;

		trace_clk_enable_rcuidle(core);

		if (core->ops->enable)
			ret = core->ops->enable(core->hw);  /* 寄存器使能 */

		trace_clk_enable_complete_rcuidle(core);

		if (ret) {  /* 失败回滚关闭父时钟 */
			clk_core_disable(core->parent);   /* 内部采用计数管理 */
			return ret;
		}
	}

	core->enable_count++;
	return 0;
}
```



## clk_prepare

准备时钟的软件状态，确保后续对时钟的操作（如启用、禁用、频率调整）能安全执行

初始化时钟的软件状态

1. 配置时钟的依赖关系（例如父时钟、子时钟的分层结构）。
2. 分配时钟运行时的资源（如锁、内部数据结构）。
3. 可能涉及频率或分频器的预计算，但不会直接操作硬件。

与`clk_enable()`的区别

1. `clk_prepare()`：仅初始化软件状态，不实际启用硬件时钟信号。
2. `clk_enable()`：实际启用硬件时钟（例如通过写寄存器打开时钟源）。
3. 通常需要先调用clk_prepare()，再调用clk_enable()。

电源管理优化

1. 再系统休眠（suspend）时，`clk_unprepare()`可以释放时钟资源，减少功耗。
2. 在恢复（resume）时，重新调用`clk_prepare()`避免重复初始化。

```C
// 初始化阶段（非原子上下文，例如驱动 probe 函数）

clk_prepare(clk);  // 准备时钟的软件状态

// 运行时（可能处于原子上下文）
clk_enable(clk);   // 实际启用硬件时钟
clk_disable(clk);  // 关闭硬件时钟

// 卸载阶段
clk_unprepare(clk); // 释放时钟资源
```

**为何要区分 prepare 和 enable**

1. 硬件操作延迟：

   `clk_enable()`可能需要操作硬件寄存器，而某些上下文（如原子上下文）不允许睡眠。
   `clk_prepare()`可以在非原子上下文中提前完成可能引发睡眠的操作。

2. 避免重复初始化

   如果时钟需要频繁启用/禁用，`clk_prepare()`只需要调用一次，而`clk_enable()` 、 `clk_disable()`可多次调用。

3. 时钟树管理：

   复杂时钟系统（如多级分频、PLL）需要先通过 `clk_prepare()`建立依赖关系，在通过`clk_enable()`按顺序启用。



## 锁的场景差异

`prepare_lock`：普通 `mutex`，可以休眠，所以不能在中断里调用 `clk_prepare`

`enable_lock`：自研递归 `spinlock`，关本地中断、不可休眠，支持同一任务递归（时钟树递归使能），中断上下文可以调用 `clk_enable`