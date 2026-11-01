---
title: FreeRTOS
---

# FreeRTOS

## 任务状态与调度器状态

任务具有就绪、运行、阻塞和挂起等状态。单核抢占式调度器从就绪任务中选择最高优先级任务执行；相同优先级任务是否轮转取决于时间片配置。高优先级任务持续运行且不阻塞时，低优先级任务可能得不到执行机会。

调度器状态与任务状态是两个概念。`xTaskGetSchedulerState()` 返回 `taskSCHEDULER_NOT_STARTED`、`taskSCHEDULER_RUNNING` 或 `taskSCHEDULER_SUSPENDED`，没有 `taskSCHEDULER_STOP` 状态。

## 固定周期延时

`vTaskDelay()` 相对于调用时刻延时，任务执行时间会累积到周期中。`vTaskDelayUntil()` 以保存的上次唤醒时间为基准推进下一周期，适用于周期性采集或控制。

当目标唤醒时间已经到达或过去时，本次调用不阻塞；周期严重超时时可能连续出现不阻塞的调用，直到周期基准追上当前时间。该函数不能补回已经错过的执行时间。

```c
TickType_t lastWake = xTaskGetTickCount();
for (;;) {
    sampleSensor();
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(20));
}
```

## 任务通知

每个通知槽具有通知状态和一个 32 位通知值。通知直接投递给任务，适用于单接收任务的事件和计数同步。发送接口的区别如下。

| 接口 | 调用环境 | 通知值与返回信息 |
| --- | --- | --- |
| `xTaskNotify()` | 任务 | 按 `eAction` 更新通知值 |
| `xTaskNotifyFromISR()` | ISR | 中断版本，能够报告是否唤醒更高优先级任务 |
| `xTaskNotifyGive()` | 任务 | 通知值加一，等价于 `eIncrement` 动作 |
| `vTaskNotifyGiveFromISR()` | ISR | 在中断中递增通知值 |
| `xTaskNotifyAndQuery()` | 任务 | 执行通知动作，并经输出参数返回更新前的通知值 |
| `xTaskNotifyAndQueryFromISR()` | ISR | 查询旧值并发送通知的中断版本 |

`xTaskNotifyAndQuery()` 中的“查询旧值”不代表旧值在接收任务中保持不变。`eSetBits` 按位或，`eIncrement` 递增，`eSetValueWithOverwrite` 覆盖，`eSetValueWithoutOverwrite` 在已有待处理通知时拒绝覆盖，`eNoAction` 只改变通知状态。

接收方可用 `ulTaskNotifyTake()` 实现计数信号量语义，也可用 `xTaskNotifyWait()` 读取通知值并按掩码清位。ISR 使用相应的 `FromISR` 接口，并按移植层约定处理 `pxHigherPriorityTaskWoken` 和退出中断时的调度请求。接口语义见 [FreeRTOS task.h](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/include/task.h)。

### 中断通知任务

以下示例使用通知值作为待处理事件计数。`processSample()` 由应用实现；实际设备中断应先确认并清除中断源，再调用 `signalSampleFromISR()`。必须先成功创建任务，再启用对应外设中断。

```c
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t sampleTaskHandle;
extern void processSample(void);

static void sampleTask(void *argument)
{
    (void)argument;
    for (;;) {
        /* pdFALSE：每次成功取得通知时减一，逐个消费计数。 */
        if (ulTaskNotifyTake(pdFALSE, portMAX_DELAY) > 0) {
            processSample();
        }
    }
}

BaseType_t createSampleTask(void)
{
    return xTaskCreate(sampleTask, "sample", 256, NULL,
                       tskIDLE_PRIORITY + 2, &sampleTaskHandle);
}

void signalSampleFromISR(void)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(sampleTaskHandle, &higherPriorityTaskWoken);
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}
```

示例要求启用任务通知和动态任务创建，优先级及栈容量需满足配置。这里的 256 是原生 FreeRTOS 常用移植中的栈元素数；ESP-IDF 的单位另见 ESP32 专题。`portYIELD_FROM_ISR` 的形式须匹配移植层，ISR 优先级也须处于允许调用内核 API 的范围。通知计数不保存采样数据本身，数据传输仍需配套缓冲区或队列。

