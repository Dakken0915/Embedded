# IPC

## pipe

半双工，只能用于亲缘进程。可以看成特殊的文件，但是不存在文件系统当中，只存在于内存当中

```C
1 #include <unistd.h>
2 int pipe(int fd[2]);    // 返回值：若成功返回0，失败返回-1
```



创建两个文件描述符：`fd[0]`为读而打开，`fd[1]`为写而打开。

若要数据流从父进程流向子进程，则关闭父进程的读端（`fd[0]`）与子进程的写端（`fd[1]`）；反之，则可以使数据流从子进程流向父进程。

```C
 1 #include<stdio.h>
 2 #include<unistd.h>
 3 
 4 int main()
 5 {
 6     int fd[2];  // 两个文件描述符
 7     pid_t pid;
 8     char buff[20];
 9 
10     if(pipe(fd) < 0)  // 创建管道
11         printf("Create Pipe Error!\n");
12 
13     if((pid = fork()) < 0)  // 创建子进程
14         printf("Fork Error!\n");
15     else if(pid > 0)  // 父进程
16     {
17         close(fd[0]); // 关闭读端
18         write(fd[1], "hello world\n", 12);
19     }
20     else
21     {
22         close(fd[1]); // 关闭写端
23         read(fd[0], buff, 20);
24         printf("%s", buff);
25     }
26 
27     return 0;
28 }
```



## fifo