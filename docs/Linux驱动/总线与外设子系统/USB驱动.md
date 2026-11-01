---
title: USB 驱动与 URB
---

# USB 驱动与 URB

本文以 Linux 6.6 主机侧 API 为基准。USB 用户态访问另见 [libusb 设备访问](../../Linux应用/libusb设备访问.md)。

## 总线、枚举与描述符

主机控制器驱动 HCD 负责根集线器与传输调度，usbcore 负责枚举及设备模型，接口驱动处理具体功能。拓扑由主机、Hub 和设备组成；端点是逻辑通信对象，pipe 由设备、端点号、方向及传输类型构造。

| 描述符 | 主要字段及边界 |
| --- | --- |
| Device | USB 版本、VID/PID、EP0 最大包长、配置数量；类可定义在接口层 |
| Configuration | 总长度、接口数、供电属性与配置值；不是接口编号 |
| Interface | interface number、alternate setting、class/subclass/protocol、端点数 |
| Endpoint | 地址、方向、类型、最大包长、轮询间隔；按当前 alternate setting 解释 |
| String / 类描述符 | 字符串及 HID、UVC 等类特定数据 |

`bEndpointAddress` bit7 表示 IN/OUT，方向以主机视角解释。`wMaxPacketSize` 不等于业务消息长度；高速周期端点还含额外事务信息，需使用接口 helper 正确解析。`bInterval` 的单位和编码随速度/类型变化，不能统一当作毫秒。

枚举涉及端口上电/复位、速度识别、默认地址控制传输、分配地址、读取设备和配置/字符串描述符、选择配置等；具体请求顺序及读取长度由协议、HCD 与设备行为决定。USB 2.0 的低/全/高速分别为 1.5/12/480 Mbit/s，不能将其电气编码推广到 USB 3.x。

USB 2.0 的包可分为 Token、Data、Handshake 等，PID 带类型及互补校验；一次 transfer 可以包含多次 transaction。控制传输由 setup、可选 data、status 阶段组成；NRZI、bit stuffing、SYNC/EOP 与 CRC 属于底层链路约束，不由普通功能驱动手工构造。

## 传输类型

| 类型 | 调度与错误语义 |
| --- | --- |
| Control | 配置与控制，EP0 是标准枚举入口 |
| Bulk | 有错误检测/重试，利用可用带宽，不保证固定服务时延 |
| Interrupt | 主机按周期调度，不表示设备直接触发 CPU 中断 |
| Isochronous | 为时限数据保留周期资源，不以重传保障每包正确 |

USB 驱动通常绑定 `usb_interface`，通过 `usb_driver.id_table` 的 VID/PID 或接口类匹配。复合设备可由不同接口驱动分别处理；不能将同一设备的全部端点都认领到一个接口。

## URB 的字段与所有权

| 字段 | 含义 |
| --- | --- |
| `dev`、`pipe` | 目标设备与端点通信路径 |
| `transfer_buffer`、`transfer_buffer_length` | CPU 缓冲及容量 |
| `transfer_dma`、`transfer_flags` | DMA 映射相关状态 |
| `complete`、`context` | 完成回调与私有对象 |
| `status`、`actual_length` | 完成状态、实际有效字节数 |
| `interval`、等时包描述符 | 周期及各等时包的长度/状态 |

`usb_alloc_urb` 分配请求，`usb_fill_control/bulk/int_urb` 填入必要信息。成功 `usb_submit_urb` 后，请求交由 usbcore/HCD，不能同时修改在途载荷或复用请求；每次成功提交对应一次完成回调，包含成功、失败或取消。提交失败则没有该次完成通知，驱动必须自行处理失败。

同步 `usb_control_msg`、`usb_bulk_msg`、`usb_interrupt_msg` 等在调用线程等待，不用于原子上下文；断开路径仍需等待使用它们的线程退出。同步 helper 使用内部 URB，调用者不能用自己的 URB 指针取消它。

## 单次中断请求的准备与提交

以下片段准备一次输入请求。调用方持有有效 USB 设备引用，已从当前接口挑出 `usb_endpoint_is_int_in` 的端点；对象和 DMA 缓冲在完成前一直存在。回调只记录结果，连续采集需另行实现受停止协议约束的重提交流程。