## 信号量、互斥量与队列

信号量用于事件同步或资源数量计数；互斥量用于任务间资源所有权保护，并支持优先级继承以缓解优先级反转。互斥量不能在 ISR 中获取或释放。队列用于传递固定大小的数据项；队列只复制指针时，数据缓冲区的所有权和生命周期仍须由应用约定。

下面的队列复制整个结构体；发送成功后，生产者可以继续修改其局部变量，不影响已经入队的数据。代码在任务上下文执行，返回 `pdFALSE` 表示创建、发送或接收失败。

```c
#include "FreeRTOS.h"
#include "queue.h"

typedef struct {
    uint32_t sequence;
    int32_t value;
} Sample;

BaseType_t queueCopyExample(void)
{
    QueueHandle_t queue = xQueueCreate(4, sizeof(Sample));
    if (queue == NULL) {
        return pdFALSE;
    }

    Sample sent = { .sequence = 1, .value = 123 };
    Sample received;
    BaseType_t result = xQueueSend(queue, &sent, 0);
    sent.value = 456;
    if (result == pdPASS) {
        result = xQueueReceive(queue, &received, 0);
        if (result == pdPASS) {
            result = (received.value == 123) ? pdTRUE : pdFALSE;
        }
    }
    vQueueDelete(queue);
    return result;
}
```

## 事件组

事件组用多个标志位表达条件，可等待任一事件或全部事件。历史配置 `configUSE_16_BIT_TICKS=1` 时有 8 个可用事件位，为 0 时有 24 个。支持 `configTICK_TYPE_WIDTH_IN_BITS` 的内核中，16、32、64 位 Tick 分别对应 8、24、56 个可用事件位，其余高位由内核占用。具体配置应以所用内核版本的 [event_groups.h](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/include/event_groups.h) 为准。

## 临界区与中断

单核移植通常维护临界区嵌套计数，匹配的退出操作在嵌套归零时才恢复相应的中断屏蔽状态。临界区中不能阻塞或执行长时间操作。

中断屏蔽范围由架构和移植层决定。例如，使用 BASEPRI 的 Cortex-M 移植通常仍允许更高紧急程度的中断运行，这些中断不能调用 FreeRTOS API，也不能依赖该临界区保护共享数据。挂起调度器只暂停任务切换，不等于屏蔽中断；SMP 平台还需要考虑其他核心的并发访问。

## 栈溢出检测

`configCHECK_FOR_STACK_OVERFLOW=2` 在受支持的移植中启用额外的栈边界检查，检查通常发生在上下文切换附近。它不能保证发现所有越界写。栈高水位、栈填充检查和溢出钩子可用于发现栈空间不足；任务栈与中断栈需要分别考虑。

## 软件定时器

软件定时器回调由定时器服务任务执行，不能执行会阻塞该任务的长时间操作。`xTimerChangePeriod()` 向定时器命令队列提交周期修改；对休眠定时器执行该操作也会使其启动。接口返回成功表示命令已入队，实际状态变更由服务任务处理。

以下定时器复用前面的采样任务，回调只发送通知。调用前须已创建采样任务，调用者处于任务上下文；要求 `configUSE_TIMERS=1`，且 20 ms 可转换为至少一个 Tick。

```c
#include "timers.h"

static TimerHandle_t sampleTimer;

static void sampleTimerCallback(TimerHandle_t timer)
{
    (void)timer;
    xTaskNotifyGive(sampleTaskHandle);
}

BaseType_t startSampleTimer(void)
{
    if (sampleTimer == NULL) {
        sampleTimer = xTimerCreate("sample", pdMS_TO_TICKS(100),
                                  pdTRUE, NULL, sampleTimerCallback);
        if (sampleTimer == NULL) {
            return pdFAIL;
        }
    }
    /* 新建定时器尚未运行；改变周期的命令同时将其启动。 */
    return xTimerChangePeriod(sampleTimer, pdMS_TO_TICKS(20),
                              pdMS_TO_TICKS(100));
}
```

