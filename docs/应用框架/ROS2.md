---
title: ROS 2
---

# ROS2 基础

## 加载 ROS2 环境变量

```shell
$ echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
$ source ~/.bashrc
```





## 使用功能包组织节点

```shell
$ ros2 pkg create demo_cpp_pkg --build-type ament_cmake --license Apache-2.0
```

> ros2 pkg create 是用于创建功能包的命令，其中 demo_cpp_pkg 是功能包的名字
>
> --build-type ament_cmake 表示指定功能包的构建类型为 ament_cmake
>
> --license Apache-2.0 用于声明功能包的开源协议。

该命令在当前文件夹下创建了 demo_cpp_pkg 文件夹，并在其下创建了一些默认的文件和文件夹



## 构建功能包

```shell
$ colcon build
```

colcon 是 ROS2 中用于构建功能包的工具

该命令用于构建当前及子目录下所有的功能包

```shell
$ source install/setup.bash
$ ros2 run pkg node
```



## 查看节点详细信息

```shell
$ ros2 node info 节点
```



## 输出话题数据

```shell
$ ros2 topic echo 话题
```



## 查看某个话题具体信息

```shell
$ ros2 topic info 话题
```



## 查看消息接口的详细定义

```shell
$ ros2 interface show 接口
```



## 发布话题数据

```shell
$ ros2 topic pub 话题  消息接口  数据
```



## ROS2 消息接口支持的数据类型

```
bool
byte
char
float32、float64
int8、uint8
int16、uint16
int32、uint32
int64、uint64
string
```



# ROS2 节点



## 编写 ROS2 节点

​	推荐以继承的方式来创建节点对象,继承方式可以在一个进程内组织多个节点，这对于提高节点间通信效率（进程内通信 intra-process communication，不需要 DDS 拷贝内存，直接传指针）是很有帮助的，但是直接实例化则与该功能不兼容

```c++
#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("cpp_node"); // 创建名为cpp_node的ROS2节点
    RCLCPP_INFO(node->get_logger(), "你好 C++ 节点！");      // 日志打印宏
    rclcpp::spin(node);   // 启动节点并不断循环检测处理事件
    rclcpp::shutdown();   // 清理资源
    return 0;
}
```



cMake

```cmake
# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.

// 在此位置添加
# 1.查找 rclcpp 头文件和库
find_package(rclcpp REQUIRED)
# 2.添加可执行文件 cpp_node
add_executable(cpp_node src/cpp_node.cpp)
# 3.为 cpp_node 添加依赖
ament_target_dependencies(cpp_node rclcpp)
# 4.将 cpp_node 复制到 install 目录
install(TARGETS
cpp_node
DESTINATION lib/${PROJECT_NAME}
)

# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
```

> 首先添加了 find_package 和 add_executable 用于查找依赖以及添加可执行文件
>
> 然后采用 ament_cmake 提供的 ament_target_dependencies 指令来添加依赖
>
> 最后添加的是 install 指令，该指令将编译好的可执行文件复制到 install/demo_cpp_pkg/lib/demo_cpp_pkg 目录下，这样使用 ros2 run 才能找到该节点。



## 多功能包

选择一个功能包进行构建

```shell
$ colcon build --packages-select demo_cpp_pkg
```

添加依赖关系，在 package.xml 中添加

```
<depend>rclcpp</depend>
```





## ROS2 编程基础

### 面向对象编程

> 使用 class 关键字定义了 PersonNode 类，使其继承 rclcpp::Node
> 在类的内部定义了 private 部分，即姓名和年龄两个属性
> 在 public 部分定义了构造函数 PersonNode，并传入节点名称、姓名和年龄作为参数。
> 在构造函数后添加 :Node (node_name) 用于调用父类的构造函数，传递节点名称参数
> **RCLCPP_INFO 采用的是 C 风格的格式化输出，所以需要调用 c_str () 将字符串类型转换成 C 风格类型的字符串**

```C++
#include <string>
#include "rclcpp/rclcpp.hpp"

class PersonNode : public rclcpp::Node
{
private:
    std::string name_;
    int age_;

public:
    PersonNode(const std::string &node_name,
        const std::string &name,
        const int &age) : Node(node_name)  // 把节点名`node_name`传给父类
        {
            this->name_ = name;
            this->age_  = age;
        };

        void do(const std::string &food_name)
        {
            RCLCPP_INFO(this->get_logger(), "我是%s, 今年%d岁, 我现在正在学习%s", 
                name_.c_str(), age_, thing_name.c_str());
        }
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PersonNode>("cpp_node", "hxh", 20);
    node->do("ROS");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

```





