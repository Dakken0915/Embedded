// @ts-check

/** @type {import('@docusaurus/plugin-content-docs').SidebarsConfig} */
const sidebars = {
  knowledgeSidebar: [
    '初始化说明',
    '知识索引',
    {
      type: 'category',
      label: '语言与构建',
      items: [
        {
          type: 'category',
          label: 'C 语言',
          link: { type: 'doc', id: '语言与构建/C语言/概述' },
          items: [
            '语言与构建/C语言/对象存储与编译链接',
            '语言与构建/C语言/内存管理',
            '语言与构建/C语言/指针数组与复合类型',
            '语言与构建/C语言/函数参数与回调',
            '语言与构建/C语言/标准库与字符串',
            '语言与构建/C语言/C语言基础语法考点',
          ],
        },
        {
          type: 'category',
          label: 'C++ 语言',
          link: { type: 'doc', id: '语言与构建/C++语言/概述' },
          items: [
            '语言与构建/C++语言/类与对象',
            '语言与构建/C++语言/浅拷贝与深拷贝',
            '语言与构建/C++语言/函数与参数',
            '语言与构建/C++语言/模板与泛型',
            '语言与构建/C++语言/现代语言特性',
            '语言与构建/C++语言/异常处理',
            '语言与构建/C++语言/类型转换与内存管理',
            '语言与构建/C++语言/函数绑定与结构化绑定',
            '语言与构建/C++语言/C与C++的区别',
            '语言与构建/C++语言/对象资源与并发',
            '语言与构建/C++语言/STL容器与算法',
          ],
        },
        '语言与构建/Makefile',
        '语言与构建/Shell',
      ],
    },
    {
      type: 'category',
      label: '开发工具',
      items: [
        '开发工具/GDB',
        '开发工具/Git',
      ],
    },
    {
      type: 'category',
      label: 'Linux 应用',
      items: [
        'Linux应用/系统调用',
        'Linux应用/错误代码速查',
        'Linux应用/线程控制',
        'Linux应用/Linux内存管理',
        'Linux应用/CPU缓存',
        'Linux应用/程序调试与故障定位',
        'Linux应用/V4L2视频采集',
        'Linux应用/libusb设备访问',
        'Linux应用/IPC',
      ],
    },
    {
      type: 'category',
      label: 'Linux 驱动',
      items: [
        {
          type: 'category',
          label: '基础理论',
          items: [
            'Linux驱动/基础理论/Linux驱动与设备模型',
            'Linux驱动/基础理论/字符设备与IO模型',
            'Linux驱动/基础理论/内核内存与地址映射',
            'Linux驱动/基础理论/同步与互斥',
          ],
        },
        {
          type: 'category',
          label: '中断机制',
          items: ['Linux驱动/中断机制/中断机制'],
        },
        {
          type: 'category',
          label: '平台基础',
          items: ['Linux驱动/平台基础/设备树与Platform'],
        },
        {
          type: 'category',
          label: '底层引脚与时钟',
          items: [
            'Linux驱动/底层引脚与时钟/CLK',
            'Linux驱动/底层引脚与时钟/GPIO子系统',
            'Linux驱动/底层引脚与时钟/Pinctrl子系统',
          ],
        },
        {
          type: 'category',
          label: '总线与外设子系统',
          items: [
            'Linux驱动/总线与外设子系统/I2C驱动',
            'Linux驱动/总线与外设子系统/Input子系统',
            'Linux驱动/总线与外设子系统/USB驱动',
          ],
        },
        {
          type: 'category',
          label: '多媒体驱动',
          items: [
            'Linux驱动/多媒体驱动/LCD与Framebuffer',
            'Linux驱动/多媒体驱动/V4L2驱动',
          ],
        },
        {
          type: 'category',
          label: '调试工具',
          items: ['Linux驱动/调试工具/Linux内核日志'],
        },
      ],
    },
    {
      type: 'category',
      label: '应用框架',
      items: [
        '应用框架/嵌入式软件分层架构',
        '应用框架/Qt',
        '应用框架/ROS2',
      ],
    },
    {
      type: 'category',
      label: '实时操作系统',
      items: [
        '实时操作系统/FreeRTOS',
        {
          type: 'category',
          label: 'uC/OS-III',
          link: { type: 'doc', id: '实时操作系统/uC-OS-III/概述' },
          items: [
            '实时操作系统/uC-OS-III/内核结构与移植',
            '实时操作系统/uC-OS-III/任务控制块与创建',
            '实时操作系统/uC-OS-III/栈帧与上下文切换',
            '实时操作系统/uC-OS-III/优先级位图与就绪链表',
            '实时操作系统/uC-OS-III/调度与中断',
            '实时操作系统/uC-OS-III/挂起删除与任务通信',
          ],
        },
      ],
    },
    {
      type: 'category',
      label: '硬件与接口',
      items: [
        '硬件与接口/ARM与RISC-V',
        '硬件与接口/MCU启动与异常',
        '硬件与接口/Cache与DMA',
        '硬件与接口/UART',
        '硬件与接口/I2C',
        '硬件与接口/SPI',
        '硬件与接口/RTC',
        '硬件与接口/ADC',
        '硬件与接口/看门狗',
        '硬件与接口/Flash与备份域',
        '硬件与接口/PWR与低功耗',
        '硬件与接口/CAN',
        '硬件与接口/FPGA',
      ],
    },
    {
      type: 'category',
      label: '算法与数据结构',
      items: [
        '算法与数据结构/数据结构与算法',
        '算法与数据结构/算法模板',
        {
          type: 'category',
          label: '算法题解',
          items: ['算法与数据结构/算法题解/LeeCode'],
        },
      ],
    },
    {
      type: 'category',
      label: '图像处理',
      items: ['图像处理/OpenCV/openCV'],
    },
    {
      type: 'category',
      label: 'ESP32',
      items: [
        'ESP32/工程与外设',
        'ESP32/存储与显示',
        'ESP32/WiFi与配网',
      ],
    },
    {
      type: 'category',
      label: '网络与通信',
      items: [
        '网络与通信/Socket与IO模型',
        '网络与通信/多连接处理',
        '网络与通信/AT指令与链路状态机',
        '网络与通信/MQTT与Modbus',
      ],
    },
    {
      type: 'category',
      label: '系统与可靠性',
      items: [
        '系统与可靠性/Flash存储与日志',
        '系统与可靠性/Bootloader与OTA',
      ],
    },
    {
      type: 'category',
      label: '控制算法',
      items: ['控制算法/PID控制'],
    },
  ],
};

export default sidebars;
