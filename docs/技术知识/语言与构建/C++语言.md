---
title: C++ 语言
---

### 函数增强

- **函数重载**：同一作用域内，函数名相同但参数列表不同

  ```C++
  void show(int a)        { std::cout<<"int:"<<a; }
  void show(double a)     { std::cout<<"double:"<<a; }
  void show(int a,int b) { std::cout<<"两个int:"<<a<<b; }
  译器靠传入的实参自动选对应函数。
  ```

- **默认参数**：函数参数可以指定默认值

  ```
  void show(int a, int b = 10)
  ```

- **内联函数**：`inline`关键字，替代 C 的宏函数，类型安全

- C 语言用 **宏（#define）** 宏没有类型检查、没有优先级，不安全

  C++ 用 **inline 内联函数**，**又快又安全**

  ```C++
  inline int add(int a, int b) {
      return a + b;
  }
  有类型检查（不会乱替换）
  有优先级（不会算错）
  速度和宏一样快
  写法和普通函数一样
  ```

- **函数模板**：泛型函数，支持多种类型参数

  ```C++
  template <typename T>   // 模板声明
  T 函数名(T a, T b)      // T 代表“任意类型” int、char、double...
  {
      return a > b ? a : b;
  }
  ```







## 面向对象编程（OOP）核心

这是 C++ 与 C 语言最本质的区别，引入了**类和对象**的概念，支持封装、继承、多态三大特性。

### 1 类与对象

- **类（class）**：用户自定义的数据类型，封装了数据（成员变量）和操作数据的函数（成员函数）

- **对象（object）**：类的实例

- 访问控制：

  - `public`：公有成员，类内外都可访问
  - `private`：私有成员，只有类内部可访问
  - `protected`：保护成员，类内部和派生类可访问

- 成员函数可以在类内定义，也可以在类外定义（使用`类名::函数名`）

  ```
  // 1. 先在类里声明
  class Person {
  public:
      void show();
  };
  
  // 2. 在类外定义，写 Person::
  void Person::show()
  {
      // 函数体
  }
  ```

### 2 构造函数与析构函数

- 构造函数：**对象创建时自动调用，用于初始化成员变量**

  - 与类同名，无返回值
  - 可以重载（同一个名字，用不同的参数，实现不同的功能 ）
  - 默认构造函数（无参数）
  - 拷贝构造函数（用已有对象初始化新对象，拷贝的是成员的当前值）
  - 移动构造函数（C++11，把临时对象的资源转移给新对象, 效率更高，不用拷贝）
  
  ```
  class Person {
  public:
      // 构造函数  构造函数的名字 = 类名
      Person()  
      {}
       // 2. 有参构造（重载）
      Person(int a)
      {}
      
       Person(Person && temp)  // temp 是临时对象
      {
          // 1. 直接拿走临时对象的指针
          this -> age = temp.age;
  
          // 2. 把临时对象指针置空
          temp.age = nullptr;
      }
  };
  
  // 返回临时对象
  Person createTemp()
  {
      return Person();
  }
  
  int main()
  {
      Person p;  // 一创建对象 p，自动调用 Person()
      
      Person p1;      // 调用无参构造
      Person p2(20);  // 调用有参构造
      
      Person p1(20);
  	Person p2 = p1;  // 调用拷贝构造
  	
  	Person p = createTemp(); // 调用移动构造
      return 0;
  }
  ```
  
- 析构函数

  ：对象销毁时自动调用，用于释放资源，清理动态内存

  - 名称为`~类名()`，无参数，无返回值
  - 不能重载
  - 当类中有动态分配的内存时，必须显式定义析构函数

  ```
  class Person
  {
  public:
      // 构造函数
      Person()
      {}
  
      // 析构函数
      ~Person()
      {}
  };
  ```

### 3 封装

- 将数据和操作数据的方法捆绑在一起
- 通过访问控制隐藏内部实现细节，只暴露必要的接口
- 提高代码的**安全性**和可维护性

### 4 继承

- 允许一个类（派生类 / 子类）继承另一个类（基类 / 父类）的属性和方法

