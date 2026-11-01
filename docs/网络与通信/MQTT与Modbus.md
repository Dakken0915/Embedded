---
title: MQTT 与 Modbus
---

# MQTT 与 Modbus

## MQTT 发布与订阅

MQTT 客户端经 Broker 交换消息。发布者向 Topic 发布载荷，订阅者通过 Topic Filter 接收匹配消息。`+` 匹配一个主题层级，`#` 匹配后续多个层级，发布主题本身不能包含通配符。

| QoS | 交付语义 | 需要考虑的问题 |
| --- | --- | --- |
| 0 | 至多一次 | 无协议级重发保证，可能丢失 |
| 1 | 至少一次 | 可能重复，接收业务需幂等 |
| 2 | 协议范围内恰好一次 | 握手更多，不等于所有业务副作用都自动恰好一次 |

保留消息使后续匹配订阅者获得某主题最近的保留状态。遗嘱用于异常断开时由 Broker 发布预设消息。Keep Alive、重连、订阅恢复和业务序列号分别解决不同问题，不能仅依赖 TCP 连接表示设备业务健康。

ESP-IDF 5.2 中，客户端配置经 `esp_mqtt_client_init()` 创建，注册事件回调后用 `esp_mqtt_client_start()` 启动。连接成功后订阅，数据事件按长度读取主题和载荷；两者不能假定以 `\0` 结尾，大消息还可能拆成多次事件。TLS 连接需要校验服务器证书和身份。接口见 [ESP-MQTT 5.2](https://docs.espressif.com/projects/esp-idf/en/v5.2/esp32/api-reference/protocols/mqtt.html)。

## Modbus 数据模型

Modbus 定义线圈、离散输入、输入寄存器和保持寄存器等数据区。协议地址与设备手册中使用的展示编号须明确换算。寄存器为 16 位；32 位整型和浮点值的跨寄存器字序由设备约定，不能仅按主机端序解释。

## Modbus RTU

RTU 帧通常包含地址、功能码、数据和 CRC16。协议校验使用多项式的反射表示 `0xA001`，CRC 低字节先发送。帧内间隔与帧间静默时间用于判帧，应根据波特率和协议要求实现；不能单凭 UART 一次中断或一次 DMA 回调认定收到了完整帧。

RS-485 是物理层，Modbus RTU 是应用协议。半双工收发切换需要在最后一个停止位实际发送完毕后释放驱动，而不是仅等待 DMA 已搬完数据。

### RTU CRC16 计算

CRC 覆盖地址、功能码和数据，不包含 CRC 字段自身。下列函数返回 16 位校验值；组帧时显式写入低字节、高字节，不通过主机端序隐式决定线缆字节顺序。算法参数见 [Modbus 官方规范目录](https://www.modbus.org/modbus-specifications)中的串行链路规范 V1.02。

```c
#include <stddef.h>
#include <stdint.h>

uint16_t modbusCrc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit) {
            if ((crc & 1u) != 0u) {
                crc = (uint16_t)((crc >> 1) ^ 0xA001u);
            } else {
                crc = (uint16_t)(crc >> 1);
            }
        }
    }
    return crc;
}

/* 从站 1，功能码 03，从地址 0 开始读取 2 个保持寄存器。 */
void buildReadRequest(uint8_t frame[8])
{
    const uint8_t request[6] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x02 };
    for (size_t i = 0; i < sizeof(request); ++i) {
        frame[i] = request[i];
    }
    uint16_t crc = modbusCrc16(frame, sizeof(request));
    frame[6] = (uint8_t)(crc & 0xFFu);  /* 0xC4 */
    frame[7] = (uint8_t)(crc >> 8);     /* 0x0B */
}
```

函数调用方须保证数据指针有效、长度与缓冲区容量一致，`frame` 至少容纳 8 字节。校验通过只表示 CRC 一致，还需要按帧格式检查地址、长度、功能码和事务状态。

## 解析与异常

解析器首先检查长度与边界，再确认地址、功能码、字节数和校验。请求寄存器范围、响应数量和异常码都须验证。多请求并发时需要串行化总线访问或匹配事务，设置超时并限制重试次数。
