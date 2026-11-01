---
title: Input 输入子系统
---

# Input 输入子系统

Input 将按键、鼠标、触摸等设备转换为统一事件。本文以 Linux 6.6 的接口说明对象和生命周期，Goodix 与 gpio-keys 的具体旧源码路径需按其 4.9/5.4 版本理解。

## 对象与匹配

`input_dev` 表示设备及能力位图、名称、ID、绝对轴参数等；`input_handler` 提供匹配规则及 connect/disconnect、event/events/filter 等处理；`input_handle` 关联一对已连接设备与 handler。

设备注册时遍历已有 handler，handler 注册时遍历已有设备；能力/ID 匹配后可继续执行 handler 的可选 match，成功后由 connect 建立并注册 handle。evdev 是 handler 之一，不是每个设备都需要另写专用字符设备。

```text
硬件 IRQ / 总线读取
 → input_report_key / input_report_abs / input_event
 → input core：状态、过滤及事件分派
 → input_handle → evdev handler
 → 各打开实例的 evdev_client 缓冲
 → 唤醒 read/poll → /dev/input/eventX
```

每个打开实例有自己的事件消费状态。`input_sync` 上报 `EV_SYN/SYN_REPORT`，标记一批事件结束；它不代表完成磁盘同步。应用遇到 `SYN_DROPPED` 需要按 evdev 协议重新同步状态。

## 注册与上报

下例为 Linux 6.6 的设备管理分配/注册片段，传入有效父设备及输出指针。注册失败时 devm 回收未注册对象；成功后 input core 与 devres 按对应协议管理注销，不再另行 `input_free_device`。

```c
#include <linux/device.h>
#include <linux/input.h>

int register_demo_key(struct device *dev, struct input_dev **out)
{
    struct input_dev *input = devm_input_allocate_device(dev);
    int ret;
    if (!input)
        return -ENOMEM;
    input->name = "demo-key";
    input->id.bustype = BUS_HOST;
    input_set_capability(input, EV_KEY, KEY_ENTER);
    ret = input_register_device(input);
    if (ret)
        return ret;
    *out = input;
    return 0;
}

void report_demo_key(struct input_dev *input, int pressed)
{
    input_report_key(input, KEY_ENTER, !!pressed);
    input_sync(input);
}
```

普通 `input_allocate_device` 则在注册失败前用 `input_free_device`，成功后用 `input_unregister_device`；注销通常还释放设备引用，不能再次 free。所有 IRQ、定时器、work 在设备注销前都须停止并完成同步。

## GPIO 按键与消抖

```dts
#include <dt-bindings/gpio/gpio.h>
#include <dt-bindings/input/input.h>
/ {
    buttons {
        compatible = "gpio-keys";
        user-button {
            label = "user";
            gpios = <&gpio5 1 GPIO_ACTIVE_LOW>;
            linux,code = <KEY_ENTER>;
            debounce-interval = <10>;
            wakeup-source;
        };
    };
};
```

该片段引用板级已定义的 gpio5。已有标准 gpio-keys 驱动时，无需再注册自定义按键字符设备。`autorepeat` 可启用自动重复；GPIO 与纯 IRQ 按键各有 binding，唤醒还涉及硬件电源域和 IRQ wake 支持。

GPIO 方式可在消抖后读取真实电平，从而分别报告按下/释放；仅有脉冲 IRQ 时可能由驱动在延时后合成释放，不能据此判断物理按键已松开。消抖优先使用硬件能力，否则重排定时器/延迟工作；访问会睡眠的 GPIO 时选择工作线程，不在 timer 或 hardirq 中执行 I2C 访问。

## 触摸屏与多点状态

单点触摸可使用 `BTN_TOUCH` 与 `ABS_X/ABS_Y`，设置轴最小/最大范围后上报；QEMU 教学设备中固定地址的 pressure/x/y/clear 寄存器只适用于该仿真硬件，不能用于真实屏幕。

I2C 触摸屏通常在 probe 中取得电源、复位/中断 GPIO、芯片信息，设置坐标轴并注册 input，再注册线程化 IRQ。IRQ 线程读取控制器数据，验证报文长度/触点数量、解析 ID 和坐标，最后完成本帧上报。GPIO 极性与 IRQ 触发类型是不同的 binding flags，不能把 `IRQ_TYPE_EDGE_FALLING` 当作 `irq-gpios` 的 active-low 标志。

```c
/* Linux 6.6；input 已完成 input_mt_init_slots，slot、坐标和数量已经验证。 */
#include <linux/input/mt.h>

void report_demo_touch(struct input_dev *input, unsigned int slot,
                       bool active, int x, int y)
{
    input_mt_slot(input, slot);
    input_mt_report_slot_state(input, MT_TOOL_FINGER, active);
    if (active) {
        input_report_abs(input, ABS_MT_POSITION_X, x);
        input_report_abs(input, ABS_MT_POSITION_Y, y);
    }
    input_mt_sync_frame(input);
    input_sync(input);
}
```

这是单触点的一帧片段，多触点设备要先上报本帧全部槽位，再统一 sync；槽位范围应小于已初始化数量，离开的触点要被释放，避免“粘住”。Goodix 旧工程的私有属性并非全部属于上游 binding，应按目标驱动校验复位时序、IRQ 电平及配置下载流程。