### C++多线程编程

#### 1.安装库

先下载 C++ 的 HTTP 请求库 `cpp-httplib`

```
git clone https://gitee.com/ohhuo/cpp-httplib.git
```



#### 2.添加 cMake

指定 include 文件夹为头文件目录

```cmake
...
find_package(rclcpp REQUIRED)
include_directories(include)
...
ament_package()
```



#### 3.编写程序

```c++
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <cpp-httplib/httplib.h>

class Download
{
public:
    // 输入服务器地址、资源路径、引用传入回调函数对象
    void download(const std::string &host, const std::string &path, 
        const std::function<void(const std::string &, const std::string &)> &callback)
    {
        std::cout << "线程 ID: " << std::this_thread::get_id() << std::endl;
        httplib::Client client(host);
        auto response = client.Get(path);
        if (response && response->status == 200)
        {
            callback(path, response->body);
        }
    }

    void start_download(const std::string &host, const std::string &path, const 
        std::function<void(const std::string &, const std::string &)> &callback)
    {
        auto download_fun = std::bind(&Download::download, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        // 新建一个独立子线程
        std::thread download_thread(download_fun, host, path, callback);
        // 线程和主线程分离，子线程后台独立运行，主线程不用等待子线程结束
        download_thread.detach();
    }
};

int main()
{
    Download download;
    auto download_finish_callback = [](const std::string &path, const std::string &result) -> void
    {
        std::cout << "下载完成: " << path << " 共: " << result.length() << " 字, 内容为: "
                  << result.substr(0, 16) << std::endl;
    };

    download.start_download("http://localhost:8000", "/novel1.txt", download_finish_callback);
    download.start_download("http://localhost:8000", "/novel2.txt", download_finish_callback);
    download.start_download("http://localhost:8000", "/novel3.txt", download_finish_callback);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 10));
    return 0;
}
```

> 首先包含了线程相关头文件 thread
> 时间相关头文件 chrono
> 函数包装器头文件 functional 
> 用于下载的 cpp-httplib/httplib.h 头文件
>
> 然后声明了 Download 类
> 在其中添加了 download 函数和 start_download 函数。

C++ 的线程创建完之后就会直接运行



#### 4.添加节点相关配置

cMake

```cmake
find_package(Threads REQUIRED)
```







# 订阅和发布——话题通信



## 节点话题通信

```shell
$ ros2 node info /turlesim

Subscribers:
    /parameter_events: rcl_interfaces/msg/ParameterEvent
    /turtle1/cmd_vel: geometry_msgs/msg/Twist
Publishers:
    /parameter_events: rcl_interfaces/msg/ParameterEvent
    /rosout: rcl_interfaces/msg/Log
    /turtle1/color_sensor: turtlesim/msg/Color
    /turtle1/pose: turtlesim/msg/Pose
Service Servers:
    /clear: std_srvs/srv/Empty
    /kill: turtlesim/srv/Kill
    /reset: std_srvs/srv/Empty
    /spawn: turtlesim/srv/Spawn
    /turtle1/set_pen: turtlesim/srv/SetPen
    /turtle1/teleport_absolute: turtlesim/srv/TeleportAbsolute
    /turtle1/teleport_relative: turtlesim/srv/TeleportRelative
    /turtlesim/describe_parameters: rcl_interfaces/srv/DescribeParameters
    /turtlesim/get_parameter_types: rcl_interfaces/srv/GetParameterTypes
    /turtlesim/get_parameters: rcl_interfaces/srv/GetParameters
    /turtlesim/get_type_description: type_description_interfaces/srv/GetTypeDescription
    /turtlesim/list_parameters: rcl_interfaces/srv/ListParameters
    /turtlesim/set_parameters: rcl_interfaces/srv/SetParameters
    /turtlesim/set_parameters_atomically: rcl_interfaces/srv/SetParametersAtomically
Service Clients:

Action Servers:
    /turtle1/rotate_absolute: turtlesim/action/RotateAbsolute
Action Clients:
```

其中 `Subscribers` 部分是 `/turtlesim` 节点订阅的所有话题
可以看出该节点订阅了名为 `/turtle1/cmd_vel` 的话题，用于接收控制指令，它的消息接口是 `geometry_msgs/msg/Twist`

`Publishers` 部分下是该节点发布的所有话题，需要关注的话题是 `/turtle1/pose`
该话题用于发布当前海龟的位置和速度，它的消息接口是 `turtlesim/msg/Pose`   下面是服务和动作相关的信息



