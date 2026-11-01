---
title: I2C 驱动与传输模型
---

# I2C 驱动与传输模型

本文以 Linux 6.6 为基准。电气与总线协议见 [I2C 接口](../../硬件与接口/I2C.md)；本页讨论控制器、设备驱动及 `i2c-dev` 的软件模型。

## 对象与调用链

| 对象 | 关键内容 |
| --- | --- |
| `i2c_adapter` | 一条逻辑 I2C 总线，包含设备对象、编号 nr、锁及 algorithm；mux 可产生额外逻辑总线 |
| `i2c_algorithm` | `master_xfer`、`smbus_xfer`、`functionality` 等控制器能力与传输回调 |
| `i2c_client` | 设备地址 addr、flags、adapter、dev 及设备名称等 |
| `i2c_driver` | probe/remove、OF/ACPI/ID 匹配及驱动实现 |
| `i2c_msg` | addr、flags、len、buf；一条消息只表示一个方向的数据 |

```text
设备驱动 → i2c_transfer(adapter,msgs,n)
 → I2C core 加锁、能力及限制检查
 → adapter->algo->master_xfer
 → 控制器寄存器、FIFO/DMA、中断与超时
```

`i2c_transfer` 返回完成的**消息数**，不是字节数；返回非负但小于请求消息数也应按不完整传输处理。消息地址不包含左移后的 R/W 位，普通 7 位设备地址不要再乘二。十位地址需相应 flags 和适配器支持。

## 寄存器组合读取

下例读取 8 位寄存器地址对应的一个字节，两条消息由一个 transfer 提交，设备协议需支持这种 repeated START 形式。调用前设备 client 已绑定，函数在允许睡眠的上下文执行。

```c
#include <linux/i2c.h>

int demo_read_register(struct i2c_client *client, u8 reg, u8 *value)
{
    struct i2c_msg msgs[2] = {
        { .addr = client->addr,
          .flags = client->flags & I2C_M_TEN, .len = 1, .buf = &reg },
        { .addr = client->addr,
          .flags = (client->flags & I2C_M_TEN) | I2C_M_RD,
          .len = 1, .buf = value },
    };
    int ret;
    if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
        return -EOPNOTSUPP;
    ret = i2c_transfer(client->adapter, msgs, 2);
    return ret == 2 ? 0 : (ret < 0 ? ret : -EIO);
}
```

拆成独立 write/read 可能在中间产生 STOP，不能假定与组合事务等价。更长寄存器地址、数据端序、自动递增和读副作用都依芯片协议；一个传输失败后也不能无条件重试有副作用的命令。

## SMBus 与能力检查

Linux 建议在设备确实使用相应协议时优先使用 SMBus helper，以兼容纯 SMBus 适配器；不是所有 I2C 设备都能套用 SMBus。调用前检查对应 `I2C_FUNC_*` 标志。

| 操作 | helper 与线上数据语义 |
| --- | --- |
| Quick | `i2c_smbus_write_quick`，地址方向位携带操作，无数据字节 |
| Receive / Send Byte | `read_byte` / `write_byte`，一个数据字节，无单独 command |
| Read / Write Byte Data | `read_byte_data` / `write_byte_data`，command 加一个数据字节 |
| Read / Write Word Data | `read_word_data` / `write_word_data`，SMBus word 低字节先传，芯片未必使用同样端序 |
| Process Call | `i2c_smbus_process_call`，写 word 后读 word |
| SMBus Block | `read_block_data` / `write_block_data`，线上含长度字节，传统块最多 32 个数据字节 |
| I2C Block | `read_i2c_block_data` / `write_i2c_block_data`，长度由调用者指定，线上不额外带 SMBus 长度字节 |
| Block Process Call | `i2c_smbus_block_process_call`，块写后块读 |
| PEC | CRC-8 校验，需设备与适配器/模拟路径支持 |

SMBus 读 helper 返回有符号值，先判断负错误再转换到 u8/u16。原资料中“普通 I2C Block 也先发送长度字节”的说明有误，不能与 SMBus Block 混用。协议细节见 [内核 SMBus 文档](https://docs.kernel.org/6.6/i2c/smbus-protocol.html)。

## 设备实例与绑定

设备树将外设放在实际适配器节点下：

```dts
&i2c1 {
    status = "okay";
    sensor@1e {
        compatible = "liteon,ap3216c";
        reg = <0x1e>;
    };
};
```

compatible 拼写必须与目标驱动 binding 匹配，不可根据厂商名称随意添加连字符。`i2c1` 标签与 Linux 总线编号由板级 aliases/注册决定；节点存在也不代表器件物理在线或型号正确。

Linux 6.6 的 `i2c_driver.probe` 为单 client 参数，remove 为 void；旧笔记中的双参数 probe 与返回 int 的 remove 需要按版本迁移。OF 匹配比较设备节点的兼容信息，ID 匹配比较 client name 与 `id_table`；不能说 client name 直接等于完整 OF compatible 就一定成功。见 [6.6 i2c.h](https://github.com/torvalds/linux/blob/v6.6/include/linux/i2c.h)。

实例可来自设备树、板级 `i2c_register_board_info`、`i2c_new_client_device`、`i2c_new_scanned_device` 或 sysfs `new_device`。旧的 `i2c_new_device` / `i2c_new_probed_device` 名称不能直接用于 6.6。扫描式创建检测到应答也不证明型号正确，probe 仍需按芯片规范识别设备；主动扫描命令可能有副作用。

动态 client 应在拥有它的组件退出时 `i2c_unregister_device`，通过 `i2c_get_adapter` 获取的引用要 `i2c_put_adapter`。固定硬件优先由固件描述，避免重复创建同一地址的实例。

## i2c-dev 与用户态路径

```text
open /dev/i2c-N → i2cdev_open：按 minor 获取 adapter 引用及每文件 client 状态
ioctl I2C_SLAVE：设置该打开实例的默认目标地址，检查占用
ioctl I2C_RDWR：复制消息数组与载荷 → i2c_transfer → 拷回读结果
ioctl I2C_SMBUS：校验协议、方向与数据 → i2c_smbus_xfer
close → 释放每文件 client 状态及 adapter 引用
```

该临时 client 只是每文件访问状态，不等同于向总线注册一个真实 `i2c_client` 设备。`I2C_SLAVE_FORCE` 绕过占用检查并不协调已有设备驱动，可能破坏其协议状态；不能作为普通访问方案。

`I2C_RDWR` 的消息各自带地址，适合组合事务；普通 `read/write` 使用此前选择的地址，不能表达所有 repeated START 模式。i2c-tools 中 `i2cdetect -l` 列出总线，`-F` 查询能力，`i2cget/set` 使用相应 SMBus/I2C 模式，`i2ctransfer` 直接描述消息序列。例如 `i2ctransfer 0 w1@0x50 0x64 r8` 表示向 0x50 写一个偏移字节再读八字节，实际总线、寄存器与设备必须匹配。

## 控制器与传感器的边界

控制器驱动建立 adapter、填入 algorithm/quirks，并管理总线时钟、IRQ、超时、总线恢复及 adapter 注册/注销；功能设备驱动只处理自己的寄存器协议，不直接重配整个控制器。

AP3216C 示例的复位、使能及 ALS/PS 寄存器读取应保留芯片规定的延时、数据无效标志、位掩码和高低字节拼接。MPU6050 还涉及采样配置、FIFO/DMP、比例换算和校准，不能把原工程的裸 read 字节当成已校准物理量。多次传输之间若构成一个设备级事务，还需驱动私有 mutex；总线单次 transfer 锁不保护跨调用设备状态。
