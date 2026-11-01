---
title: Linux 驱动与设备模型
---

# Linux 驱动与设备模型

## 对象与生命周期

`kobject` 提供引用计数和 sysfs 对象基础，`kset` 组织相关对象。`device` 表示设备，`device_driver` 表示驱动，`bus_type` 定义总线匹配与管理规则，`class` 按功能组织用户可见设备。

设备与驱动的匹配方式由总线定义，可包括设备树 `compatible`、ID 表及名称匹配等。不能将 platform 的回退规则当作所有总线的统一规则。匹配成功后调用 `probe()` 建立运行资源，解绑时按依赖顺序清理资源并阻止后续访问。

## 设备树与资源

设备树描述寄存器、IRQ、时钟、GPIO、总线连接和硬件属性，驱动从固件描述或平台资源中取得这些信息。设备树描述的是硬件，不应承载随意的业务流程。

平台驱动可使用 `platform_get_resource()`、`platform_get_irq()` 和相应的设备管理资源接口。GPIO 描述符、时钟和电源接口分别由对应子系统提供，不能将所有资源都替换为手工解析地址。

## 字符设备与系统调用

应用打开设备节点后，VFS 将文件操作转交到注册的 `file_operations`。`open` 可初始化每个打开实例的私有状态，`release` 在文件实例最终释放时清理；`read`、`write`、`unlocked_ioctl` 和 `poll` 分别定义数据、控制和等待语义。

用户地址不能直接当作可安全解引用的内核地址，使用 `copy_to_user()`、`copy_from_user()` 等接口并检查未复制的字节数。`file_operations.owner` 等机制参与模块引用管理，不能简单宣称每个 `.open` 实现都会自动完成所有引用计数。

### 只读字符设备示例