- 语法：`class 派生类 : 继承方式 基类`

- 继承方式：
  
  - `public`继承：父类的 public 和 protected 成员在子类中保持原有访问权限
    父类 **public** → 子类 **public**
  
    父类 **protected** → 子类 **protected**
  
    父类 **private** → 子类 **不可访问**
  
  - `protected`继承：父类的 public 和 protected 成员在子类中变为 protected
  
    父类 **public** → 子类 **protected**
  
    父类 **protected** → 子类 **protected**
  
  - `private`继承：父类的所有成员在子类中变为 private
    父类所有东西 → 子类全部变成 **private**
  
- 派生类可以**添加新的成员**，也可以**重写基类**的成员函数

- 多继承：一个类可以同时继承多个基类

- 子类的构造函数**必须**先调用父类的构造函数

```
class Widget : public QWidget
{
    Q_OBJECT
public:
    // 父类QWidget的构造要求传入父窗口指针
    explicit Widget(QWidget *parent = nullptr);
};
```

### 5 多态

同一接口，不同实现。**用父类指针 / 引用调用函数，运行时自动执行子类重写的版本**。

分为两类：

1. **静态多态（编译期）**：函数重载、运算符重载，编译时就确定调用哪个函数。
2. **动态多态（运行期）**：**虚函数实现**，运行时根据**对象实际类型**决定调用版本，日常说的多态一般指它。

#### 虚函数（Virtual Functions）：

- 在基类中声明一个函数为虚函数，使用关键字`virtual`。
- 派生类可以重写（override）这个虚函数。
- 调用虚函数时，会根据对象的实际类型来决定调用哪个版本的函数。

#### 多态的实现机制：

- 虚函数表（V-Table）：C++运行时使用虚函数表来实现多态。每个包含虚函数的类都有一个虚函数表，表中存储了指向类中所有虚函数的指针。
- 虚函数指针（V-Ptr）：对象中包含一个指向该类虚函数表的指针。

#### 使用多态的优势：

- **代码复用**：通过基类指针或引用，可以操作不同类型的派生类对象，实现代码的复用。
- **扩展性**：新增派生类时，不需要修改依赖于基类的代码，只需要确保新类正确重写了虚函数。
- **解耦**：多态允许程序设计更加模块化，降低类之间的耦合度。

```
#include <iostream>
using namespace std;

// 基类
class Animal
{
public:
    // 虚函数
    virtual void cry()
    {
        cout << "动物发出叫声" << endl;
    }
};

// 子类1：继承 Animal
class Dog : public Animal
{
public:
    // 重写虚函数
    void cry() override
    {
        cout << "小狗汪汪叫" << endl;
    }
};

// 子类2：继承 Animal
class Cat : public Animal
{
public:
    void cry() override
    {
        cout << "小猫喵喵叫" << endl;
    }
};

// 接收基类引用
void makeSound(Animal& ani)
{
    ani.cry(); // 多态触发点
}

int main()
{
    Dog dog;
    Cat cat;

    // 基类引用指向子类对象，调用同一个接口，执行不同代码
    makeSound(dog);
    makeSound(cat);

    return 0;
}
```



### 6 运算符重载

重定义或重载大部分 C++ 内置的运算符，这样就能使用自定义类型的运算符

```
Box operator+(const Box&)；
```

声明加法运算符用于把两个 Box 对象相加，返回最终的 Box 对象

```
Box operator+(const Box&， const Box&)
```

```
#include <iostream>
using namespace std;

class Box {
public:
    // 构造函数：初始化长、宽、高
    Box(double l, double w, double h) : length(l), width(w), height(h) {}

    // 【核心】重载 + 运算符（成员函数）
    Box operator+(const Box& b) {
        // 规则：新盒子的长宽高 = 两个盒子对应维度相加
        Box newBox(
            this->length + b.length,
            this->width + b.width,
            this->height + b.height
        );
        return newBox; // 返回相加后的新对象
    }

    // 计算体积
    double getVolume() {
        return length * width * height;
    }

private:
    double length;  // 长
    double width;   // 宽
    double height;  // 高
};

// 测试
int main() {
    Box box1(2, 3, 4);   // 盒子1
    Box box2(1, 1, 1);   // 盒子2

    // 直接使用 + 号相加两个对象！
    Box box3 = box1 + box2;

    cout << "盒子1体积：" << box1.getVolume() << endl;          体积为24
    cout << "盒子2体积：" << box2.getVolume() << endl;          体积为1
    cout << "相加后盒子体积：" << box3.getVolume() << endl;      体积为60

    return 0;
}
```

