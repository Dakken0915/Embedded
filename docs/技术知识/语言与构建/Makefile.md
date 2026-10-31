---
title: Makefile
---

## 基本语法

```
target: dependencies
	command
```

- **target**：目标文件，可以是一个目标文件或一个动作名称（例如：`all`, `clean`）
- **dependencies**：生成目标所依赖的文件或其他目标
- **command**：构建目标的命令，必须以 `Tab` 键开头

**e.g.**

```
main.o: main.cpp
    g++ -c main.cpp -o main.o
```

上述规则表示，当 `main.o` 文件**不存在**或 `main.cpp` **被修改**后，执行命令 `g++ -c main.cpp -o main.o` 来生成 `main.o`



## Makefile 的工作原理

​	`make` 会根据 Makefile 中的规则，依次检查目标文件的时间戳和依赖文件的时间戳。如果依赖文件的时间戳比目标文件新，或者目标文件不存在，`make` 就会执行对应的命令来更新目标文件。`make` 会递归检查依赖关系，直到所有目标都更新完成。



## 变量的使用

​	Makefile 支持定义变量，可以简化重复的命令。变量的定义格式为：

```
VARIABLE_NAME = value

在引用变量时使用 $(VARIABLE_NAME)，如：
CC = g++
CFLAGS = -Wall -g

main.o: main.cpp
    $(CC) $(CFLAGS) -c main.cpp -o main.o

上述例子中，使用变量 $(CC) 和 $(CFLAGS) 来替代具体的编译器(g++)和编译选项(-wall -g)。
```



## 常用的内置变量

- $@：表示目标文件。
- $^：表示所有的依赖文件。
- $<：表示第一个依赖文件。

**e.g.**

```
main: main.o utils.o
	$(CC) -o $@ $^
	
@ 表示目标文件 main，$^ 表示所有的依赖文件 main.o 和 utils.o。
```



## 伪目标

伪目标（phony targets）并不是文件，而是一种命令名称，可以用于执行一些常见的操作，如清理构建文件。它们通常定义为：

```
.PHONY: clean
clean:
    rm -f *.o main
    
clean 是一个伪目标，用于删除中间文件和可执行文件
```





## makefile编写方式



### Makefile+直接编译链接







## 常用函数



### patsubst

​	模式替换函数

```
$(patsubst %.c, %.o, $(wildcard*.c))
```

将 wildcard 得到的所有 .c 文件批量替换成同名的 .o 文件









## 补充

**`make -C $(KERN_DIR) M=pwd modules`** 

`-C` ：切换到内核目录 dir，调用内核官方 Makefile

`$(KERN_DIR)`：shell 环境变量，保存**内核源码顶层目录路径**

`M=${PWD}`这是传给**内核顶层 Makefile** 的专用参数

- `M=` 是内核 Makefile 约定变量：指定「外部模块源码所在目录」；
- `${PWD}`：shell 内置变量，代表**当前命令行所在目录**；
- 外部驱动代码在当前目录，只编译这个目录下的模块，不完整编译整个内核。

`modules`指定编译行为为「编译外部可加载内核模块」，输出 `.ko` 文件；

配套目标补充：

- `modules_install`：编译 + 把.ko 安装到系统内核模块目录；
- `clean`：清理编译生成的.o、.ko、.mod.c 等临时文件



## modules.order

`modules.order` 是 Linux 内核编译过程中自动生成的文件，用于记录**模块编译顺序**和**模块路径列表**。





## obj

- `obj-m`：表示编译为**可加载内核模块**（`.ko` 文件）
- `obj-y`：表示编译为**内建**到内核（`.o` 文件，静态链接进内核）
- `obj-$(CONFIG_XXX)`：根据内核配置决定编译方式
