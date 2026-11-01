---
title: Pinctrl 子系统
---

# Pinctrl 子系统

Pinctrl 将引脚枚举、功能复用 pinmux、电气配置 pinconf 分离。以下概念以 Linux 6.6 为准，IMX6ULL 调用路径源于旧 4.9/5.4 驱动，路径中的内部函数会随版本调整。

## 控制器端与消费者端对象

| 对象 | 内容及关联 |
| --- | --- |
| `pinctrl_pin_desc` | 控制器内部 pin 编号与名称，编号不要求连续 |
| `pinctrl_desc` | name、pins、npins、owner，及 pctlops/pmxops/confops |
| `pinctrl_dev` | 注册后由核心维护的控制器实例，关联 desc、设备及驱动私有数据 |
| `pinctrl_ops` | 查询 group、解析 DT 为 map、释放 map |
| `pinmux_ops` | 查询 function 与适用 group，通过 `set_mux` 应用复用 |
| `pinconf_ops` | 对单 pin 或 group 设置/读取电气配置 |
| `pinctrl` / `pinctrl_state` | 消费设备的控制句柄及具名状态集合 |
| `pinctrl_map` / `pinctrl_setting` | 外部配置描述及解析后的运行设置 |
| `dev_pin_info` | 驱动核心使用的设备 pinctrl 关联和标准状态信息，不供驱动私自改字段 |

一个 function 可以对应多个 group，一个 state 也可引用多个控制器配置节点。Group 包含一组 pin，不能把 group selector、pin number 与 GPIO offset 当作同一编号。

## 设备树状态

```dts
/* 引脚配置节点由 SoC 的 binding 定义，以下为消费者片段。 */
&i2c1 {
    pinctrl-names = "default", "sleep";
    pinctrl-0 = <&i2c1_default>;
    pinctrl-1 = <&i2c1_sleep>;
    clock-frequency = <100000>;
    status = "okay";
};
```

`pinctrl-N` 对应 `pinctrl-names` 中第 N 个状态。IMX6ULL 的 `fsl,pins` 常用 SoC 宏展开 mux 寄存器、pad 寄存器、输入选择及配置；数值如 `0x18b0` 的含义依 SoC 和 binding，不能作为跨芯片通用的“开漏设置”。I2C 的电气要求还需结合外部上拉与控制器能力。

驱动核心会在 probe 周围处理标准 init/default 状态；睡眠/恢复是否选择 sleep/default 取决于总线、PM helper 和驱动实现，不会因为 DTS 写了名字就自动在任意业务状态切换。

## 解析与应用路径

```text
消费设备取得 pinctrl
 → pinctrl_dt_to_map：遍历 pinctrl-N 引用
 → 控制器 dt_node_to_map：输出 MUX_GROUP / CONFIGS_PIN / CONFIGS_GROUP 等 map
 → add_setting：解析为该消费者各 state 的 setting
 → pinctrl_select_state
 → pinmux_enable_setting → pinmux_ops.set_mux
 → pinconf_apply_setting → pin_config_set / pin_config_group_set
```

map 指向的名称、配置数组等必须在使用期间有效；核心不替驱动修复错误的索引和释放协议。选择 state 可能因所有权冲突或硬件设置失败而返回错误，需要上层停止依赖该状态的操作。

IMX6ULL 的 `imx6ul_pinctrl_probe` 选择 SoC pin 描述，再进入 `imx_pinctrl_probe`：建立 desc、解析 function/group 信息、绑定 `imx_pctrl_ops` / `imx_pmx_ops` / `imx_pinconf_ops`，随后注册控制器。消费者不是直接写 IOMUX 寄存器，而是由该控制器的 set_mux/pinconf 回调执行。

## 显式选择状态

下例为可编译函数片段，传入有效设备、已初始化的输出指针。devm 管理句柄；state 属于该句柄，不单独释放。

```c
#include <linux/device.h>
#include <linux/err.h>
#include <linux/pinctrl/consumer.h>

int select_idle_pins(struct device *dev, struct pinctrl **out)
{
    struct pinctrl *p;
    struct pinctrl_state *idle;
    int ret;
    p = devm_pinctrl_get(dev);
    if (IS_ERR(p))
        return PTR_ERR(p);
    idle = pinctrl_lookup_state(p, "idle");
    if (IS_ERR(idle))
        return PTR_ERR(idle);
    ret = pinctrl_select_state(p, idle);
    if (ret)
        return ret;
    *out = p;
    return 0;
}
```

如没有 idle 状态，应按业务决定使用默认态还是失败，不应忽略错误继续访问硬件。普通 `pinctrl_get` 取得的句柄由 `pinctrl_put` 回收，devm 版本不重复 put。

## 虚拟控制器与 map 生命周期

四 pin 的虚拟控制器可以规定 GPIO 支持 pin0～3，I2C 支持 pin0/1，UART 支持 pin2/3。`get_groups_count/name/pins`、`get_functions_count/name/groups` 必须使用同一组有效 selector，`set_mux` 要校验 function/group 是否兼容；只有 printk 不代表真实硬件已经切换。

自定义 DT 示例中的 `functions`、`groups`、`configs` 平行数组属于该虚拟驱动自己的 binding，不是所有 pinctrl 通用格式。解析时应检查数组长度一致、名称有效、乘法溢出及全部分配/属性读取返回值。

每根 pin 若转换为一条 mux map 与一条 config map，可以采用如下释放片段；与之配套的分配器使用一个 `kcalloc` 分配 map 数组，为每条配置独立分配 configs。函数不释放指向 DT 内部字符串的借用指针。

```c
#include <linux/pinctrl/machine.h>
#include <linux/slab.h>

void demo_free_maps(struct pinctrl_map *map, unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; ++i) {
        if (map[i].type == PIN_MAP_TYPE_CONFIGS_PIN ||
            map[i].type == PIN_MAP_TYPE_CONFIGS_GROUP)
            kfree(map[i].data.configs.configs);
    }
    kfree(map);
}
```

原资料的释放循环对数组内每项执行 `kfree(map)` 并继续递增，会导致非法释放；应先释放各项独立载荷，再对数组首地址只释放一次。部分构造失败时只清理已经初始化的项，且不能让未初始化指针进入释放路径。驱动使用公开 desc/ops 与 `pinctrl_dev_get_drvdata` 等接口，不依赖复制内核私有 `core.h` 访问隐藏结构。

`devm_pinctrl_register` 的返回值要检查错误指针；若使用 register-and-init / enable 分离接口，必须按目标版本完成注册和使能两阶段。

## 调试视图

`/sys/kernel/debug/pinctrl/` 中的 `pins`、`pingroups`、`pinmux-pins`、`pinmux-functions`、`pinconf-pins`、`pinconf-groups` 分别反映 pin、group、所有权和配置。名称、可写能力及文件存在性取决于驱动与内核版本，debugfs 不是稳定用户 ABI。引脚被其他设备占用时，应修正设备树和所有权，而不是绕过子系统直接写寄存器。

完整接口定义见 [Linux 6.6 Pinctrl 文档](https://docs.kernel.org/6.6/driver-api/pin-control.html)。
