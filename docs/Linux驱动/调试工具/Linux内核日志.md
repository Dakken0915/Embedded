---
title: Linux 内核日志 printk
---

# Linux 内核日志

`printk` 按日志级别输出内核消息。级别数值越小，优先级越高。

```C
#define KERN_SOH      "\001"
#define KERN_EMERG    KERN_SOH "0"  /* 紧急事件，通常表示系统崩溃 */
#define KERN_ALERT    KERN_SOH "1"  /* 必须立即采取行动 */
#define KERN_CRIT     KERN_SOH "2"  /* 严重的软件或硬件错误 */
#define KERN_ERR      KERN_SOH "3"  /* 设备或驱动错误 */
#define KERN_WARNING  KERN_SOH "4"  /* 警告信息 */
#define KERN_NOTICE   KERN_SOH "5"  /* 需要关注的普通信息 */
#define KERN_INFO     KERN_SOH "6"  /* 提示性信息 */
#define KERN_DEBUG    KERN_SOH "7"  /* 调试信息 */
```

## 默认级别与控制台阈值

未显式设置级别时，消息使用当前默认消息级别 `default_message_loglevel`，常见初始值为 4。控制台阈值为 `console_loglevel`，只有消息级别的数值严格小于该阈值时，消息才具备输出到控制台的条件。

在使用这些固定默认宏的部分内核版本中，可以看到如下定义；其他版本可能通过 Kconfig 派生默认值：

```c
#define MESSAGE_LOGLEVEL_DEFAULT 4
#define CONSOLE_LOGLEVEL_DEFAULT 7
```

例如阈值为 7 时，级别 0～6 可以输出，级别 7 的调试日志不会因此输出；若要包含调试级别，应将阈值设为 8。`MESSAGE_LOGLEVEL_DEFAULT`、`CONSOLE_LOGLEVEL_DEFAULT` 及相关配置定义初始值，实际运行值还可由启动参数和运行时设置改变，不能只看头文件中的默认值。

```shell
cat /proc/sys/kernel/printk
# 依次为当前控制台级别、默认消息级别、最低控制台级别、默认控制台级别
dmesg
```

## 与 printf 的区别

`printk` 面向内核日志缓冲区，并使用日志级别控制控制台输出；`printf` 面向用户态标准输出。被控制台阈值过滤的消息仍可进入日志缓冲区，随后通过 `dmesg` 等接口读取。内核格式化还存在限制，例如不支持浮点数格式输出。

```c
printk(KERN_INFO "device ready: %d\n", id);
pr_err("device initialization failed: %d\n", ret);
```

驱动中可使用 `dev_err()`、`dev_info()` 关联设备上下文。官方定义见 [Linux 内核 printk 文档](https://www.kernel.org/doc/html/latest/core-api/printk-basics.html)。
