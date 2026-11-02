---
title: FreeRTOS
---

# FreeRTOS

本文以原生 FreeRTOS Kernel V11.1.0 的单核配置为主，Cortex-M4F 示例另标明移植条件；SMP、ESP-IDF 和 MPU 端口需要按各自实现核对。

## 相比裸机增加的机制

FreeRTOS 在应用与硬件之间提供任务调度与上下文切换、任务间通信和同步、延时与事件阻塞，以及可选的动态内存分配接口。裸机也可以自行实现这些能力，区别是 RTOS 提供统一的内核机制；动态分配并非启用 RTOS 的必要条件，任务和同步对象也可静态创建。

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

| 比较项 | 二值/计数信号量 | 互斥量 |
| --- | --- | --- |
| 主要用途 | 事件通知、可用资源计数 | 任务间独占访问共享资源 |
| 所有权 | 不记录持有任务，允许一个任务或 ISR 发出、另一个任务取得 | 记录持有任务，必须由成功取得它的任务释放 |
| 优先级继承 | 不提供 | 提供基本继承机制，不能据此保证不存在优先级反转或死锁 |
| ISR 使用 | 仅使用适用的 FromISR 接口，不得阻塞 | 不得获取或释放 |

二值信号量也能限制同时进入的任务数，但缺少互斥量的所有权与优先级继承语义；多个事件在二值信号量已满时还可能合并。普通互斥量不可重复获取；需要递归锁时使用对应创建和递归 take/give 接口，并配对释放。实现可核对 [V11.1.0 queue.c](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/queue.c)。

### 共享变量与竞态

多个任务可以访问同一个变量，是否安全取决于访问方式与同步协议，并非“两个线程操作变量一定不行”。例如 `counter++` 通常包含读取、加一和写回：两个任务都读到 5，再分别写回 6，会丢失一次更新；即便单次对齐读写在某 CPU 上不可分割，也不能使整个读改写或多个字段的不变量原子化。`volatile` 不提供互斥、原子性或线程同步。

下面是 V11.1.0、C11 任务上下文片段，依赖 `configUSE_MUTEXES=1`、动态创建和 `semphr.h`。必须在启动使用者任务前成功初始化，全部读写都经同一互斥量；锁存活至所有使用者停止之后，才能由管理任务删除。计数使用无符号类型，达到上限后按无符号规则回绕。

```c
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t counterMutex;
static unsigned counter;

BaseType_t initCounter(void) /* 仅调用一次，失败时不得启动使用者 */
{
    counterMutex = xSemaphoreCreateMutex();
    return counterMutex != NULL ? pdPASS : pdFAIL;
}

BaseType_t incrementCounter(unsigned *snapshot, TickType_t wait)
{
    if (snapshot == NULL || counterMutex == NULL ||
        xSemaphoreTake(counterMutex, wait) != pdTRUE) {
        return pdFAIL;
    }
    ++counter;
    *snapshot = counter;
    return xSemaphoreGive(counterMutex);
}
```

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

| 场景 | 保护方式与条件 |
| --- | --- |
| 单核、任务与允许被屏蔽的 ISR 共享少量状态 | 可用短临界区，保证相关中断确实在屏蔽范围内；ISR 使用对应中断版本的进入/退出接口并恢复原屏蔽值 |
| 只有任务访问，操作可能需要等待资源 | 通常使用互斥量，允许其他任务运行，避免长时间禁止调度 |
| 单核、仅任务访问的短操作，且必须保持中断响应 | 可以在满足 API 限制时挂起调度器；它会推迟所有任务切换，包括高优先级任务 |
| 更高紧急程度 ISR 或其他 CPU 核也访问 | 不能只靠本核临界区或挂起调度器，需要该平台允许的原子操作、跨核锁或数据交接协议 |