## 抢占、协作与时间片配置

`configUSE_PREEMPTION` 控制抢占式或协作式行为，`configUSE_TIME_SLICING` 控制相应配置下同优先级任务的 Tick 轮转。关闭时间片并不意味着任何情况下同级任务都不会切换：主动让出、阻塞、唤醒及调度路径仍会影响执行顺序。

任务切换需要保存必要寄存器和任务栈指针、选择下一就绪任务并恢复其现场。任务栈保留调用链和栈上局部对象，不能因某时刻只有一个任务运行就让任意抢占式任务共用同一工作栈。Cortex-M 的 SysTick/PendSV 是常见实现，其他架构采用各自 Tick 与软件中断机制。

## 五种堆实现

| 实现 | 分配与释放 | 空闲块组织及适用限制 |
| --- | --- | --- |
| heap_1 | 分配后不回收 | 简单顺序分配，适用于对象不删除的设计 |
| heap_2 | 支持释放 | 按块大小组织的最佳适配风格，不合并相邻空闲块 |
| heap_3 | 包装 C 库 malloc/free | 依赖 C 库的堆、线程支持和实时性特征 |
| heap_4 | 支持释放与合并 | 单个堆区，按地址组织空闲块，可合并相邻块 |
| heap_5 | 支持多段堆区 | 在 heap_4 思路上支持多个非连续区域，须先定义区域 |

heap_2 不是 heap_4 去掉合并后的完全等价实现。算法名称和行为可对照 [FreeRTOS MemMang 源码](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/main/portable/MemMang)。不同实现通常择一加入构建，不能把同名 pvPortMalloc 定义同时链接。

动态任务的 TCB 和栈需要内存。支持静态创建的配置可通过 xTaskCreateStatic 提供存储；静态存储也必须覆盖任务整个生命周期。分配失败钩子、剩余堆与最小剩余堆反映不同信息，空闲总量足够不代表一定存在所需连续块。

## 空闲任务与删除回收

空闲任务在没有更高优先级就绪任务时执行，参与清理由任务自删除等路径遗留的内核分配资源，并可执行空闲钩子。钩子不能阻塞，也不能无条件承担整个系统的健康证明。应用持有的载荷、文件或外设仍由应用负责释放。

## Tickless Idle

无滴答空闲在预期较长空闲时抑制周期 Tick，安排可唤醒的低功耗时基，并在唤醒后补偿内核时间。外设中断可提前唤醒，低功耗计时器到期则唤醒到期任务。它不是简单关闭 SysTick 后执行 WFI：睡眠确认与中断竞态、时钟停启、计时精度和最大抑制时长都由端口处理。

## 优先级反转与锁定

低优先级任务持有高优先级任务需要的互斥量，中优先级任务抢占低优先级任务时，会延长高优先级任务等待，这构成典型优先级反转。继承机制临时提升持锁者的有效优先级，释放后按内核规则恢复；嵌套锁与多个互斥量的具体继承策略依版本而异。

继承不能解决循环等待。任务间共享复杂资源通常应使用互斥量，而不是把“只有任务访问”统一替换成挂起调度器。中断共享数据、多核并发以及需要阻塞的操作分别具有不同保护要求。

## 实时性与 Linux 的比较

实时性是对截止期限和最坏响应的约束，不等于平均运行更快。RTOS 的短临界区和可控内存路径便于分析，但中断风暴、过长高优先级任务、总线争用仍可破坏期限。Linux 的抢占配置、实时调度策略和 PREEMPT_RT 等会改变响应特征，不能笼统称 Linux 完全无法实时。

FreeRTOS 存在 MPU 等保护方案，Linux 也可能运行在不同保护条件的平台；不能仅用“一个任务崩溃必然导致整机崩溃”区分两者。FOTA/SOTA 是升级对象与部署策略，也不是某个内核独占的能力。