```c
#include <linux/usb.h>
#include <linux/completion.h>

struct demo_usb_rx {
    struct usb_device *udev;
    struct urb *urb;
    void *buffer;
    dma_addr_t dma;
    size_t capacity;
    struct completion done;
    int status;
    unsigned int actual;
};

static void demo_usb_complete(struct urb *urb)
{
    struct demo_usb_rx *rx = urb->context;
    rx->status = urb->status;
    rx->actual = urb->actual_length;
    complete(&rx->done); /* 回调之后不再访问 rx */
}

int demo_usb_submit(struct demo_usb_rx *rx,
                    const struct usb_endpoint_descriptor *ep)
{
    if (!usb_endpoint_is_int_in(ep))
        return -EINVAL;
    rx->capacity = usb_endpoint_maxp(ep);
    if (!rx->capacity)
        return -EINVAL;
    init_completion(&rx->done);
    rx->urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!rx->urb)
        return -ENOMEM;
    rx->buffer = usb_alloc_coherent(rx->udev, rx->capacity,
                                    GFP_KERNEL, &rx->dma);
    if (!rx->buffer) {
        usb_free_urb(rx->urb);
        rx->urb = NULL;
        return -ENOMEM;
    }
    usb_fill_int_urb(rx->urb, rx->udev,
        usb_rcvintpipe(rx->udev, usb_endpoint_num(ep)),
        rx->buffer, rx->capacity, demo_usb_complete, rx, ep->bInterval);
    rx->urb->transfer_dma = rx->dma;
    rx->urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
    return usb_submit_urb(rx->urb, GFP_KERNEL);
}

void demo_usb_release(struct demo_usb_rx *rx)
{
    usb_kill_urb(rx->urb);
    if (rx->buffer)
        usb_free_coherent(rx->udev, rx->capacity, rx->buffer, rx->dma);
    usb_free_urb(rx->urb);
    rx->buffer = NULL;
    rx->urb = NULL;
}
```

结构应零初始化，`udev` 由调用者正确设置；准备函数只调用一次，分配成功后无论 submit 是否成功都由 `demo_usb_release` 清理。成功提交时，可等待 completion 并检查 status/actual，随后仍通过 kill 同步确认回调完全退出再释放。该片段不是完整热拔插驱动，提交与 disconnect 必须在外层串行化。

普通可用于 DMA 的 kmalloc 缓冲通常由 USB 核心映射，是否需要 bounce 取决于平台；不能说普通缓冲每次必然复制到另一块 DMA 缓冲。coherent 缓冲配合正确 DMA 地址和 `URB_NO_TRANSFER_DMA_MAP`，释放参数必须与分配一致。

## 取消、断开与 Input 对接

`usb_unlink_urb` 异步请求取消，返回后回调可能仍在运行；`usb_kill_urb` 等待完成与回调结束，可睡眠，不在 hardirq、完成回调或持自旋锁时使用。仅 `usb_free_urb` 不能代替停止在途传输。多 URB 可用 anchor 统一跟踪，disconnect 先阻止新提交，再 kill、等待工作线程、注销上层接口并释放私有状态。

鼠标/键盘驱动还可注册 `input_dev`，在 URB 回调根据实际报告解析 EV_KEY/EV_REL 并同步；终止状态如 `-ENOENT`、`-ECONNRESET`、`-ESHUTDOWN` 不应继续重提交。其他错误需有限恢复，避免完成回调无限刷日志和提交失败循环。

HID 报告由 Report Descriptor 决定，不能把某只鼠标的 8 字节布局当作通用协议。Boot 键盘常见报告为 modifier、保留字节和 6 个 usage；这不限制所有 HID 键盘只能六键，也不能说 usage 224 无法由 8 位表示。Report ID、位宽、符号、滚轮和按键字段均须按描述符解析。

主机侧完整 API 见 [Linux USB 文档](https://docs.kernel.org/6.6/driver-api/usb/usb.html)，URB 概念见 [URB 说明](https://docs.kernel.org/6.6/driver-api/usb/URB.html)。
