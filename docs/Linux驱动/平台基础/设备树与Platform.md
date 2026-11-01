---
title: 设备树与 Platform 驱动
---

# 设备树与 Platform 驱动

本文采用 Linux 6.6。设备树描述硬件拓扑与资源；platform 总线连接缺乏硬件自枚举能力的一类设备与驱动。设备树节点、`device_node`、`platform_device` 是不同表示，不能假定每一个节点都生成 platform 设备。

## DTS、DTB 与属性格式

DTS 是源文件，DTSI 复用公共描述，DTB 是编译后的扁平设备树。引导程序将 DTB 传给内核，早期扫描用于内存等启动信息，后续展开为节点及属性对象；总线枚举代码再按 binding 和节点状态创建设备。

```dts
/dts-v1/;
/ {
    #address-cells = <1>;
    #size-cells = <1>;
    compatible = "example,demo-board";
    model = "Demo board";

    demo: demo@12340000 {
        compatible = "example,register-demo";
        reg = <0x12340000 0x1000>;
        example,channels = <4>;
        status = "okay";
    };
};
```

此例为语法与匹配示意，地址和 compatible 不代表真实硬件。`reg` 由**父节点**的 `#address-cells` 与 `#size-cells` 定义编码；位于 I2C 总线下时通常是设备地址而不是 MMIO 区间。地址翻译还要考虑 `ranges`。字符串使用双引号，cell 数组使用 `<...>`，原始字节串使用 `[...]`，布尔属性可无值。

`compatible` 按具体到通用列出兼容硬件；`model` 用于标识型号，不代替匹配表。`status` 缺失或为 `okay` / `ok` 时通常可用，`disabled` 不枚举；`fail` / `fail-sss` 描述不可用状态。节点名称与 `@unit-address` 应符合总线 binding。

`cpus` 描述处理器，`memory` 描述可用内存，`chosen` 可传递启动参数与控制台选择，`aliases` 为总线编号等提供别名。不能用“i2c1 一定对应 `/dev/i2c-0`”替代实际 aliases 和适配器编号。

独立、无 C 预处理依赖的示意 DTS 可通过 `dtc -I dts -O dtb -o demo.dtb demo.dts` 编译。内核 DTS 常使用 `#include` 与绑定宏，应通过匹配架构的内核 `make dtbs` 构建，并进行 binding 校验；目标范围取决于配置与 Makefile。

## 枚举与匹配路径

```text
DTB → device_node/property
    → platform/AMBA 等总线的枚举逻辑
    → 控制器驱动初始化
    → I2C client、SPI device 等子设备
```

`simple-bus` 等节点允许继续遍历其子设备，但 AMBA 等有专门对象与枚举规则；CPU、内存、配置组、端点等节点通常不直接变成 platform 设备。I2C/SPI 控制器下的外设由各自总线核心创建。

