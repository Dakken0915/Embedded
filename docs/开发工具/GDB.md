## GDB

### 程序执行控制

| 命令     | 简写  | 作用                                     |
| -------- | ----- | ---------------------------------------- |
| `step`   | `s`   | C 源码单步，遇到函数调用会进入函数内部   |
| `next`   | `n`   | C 源码单步，遇到函数直接跑完、不进子函数 |
| `stepi`  | `si`  | 汇编指令单步，一条机器指令               |
| `nexti`  | `ni`  | 汇编单步，跳过调用                       |
| `finish` | `fin` | 跳出当前函数                             |



### 断点相关

```shell
b  main                # break：在main函数打断点
b  file.c:120          # 在指定文件的120行打断点
b  func_name           # 在函数入口打断点
tb func_name           # tbreak 临时断点：命中一次后自动销毁，用完即删
info b                 # 查看所有断点列表，看断点编号
del 1                  # delete：删除编号1的断点；del不加编号删除全部
disable 1              # 禁用断点（保留，不删除）
enable 1               # 启用断点
```



### 查看、打印变量与内存

```shell
p  var                 # print：打印变量var的值
p/x var                # 十六进制打印（最常用，寄存器、地址）
p/d var                # 十进制打印
display var            # 程序每次停下，自动打印这个变量（持续观察）
undisplay 1            # 取消自动显示，编号来自info display
x  /20x 0x80000000     # x：查看内存；格式：x/数量格式 地址
# 格式：x十六进制，d十进制，s字符串，i汇编指令
```



### 栈回溯

```shell
bt                     # backtrace：打印调用栈，看函数调用链路
bt full                # 打印栈+局部变量
up                     # 向上切换栈帧（看上层调用函数）
down                   # 向下切换栈帧
frame 0                # 直接切换到第0号栈帧（当前栈）
```



### 运行、继续、停止

```shell
run                    # r，从头运行程序（本地ELF）
continue               # c，从当前位置继续跑，直到下一个断点
attach 进程号           # 附着到正在运行的进程（Linux用户态）
kill                   # 终止正在调试的程序
```



### 寄存器查看

```shell
info registers         # 查看全部通用寄存器
info reg pc            # 单独看pc寄存器
p $pc                  # 打印pc
p $sp                  # 打印栈指针
```


