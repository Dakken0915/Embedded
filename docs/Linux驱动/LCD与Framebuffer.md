---
title: LCD 与 Framebuffer
---

# LCD 与 Framebuffer

本文区分通用显示接口、Linux fbdev 框架，以及原资料中 **IMX6ULL / Linux 4.9 eLCDIF** 的寄存器路径。寄存器说明不是跨芯片驱动代码；新平台还应确认实际采用 fbdev 还是 DRM/KMS。

## 像素、总线与存储格式

RGB565 为 5/6/5 位颜色，RGB555 为 5/5/5，RGB888 有 24 位颜色。24 位有效颜色既可能用三个字节紧凑存储，也可能放在 32 位槽中；不能说“24bpp 总是占 32 位”。显存布局、LCD 数据总线宽度和有效颜色位数必须分别确认。

8080/DBI 类型常通过命令、地址/数据选择及读写选通信号访问显示控制器的 GRAM；具体锁存边沿以屏幕手册为准。RGB/DPI 接口连续发送像素、PCLK、HSYNC、VSYNC、DE，主控通常持续从显存扫描。DSI 为串行显示链路，lane 数、速率、命令/视频模式取决于协议版本和芯片，不能沿用旧资料中“固定一个 lane、500 Mbit/s”的限制。

## 时序参数

```text
htotal = hactive + hfront_porch + hsync_len + hback_porch
vtotal = vactive + vfront_porch + vsync_len + vback_porch
pixelclock ≈ htotal × vtotal × refresh_rate
```

水平方向以像素时钟为单位，垂直方向通常以行为单位。前肩位于有效显示之后、同步之前，后肩位于同步之后、下段有效显示之前；同步极性、数据有效电平、发送和采样边沿均须匹配面板。消隐区为扫描与接口时序留下余量，不应将每类面板的前后肩都解释成唯一固定的“像素充电时间”。

原资料示例的 1024×600、HFP=160、HBP=140、HSYNC=20、VFP=12、VBP=20、VSYNC=3，对应 htotal=1344、vtotal=635；50 MHz 像素时钟得到约 58.6 Hz，不能直接标为 60 Hz。

## fbdev 数据结构与调用链

| 对象/字段 | 职责 |
| --- | --- |
| `fb_info` | fbops、var、fix、screen_base、pseudo_palette 及设备私有状态 |
| `fb_var_screeninfo` | xres/yres、虚拟分辨率、偏移、bits_per_pixel、颜色 bitfield、pixclock 与时序 |
| `fb_fix_screeninfo` | 显存地址/长度、line_length、视觉类型等固定布局信息 |
| `fb_ops` | 模式校验/设置、blank、pan_display、mmap、绘图等回调 |

```text
open/read/ioctl/mmap /dev/fbN
 → fbmem.c 的通用分派
 → registered framebuffer 对象
 → fb_ops 或框架回退实现
 → 控制器/显存
```

传统驱动使用 `framebuffer_alloc` 分配对象，初始化格式、显存、fbops 与硬件后 `register_framebuffer`；失败逆序释放。卸载时先停止扫描和异步操作，再注销并释放。已存在的 fd/mmap 仍需保证生命周期，不能只看到节点消失就释放正在访问的缓冲。

fbdev `pixclock` 常用皮秒单位，不等于 Hz，`clock-frequency` 则通常为 Hz。设备树 `display-timings` 解析到 `display_timing`，再可转为 `videomode`、`fb_videomode` 和 var；不能在这些结构间直接按同名整数盲目复制时钟单位。

## eLCDIF 关键寄存器与字段

以下仅描述原 IMX6ULL DOTCLK 分析范围，字段编码、保留位、复位值与限宽仍以该 SoC 参考手册为准。

