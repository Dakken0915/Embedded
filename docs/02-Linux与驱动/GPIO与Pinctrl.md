---
title: GPIO 与 Pinctrl
---

# GPIO 与 Pinctrl

Pinctrl 负责引脚复用和电气配置，GPIO 负责方向和值；两者分工明确后，设备树中的 pin group 才能被多个设备安全复用。排查 GPIO 不工作时，依次检查 pinmux、方向、有效电平、请求是否成功以及是否被其他驱动占用。

## 实践关注点

- 设备树节点的 `pinctrl-names` 与 `pinctrl-0` 是否匹配驱动选择。
- LED、按键和中断的 active-low 约定是否贯穿硬件、设备树和应用。
- 使用 gpiod 描述符接口表达设备关系，避免在驱动中散落全局 GPIO 编号。
- 中断处理程序只做快速确认，把耗时工作交给 threaded IRQ 或工作队列。

原始 GPIO/Pinctrl 笔记和虚拟控制器示例可在本分类的“原始驱动笔记”中继续查阅。
