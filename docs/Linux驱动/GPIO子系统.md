---
title: GPIO 子系统
---

# GPIO 子系统

本文使用 Linux 6.6 的描述符接口。GPIO controller 提供引脚方向与电平操作，GPIOLIB 管理资源及消费者映射，pinctrl 处理复用和电气配置；驱动某颗 LED 是消费者，提供整组 GPIO 的 `gpio_chip` 才是控制器驱动。

## 数据结构与调用路径

| 对象 | 职责与关键内容 |
| --- | --- |
| `gpio_chip` | 驱动提供的回调与能力：`label`、`parent`、`owner`、`base`、`ngpio`、`can_sleep`、direction/get/set 等 |
| `gpio_device` | 核心层维护的控制器对象，关联 chip、描述符集合和用户接口生命周期 |
| `gpio_desc` | 单根引脚的描述符，关联控制器、局部 offset、极性和占用等状态 |

```text
消费者 gpiod_get(dev,"led",...)
  → 固件映射 led-gpios → 控制器引用、局部 offset 与 flags
  → 获得 gpio_desc 并申请引脚
  → gpiod_set_value[_cansleep]
  → 逻辑电平按 active-low 转换
  → gpio_chip.set(chip, offset, physical_value)
```

旧 IMX6ULL Linux 4.9 的 `mxc_gpio_probe` 通过 `mxc_gpio_port` 保存寄存器与 gpio_chip，并借助 generic GPIO/MMIO helper 提供读写与方向回调，再注册控制器及 IRQ 能力。不同版本可能使用嵌入的 `bgpio_chip` 或重构后的 helper，不应将结构布局固定到所有版本。

## 设备树与消费者

```dts
/* 引用已存在的 GPIO 控制器，GPIO_ACTIVE_LOW 来自 binding 头文件。 */
status-led {
    compatible = "example,status-led";
    led-gpios = <&gpio1 10 GPIO_ACTIVE_LOW>;
};
```

`#gpio-cells=<2>` 的控制器通常将参数解释为 offset 与 flags，最终由控制器 binding 定义。这里 10 是 gpio1 组内偏移，不是全局 GPIO 号；逻辑值 1 表示有效状态，active-low 时对应物理低电平，不要重复反相。

```c
/* Linux 6.6；在消费者 probe 中调用。 */
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>

int setup_demo_led(struct platform_device *pdev)
{
    struct gpio_desc *led;
    led = devm_gpiod_get(&pdev->dev, "led", GPIOD_OUT_LOW);
    if (IS_ERR(led))
        return dev_err_probe(&pdev->dev, PTR_ERR(led), "get LED GPIO\n");
    gpiod_set_value_cansleep(led, 1);
    return 0;
}
```

`GPIOD_OUT_LOW` 指逻辑低初值。设置方向时同步提供初值有助于减少毛刺，但仍受硬件能力约束。`get_index`、`get_array` 可管理多引脚，方向切换使用 `gpiod_direction_input/output`；显式取得的描述符使用 `gpiod_put`，devm 版本通常由设备资源管理，不混用 `gpio_free`。optional 接口还可能返回 NULL，错误指针仍须检查。

GPIO 扩展器可经 I2C/SPI 访问，`can_sleep` 为真时应使用 `_cansleep` API，在线程化 IRQ 或工作队列读取。GPIO 能用于输入不代表必然能映射 IRQ，`gpiod_to_irq` 也要检查负值。

## 虚拟控制器的核心实现

下面为四线虚拟 GPIO 的数据与回调片段，演示多实例数据和锁；不访问真实硬件。上下文为 Linux 6.6 非 RT，输入值在示例中由已保存的值模拟。

```c
#include <linux/bitops.h>
#include <linux/gpio/driver.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

struct virtual_gpio {
    struct gpio_chip chip;
    spinlock_t lock;
    unsigned long values;
    unsigned long outputs;
};

static int vg_get(struct gpio_chip *gc, unsigned int offset)
{
    struct virtual_gpio *v = gpiochip_get_data(gc);
    unsigned long flags;
    int value;
    spin_lock_irqsave(&v->lock, flags);
    value = !!(v->values & BIT(offset));
    spin_unlock_irqrestore(&v->lock, flags);
    return value;
}

static void vg_set(struct gpio_chip *gc, unsigned int offset, int value)
{
    struct virtual_gpio *v = gpiochip_get_data(gc);
    unsigned long flags;
    spin_lock_irqsave(&v->lock, flags);
    if (value)
        v->values |= BIT(offset);
    else
        v->values &= ~BIT(offset);
    spin_unlock_irqrestore(&v->lock, flags);
}

static int vg_input(struct gpio_chip *gc, unsigned int offset)
{
    struct virtual_gpio *v = gpiochip_get_data(gc);
    unsigned long flags;
    spin_lock_irqsave(&v->lock, flags);
    v->outputs &= ~BIT(offset);
    spin_unlock_irqrestore(&v->lock, flags);
    return 0;
}

static int vg_output(struct gpio_chip *gc, unsigned int offset, int value)
{
    struct virtual_gpio *v = gpiochip_get_data(gc);
    unsigned long flags;
    spin_lock_irqsave(&v->lock, flags);
    if (value)
        v->values |= BIT(offset);
    else
        v->values &= ~BIT(offset);
    v->outputs |= BIT(offset);
    spin_unlock_irqrestore(&v->lock, flags);
    return 0;
}

static int vg_probe(struct platform_device *pdev)
{
    struct virtual_gpio *v;
    v = devm_kzalloc(&pdev->dev, sizeof(*v), GFP_KERNEL);
    if (!v)
        return -ENOMEM;
    spin_lock_init(&v->lock);
    v->chip.label = dev_name(&pdev->dev);
    v->chip.parent = &pdev->dev;
    v->chip.owner = THIS_MODULE;
    v->chip.base = -1;
    v->chip.ngpio = 4;
    v->chip.get = vg_get;
    v->chip.set = vg_set;
    v->chip.direction_input = vg_input;
    v->chip.direction_output = vg_output;
    return devm_gpiochip_add_data(&pdev->dev, &v->chip, v);
}
```

GPIOLIB 验证调用 offset 在 `ngpio` 范围内；若从外部属性动态读取 ngpios，还要在注册前验证非零、实现上限和位图容量。此片段需配套 platform 匹配表及模块注册；固定四线不提供 IRQ。原虚拟驱动中的全局值、漏检分配与注册错误、direction_output 忽略初值等问题不能照搬。

## GPIO 与 Pinctrl 的联系

GPIO offset 与 pinctrl pin 编号未必相同，`gpio-ranges` 或相应映射 API 建立二者联系；GPIO 请求可以通过 generic request/free helper 进入 pinctrl 的 GPIO 申请路径，协调复用所有权。简单地设置 GPIO 输出不能替代 UART/I2C 的 pinmux 配置。

用户态优先使用 `/dev/gpiochipX` 的 GPIO character device 接口和 libgpiod。旧 `/sys/class/gpio/export` 接口已弃用，全局 base 动态分配，不能硬编码 `bank*32+offset`；即使历史 sysfs 也存在 edge 事件等待能力，不能说“涉及中断就必须自己写内核驱动”。`/sys/kernel/debug/gpio` 可显示占用，需相应 debugfs 配置与权限。

接口约束见 [GPIO consumer](https://docs.kernel.org/6.6/driver-api/gpio/consumer.html)；复用状态见 [Pinctrl 子系统](./Pinctrl子系统.md)。