### 订阅

尝试使用命令订阅 `/turtle1/pose`，实时输出海龟当前的位姿

```shell
$ ros2 topic echo /turtle1/pose

x: 5.544444561004639
y: 5.544444561004639
theta: 0.0             # 朝向
linear_velocity: 0.0   # 线速度：前进的速度
angular_velocity: 0.0  # 角速度：海龟绕自身旋转的速度
---
```



### 发布

发布数据前需要确认消息接口

```shell
$ ros2 topic info /turtle1/cmd_vel -v

Type: geometry_msgs/msg/Twist

Publisher count: 0

Subscription count: 1

Node name: turtlesim
Node namespace: /
Topic type: geometry_msgs/msg/Twist
Topic type hash: RIHS01_9c45bf16fe0983d80e3cfe750d6835843d265a9a6c46bd2e609fcddde6fb8d2a
Endpoint type: SUBSCRIPTION
GID: 01.0f.57.66.08.25.19.52.00.00.00.00.00.00.1d.04
QoS profile:
  Reliability: RELIABLE
  History (Depth): UNKNOWN
  Durability: VOLATILE
  Lifespan: Infinite
  Deadline: Infinite
  Liveliness: AUTOMATIC
  Liveliness lease duration: Infinite
```

根据输出，**消息接口**为 Topic type: `geometry_msgs/msg/Twist`

`geometry_msgs/msg/Twist`定义内容如下，==消息接口规定了数据的格式，不同的 topic 可以用相同的消息接口==

```shell
$ ros2 interface show geometry_msgs/msg/Twist

# This expresses velocity in free space broken into its linear and angular parts.

Vector3  linear   # 线速度
	float64 x
	float64 y
	float64 z
Vector3  angular  # 角速度
	float64 x
	float64 y
	float64 z
```



**命令发布线速度**

```shell
$ ros2 topic pub /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 1.0}}"

publisher: beginning loop
publishing #1: geometry_msgs.msg.Twist(linear=geometry_msgs.msg.Vector3(x=1.0, y=0.0, z=0.0), 
	angular=geometry_msgs.msg.Vector3(x=0.0, y=0.0, z=0.0))
```

大括号用于区分消息结构层级，冒号后需要添加空格用于区分。



## C++ 话题订阅与发布

```C++
template<typename MessageT>    // 创建订阅者
typename Publisher<MessageT>::SharedPtr
create_publisher(
    const std::string & topic_name,
    size_t qos,     // 队列长度
    const rclcpp::QoS & qos_profile = rclcpp::QoS(10),  // 不填默认10
    rclcpp::CallbackGroup::SharedPtr callback_group = nullptr
);

// 创建定时器
TimerBase::SharedPtr create_wall_timer(
  std::chrono::duration<long double, std::milli> period,  // 周期
  std::function<void()> callback,
  CallbackGroup::SharedPtr callback_group = nullptr
);
```

程序编写

```C++
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>  // 引入时间相关头文件

// 使用时间单位的字面量，可以在代码中使用 s 和 ms 表示时间
using namespace std::chrono_literals;

class TurtleCircle : public rclcpp::Node
{
private:
    rclcpp::TimerBase::SharedPtr timer_;     // 定时器智能指针
    // 发布者智能指针  geometry_msgs::msg::Twist ： 话题接口类型
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

public:
    explicit TurtleCircle(const std::string& node_name) : Node(node_name)
    {
        // 调用继承而来的父类函数创建订阅者
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        // 调用继承而来的父类函数创建定时器 std::bind: 包装后生成一个全新无参函数对象
        timer_ = this->create_wall_timer(1000ms, std::bind(&TurtleCircle::timer_callback, this));
    }

private:
    void timer_callback()
    {
        auto msg = geometry_msgs::msg::Twist();
        //  ↓         ↓                      ↓
        // 自动类型   包名::msg::消息类型     构造函数
        msg.linear.x  = 1.0;
        msg.angular.z = 0.5;
        publisher_->publish(msg);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleCircle>("turtle_square");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```

cMake

```cmake
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
include_directories(include)
add_executable(turtle_circle src/turtle_circle.cpp)
ament_target_dependencies(turtle_circle rclcpp geometry_msgs)
install(TARGETS
turtle_circle
DESTINATION lib/${PROJECT_NAME}
)
```





## 订阅 Pose 实现闭环控制

