---
title: Shell
---

# 命令解释器

- Shell 命令解释器：bash 编程
- 常用命令解释器

## 表格

| 命令解释器 | 说明                                                         |
| :--------- | :----------------------------------------------------------- |
| bash       | 目前应用最广泛一款命令解释器，红帽系列 (默认)，Debian，UbuntuBASH 全称：Bourne-Again SHell |
| dash       | 一般 debian/ubuntu 系统默认的，运行脚本推荐使用 bash lidao.sh |
| csh,tcsh   | 一些 unix 系统使用                                           |
| zsh        | 功能更多，支持更多的插件，可以更好看。                       |

- shell 脚本（.bash .sh），运行在 ubuntu 中的时候，不推荐使用 sh 运行，推荐使用 bash 运行。
- `/bin/sh` 是指向解释器的软链接



# shell 脚本文件

## 创建脚本文件

-  `vim test.sh` ：用vim创建/编辑脚本

-  `nano test.sh` ：用nano创建/编辑脚本

------

## 解释器指定 （首行）

`#!/bin/bash` 写在脚本第一行，叫做释伴行。

> 告诉系统这个文件要用  bash  解释器来解析运行。 
>
> 如果不加这行， ./test.sh  执行时会使用系统默认shell，可能出现语法不兼容问题。

------

## 执行脚本的方式

方式1： `./script.sh` 仅路径执行

1. 前提：脚本必须有可执行权限，需要先执行授权命令： 
  `chmod +x script.sh`

  > `chmod a+x script.sh`添加可执行权限

2. 原理：系统读取文件第一行  `#!/bin/bash` ，自动调用 bash 运行脚本。

方式2： `bash script.sh` 解析器执行 

1. 不需要可执行权限，直接调用 bash 程序去读取脚本文件。
2. 即使脚本第一行没有  `#!/bin/bash` ，也会强制用 bash 执行

------

## 注释规范

单行注释：`# 注释内容`

多行注释：:<<!
		   `# 注释内容`
		   `!`



# shell 多命令处理

需求：在`/home/book/script/`目录下创建`one.txt`，并增加内容 `”hello shell“`

```
#!/bin/bash 
mkdir -p /home/book/script
touch /home/book/script/one.txt
echo "hello shell" >> /home/book/script/one.txt
```



# Shell 变量

## 环境变量

### 1.Shell 的配置文件分类

#### 1.1 全局配置文件

> /etc/profile
> /etc/profile.d/*.sh
> /etc/bashrc

#### 1.2 个人配置文件

> 当前用户 /.bash_profile
> 当前用户 /.bashrc

一般情况下，都是直接针对全局配置进行操作。



### 2.环境变量分类

在 Linux 系统中，环境变量按照其作用范围不同大致可以分为系统级环境变量和用户级环境变量。

- 系统级环境变量：Shell 环境加载全局配置文件中的变量共享给所有用户所有 Shell 程序使用，全局共享
- 用户级环境变量：Shell 环境加载个人配置文件中的变量共享给当前用户的 Shell 程序使用，登录用户使用



### 3.查看环境变量

> 查看当前 shell 系统环境变量：`env`
> 查看 Shell 变量 (系统环境变量 + 自定义变量 + 函数)：`set`



### 4.常用环境变量

| 变量名称     | 含义                                                         |
| ------------ | ------------------------------------------------------------ |
| ==PATH==     | 与 Windows 环境变量 PATH 功能一样，设置命令的搜索路径，以冒号为分割 |
| HOME         | 当前用户主目录：/root                                        |
| SHELL        | 当前 shell 解析器类型：/bin/bash                             |
| ==HISTFILE== | 显示当前用户执行命令的历史列表文件：/root/.bash_history      |
| PWD          | 显示当前所在路径：/root                                      |
| OLDPWD       | 显示之前的路径                                               |
| HOSTNAME     | 显示当前主机名：itheima                                      |
| HOSTTYPE     | 显示主机的架构，是 i386、i686、还是 x86、x64 等：x86_64      |
| ==LANG==     | 设置当前系统语言环境：zh_CN.UTF-8                            |



------

## 局部变量

### 变量定义

语法： `var_name=value` 

> 1.**等号左右不能加空格**，否则会报错
>
> 2.变量名仅支持字母、数字、下划线，不能以数字开头
>
> 3.在 bash 环境中，变量的默认类型都是字符串类型，无法直接进行数值运算
>
> 4.变量的值如果有空格，必须使用双引号括起来
>
> 5.不能使用 Shell 的关键字作为变量名称



### 变量使用

> 语法：在变量名前加 `$`，推荐 `$`{变量名}  **避免字符串拼接歧义**
> `echo $var_name`
> `echo ${var_name}_file`



### 特殊内置变量

> `$0`  当前脚本的文件名 
>  `$1`   `$2`  ... 脚本执行时传入的第1、第2...个参数 
>  `$#`  传入参数总个数 
>  `$@` / `$*`  所有传入参数列表 



### 变量删除

> `unset var_name`

# Shell 流程控制

## if 条件判断语句

```
 if [ condition ]; then
	#command
elif [ another_condition ]; then
	#command
else
	#command
fi
```

- `[ ]` 左右必须有空格；

-  `if`  结尾必须用  `fi`  闭合