`vTaskSuspendAll()` 与 `xTaskResumeAll()` 必须配对，嵌套时要全部恢复；期间中断仍可响应，Tick 与唤醒等内核处理可能被挂起到恢复时完成。不要在其中延时、等待信号量或调用可能引发调度的 API；具体限制按所用接口说明核对。临界区内也不得阻塞或进行耗时 I/O，不能把两种机制作为长事务锁。见 [xTaskResumeAll](https://www.freertos.org/Documentation/02-Kernel/04-API-references/04-RTOS-kernel-control/06-xTaskResumeAll)。

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

| 调度方式 | PREEMPTION | TIME_SLICING | 行为 |
| --- | --- | --- | --- |
| 抢占式与时间片轮转 | 1 | 1 | 更高优先级任务就绪时可触发抢占；最高就绪优先级有多个任务时，Tick 可触发同级轮转 |
| 抢占式、关闭 Tick 时间片 | 1 | 0 | 保留高优先级抢占；不因每次 Tick 单纯轮转同级任务，同级仍可能在其他调度点切换 |
| 协作式 | 0 | 任意 | 不因普通就绪事件自动抢占当前任务，通常靠主动让出、阻塞或删除当前任务推进；ISR 仍可打断执行，显式请求切换须按端口行为处理 |

`taskYIELD()` 让调度器重新选择最高优先级就绪任务，不保证让低优先级任务运行；要使低优先级任务取得 CPU，持续运行的高优先级任务通常需要阻塞或挂起。时间片也不保证同级任务得到完全相同的实际 CPU 时间。调度规则见 [FreeRTOS 调度说明](https://github.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/04-Task-scheduling)。

常见 Cortex-M 端口中，SysTick 更新 Tick，并在需要时挂起 PendSV，实际保存/恢复现场通常由 PendSV 完成。任务主动阻塞、让出以及 ISR 唤醒后请求调度也能触发切换，不必等下一个 Tick；任务暂时处于临界区或调度器挂起时，切换还可能推迟。

任务切换需要保存必要寄存器和任务栈指针、选择下一就绪任务并恢复其现场。任务栈保留调用链和栈上局部对象，不能因某时刻只有一个任务运行就让任意抢占式任务共用同一工作栈。Cortex-M 的 SysTick/PendSV 是常见实现，其他架构采用各自 Tick 与软件中断机制。

### 独立任务栈与保存的上下文

标准 FreeRTOS 任务即使暂时未运行，仍可能保留多层调用、返回地址和自动变量，因此各个存活任务必须拥有互不覆盖的栈空间。可以从同一大块静态内存划出多个独立栈，但不能让这些任务使用重叠区域；静态创建接口不会替调用方检测所有重叠。只有改用显式保存状态的状态机、协程等不同执行模型，才可能设计共享工作栈，不能直接套用普通抢占任务模型。

以 [V11.1.0 GCC ARM_CM4F 端口](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/portable/GCC/ARM_CM4F/port.c) 为例：

| 保存者 | 主要内容 |
| --- | --- |
| Cortex-M 异常入口硬件 | 基本帧 R0～R3、R12、任务 LR、PC、xPSR；存在栈对齐及浮点扩展条件 |
| PendSV 移植层软件 | R4～R11、处理程序 LR 中的 EXC_RETURN，并将更新后的 PSP 保存到当前 TCB |
| 浮点上下文 | 启用并使用 FPU 时涉及硬件/惰性压栈的 S0～S15、FPSCR 等，以及端口按条件保存的 S16～S31 |
| 其他端口扩展 | MPU、权限、安全上下文、TLS 等按具体移植和配置保存，不能假定都在同一寄存器帧中 |

这不是“软件自动保存 R4～R11，硬件保存所有其他寄存器”的通用规则；软件部分由端口汇编实现，硬件只保存架构定义的异常状态。常见任务使用 PSP，处理程序使用 MSP，两类栈都要留足空间。更多异常帧说明见 [MCU 启动、异常与缓存](../硬件与接口/MCU启动与异常.md)。

## 五种堆实现

| 实现 | 分配与释放 | 空闲块组织及适用限制 |
| --- | --- | --- |
| heap_1 | 分配后不回收 | 简单顺序分配，适用于对象不删除的设计 |
| heap_2 | 支持释放 | 按块大小组织的最佳适配风格，不合并相邻空闲块 |
| heap_3 | 包装 C 库 malloc/free | 依赖 C 库的堆、线程支持和实时性特征 |
| heap_4 | 支持释放与合并 | 单个堆区，按地址组织空闲块，首次适配并合并相邻块 |
| heap_5 | 支持多段堆区 | 在 heap_4 思路上支持多个非连续区域，须先定义区域 |

heap_2 不是 heap_4 去掉合并后的完全等价实现。算法名称和行为可对照 [FreeRTOS MemMang 源码](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/main/portable/MemMang)。不同实现通常择一加入构建，不能把同名 pvPortMalloc 定义同时链接。

动态任务的 TCB 和栈需要内存。支持静态创建的配置可通过 xTaskCreateStatic 提供存储；静态存储也必须覆盖任务整个生命周期。分配失败钩子、剩余堆与最小剩余堆反映不同信息，空闲总量足够不代表一定存在所需连续块。

`heap_3` 在包装的 malloc/free 外协调任务调度，但不会使 ISR 中分配合法，也不保证其他代码绕过包装直接调用 C 分配器时自动安全。`heap_5` 须在任何内核动态分配之前调用 `vPortDefineHeapRegions()`，区域数组按起始地址递增排列、互不重叠，以 `{ NULL, 0 }` 结束；同一次分配仍不能跨不连续区域拼接。可核对 [V11.1.0 heap_2](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/portable/MemMang/heap_2.c)、[heap_4](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/portable/MemMang/heap_4.c) 与 [heap_5](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/portable/MemMang/heap_5.c)。

## 空闲任务与删除回收

空闲任务在没有更高优先级就绪任务时执行，参与清理由任务自删除等路径遗留的内核分配资源，并可执行空闲钩子。钩子不能阻塞，也不能无条件承担整个系统的健康证明。应用持有的载荷、文件或外设仍由应用负责释放。

单核空闲任务由内核创建，优先级为 `tskIDLE_PRIORITY`（0），应用也可存在同优先级任务。它在循环中处理待回收任务、可选空闲钩子及低功耗入口；不能阻塞空闲钩子，否则可能阻碍清理和其他内核空闲工作。动态任务自删除后常由空闲任务回收 TCB 和栈，单核删除另一个非运行任务则可能立即回收；不能认为所有删除都必须延迟。静态提供的存储不交给堆分配器释放。

`configUSE_IDLE_HOOK=1` 时可提供 `vApplicationIdleHook()` 做短时、非阻塞工作。仅在空闲钩子喂狗不能证明所有业务任务健康，必要时应检查各任务的进展。让空闲任务有机会运行与让任务被正确删除同样重要；细节见 [V11.1.0 tasks.c](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/tasks.c)。

## Tickless Idle

无滴答空闲在预期较长空闲时抑制周期 Tick，安排可唤醒的低功耗时基，并在唤醒后补偿内核时间。外设中断可提前唤醒，低功耗计时器到期则唤醒到期任务。它不是简单关闭 SysTick 后执行 WFI：睡眠确认与中断竞态、时钟停启、计时精度和最大抑制时长都由端口处理。

启用支持端口的 `configUSE_TICKLESS_IDLE=1` 后，空闲路径估计到下一内核超时的可睡眠时间，结合 `configEXPECTED_IDLE_TIME_BEFORE_SLEEP` 决定是否进入端口睡眠函数。典型流程为确认仍可睡眠、重设唤醒计时、抑制周期 Tick、执行低功耗等待，醒来后恢复时钟与节拍并补偿经过的 Tick。

| 唤醒来源 | 后续动作 |
| --- | --- |
| GPIO、通信或其他被配置为唤醒源的外设事件 | 提前结束睡眠，ISR 处理事件并按需唤醒任务；不是所有外设中断在所有深睡模式下都可唤醒 |
| 预计空闲时限到达 | 唤醒时基触发，内核更新时间并使到期阻塞任务就绪，随后按优先级调度 |

默认 Cortex-M4F 端口可重编程 SysTick 作为较长间隔计时器，不一定另外使用低功耗定时器。若所选深睡模式会停止 SysTick 时钟，就需要平台提供仍运行的 RTC/LPTIM 等时基与自定义抑制/补偿实现。SysTick 位宽、低功耗时钟精度、外设状态和进入睡眠的竞态都会限制睡眠长度及精度。唤醒不保证指定任务立刻运行，也不等于唤醒中断里重新创建 SysTick；应由端口恢复流程统一管理。

## 优先级反转与锁定

低优先级任务持有高优先级任务需要的互斥量，中优先级任务抢占低优先级任务时，会延长高优先级任务等待，这构成典型优先级反转。继承机制临时提升持锁者的有效优先级，释放后按内核规则恢复；嵌套锁与多个互斥量的具体继承策略依版本而异。

继承不能解决循环等待。任务间共享复杂资源通常应使用互斥量，而不是把“只有任务访问”统一替换成挂起调度器。中断共享数据、多核并发以及需要阻塞的操作分别具有不同保护要求。

## 死锁及资源顺序

典型死锁中，任务 A 持有锁 1 等待锁 2，任务 B 持有锁 2 等待锁 1，参与任务都无法继续完成操作。经典资源死锁的四个必要条件是：互斥、占有且等待、资源不可被强制剥夺、循环等待；破坏其中至少一个条件可以防止这种死锁。

| 条件 | 可采用的约束 |
| --- | --- |
| 互斥 | 可共享的数据采用只读快照或消息交接，减少必须独占的资源；不能取消确实需要的互斥保护 |
| 占有且等待 | 必要资源作为一组授予，或者后续获取失败就释放已持资源再重试；简单依次阻塞地申请多把锁不等于“一次性申请” |
| 不可剥夺 | 仅在资源状态可安全回滚时设计撤销/回收协议，不能让其他任务强行 give 普通互斥量 |
| 循环等待 | 为所有锁建立统一顺序，任何任务按同一顺序获取，通常逆序释放 |

申请失败后主动释放已有锁主要消除“占有且等待”，不是使普通互斥量可被其他任务剥夺。超时有助于发现和处理等待失败，但必须配套释放、回滚与重试策略；反复同步重试还可能活锁。优先级继承不改变循环依赖，也不能自动解除死锁。

## HardFault 与低干扰故障记录

HardFault 是 Cortex-M 异常，不是 FreeRTOS 专属错误。任务栈溢出、失效指针、非法返回地址、越界破坏 TCB 或错误中断优先级都可能导致相关故障，但必须根据现场判断。故障 PC/状态寄存器、MSP/PSP、ELF 符号定位与不停机记录的边界见 [HardFault 信息](../硬件与接口/MCU启动与异常.md#hardfault-信息) 和 [故障现场保存与恢复边界](../硬件与接口/MCU启动与异常.md#故障现场保存与恢复边界)。

## 实时性与 Linux 的比较

实时性是对截止期限和最坏响应的约束，不等于平均运行更快。RTOS 的短临界区和可控内存路径便于分析，但中断风暴、过长高优先级任务、总线争用仍可破坏期限。Linux 的抢占配置、实时调度策略和 PREEMPT_RT 等会改变响应特征，不能笼统称 Linux 完全无法实时。

FreeRTOS 存在 MPU 等保护方案，Linux 也可能运行在不同保护条件的平台；不能仅用“一个任务崩溃必然导致整机崩溃”区分两者。FOTA/SOTA 是升级对象与部署策略，也不是某个内核独占的能力。

| 方面 | 常见 MCU FreeRTOS 系统 | 常见带 MMU 的 Linux 系统 |
| --- | --- | --- |
| 系统范围 | 可裁剪任务内核，配套组件按需加入；不能用固定“千行代码”概括全部版本和配置 | 进程、虚拟内存、文件系统、网络与驱动框架等完整服务，也支持实时调度策略 |
| 调度与延迟 | 固定优先级调度和可控短路径便于分析最坏延迟，仍受临界区和高优先级负载影响 | 抢占模型、内核锁、中断和驱动实现都会影响延迟，不能概括为执行整个内核后才可抢占 |
| 内存路径 | 通常无需求分页，运行期可静态分配；仍受堆搜索、Cache、Flash 等待等影响 | 按需分页、换页和动态分配可能引入长延迟；可结合预触页、锁内存等措施减少运行期缺页 |
| 中断处理 | ISR 与任务优先级分属不同机制，过长 ISR 或屏蔽仍会造成抖动 | hardirq、softirq、线程化 IRQ 等路径按内核配置执行，系统负载与优先级配置决定干扰 |

FreeRTOS 的实时性优势来自较少且更可控制的延迟来源，并不等于在任何硬件和应用负载下都比 Linux 响应更快。Linux PREEMPT_RT 通过可抢占的锁与大量线程化中断降低调度延迟，仍保留少量不可抢占路径；应根据目标板上的最坏响应测试判断是否满足截止期限。见 [Linux PREEMPT_RT 原理](https://docs.kernel.org/core-api/real-time/theory.html)。