#### 可重载运算符

| 分类        | 运算符                                                |
| ----------- | ----------------------------------------------------- |
| 双目算术    | `+` `-` `*` `/` `%`                                   |
| 关系        | `==` `!=` `<` `>` `<=` `>=`                           |
| 逻辑        | `&&` `!`                                              |
| 单目        | `+` `-` `*` `&`                                       |
| 自增 / 自减 | `++` `--`                                             |
| 位运算      | `&` `^` `~` `<<` `>>`                                 |
| 赋值        | `=` `+=` `-=` `*=` `/=` `%=` `&=`  =` `^=` `<<=` `>>= |
| 内存管理    | `new` `delete` `new[]` `delete[]`                     |
| 其他        | `()` `->` `[]` `,`                                    |

#### 不可重载运算符

| 运算符   | 名称               |
| -------- | ------------------ |
| `.`      | 成员访问运算符     |
| `.*`     | 成员指针访问运算符 |
| `->*`    | 成员指针访问运算符 |
| `::`     | 域解析运算符       |
| `sizeof` | 长度运算符         |
| `?:`     | 条件运算符（三目） |
| `#`      | 预处理符号         |

------

###  explicit

`explicit` 是C++中的一个关键字，***\*它用来修饰只有一个参数的类构造函数，以表明该构造函数是显式的，而非隐式的。\****
当使用explicit修饰构造函数时，它将***\*禁止类对象之间的隐式转换，以及禁止隐式调用拷贝构造函数。\****







## 泛型编程与模板

泛型编程是 C++ 的另一大核心特性，允许编写与类型无关的通用代码，提高代码的复用性。

### 1 函数模板

- 语法：`template <typename T> 返回类型 函数名(参数列表) { 函数体 }`
- `typename`可以用`class`替代
- 编译器会根据实际参数类型生成对应的函数实例

```C++
// 函数模板定义
template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    // 自动生成 int 版本
    cout << add(1, 2) << endl;
    
    // 自动生成 double 版本
    cout << add(1.5, 2.5) << endl;
    
    return 0;
}
```

### 2 类模板

- 语法：`template <typename T> class 类名 { 类定义 };`
- 类模板的成员函数通常也需要定义为模板函数
- 使用时需要显式指定模板参数：`类名<类型> 对象名;`

```C++
// 类模板定义
template <typename T>
class Box {
private:
    T val;
public:
    void set(T v) { val = v; }
    T get() { return val; }
};

int main() {
    Box<int> b1;        // 必须写 <int>
    b1.set(100);
    cout << b1.get();   // 输出100
    return 0;
}
```

### 3 模板特化

**现有通用模板才能写特化模板**

- 针对特定类型提供模板的特殊实现
- 全特化：针对所有模板参数提供特化
- 偏特化：针对部分模板参数提供特化

```C++
// 通用版本
template <typename T>
void print (T t) { cout << t; }

// 特化：只给 bool 特殊处理
template <>                         // 特殊处理不需要占位符
void print <bool> (bool t) {
    cout << (t ? "true" : "false");
}
```

### 4 可变参数模板（C++11）

- 允许模板接受任意数量和类型的参数

- 语法：`template <typename... Args> 返回类型 函数名(Args... args) { 函数体 }`

  **`...`**：代表**一包参数**（任意个数、任意类型）

  **`Args`**：类型的包（比如 `int, string, double`）

  **`args`**：数据的包（比如 `10, "abc", 3.14`）

```C++
template <typename... Args>
void print (Args... args){
	(count << ... << args) << end1；
}

int main() {
    print(1);            // 1个参数
    print(1, 2.5);       // 2个参数
    print(1, "A", 3.14); // 3个参数
    return 0;
}
```