下面采用 Linux 6.6 接口，通过 misc 子系统注册一个只读字符设备，说明模块生命周期与 `read` 的关联。`simple_read_from_buffer()` 根据文件偏移处理短读、用户空间复制和 EOF，避免每次读取都从头重复输出；其实现见 [Linux 6.6 libfs.c](https://github.com/torvalds/linux/blob/v6.6/fs/libfs.c)。

```c
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

static const char deviceMessage[] = "embedded device ready\n";

static ssize_t knowledgeRead(struct file *file, char __user *buffer,
                             size_t count, loff_t *position)
{
    (void)file;
    return simple_read_from_buffer(buffer, count, position,
                                    deviceMessage,
                                    sizeof(deviceMessage) - 1);
}

static const struct file_operations knowledgeOperations = {
    .owner = THIS_MODULE,
    .read = knowledgeRead,
    .llseek = no_llseek
};

static struct miscdevice knowledgeDevice = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "knowledge_demo",
    .fops = &knowledgeOperations,
    .mode = 0444
};

static int __init knowledgeInit(void)
{
    int error = misc_register(&knowledgeDevice);
    if (error) {
        pr_err("knowledge_demo: registration failed: %d\n", error);
        return error;
    }
    pr_info("knowledge_demo: registered\n");
    return 0;
}

static void __exit knowledgeExit(void)
{
    misc_deregister(&knowledgeDevice);
    pr_info("knowledge_demo: unregistered\n");
}

module_init(knowledgeInit);
module_exit(knowledgeExit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Read-only misc device example");
```

将代码保存为 `knowledge_demo.c`，同目录的外部模块 Makefile 可写为：

```makefile
obj-m += knowledge_demo.o
```

在安装了匹配内核构建目录的 Linux 环境中执行以下命令；内核版本与配置须允许模块加载，设备节点由所在系统的设备管理机制建立。

```shell
make -C /lib/modules/$(uname -r)/build M="$PWD" modules
sudo insmod knowledge_demo.ko
cat /dev/knowledge_demo
sudo rmmod knowledge_demo
```

这段代码不绑定实际硬件。平台设备的寄存器、中断和电源管理仍须放入对应驱动的 `probe()`、解绑及电源回调，并处理设备移除时的并发访问。

## GPIO、I2C 与中断

GPIO 控制器驱动提供引脚读写与方向能力，GPIO 消费者通过描述符获取具体引脚，pinctrl 负责复用与电气配置。I2C 子系统区分适配器控制器、客户端设备和设备驱动，传输结果与协议错误需要逐层传播。

设备树应使用 `pinctrl-names` 与 `pinctrl-0` 选择对应引脚状态，并按硬件有效电平设置 GPIO 极性。消费者驱动通过 `devm_gpiod_get()` 等描述符接口获取引脚；逻辑值遵循 active-low 定义，不应再在应用层重复反相。设备资源释放时，devres 可随设备解绑自动释放，但异步工作仍须在解绑路径中停止并等待结束。

硬中断中不能睡眠，较长处理可放到线程化中断或工作队列。自旋锁适合不能睡眠的短临界区，互斥锁适合允许睡眠的任务上下文。中断与任务共享状态时需要匹配的锁和中断屏蔽方式；`volatile` 不提供并发互斥。

## Input 输入子系统

Input 子系统将按键、触摸屏、鼠标等输入设备转换为统一的事件流。设备驱动注册 `input_dev`，声明支持的事件类型与码值，并在状态变化时调用 `input_report_key()`、`input_report_abs()` 等接口，最后用 `input_sync()` 标记一组事件结束。用户态通常通过 evdev handler 读取 `/dev/input/eventX`。

```c
/* Linux 6.x；GPIO 必须可在硬中断上下文读取，input_dev 已注册。 */
#include <linux/gpio/consumer.h>
#include <linux/input.h>
#include <linux/interrupt.h>

struct button_data {
    struct gpio_desc *gpio;
    struct input_dev *input;
};

static irqreturn_t button_irq(int irq, void *data)
{
    struct button_data *button = data;
    int pressed = gpiod_get_value(button->gpio);
    if (pressed < 0)
        return IRQ_NONE;

    input_report_key(button->input, KEY_ENTER, pressed);
    input_sync(button->input);
    return IRQ_HANDLED;
}
```

如果 GPIO 通过 I2C 扩展器读取，或按键需要消抖，应使用线程化中断、定时器或工作队列，在可睡眠上下文读取状态后再上报。消抖时间应与硬件和按键特性对应；不能在硬中断中调用 `msleep()`。解绑时先禁用 IRQ 并取消延迟工作，再注销并释放输入设备，避免异步回调访问已释放的 `input_dev`。

## USB 枚举与驱动匹配

USB 描述符构成层级为设备、配置、接口和端点。设备描述符标识整台设备；配置描述符包含接口集合；接口描述符表示一个功能及其 alternate setting；端点描述符给出方向、传输类型和最大包长。Bulk、Interrupt、Isochronous 传输分别适用于可靠批量传输、周期性小数据及有时限的数据流，控制传输用于枚举和设备控制。

Linux USB 驱动通过 `struct usb_driver` 注册，`id_table` 中的厂商/产品 ID 或接口类匹配项用于绑定接口，`probe()` 获取接口端点并建立 URB 或 usbcore 提供的传输资源，`disconnect()` 停止并回收异步请求。枚举问题应沿供电、速度协商、设备与配置描述符、接口端点和驱动匹配逐层判断；端点方向必须结合主机视角读取，不能只凭设备资料中的收发命名判断。

## 地址空间与虚拟内存

VMA 描述进程虚拟地址区域及其权限等属性。缺页处理可能建立匿名页、读取文件页，也可能因权限或地址非法而失败。AArch64 页表层数和每级位宽取决于页面粒度、虚拟地址宽度和配置，不能离开这些前提直接给出唯一拆分。

`/dev` 在常见 Linux 系统中由 devtmpfs 与用户态设备管理协作维护，但也可采用其他挂载和静态设备节点布局。设备节点是访问接口，不是驱动代码本身。


## 分专题接口

- [字符设备与 I/O 模型](./字符设备与IO模型.md)：设备号、cdev、file_operations、用户复制与 poll。
- [设备树与 Platform 驱动](./设备树与Platform.md)：节点、资源、匹配、probe 和退出。
- [内核内存与地址映射](./内核内存与地址映射.md)、[中断处理](./中断处理.md)、[同步与互斥](./同步与互斥.md)。
- [GPIO](./GPIO子系统.md)、[Pinctrl](./Pinctrl子系统.md)、[I2C](./I2C驱动.md)、[Input](./Input子系统.md)、[USB](./USB驱动.md)。
- [LCD 与 Framebuffer](./LCD与Framebuffer.md)、[V4L2 与 videobuf2](./V4L2驱动.md)。
