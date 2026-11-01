---
title: AT 指令与链路状态机
---

# AT 指令与链路状态机

## 模块与主机的协议边界

带内置 TCP/IP 协议栈的蜂窝模块可由 MCU 通过 AT 指令建立链路；主机仍需解析命令响应、异步通知及数据载荷。模块串口收发与远端 TCP 字节流是不同层次，串口一次 DMA 完成不能当作一条完整响应。

状态可划分为模块同步、SIM 检查、网络注册、数据上下文激活、连接建立、在线和恢复。每个状态定义命令、期望响应、超时时间、重试预算及下一状态。网络注册和数据连接具有不同时间尺度，不应统一采用固定短延时。

## 非阻塞状态推进

以下独立 C11 示例只展示命令级重试状态，不包括真实 UART 或 AT 解析。deadline 使用无符号毫秒计数，要求相邻检查间隔及超时小于半个 uint32_t 计数周期；解析器须将 OK/ERROR 与当前命令关联。

```c
#include <stdbool.h>
#include <stdint.h>
typedef enum { AT_IDLE, AT_WAITING, AT_DONE, AT_FAILED } AtState;
typedef struct {
    AtState state;
    uint32_t started;
    uint32_t timeout;
    unsigned attempts;
} AtCommand;

void beginAtCommand(AtCommand *command, uint32_t now, uint32_t timeout)
{
    command->state = AT_WAITING;
    command->started = now;
    command->timeout = timeout;
    command->attempts = 1;
    /* 调用方在此阶段提交首次命令。 */
}

/* 返回 true 表示调用方需要重新发送同一命令。 */
bool pollAtCommand(AtCommand *command, uint32_t now, bool ok, bool error)
{
    if (command->state != AT_WAITING) {
        return false;
    }
    if (ok) {
        command->state = AT_DONE;
        return false;
    }
    if (error || (uint32_t)(now - command->started) >= command->timeout) {
        if (command->attempts >= 3) {
            command->state = AT_FAILED;
            return false;
        }
        ++command->attempts;
        command->started = now;
        return true;
    }
    return false;
}
```

## 响应解析与恢复

环形缓冲累积串口字节，行解析器识别文本终止符，但二进制载荷可能含同样字节，必须按模块声明的长度模式读取。URC 异步通知可穿插在命令响应中，应独立路由，不能把任意 OK 都认为当前业务成功。

断线后清理旧事务与未确认状态，按有限退避重连；模块长期无响应可进入受控硬件复位。TCP Keep Alive 与应用心跳检测不同故障，业务消息仍需序列号、幂等与完成确认。下载快于 Flash 写入时应靠有界队列和流控形成背压，不能只插入任意固定延时。
