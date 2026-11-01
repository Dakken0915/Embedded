---
title: libusb 用户态设备访问
---

# libusb 用户态设备访问

libusb 1.0 提供用户态 USB 枚举、接口认领以及同步/异步传输。Linux 下仍依赖 usbcore 和主机控制器驱动，通过 usbfs 等机制访问设备；“不需要自己编写功能驱动”不等于没有内核驱动，也不等于绕过设备文件权限。

## 设备、配置、接口与端点

```text
libusb_init → get_device_list / open
 → 读取当前配置，遍历 interface 的全部 alternate setting
 → 找到业务目标接口与端点
 → 必要时解绑已有内核接口驱动，claim_interface
 → 传输与处理事件
 → 取消并等待异步请求 → release_interface → close → exit
```

`libusb_get_device_list` 返回 `ssize_t` 数量或负错误值，列表用 `libusb_free_device_list(list, 1)` 释放；若需要在列表释放后保留某个 device，使用引用计数接口或已打开的 handle。配置描述符用 `libusb_free_config_descriptor` 释放，`altsetting[0]` 不保证就是当前或需要的设置。

端点从描述符取得完整 `bEndpointAddress`，不能把 `LIBUSB_ENDPOINT_IN` 常量直接当成非零端点地址。配置值、接口号与 alternate setting 是不同字段；已认领接口需要按要求选择 alternate setting。识别 HID 鼠标不能只检查一个 protocol 值而忽略 class/subclass、Report Descriptor 与当前协议模式。

`libusb_set_auto_detach_kernel_driver` 依平台支持自动 detach/reattach；也可查询 active、显式 detach 并在结束后恢复。必须检查返回值和记录自己做过的操作，不能为尝试访问而无条件让整机键盘鼠标失效。

## 同步传输的返回约定

| 接口 | 返回及数据长度 |
| --- | --- |
| `libusb_control_transfer` | 非负值为实际传输字节数，负值为错误 |
| `libusb_bulk_transfer` | 成功 0，长度写入 transferred；超时也可能有部分数据 |
| `libusb_interrupt_transfer` | 同样通过 transferred 返回长度，端点须为相应类型 |
| 异步 transfer | submit 返回提交状态，实际结果从回调 status/actual_length 获得 |

超时参数通常以毫秒计，0 表示不设传输超时；业务需要退出时不要使用无限等待却没有取消协议。`TIMEOUT`、`NO_DEVICE`、`PIPE`/stall、权限错误和短包要分别处理。USB transfer 不是应用消息边界，是否允许部分消息、如何重发和拼接由设备协议决定。

下面是完整 C11 程序，从参数取得 VID、PID、接口与中断 IN 端点，最多读取 20 次后退出。调用前须已确认设备描述符与协议匹配、当前配置/alternate setting 正确、接口访问权限已配置。程序只打印有效字节，不假设鼠标报告布局。

```c
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

static int parse_number(const char *s, unsigned long max, int *out)
{
    char *end;
    unsigned long value;
    errno = 0;
    value = strtoul(s, &end, 0);
    if (errno || end == s || *end || value > max)
        return -1;
    *out = (int)value;
    return 0;
}

int main(int argc, char **argv)
{
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    int vid, pid, interface, endpoint;
    int rc, result = EXIT_FAILURE;
    int claimed = 0, detached = 0;
    unsigned char buffer[1024];

    if (argc != 5 ||
        parse_number(argv[1], 0xffff, &vid) ||
        parse_number(argv[2], 0xffff, &pid) ||
        parse_number(argv[3], 255, &interface) ||
        parse_number(argv[4], 255, &endpoint) ||
        !(endpoint & LIBUSB_ENDPOINT_IN) || !(endpoint & 0x0f) ||
        (endpoint & 0x70)) {
        fprintf(stderr, "usage: usb_rx VID PID interface interrupt-in-endpoint\n");
        return EXIT_FAILURE;
    }
    rc = libusb_init(&ctx);
    if (rc < 0) return EXIT_FAILURE;
    handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
    if (!handle) goto done;
    rc = libusb_kernel_driver_active(handle, interface);
    if (rc == 1) {
        rc = libusb_detach_kernel_driver(handle, interface);
        if (rc < 0) goto done;
        detached = 1;
    } else if (rc < 0) {
        goto done;
    }
    rc = libusb_claim_interface(handle, interface);
    if (rc < 0) goto done;
    claimed = 1;
    for (unsigned attempt = 0; attempt < 20; ++attempt) {
        int actual = 0;
        rc = libusb_interrupt_transfer(handle, (unsigned char)endpoint,
                                       buffer, sizeof(buffer), &actual, 500);
        if (rc == 0 || rc == LIBUSB_ERROR_TIMEOUT) {
            for (int i = 0; i < actual; ++i)
                printf("%02x ", buffer[i]);
            if (actual) putchar('\n');
            continue;
        }
        fprintf(stderr, "transfer: %s\n", libusb_error_name(rc));
        goto done;
    }
    result = EXIT_SUCCESS;
done:
    if (claimed && libusb_release_interface(handle, interface) < 0)
        result = EXIT_FAILURE;
    if (detached && libusb_attach_kernel_driver(handle, interface) < 0)
        result = EXIT_FAILURE;
    if (handle) libusb_close(handle);
    libusb_exit(ctx);
    return result;
}
```

```bash
cc -std=c11 -Wall -Wextra -Werror usb_rx.c -o usb_rx $(pkg-config --cflags --libs libusb-1.0)
```

相同 VID/PID 的多个设备可能存在，应在实际产品中结合总线位置、序列号等选择；此简化程序只打开第一个匹配设备，参数不替代运行时描述符验证。

## 异步传输、事件循环与取消

`libusb_alloc_transfer` → `libusb_fill_control/bulk/interrupt_transfer` → `libusb_submit_transfer` 构成请求。填充函数只设置对象，不发出传输；成功提交后，buffer、transfer 与 user_data 必须存活到完成回调。

事件线程或循环调用 `libusb_handle_events*` 推进结果分派。完成回调检查 `status` 与 `actual_length`，决定处理、有限重试或在仍允许运行时重新提交；不应在回调中调用会递归处理事件的同步传输。

退出时先设置停止状态，阻止重提交，再 `libusb_cancel_transfer`；取消是异步请求，必须继续处理事件直到全部在途请求回调结束，然后才 `libusb_free_transfer`、释放 buffer、关闭 handle 和 context。提交失败的请求不等待该次完成回调，需单独回收。线程安全不意味着可在另一个线程仍使用 handle 时直接 close。

控制传输的 setup 包与 payload 容量须正确计算，控制请求回调中的 actual_length 不包含 setup 包。等时传输还要检查每个 packet 的 status 和实际长度，不能只看整个 transfer 的状态。生命周期见 [libusb 异步 I/O 官方文档](https://libusb.sourceforge.io/api-1.0/group__libusb__asyncio.html)。

## HID 报告与请求

Boot 键盘常见输入含 modifier、保留字节与 6 个 usage，modifier 各位表示左右 Ctrl/Shift/Alt/GUI；鼠标的位移位宽、符号及 Report ID 依 Report Descriptor。通用鼠标不能固定解析为“8 字节且 X/Y 一定为某个偏移”。

HID `SET_REPORT` 的 `wValue` 高字节是 Report Type，低字节是 Report ID；2 表示 Output Report，不能解释成方向。`wIndex` 为目标接口，`bmRequestType` 决定方向/类型/接收者。键盘 LED 位图也须与该报告描述符一致。内核侧模型见 [USB 驱动](../Linux驱动/总线与外设子系统/USB驱动.md)。