## 异常处理

C++ 提供了完善的异常处理机制，用于处理程序运行时的错误。

### 1 基本语法

- 抛出异常：`throw 异常对象;`

- 捕获异常：

  ```C++
  try {
      // 可能抛出异常的代码
  } catch (异常类型1 e) {
      // 处理异常类型1
  } catch (异常类型2 e) {
      // 处理异常类型2
  } catch (...) {
      // 处理所有其他异常
  }
  ```

### 2 异常类型

- 可以抛出任何类型的对象，包括基本类型、自定义类型
- 标准异常类：`std::exception`及其派生类，如`std::runtime_error`、`std::logic_error`等

### 3 异常规范（C++11 已弃用，C++17 移除）

- `throw()`：表示函数不会抛出任何异常
- `noexcept`（C++11）：表示函数不会抛出异常，编译器会进行优化

------

## 现代 C++ 特性（C++11 及以后）

C++11 是 C++ 发展史上的一个重要里程碑，引入了大量新特性，极大地改变了 C++ 的编程风格。后续的 C++14、C++17、C++20 等版本又不断完善和扩展。

### 1 智能指针（Smart Pointers）

- 解决 C 语言中手动管理内存导致的内存泄漏、野指针等问题
- `unique_ptr`（C++11）：独占所有权的智能指针
- `shared_ptr`（C++11）：共享所有权的智能指针，使用引用计数
- `weak_ptr`（C++11）：弱引用，不增加引用计数，用于解决 shared_ptr 的循环引用问题

### 2 右值引用与移动语义（C++11）

- 右值：临时对象、字面量等，不能取地址
- 右值引用：`类型 &&引用名 = 右值;`
- 移动语义：将资源从一个对象转移到另一个对象，避免不必要的拷贝
- `std::move`：将左值转换为右值引用

### 3 Lambda 表达式（C++11）

- 匿名函数，用于编写简短的函数对象
- 语法：`[捕获列表](参数列表) -> 返回类型 { 函数体 }`
- 捕获列表：可以捕获外部变量
- 常用于 STL 算法的参数

捕获列表速查

| 写法     | 含义                       |
| :------- | :------------------------- |
| `[]`     | 不捕获任何外部变量         |
| `[&]`    | 以引用方式捕获所有外部变量 |
| `[=]`    | 以值方式捕获所有外部变量   |
| `[&obj]` | 只捕获 `obj`，用引用       |
| `[=obj]` | 只捕获 `obj`，用值         |



### 4 范围 for 循环（C++11）

- 简化容器的遍历
- 语法：`for (auto &元素 : 容器) { 循环体 }`

### 5 类型推导增强

- `auto`：自动推导变量类型
- `decltype`（C++11）：推导表达式的类型
- `auto`返回值（C++14）：函数返回值自动推导

### 6 结构化绑定（C++17）

- 将元组、结构体等的成员绑定到多个变量
- 语法：`auto [a, b, c] = 元组/结构体;`

### 7 概念与约束（C++20）

- 用于限制模板参数的类型
- 语法：`template <概念名 T> 返回类型 函数名(参数列表) { 函数体 }`
- 提高模板代码的可读性和错误信息的清晰度

### 8 协程（C++20）

- 用于编写异步代码
- 语法：`co_await`、`co_yield`、`co_return`
- 简化异步编程模型







## 其他重要特性

### 1 内存管理

- ```C++
  new
  ```

  ```C++
  delete
  ```

  运算符：替代 C 的

  ```C++
  malloc
  ```

  ```c++
  free
  ```

  - `new`会调用构造函数，`delete`会调用析构函数
  - `new[]`/`delete[]`：用于数组

- 定位 new：在已分配的内存上构造对象

### 2 类型转换

- C++ 提供了四种显式类型转换运算符，替代 C 的强制类型转换
  - `static_cast`：静态转换，用于基本类型之间的转换、类层次结构中基类和派生类之间的转换
  - `dynamic_cast`：动态转换，用于类层次结构中基类和派生类之间的安全转换，运行时检查
  - `const_cast`：用于移除`const`属性
  - `reinterpret_cast`：重新解释类型，用于指针之间的转换、指针和整数之间的转换