Linux 6.6 的 `platform_match` 先处理 `driver_override`，然后尝试 OF、ACPI、platform ID，最后按相应条件进行名称匹配。存在 `id_table` 时其查找结果可直接决定是否匹配，不能假定失败后总会再比较驱动名。`of_match_table` 位于 `.driver` 中，platform 的 `id_table` 位于 `struct platform_driver` 本身。见 [platform.c](https://github.com/torvalds/linux/blob/v6.6/drivers/base/platform.c)。

注册设备时，驱动核心为它寻找已有驱动；注册驱动时，为它寻找尚未绑定的设备。匹配只决定可尝试绑定，`probe` 仍可失败。供应者尚未就绪时返回 `-EPROBE_DEFER`，不要转换成永久的 `-EINVAL`。

## 资源、节点引用与查询接口

`platform_device` 通过内嵌 `device` 关联 OF 节点和生命周期，`resource` 数组记录地址等资源。`struct resource` 中 `start`、`end` 为包含端点的区间，长度用 `resource_size` 获取，`flags` 描述资源类型；IRQ 的映射最好使用 `platform_get_irq`，不能直接拿设备树 cell 当 Linux IRQ。

| 需求 | 常见接口与返回约定 |
| --- | --- |
| 寄存器资源 | `platform_get_resource` 返回指针或 NULL；`devm_platform_ioremap_resource` 返回映射或错误指针 |
| IRQ | `platform_get_irq`、`of_irq_get`，负值为错误，可能需要 deferred probe |
| 普通属性 | `device_property_read_u32`、`of_property_read_u32/array/string`，检查错误和长度 |
| 布尔/字符串列表 | `of_property_read_bool`、`of_property_match_string`、`of_property_read_string_index` |
| 节点查找 | `of_find_node_by_path/name/type`、`of_find_compatible_node`、`of_get_parent` |
| Phandle | `of_parse_phandle`、`of_parse_phandle_with_args`、`of_count_phandle_with_args` |
| 地址翻译 | `of_address_to_resource`、`of_translate_address`，处理总线地址规则 |
| GPIO/时钟/DMA | `devm_gpiod_get`、`devm_clk_get`、`dma_request_chan`，优先使用相应子系统 |
| 原始属性 | `of_find_property` 返回属性与字节长度；需自行处理端序和格式 |

查找或解析获得的有引用节点用完后 `of_node_put`。`for_each_child_of_node` 正常迭代会处理前一节点引用，提前 `break`/返回时仍须释放当前节点；设备自身的 `dev->of_node` 通常是借用指针，不应无条件 put。`of_device_is_big_endian` 判断设备的端序属性，不是在查询 CPU 端序。

## Platform 驱动骨架

下面是完整的 Linux 6.6 模块骨架，只取得并报告资源，不擅自写寄存器。需要内核提供 OF 与 platform 支持、匹配的真实硬件描述和有效资源；将 `reg_demo.c` 配置为外部模块即可编译。驱动实例使用每设备数据，避免以全局变量混合多个设备。

```c
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

struct reg_demo {
    void __iomem *base;
    u32 channels;
};

static int reg_demo_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;
    struct reg_demo *data;
    int ret;

    data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;
    data->base = devm_platform_ioremap_resource(pdev, 0);
    if (IS_ERR(data->base))
        return dev_err_probe(dev, PTR_ERR(data->base), "register mapping\n");
    ret = of_property_read_u32(dev->of_node, "example,channels", &data->channels);
    if (ret)
        return dev_err_probe(dev, ret, "missing channel count\n");
    if (!data->channels || data->channels > 32)
        return -EINVAL;
    platform_set_drvdata(pdev, data);
    dev_info(dev, "mapped %u channels\n", data->channels);
    return 0;
}

static const struct of_device_id reg_demo_ids[] = {
    { .compatible = "example,register-demo" },
    { }
};
MODULE_DEVICE_TABLE(of, reg_demo_ids);

static struct platform_driver reg_demo_driver = {
    .probe = reg_demo_probe,
    .driver = {
        .name = "reg_demo",
        .of_match_table = reg_demo_ids,
    },
};
module_platform_driver(reg_demo_driver);
MODULE_LICENSE("GPL");
```

没有异步活动时，以上设备管理资源可由 devres 回收。加入 DMA、IRQ、定时器、工作队列或字符设备后，解绑路径必须先阻止新操作、停止硬件并同步回调，再释放底层资源。Linux 6.6 可用 `.remove_new` 接收 void 回调；其他内核版本应检查真实头文件，不能盲目改函数返回类型。

`module_init` 在加载时注册驱动，`probe` 在设备绑定时运行；二者不是一次一一对应。内建驱动的初始化由启动 initcall 执行。模块退出注销驱动，设备 unbind 也可触发 remove，并不要求模块必须卸载。`EXPORT_SYMBOL` / `EXPORT_SYMBOL_GPL` 导出模块间符号，不是用户态 ABI。