```C++
/* 订阅小乌龟实时位置，根据位置实时控制小乌龟 */
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"

class TurtleController : public rclcpp::Node 
{
public:
    TurtleController() : Node("turtle_controller") {
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        pose_subscription_  = this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose", 10, 
            std::bind(&TurtleController::on_pose_received_, this, std::placeholders::_1));
    }

private:
    void on_pose_received_(const turtlesim::msg::Pose::SharedPtr pose)
    {
        // TODO: 收到位置计算我误差，发布速度指令
        auto message = geometry_msgs::msg::Twist();

        // 1. 记录当前位置
        double current_x = pose->x;
        double current_y = pose->y;
        RCLCPP_INFO(this->get_logger(), "当前位置：(x=%f, y=%f)", current_x, current_y);

        // 2. 计算与目标之间的距离，以及与当前海龟朝向的角度差
        double distance = std::sqrt((target_x_ - current_x) * (target_x_ - current_x) + 
            (target_y_ - current_y) * (target_y_ - current_y));
        double angle =  std::atan2(target_y_ - current_y, target_x_ - current_x) - pose->theta;

        // 3.控制策略：距离大于0.1继续运动，角度差大于0.2则原地旋转，否则直行
        if (distance > 0.1) {
            if (fabs(angle) > 0.2)
            {
                message.angular.z = fabs(angle);
            }else{
                // 通过比例控制器计算输出速度
                message.linear.x = k_ * distance;
            }
        }

        // 4.限制最大值并发布消息
        if (message.linear.x > max_speed_) {
            message.linear.x = max_speed_;
        }
        velocity_publisher_->publish(message);
    }

    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    double target_x_{1.0};   // 目标位置x, 设置默认值1.0
    double target_y_{1.0};   // 目标位置y, 设置默认值1.0
    double k_{1.0};          // 比例系数, 控制输出 = 误差 × 比例系数
    double max_speed_{3.0};  // 最大线速度, 设置默认值3.0
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```





# 话题通信实践

> 第一，通过这个小工具可以看到系统的实时状态信息，包括记录信息的时间、主机名称、CPU 使用率、内存使用率、内存总大小、剩余内存、网络接收数据量和网络发送数据量； 
> 第二，要有一个简单的界面，可以将系统信息显示出来；
> 第三，要能在局域网内其他主机上查看数据（共享数据）。



## 自定义通信接口

### 创建接口功能包

```shell
$ ros2 pkg create status_interfaces --build-type ament_cmake --dependencies rosidl_default_generators builtin_interfaces --license Apache-2.0
```

​	上面的指令用于创建一个 status_interfaces 功能包（构建类型为 ament_cmake），并为其添加 builtin_interfaces 和 rosidl_default_generators 两个依赖。builtin_interfaces 是 ROS 2 中已有的一个消息接口功能包，可以使用其时间接口 Time，表示记录信息的时间。rosidl_default_generators 用于将自定义的消息文件转换为 C++、Python 源码的模块。



### 创建消息接口文件

- 在 ROS 2 中，**话题消息定义文件需要放置到功能包的 msg 目录下**，文件名必须以大写字母开头且只能由大、小写字母及数字组成。

- 接着在功能包下创建 msg 目录，并在该目录下新建文件 SystemStatus.msg（消息接口定义文件）

  ```
  builtin_interfaces/Time stamp  # 记录时间戳
  string host_name               # 系统名称
  float32 cpu_percent            # CPU使用率
  float32 memory_percent         # 内存使用率
  float32 memory_total           # 内存总量
  float32 memory_available       # 剩余有效内存
  float64 net_sent               # 网络发送数据总量
  float64 net_recv               # 网络接收数据总量
  ```

> 定义好数据接口文件后，需要在 cMakeLists.txt 中注册该文件，声明其为消息接口文件，并为其添加 builtin_interfaces 依赖

src/status_interfaces/CMakeLists.txt

```cmake
# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
	"msg/SystemStatus.msg"
	DEPENDENCIES builtin_interfaces
)
```

package.xml 中添加声明

```xml
<license>Apache-2.0</license>
<member_of_group>rosidl_interface_packages</member_of_group>
<buildtool_depend>ament_cmake</buildtool_depend>
```

> 在 package.xml 中添加 member_of_group 是为了**声明该功能包是一个消息接口功能包**，方便 ROS 2 对其做额外处理

**查看消息接口是否构建成功**

```shell
hxh123456@hxh123456-VMware-Virtual-Platform:~/chapt3/topic_practice_ws$ ros2 interface show status_interfaces/msg/SystemStatus 
builtin_interfaces/Time stamp  # 记录时间戳
        int32 sec
        uint32 nanosec
string host_name               # 系统名称
float32 cpu_percent            # CPU使用率
float32 memory_percent         # 内存使用率
float32 memory_total           # 内存总量
float32 memory_available       # 剩余有效内存
float64 net_sent               # 网络发送数据总量
float64 net_recv               # 网络接收数据总量
```