### 3 枚举类（enum class）（C++11）

- 强类型枚举，避免命名冲突
- 语法：`enum class 枚举名 { 枚举值1, 枚举值2, ... };`
- 使用时需要指定枚举名：`枚举名::枚举值`

### 4 constexpr（C++11）

- 编译期常量表达式
- 可以用于变量、函数、构造函数
- 提高程序的性能

### 5 线程支持（C++11）

- C++ 标准库提供了线程支持
- `std::thread`：线程类
- 同步机制：`std::mutex`、`std::condition_variable`、`std::future`等

------

## C++ 与 C 的关键区别总结

| 特性       | C 语言                 | C++                                      |
| :--------- | :--------------------- | :--------------------------------------- |
| 编程范式   | 面向过程               | 面向过程、面向对象、泛型编程、函数式编程 |
| 命名空间   | 无                     | 有（namespace）                          |
| 输入输出   | printf/scanf           | iostream（cin/cout）                     |
| 函数       | 不支持重载、默认参数   | 支持重载、默认参数、内联函数             |
| 引用       | 无                     | 有（&）                                  |
| 布尔类型   | 无（用 int 代替）      | 有（bool）                               |
| 空指针     | NULL（宏）             | nullptr（关键字）                        |
| 类与对象   | 无                     | 有（class/struct）                       |
| 继承与多态 | 无                     | 有                                       |
| 模板       | 无                     | 有                                       |
| 标准库     | 简单，主要是 C 标准库  | 丰富，包含 STL                           |
| 异常处理   | 无（用返回值表示错误） | 有（try/catch/throw）                    |
| 内存管理   | malloc/free            | new/delete、智能指针                     |
| 类型转换   | 强制类型转换           | 四种显式类型转换运算符                   |



## C++ 补充

### this

`this` 是一个**指针**，它永远指向**当前正在执行成员函数的那个对象**。



### std::bind - 参数绑定器（C++11）

> 头文件：`#include <functional>`
>
> 作用：**固定函数参数，生成新函数。**

`_1, _2, _3...` 表示调用时的第 N 个参数。

```C++
std::bind(print, 10, _1);           // 固定参数
std::bind(func, _2, _1);            // 调整顺序  
std::bind(&My::method, &obj, _1);   // 绑定成员函数
```

```C++
// std::bind 版本
auto f1 = std::bind(&My::process, &obj, _2, 42, _1);
f1(10, 20);  // obj.process(20, 42, 10)

// Lambda 版本
auto f2 = [&](int a, int b) { 
    return obj.process(b, 42, a); 
};
f2(10, 20);  // 同样调用 obj.process(20, 42, 10)
```



### 结构化绑定

```C++
auto [变量1, 变量2] = pair{1, 2};     // 复制
auto& [a, b] = map.find(key);        // 引用（可修改）
const auto& [x, y] = tuple{1, 2, 3}; // 只读
```



### 常用场景

```C++
// 1. 遍历 map
for (auto& [key, val] : mp)
    cout << key << ": " << val;
}

// 2. 忽略不用的值
auto [_, cnt] = mp.insert({3, "c"}); // _ 是占位符

// 3. 接收函数返回值
auto [x, y] = getPos();
```





## 八股



**const 成员函数**

- const 成员函数内，不能修改普通成员变量
- const 成员函数只能调用 const 成员函数，不能调用普通（非 const）成员函数
- 普通对象：既可以调用普通成员函数，也可以调用 const 成员函数
- const 对象 /const 引用 /const 指针：只能调用 const 成员函数
- `mutable` 修饰成员变量，可以在 const 成员函数中修改



静态成员函数不能声明为虚函数：静态成员函数属于类而不是对象，编译时就确定了地址

内联函数不能声明为虚函数：内联函数在编译时就被展开到调用处，虚函数是运行时绑定

构造函数不能声明为虚函数：构造函数要先初始化 vtable，vptr 
