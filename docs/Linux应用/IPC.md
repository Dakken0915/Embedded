# IPC

## pipe

半双工，只能用于亲缘进程。可以看成特殊的文件，但是不存在文件系统当中，只存在于内存当中

```C
1 #include <unistd.h>
2 int pipe(int fd[2]);    // 返回值：若成功返回0，失败返回-1
```



- 创建两个文件描述符：`fd[0]`为读而打开，`fd[1]`为写而打开。
- 若要数据流从父进程流向子进程，则关闭父进程的读端（`fd[0]`）与子进程的写端（`fd[1]`）；反之，则可以使数据流从子进程流向父进程。

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

- `fifo` 可以在无关进程之间通信。
- `fifo` 有路径名与之相关联，它以一种特殊设备文件形式存在于文件系统中。

```C
1 #include <sys/stat.h>
2 // 返回值：成功返回0，出错返回-1
3 int mkfifo(const char *pathname, mode_t mode);
```

`mode` 参数与`open`函数中的 `mode` 相同。一旦创建了一个 `fifo`，就可以用一般的文件I/O函数操作它。

当 `open` 一个`fifo`时，是否设置非阻塞标志`（O_NONBLOCK）`的区别：

- 若没有指定`O_NONBLOCK`（默认），只读 `open` 要阻塞到某个其他进程为写而打开此 `fifo`。类似的，只写 `open` 要阻塞到某个其他进程为读而打开它。
- 若指定了`O_NONBLOCK`，则只读 `open` 立即返回。而只写 `open` 将出错返回 -1 如果没有进程已经为读而打开该 `fifo`，其 `errno` 置 `ENXIO`。



### write_fifo

```C
 1 #include<stdio.h>
 2 #include<stdlib.h>   // exit
 3 #include<fcntl.h>    // O_WRONLY
 4 #include<sys/stat.h>
 5 #include<time.h>     // time
 6 
 7 int main()
 8 {
 9     int fd;
10     int n, i;
11     char buf[1024];
12     time_t tp;
13 
14     printf("I am %d process.\n", getpid()); // 说明进程ID
15     
16     if((fd = open("fifo1", O_WRONLY)) < 0) // 以写打开一个FIFO 
17     {
18         perror("Open FIFO Failed");
19         exit(1);
20     }
21 
22     for(i=0; i<10; ++i)
23     {
24         time(&tp);  // 取系统当前时间
25         n=sprintf(buf,"Process %d's time is %s",getpid(),ctime(&tp));
26         printf("Send message: %s", buf); // 打印
27         if(write(fd, buf, n+1) < 0)  // 写入到FIFO中
28         {
29             perror("Write FIFO Failed");
30             close(fd);
31             exit(1);
32         }
33         sleep(1);  // 休眠1秒
34     }
35 
36     close(fd);  // 关闭FIFO文件
37     return 0;
38 }
```



### read_fifo

```C
 1 #include<stdio.h>
 2 #include<stdlib.h>
 3 #include<errno.h>
 4 #include<fcntl.h>
 5 #include<sys/stat.h>
 6 
 7 int main()
 8 {
 9     int fd;
10     int len;
11     char buf[1024];
12 
13     if(mkfifo("fifo1", 0666) < 0 && errno!=EEXIST) // 创建FIFO管道
14         perror("Create FIFO Failed");
15 
16     if((fd = open("fifo1", O_RDONLY)) < 0)  // 以读打开FIFO
17     {
18         perror("Open FIFO Failed");
19         exit(1);
20     }
21     
22     while((len = read(fd, buf, 1024)) > 0) // 读取FIFO管道
23         printf("Read message: %s", buf);
24 
25     close(fd);  // 关闭FIFO文件
26     return 0;
27 }
```





## 消息队列

1. 消息队列是面向记录的，其中的消息具有特定的格式以及特定的优先级。
2. 消息队列独立于发送与接收进程。进程终止时，消息队列及其内容并不会被删除。
3. 消息队列可以实现消息的随机查询,消息不一定要以先进先出的次序读取,也可以按消息的类型读取。

