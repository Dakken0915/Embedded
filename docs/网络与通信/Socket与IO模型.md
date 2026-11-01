---
title: Socket 与 I/O 模型
---

# Socket 与 I/O 模型

## TCP 与 UDP

TCP 提供可靠、有序的字节流，不保留应用消息边界。一次 `send()` 不对应一次 `recv()`；接收可能少于请求长度，应用需要循环处理，并以固定长度、长度头或转义分隔符定义帧边界。发送返回值表示已提交的字节数，不表示对端应用已处理。

UDP 保留数据报边界，不保证交付、顺序或去重。普通 IPv4 UDP 数据报的理论最大有效载荷为 65507 字节，但路径 MTU 会限制无需分片的大小。1500 字节 MTU、20 字节 IPv4 头且无选项时为 1472 字节，不能将该值当作所有网络的统一上限。

## TCP 服务端生命周期

服务端通过 `socket()` 创建套接字，`bind()` 绑定本地地址，`listen()` 创建监听队列，再用 `accept()` 获得连接套接字。监听 fd 与已连接 fd 职责不同。TCP 连接通过本地地址和端口、对端地址和端口组成的四元组区分。

`recv()` 返回 0 表示对端有序关闭发送方向；返回负值时按 `errno` 区分中断、暂不可用和实际错误。清理连接时还需移除事件注册并关闭文件描述符。

UDP 通常使用 `sendto()`、`recvfrom()`；服务端绑定地址和端口。UDP 也允许 `connect()` 设置默认对端，但不会产生 TCP 式握手。

## 阻塞与非阻塞

阻塞 `accept()` 在无连接时等待，阻塞 `recv()` 在无可读数据时等待，`send()` 也可能因缓冲区不足阻塞。非阻塞调用在暂不能完成时返回 `EAGAIN` 或 `EWOULDBLOCK`，非阻塞连接还需处理 `EINPROGRESS`。

`select`、`poll`、`epoll` 等待多个 fd 的就绪事件。就绪表示相应操作可取得进展或报告结束、错误，不表示已经存在完整的应用层报文。边沿触发 epoll 通常配合非阻塞 fd，持续处理到暂不可用为止，并维护未发送数据的偏移。

## 阻塞 TCP 的完整收发

以下 Linux C 函数处理短发送、短接收和 `EINTR`。`receiveExact()` 用于上层协议已经确定长度的字段；它返回 1 表示完整接收，0 表示尚未读取任何字节时对端有序关闭，-1 表示错误或报文中途断开。

```c
#include <errno.h>
#include <stddef.h>
#include <sys/socket.h>

int sendAll(int fd, const void *data, size_t length)
{
    const unsigned char *cursor = data;
    size_t offset = 0;
    while (offset < length) {
        ssize_t sent = send(fd, cursor + offset, length - offset,
                            MSG_NOSIGNAL);
        if (sent > 0) {
            offset += (size_t)sent;
        } else if (sent < 0 && errno == EINTR) {
            continue;
        } else {
            if (sent == 0) {
                errno = EIO;
            }
            return -1;
        }
    }
    return 0;
}

int receiveExact(int fd, void *data, size_t length)
{
    unsigned char *cursor = data;
    size_t offset = 0;
    while (offset < length) {
        ssize_t received = recv(fd, cursor + offset, length - offset, 0);
        if (received > 0) {
            offset += (size_t)received;
        } else if (received == 0) {
            if (offset == 0) {
                return 0;
            }
            errno = ECONNRESET;
            return -1;
        } else if (errno != EINTR) {
            return -1;
        }
    }
    return 1;
}
```

调用方须验证协议长度与缓冲区容量。函数适用于阻塞连接套接字，可能一直等待；需要接收期限时须另外设置超时或采用事件循环。`MSG_NOSIGNAL` 避免对端断开时以 SIGPIPE 终止进程，但错误仍由返回值报告。非阻塞模型需将偏移保存到连接状态中，遇到 `EAGAIN` 后等待下一次就绪，不能直接套用上述接口重新发送整帧。

## 端口复用

是否允许多个套接字绑定同一端口与协议、本地地址、套接字选项和系统实现有关。`SO_REUSEADDR` 常用于地址重用，`SO_REUSEPORT` 可支持多监听者分担流量；两者不等价，也不能绕过所有绑定冲突。