| 寄存器 | 与路径相关的字段 |
| --- | --- |
| CTRL | SFTRST、CLKGATE、MASTER、RUN；DOTCLK_MODE/BYPASS_COUNT；LCD_DATABUS_WIDTH 与 WORD_LENGTH；数据 swizzle 与 16/18/24 位格式选择 |
| CTRL1 | BYTE_PACKING_FORMAT 规定有效字节；RESET 控制外部屏复位信号 |
| TRANSFER_COUNT | 高 16 位 V_COUNT 为有效行数，低 16 位 H_COUNT 为有效列数 |
| VDCTRL0 | VSYNC/HSYNC/DE/PCLK 极性、VSYNC 输出/使能、垂直周期和脉宽单位、VSYNC_PULSE_WIDTH、半行相关选择 |
| VDCTRL1 | 垂直总周期 |
| VDCTRL2 | HSYNC_PULSE_WIDTH 与水平总周期 |
| VDCTRL3 | HORIZONTAL_WAIT_CNT=hsync+hback，VERTICAL_WAIT_CNT=vsync+vback |
| VDCTRL4 | DOTCLK 延迟选择、同步信号使能、有效水平像素数 hactive |
| CUR_BUF / NEXT_BUF | 当前/下一帧 DMA 地址，帧边界更新用于切换 |

原笔记将 VDCTRL4 的水平有效像素写为 y 分辨率有误，应为 hactive/xres。数据总线宽度与存储格式的编码可能不同，不能直接把 bpp 数字左移写入字段；W1C/SET/CLR 等寄存器也不能使用普通 C 位域写法替代规定访问。

下面为 eLCDIF 的 TRANSFER_COUNT 写入片段，仅在 MMIO 已映射、外设时钟已启用、控制器按要求处于可配置状态时调用；其他寄存器未配置，不能单靠它点亮屏幕。

```c
#include <linux/io.h>
#include <linux/errno.h>

int elcdif_set_dimensions(void __iomem *base, u32 width, u32 height)
{
    if (!width || !height || width > 0xffff || height > 0xffff)
        return -EINVAL;
    writel((height << 16) | width, base + 0x30);
    return 0;
}
```

原资料通过 `volatile struct` 直接访问 MMIO，正式 Linux 驱动应使用 `__iomem` 与 readl/writel。DMA 地址使用 `dma_addr_t`；若控制器地址寄存器仅 32 位，应正确设置 DMA mask，检查地址可表示性，不能静默截断。

## 引脚、时钟与使能顺序

probe 取得寄存器、pinctrl、背光、电源及 pix/axi 时钟 → 解析并验证显示时序 → 分配 DMA 显存和布局 → 设置像素时钟及总线时钟 → 复位/配置控制器 → 设置格式、时序、CUR/NEXT 地址 → 注册显示接口并按面板时序使能扫描与背光。

`devm_clk_get` 返回错误指针，必须先取出 `PTR_ERR` 再改指针；原资料中先赋 NULL 再取 PTR_ERR 会丢失真实错误。`clk_set_rate` 与 `clk_prepare_enable` 都要检查失败，已经使能的时钟在后续失败或退出时 `clk_disable_unprepare`。仅 devm 获取句柄不自动对称撤销所有 enable。

GPIO 背光与 PWM 背光不能同时占用同一 pad。原板级日志中的 `already requested ... cannot claim` 属于 pinctrl 所有权冲突，应按原理图选择正确控制方式，而不是让两个驱动同时写同一引脚。

## 映射、stride 与双缓冲

应用映射范围受 `fix.smem_len` 约束，行地址按 `line_length` 计算，像素格式按 var 的颜色 bitfield 解释；虚拟分辨率和 x/yoffset 也会影响地址。`width*height*bpp/8` 只是无 padding 的估算，正式分配还需检查乘法溢出与对齐。

双缓冲在非扫描页绘制，利用垂直同步或驱动的翻页能力提交下一页；仅把两个地址写成相同值不构成真正双缓冲。显示扫描必须在 DMA 内存回收前停止，CPU 写入与显示控制器读取还需遵守缓存一致性和内存顺序。
