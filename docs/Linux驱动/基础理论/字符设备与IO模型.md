---
title: 字符设备与 I/O 模型
---

# 字符设备与 I/O 模型

以下接口以 Linux 6.6 为基准。字符设备将用户态的文件操作映射为驱动回调；设备号分配、`cdev` 注册、sysfs 设备对象和 `/dev` 节点属于不同步骤，不能相互替代。

## inode、file 与设备号

`inode->i_rdev` 保存设备号，`inode->i_cdev` 关联字符设备；`struct file` 表示一次打开所引用的文件实例，包含 `f_op`、`f_flags`、`f_pos` 和 `private_data`。`dup` 与 `fork` 可以使多个文件描述符共享同一文件实例，所以 `.release` 并非每次 `close` 都调用，而是在最后的相关文件引用释放时执行。

设备号使用 `dev_t` 表示，通过 `MAJOR`、`MINOR` 和 `MKDEV` 操作。Linux 6.6 的内核编码使用 20 位次设备号，但用户态 `dev_t` 的宽度和编码不应按同样位移硬编码。主设备号用于定位字符设备注册范围，次设备号如何对应实例由驱动设计决定。

```text
open/read/write/ioctl
  → C 库和体系结构系统调用入口
  → VFS 与字符设备分派
  → file_operations
  → 设备私有状态与子系统接口
```

32 位 ARM EABI 的系统调用号通常使用 r7，AArch64 使用 x8；不能把原资料中的“系统调用号统一放入 R0”用于 Linux ABI。

## 注册、发布与失败回滚

| 阶段 | 接口及对应释放 |
| --- | --- |
| 预留设备号 | `alloc_chrdev_region` 或 `register_chrdev_region` / `unregister_chrdev_region` |
| 建立分派 | `cdev_init`、`cdev_add` / `cdev_del` |
| 建立功能类 | `class_create("name")` / `class_destroy`，6.6 不传 `THIS_MODULE` |
| 发布设备对象 | `device_create` / `device_destroy` |
| 设备节点 | 由 devtmpfs、udev/mdev 或显式 mknod 管理，依系统配置而定 |

`register_chrdev` 仍是可用的封装接口，其注销原型包含主设备号和名称；它不是只预留号码，也不等同于 `alloc_chrdev_region`。misc 设备的完整可编译示例见 [驱动与设备模型](./Linux驱动与设备模型.md)。

下面为注册过程片段，调用前 `demo_fops` 已定义，所有状态属于模块生命周期且暂不支持设备热移除。真实驱动应在 `cdev_add` 前初始化所有可被 open 访问的字段；从该步骤起用户就可能通过预先创建的节点进入驱动。

```c
/* Linux 6.6；位于驱动初始化函数内。 */
ret = alloc_chrdev_region(&devno, 0, 1, "demo");
if (ret)
    return ret;
cdev_init(&demo_cdev, &demo_fops);
demo_cdev.owner = THIS_MODULE;
ret = cdev_add(&demo_cdev, devno, 1);
if (ret)
    goto free_number;
demo_class = class_create("demo");
if (IS_ERR(demo_class)) {
    ret = PTR_ERR(demo_class);
    goto del_cdev;
}
demo_device = device_create(demo_class, NULL, devno, NULL, "demo0");
if (IS_ERR(demo_device)) {
    ret = PTR_ERR(demo_device);
    goto destroy_class;
}
return 0;
destroy_class:
class_destroy(demo_class);
del_cdev:
cdev_del(&demo_cdev);
free_number:
unregister_chrdev_region(devno, 1);
return ret;
```

正常退出按发布顺序逆向清理。`cdev_del` 阻止后续打开，但已经打开的 `file` 仍可能进入回调。热拔插设备还需断开标志、私有对象引用计数、唤醒等待者以及停止硬件访问；`.owner=THIS_MODULE` 保护模块代码，不自动保护硬件已经移除后的私有内存。

## file_operations 的接口语义