```C
1 #include <sys/msg.h>
2 // 创建或打开消息队列：成功返回队列ID，失败返回-1
3 int msgget(key_t key, int flag);
4 // 添加消息：成功返回0，失败返回-1
5 int msgsnd(int msqid, const void *ptr, size_t size, int flag);
6 // 读取消息：成功返回消息数据的长度，失败返回-1
7 int msgrcv(int msqid, void *ptr, size_t size, long type,int flag);
8 // 控制消息队列：成功返回0，失败返回-1
9 int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

在以下两种情况下，msgget将创建一个新的消息队列：

- 如果没有与键值key相对应的消息队列，并且flag中包含了`IPC_CREAT`标志位。
- `key` 参数为 `IPC_PRIVATE`。

函数 `msgrcv` 在读取消息队列时，`type`参数有下面几种情况：

- `type == 0`，返回队列中的第一个消息；
- `type > 0`，返回队列中消息类型为 type 的第一个消息；
- `type < 0`，返回队列中消息类型值小于或等于 type 绝对值的消息，如果有多个，则取类型值最小的消息。

可以看出，`type`值非 0 时用于以非先进先出次序读消息。也可以把 type 看做优先级的权值。



### msg_server

```C
 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <sys/msg.h>
 4 
 5 // 用于创建一个唯一的key
 6 #define MSG_FILE "/etc/passwd"
 7 
 8 // 消息结构
 9 struct msg_form {
10     long mtype;
11     char mtext[256];
12 };
13 
14 int main()
15 {
16     int msqid;
17     key_t key;
18     struct msg_form msg;
19     
20     // 获取key值
21     if((key = ftok(MSG_FILE,'z')) < 0)
22     {
23         perror("ftok error");
24         exit(1);
25     }
26 
27     // 打印key值
28     printf("Message Queue - Server key is: %d.\n", key);
29 
30     // 创建消息队列
31     if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
32     {
33         perror("msgget error");
34         exit(1);
35     }
36 
37     // 打印消息队列ID及进程ID
38     printf("My msqid is: %d.\n", msqid);
39     printf("My pid is: %d.\n", getpid());
40 
41     // 循环读取消息
42     for(;;) 
43     {
44         msgrcv(msqid, &msg, 256, 888, 0);// 返回类型为888的第一个消息
45         printf("Server: receive msg.mtext is: %s.\n", msg.mtext);
46         printf("Server: receive msg.mtype is: %d.\n", msg.mtype);
47 
48         msg.mtype = 999; // 客户端接收的消息类型
49         sprintf(msg.mtext, "hello, I'm server %d", getpid());
50         msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
51     }
52     return 0;
53 }
```

### msg_client

```C
 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <sys/msg.h>
 4 
 5 // 用于创建一个唯一的key
 6 #define MSG_FILE "/etc/passwd"
 7 
 8 // 消息结构
 9 struct msg_form {
10     long mtype;
11     char mtext[256];
12 };
13 
14 int main()
15 {
16     int msqid;
17     key_t key;
18     struct msg_form msg;
19 
20     // 获取key值
21     if ((key = ftok(MSG_FILE, 'z')) < 0) 
22     {
23         perror("ftok error");
24         exit(1);
25     }
26 
27     // 打印key值
28     printf("Message Queue - Client key is: %d.\n", key);
29 
30     // 打开消息队列
31     if ((msqid = msgget(key, IPC_CREAT|0777)) == -1) 
32     {
33         perror("msgget error");
34         exit(1);
35     }
36 
37     // 打印消息队列ID及进程ID
38     printf("My msqid is: %d.\n", msqid);
39     printf("My pid is: %d.\n", getpid());
40 
41     // 添加消息，类型为888
42     msg.mtype = 888;
43     sprintf(msg.mtext, "hello, I'm client %d", getpid());
44     msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
45 
46     // 读取类型为777的消息
47     msgrcv(msqid, &msg, 256, 999, 0);
48     printf("Client: receive msg.mtext is: %s.\n", msg.mtext);
49     printf("Client: receive msg.mtype is: %d.\n", msg.mtype);
50     return 0;
51 }
```



## semaphore

1. 信号量基于操作系统的 PV 操作，程序对信号量的操作都是原子操作。
2. 每次对信号量的 PV 操作不仅限于对信号量值加 1 或减 1，而且可以加减任意正整数。
3. 支持信号量组。

```C
1 #include <sys/sem.h>
2 // 创建或获取一个信号量组：若成功返回信号量集ID，失败返回-1
3 int semget(key_t key, int num_sems, int sem_flags);
4 // 对信号量组进行操作，改变信号量的值：成功返回0，失败返回-1
5 int semop(int semid, struct sembuf semoparray[], size_t numops);  
6 // 控制信号量的相关信息
7 int semctl(int semid, int sem_num, int cmd, ...);
```



```C
1 struct sembuf 
2 {
3     short sem_num; // 信号量组中对应的序号，0～sem_nums-1
4     short sem_op;  // 信号量值在一次操作中的改变量
5     short sem_flg; // IPC_NOWAIT, SEM_UNDO
6 }
```

- 若sem_op > 0，表示进程释放相应的资源数，将 sem_op 的值加到信号量的值上。如果有进程正在休眠等待此信号量，则换行它们。
- 若sem_op < 0，请求 sem_op 的绝对值的资源。
  - 如果相应的资源数可以满足请求，则将该信号量的值减去sem_op的绝对值，函数成功返回。
  - 当相应的资源数不能满足请求时，这个操作与sem_flg有关。
    - sem_flg 指定IPC_NOWAIT，则semop函数出错返回EAGAIN。
    - sem_flg 没有指定IPC_NOWAIT，则将该信号量的semncnt值加1，然后进程挂起直到下述情况发生：
      1. 当相应的资源数可以满足请求，此信号量的semncnt值减1，该信号量的值减去sem_op的绝对值。成功返回；
      2. 此信号量被删除，函数smeop出错返回EIDRM；
      3. 进程捕捉到信号，并从信号处理函数返回，此情况下将此信号量的semncnt值减1，函数semop出错返回EINTR
- 若sem_op == 0，进程阻塞直到信号量的相应值为0：
  - 当信号量已经为0，函数立即返回。
  - 如果信号量的值不为0，则依据sem_flg决定函数动作：
    - sem_flg指定IPC_NOWAIT，则出错返回EAGAIN。
    - sem_flg没有指定IPC_NOWAIT，则将该信号量的semncnt值加1，然后进程挂起直到下述情况发生
      1. 信号量值为0，将信号量的semzcnt的值减1，函数semop成功返回；
      2. 此信号量被删除，函数smeop出错返回EIDRM；
      3. 进程捕捉到信号，并从信号处理函数返回，在此情况将此信号量的semncnt值减1，函数semop出错返回EINTR



```C
  1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<sys/sem.h>
  4 
  5 // 联合体，用于semctl初始化
  6 union semun
  7 {
  8     int              val; /*for SETVAL*/
  9     struct semid_ds *buf;
 10     unsigned short  *array;
 11 };
 12 
 13 // 初始化信号量
 14 int init_sem(int sem_id, int value)
 15 {
 16     union semun tmp;
 17     tmp.val = value;
 18     if(semctl(sem_id, 0, SETVAL, tmp) == -1)
 19     {
 20         perror("Init Semaphore Error");
 21         return -1;
 22     }
 23     return 0;
 24 }
 25 
 26 // P操作:
 27 //    若信号量值为1，获取资源并将信号量值-1 
 28 //    若信号量值为0，进程挂起等待
 29 int sem_p(int sem_id)
 30 {
 31     struct sembuf sbuf;
 32     sbuf.sem_num = 0; /*序号*/
 33     sbuf.sem_op = -1; /*P操作*/
 34     sbuf.sem_flg = SEM_UNDO;
 35 
 36     if(semop(sem_id, &sbuf, 1) == -1)
 37     {
 38         perror("P operation Error");
 39         return -1;
 40     }
 41     return 0;
 42 }
 43 
 44 // V操作：
 45 //    释放资源并将信号量值+1
 46 //    如果有进程正在挂起等待，则唤醒它们
 47 int sem_v(int sem_id)
 48 {
 49     struct sembuf sbuf;
 50     sbuf.sem_num = 0; /*序号*/
 51     sbuf.sem_op = 1;  /*V操作*/
 52     sbuf.sem_flg = SEM_UNDO;
 53 
 54     if(semop(sem_id, &sbuf, 1) == -1)
 55     {
 56         perror("V operation Error");
 57         return -1;
 58     }
 59     return 0;
 60 }
 61 
 62 // 删除信号量集
 63 int del_sem(int sem_id)
 64 {
 65     union semun tmp;
 66     if(semctl(sem_id, 0, IPC_RMID, tmp) == -1)
 67     {
 68         perror("Delete Semaphore Error");
 69         return -1;
 70     }
 71     return 0;
 72 }
 73 
 74 
 75 int main()
 76 {
 77     int sem_id;  // 信号量集ID
 78     key_t key;  
 79     pid_t pid;
 80 
 81     // 获取key值
 82     if((key = ftok(".", 'z')) < 0)
 83     {
 84         perror("ftok error");
 85         exit(1);
 86     }
 87 
 88     // 创建信号量集，其中只有一个信号量
 89     if((sem_id = semget(key, 1, IPC_CREAT|0666)) == -1)
 90     {
 91         perror("semget error");
 92         exit(1);
 93     }
 94 
 95     // 初始化：初值设为0资源被占用
 96     init_sem(sem_id, 0);
 97 
 98     if((pid = fork()) == -1)
 99         perror("Fork Error");
100     else if(pid == 0) /*子进程*/ 
101     {
102         sleep(2);
103         printf("Process child: pid=%d\n", getpid());
104         sem_v(sem_id);  /*释放资源*/
105     }
106     else  /*父进程*/
107     {
108         sem_p(sem_id);   /*等待资源*/
109         printf("Process father: pid=%d\n", getpid());
110         sem_v(sem_id);   /*释放资源*/
111         del_sem(sem_id); /*删除信号量集*/
112     }
113     return 0;
114 }
```





## 共享内存

1. 共享内存是最快的一种 IPC，因为进程是直接对内存进行存取。
2. 因为多个进程可以同时操作，所以需要进行同步。
3. 信号量+共享内存通常结合在一起使用，信号量用来同步对共享内存的访问。

```C
1 #include <sys/shm.h>
2 // 创建或获取一个共享内存：成功返回共享内存ID，失败返回-1
3 int shmget(key_t key, size_t size, int flag);
4 // 连接共享内存到当前进程的地址空间：成功返回指向共享内存的指针，失败返回-1
5 void *shmat(int shm_id, const void *addr, int flag);
6 // 断开与共享内存的连接：成功返回0，失败返回-1
7 int shmdt(void *addr); 
8 // 控制共享内存的相关信息：成功返回0，失败返回-1
9 int shmctl(int shm_id, int cmd, struct shmid_ds *buf);
```

当用`shmget`函数创建一段共享内存时，必须指定其 size；而如果引用一个已存在的共享内存，则将 size 指定为0 。

共享内存+信号量+消息队列】的组合来实现服务器进程与客户进程间的通信。

- 共享内存用来传递数据；
- 信号量用来同步；
- 消息队列用来 在客户端修改了共享内存后 通知服务器读取。

### server.c

```C
  1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<sys/shm.h>  // shared memory
  4 #include<sys/sem.h>  // semaphore
  5 #include<sys/msg.h>  // message queue
  6 #include<string.h>   // memcpy
  7 
  8 // 消息队列结构
  9 struct msg_form {
 10     long mtype;
 11     char mtext;
 12 };
 13 
 14 // 联合体，用于semctl初始化
 15 union semun
 16 {
 17     int              val; /*for SETVAL*/
 18     struct semid_ds *buf;
 19     unsigned short  *array;
 20 };
 21 
 22 // 初始化信号量
 23 int init_sem(int sem_id, int value)
 24 {
 25     union semun tmp;
 26     tmp.val = value;
 27     if(semctl(sem_id, 0, SETVAL, tmp) == -1)
 28     {
 29         perror("Init Semaphore Error");
 30         return -1;
 31     }
 32     return 0;
 33 }
 34 
 35 // P操作:
 36 //  若信号量值为1，获取资源并将信号量值-1 
 37 //  若信号量值为0，进程挂起等待
 38 int sem_p(int sem_id)
 39 {
 40     struct sembuf sbuf;
 41     sbuf.sem_num = 0; /*序号*/
 42     sbuf.sem_op = -1; /*P操作*/
 43     sbuf.sem_flg = SEM_UNDO;
 44 
 45     if(semop(sem_id, &sbuf, 1) == -1)
 46     {
 47         perror("P operation Error");
 48         return -1;
 49     }
 50     return 0;
 51 }
 52 
 53 // V操作：
 54 //  释放资源并将信号量值+1
 55 //  如果有进程正在挂起等待，则唤醒它们
 56 int sem_v(int sem_id)
 57 {
 58     struct sembuf sbuf;
 59     sbuf.sem_num = 0; /*序号*/
 60     sbuf.sem_op = 1;  /*V操作*/
 61     sbuf.sem_flg = SEM_UNDO;
 62 
 63     if(semop(sem_id, &sbuf, 1) == -1)
 64     {
 65         perror("V operation Error");
 66         return -1;
 67     }
 68     return 0;
 69 }
 70 
 71 // 删除信号量集
 72 int del_sem(int sem_id)
 73 {
 74     union semun tmp;
 75     if(semctl(sem_id, 0, IPC_RMID, tmp) == -1)
 76     {
 77         perror("Delete Semaphore Error");
 78         return -1;
 79     }
 80     return 0;
 81 }
 82 
 83 // 创建一个信号量集
 84 int creat_sem(key_t key)
 85 {
 86     int sem_id;
 87     if((sem_id = semget(key, 1, IPC_CREAT|0666)) == -1)
 88     {
 89         perror("semget error");
 90         exit(-1);
 91     }
 92     init_sem(sem_id, 1);  /*初值设为1资源未占用*/
 93     return sem_id;
 94 }
 95 
 96 
 97 int main()
 98 {
 99     key_t key;
100     int shmid, semid, msqid;
101     char *shm;
102     char data[] = "this is server";
103     struct shmid_ds buf1;  /*用于删除共享内存*/
104     struct msqid_ds buf2;  /*用于删除消息队列*/
105     struct msg_form msg;  /*消息队列用于通知对方更新了共享内存*/
106 
107     // 获取key值
108     if((key = ftok(".", 'z')) < 0)
109     {
110         perror("ftok error");
111         exit(1);
112     }
113 
114     // 创建共享内存
115     if((shmid = shmget(key, 1024, IPC_CREAT|0666)) == -1)
116     {
117         perror("Create Shared Memory Error");
118         exit(1);
119     }
120 
121     // 连接共享内存
122     shm = (char*)shmat(shmid, 0, 0);
123     if((int)shm == -1)
124     {
125         perror("Attach Shared Memory Error");
126         exit(1);
127     }
128 
129 
130     // 创建消息队列
131     if ((msqid = msgget(key, IPC_CREAT|0777)) == -1)
132     {
133         perror("msgget error");
134         exit(1);
135     }
136 
137     // 创建信号量
138     semid = creat_sem(key);
139     
140     // 读数据
141     while(1)
142     {
143         msgrcv(msqid, &msg, 1, 888, 0); /*读取类型为888的消息*/
144         if(msg.mtext == 'q')  /*quit - 跳出循环*/ 
145             break;
146         if(msg.mtext == 'r')  /*read - 读共享内存*/
147         {
148             sem_p(semid);
149             printf("%s\n",shm);
150             sem_v(semid);
151         }
152     }
153 
154     // 断开连接
155     shmdt(shm);
156 
157     /*删除共享内存、消息队列、信号量*/
158     shmctl(shmid, IPC_RMID, &buf1);
159     msgctl(msqid, IPC_RMID, &buf2);
160     del_sem(semid);
161     return 0;
162 }
```



### client.c

```C
  1 #include<stdio.h>
  2 #include<stdlib.h>
  3 #include<sys/shm.h>  // shared memory
  4 #include<sys/sem.h>  // semaphore
  5 #include<sys/msg.h>  // message queue
  6 #include<string.h>   // memcpy
  7 
  8 // 消息队列结构
  9 struct msg_form {
 10     long mtype;
 11     char mtext;
 12 };
 13 
 14 // 联合体，用于semctl初始化
 15 union semun
 16 {
 17     int              val; /*for SETVAL*/
 18     struct semid_ds *buf;
 19     unsigned short  *array;
 20 };
 21 
 22 // P操作:
 23 //  若信号量值为1，获取资源并将信号量值-1 
 24 //  若信号量值为0，进程挂起等待
 25 int sem_p(int sem_id)
 26 {
 27     struct sembuf sbuf;
 28     sbuf.sem_num = 0; /*序号*/
 29     sbuf.sem_op = -1; /*P操作*/
 30     sbuf.sem_flg = SEM_UNDO;
 31 
 32     if(semop(sem_id, &sbuf, 1) == -1)
 33     {
 34         perror("P operation Error");
 35         return -1;
 36     }
 37     return 0;
 38 }
 39 
 40 // V操作：
 41 //  释放资源并将信号量值+1
 42 //  如果有进程正在挂起等待，则唤醒它们
 43 int sem_v(int sem_id)
 44 {
 45     struct sembuf sbuf;
 46     sbuf.sem_num = 0; /*序号*/
 47     sbuf.sem_op = 1;  /*V操作*/
 48     sbuf.sem_flg = SEM_UNDO;
 49 
 50     if(semop(sem_id, &sbuf, 1) == -1)
 51     {
 52         perror("V operation Error");
 53         return -1;
 54     }
 55     return 0;
 56 }
 57 
 58 
 59 int main()
 60 {
 61     key_t key;
 62     int shmid, semid, msqid;
 63     char *shm;
 64     struct msg_form msg;
 65     int flag = 1; /*while循环条件*/
 66 
 67     // 获取key值
 68     if((key = ftok(".", 'z')) < 0)
 69     {
 70         perror("ftok error");
 71         exit(1);
 72     }
 73 
 74     // 获取共享内存
 75     if((shmid = shmget(key, 1024, 0)) == -1)
 76     {
 77         perror("shmget error");
 78         exit(1);
 79     }
 80 
 81     // 连接共享内存
 82     shm = (char*)shmat(shmid, 0, 0);
 83     if((int)shm == -1)
 84     {
 85         perror("Attach Shared Memory Error");
 86         exit(1);
 87     }
 88 
 89     // 创建消息队列
 90     if ((msqid = msgget(key, 0)) == -1)
 91     {
 92         perror("msgget error");
 93         exit(1);
 94     }
 95 
 96     // 获取信号量
 97     if((semid = semget(key, 0, 0)) == -1)
 98     {
 99         perror("semget error");
100         exit(1);
101     }
102     
103     // 写数据
104     printf("***************************************\n");
105     printf("*                 IPC                 *\n");
106     printf("*    Input r to send data to server.  *\n");
107     printf("*    Input q to quit.                 *\n");
108     printf("***************************************\n");
109     
110     while(flag)
111     {
112         char c;
113         printf("Please input command: ");
114         scanf("%c", &c);
115         switch(c)
116         {
117             case 'r':
118                 printf("Data to send: ");
119                 sem_p(semid);  /*访问资源*/
120                 scanf("%s", shm);
121                 sem_v(semid);  /*释放资源*/
122                 /*清空标准输入缓冲区*/
123                 while((c=getchar())!='\n' && c!=EOF);
124                 msg.mtype = 888;  
125                 msg.mtext = 'r';  /*发送消息通知服务器读数据*/
126                 msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
127                 break;
128             case 'q':
129                 msg.mtype = 888;
130                 msg.mtext = 'q';
131                 msgsnd(msqid, &msg, sizeof(msg.mtext), 0);
132                 flag = 0;
133                 break;
134             default:
135                 printf("Wrong input!\n");
136                 /*清空标准输入缓冲区*/
137                 while((c=getchar())!='\n' && c!=EOF);
138         }
139     }
140 
141     // 断开连接
142     shmdt(shm);
143 
144     return 0;
145 }
```