## 系统信息获取与发布

使用接口来传递数据

在 src 目录下，创建 status_publisher 功能包，并添加消息接口 status_interfaces 和客户端库 rclcpp 作为其依赖。

```shell
$ ros2 pkg create status_publisher --build-type ament_cmake --dependencies rclcpp status_interfaces --license Apache-2.0
```

```C++
#include <rclcpp/rclcpp.hpp>
#include <status_interfaces/msg/system_status.hpp>
#include <chrono>
#include <string>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std::chrono_literals;
using SystemStatus = status_interfaces::msg::SystemStatus;

// 获取CPU使用率，等价psutil.cpu_percent()
float get_cpu_percent()
{
    static long long pre_total = 0, pre_idle = 0;
    std::ifstream stat("/proc/stat");
    std::string buf;
    stat >> buf;
    std::vector<long long> vals(10);
    for(int i=0;i<10;i++) stat >> vals[i];
    long long idle = vals[3];
    long long total = 0;
    for(auto v:vals) total += v;
    long long delta_total = total - pre_total;
    long long delta_idle = idle - pre_idle;
    float usage = 100.0f * (delta_total - delta_idle) / delta_total;
    pre_total = total;
    pre_idle = idle;
    return usage;
}

// 获取网络收发总字节，等价psutil.net_io_counters()
void get_net_bytes(uint64_t &sent, uint64_t &recv)
{
    std::ifstream net("/proc/net/dev");
    std::string line;
    sent = 0; recv = 0;
    std::getline(net, line); std::getline(net, line);
    while(std::getline(net, line)){
        std::istringstream iss(line);
        std::string ifname;
        uint64_t r_bytes, r_pkt, r_err, r_drop, s_bytes;
        iss >> ifname >> r_bytes >> r_pkt >> r_err >> r_drop;
        for(int i=0;i<4;i++) iss >> s_bytes;
        recv += r_bytes;
        sent += s_bytes;
    }
}

class SysStatusPub : public rclcpp::Node
{
public:
    SysStatusPub(std::string node_name) : Node(node_name)
    {
        status_publisher_ = this->create_publisher<SystemStatus>("sys_status", 10);
        timer = this->create_wall_timer(1s, std::bind(&SysStatusPub::timer_callback, this));
    }

private:
    rclcpp::Publisher<SystemStatus>::SharedPtr status_publisher_;
    rclcpp::TimerBase::SharedPtr timer;

    void timer_callback()
    {
        // CPU
        float cpu_percent = get_cpu_percent();
        // 内存信息
        struct sysinfo mem;
        sysinfo(&mem);
        float mem_total = (float)mem.totalram * mem.mem_unit / 1024.0 / 1024.0;
        float mem_avail = (float)mem.freeram * mem.mem_unit / 1024.0 / 1024.0;
        float mem_percent = (mem_total - mem_avail)/mem_total * 100.0f;
        // 网络
        uint64_t net_sent_byte, net_recv_byte;
        get_net_bytes(net_sent_byte, net_recv_byte);
        double net_sent = (double)net_sent_byte / 1024.0 / 1024.0;
        double net_recv = (double)net_recv_byte / 1024.0 / 1024.0;
        // 主机名
        struct utsname uts;
        uname(&uts);

        SystemStatus msg;
        msg.stamp = this->get_clock()->now();
        msg.host_name = uts.nodename;
        msg.cpu_percent = cpu_percent;
        msg.memory_percent = mem_percent;
        msg.memory_total = mem_total;
        msg.memory_available = mem_avail;
        msg.net_sent = net_sent;
        msg.net_recv = net_recv;

        RCLCPP_INFO_STREAM(this->get_logger(), "发布: "
            << "主机名=" << msg.host_name
            << " CPU使用率=" << msg.cpu_percent
            << "% 内存使用率=" << msg.memory_percent << "%");
        status_publisher_->publish(msg);
    }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SysStatusPub>("sys_status_pub");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```

cMake

```cmake
add_executable(sys_status_pub src/sys_status_pub.cpp)
ament_target_dependencies(sys_status_pub
  rclcpp
  status_interfaces
)
install(TARGETS
sys_status_pub
DESTINATION lib/${PROJECT_NAME}
)
```





## 在功能包中使用 QT