| 成员 | 语义与边界 |
| --- | --- |
| `owner`、`open`、`release` | 模块引用、打开实例初始化及最后释放 |
| `read`、`write` | 成功返回实际字节数；负值表示错误；read 返回 0 通常表示 EOF |
| `read_iter`、`write_iter` | 使用 `kiocb` 与 `iov_iter`，支持向量/迭代 I/O，不能保证必然更快 |
| `llseek` | 定位文件偏移；流设备通常禁止 seek |
| `unlocked_ioctl`、`compat_ioctl` | 控制命令、32 位应用兼容处理；驱动自己保护共享状态 |
| `poll`、`fasync` | 就绪查询与等待登记、SIGIO 异步通知 |
| `mmap`、`get_unmapped_area` | 映射与地址选择；需保证偏移、长度、权限和映射生命周期 |
| `flush`、`fsync` | 描述符关闭相关处理与显式同步，不能等同于 release |
| `iterate_shared` | 目录迭代，通常不是字符设备的数据接口 |
| `lock`、`flock`、`setlease` | POSIX 记录锁、BSD 文件锁及租约，不能说记录锁已被 flock 替代 |
| `splice_read`、`splice_write` | 管道相关的数据传输，是否复制由具体实现决定 |
| `fallocate`、`show_fdinfo`、`check_flags` | 空间操作、fd 信息与标志检查，按设备需要实现 |

旧笔记中的 `.sendpage` 等成员不能照搬到 6.6；接口以 [Linux 6.6 fs.h](https://github.com/torvalds/linux/blob/v6.6/include/linux/fs.h) 为准。只有设备需要的接口才实现，不能为凑全回调而提供错误的默认成功值。

## 用户访问、短读与 ioctl ABI

`char __user *` 是用户地址标注，不能直接解引用。`copy_to_user`、`copy_from_user` 返回**尚未复制的字节数**；可能触发缺页并睡眠，不能在持自旋锁的临界区执行。发生部分复制后，需要按数据协议决定返回已完成字节数还是错误，不能把所有非零返回当作成功长度。

`loff_t *ppos` 用于文件偏移。只读常量数据可以使用 `simple_read_from_buffer` 自动处理偏移、短读及 EOF。事件设备没有数据时，应根据 `O_NONBLOCK` 返回 `-EAGAIN` 或进入等待；不能返回 0 假装“暂时没数据”。

ioctl 使用 `_IO`、`_IOR`、`_IOW`、`_IOWR` 定义命令。共享结构宜采用固定宽度整数、明确的 padding 和大小，避免裸指针、`long` 或隐含编译器布局进入稳定 ABI。未知命令通常返回 `-ENOTTY`，复制失败返回 `-EFAULT`；兼容层要处理 32/64 位布局差异。

## 等待队列与 poll

`poll_wait` 只将调用者登记到等待队列，驱动 `.poll` 回调应返回当前就绪位，不能自己 sleep。下例为字段和回调片段：设备对象已存入 `file->private_data`，`ready` 表示至少一个待消费事件，`dead` 表示断开；生产者和消费者通过同一自旋锁修改状态。

```c
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/wait.h>

struct event_state {
    wait_queue_head_t wait;
    spinlock_t lock;
    bool ready;
    bool dead;
};

static __poll_t event_poll(struct file *file, poll_table *table)
{
    struct event_state *s = file->private_data;
    unsigned long flags;
    __poll_t mask = 0;

    poll_wait(file, &s->wait, table); /* 先登记，再取状态，避免遗漏唤醒 */
    spin_lock_irqsave(&s->lock, flags);
    if (s->ready)
        mask |= EPOLLIN | EPOLLRDNORM;
    if (s->dead)
        mask |= EPOLLHUP | EPOLLERR;
    spin_unlock_irqrestore(&s->lock, flags);
    return mask;
}
```

生产者在锁内发布数据及 `ready`，解锁后 `wake_up_interruptible`。读者用 `wait_event_interruptible` 循环等待“有数据或已断开”，唤醒后重新在锁内检查并取得事件，再离开自旋锁执行用户复制。多读者、事件积压与用户复制失败均需明确队列策略；单个布尔标志会合并多次事件，需要计数或 FIFO 才能保留全部事件。
