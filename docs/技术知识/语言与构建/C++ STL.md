---
title: C++ STL
---

------

# C++ STL 常用容器与算法

> 适用标准：C++17 / C++20

------

## 容器概览图

| 容器类型             | 容器名称           | 底层结构         | 主要特点                        |
| :------------------- | :----------------- | :--------------- | :------------------------------ |
| **序列容器**         | `vector`           | 动态数组         | 尾部快，中间慢，随机访问快      |
|                      | `deque`            | 双端数组         | 头尾快，中间慢，随机访问快      |
|                      | `list`             | 双向链表         | 中间插入快，不支持随机访问      |
|                      | `forward_list`     | 单向链表         | 比list更节省内存，只能单向遍历  |
|                      | `array`            | 静态数组         | 固定大小，栈上分配              |
| **关联容器（有序）** | `set` / `multiset` | 红黑树           | 自动排序，去重/可重复，O(log n) |
|                      | `map` / `multimap` | 红黑树           | 键值对，自动按key排序           |
| **关联容器（无序）** | `unordered_set`    | 哈希表           | 平均O(1)，元素无序              |
|                      | `unordered_map`    | 哈希表           | 平均O(1)，键值对无序            |
| **容器适配器**       | `stack`            | 默认deque        | LIFO（后进先出）                |
|                      | `queue`            | 默认deque        | FIFO（先进先出）                |
|                      | `priority_queue`   | 默认vector（堆） | 最大值始终在顶部                |

------

## 序列容器详解

### vector —— 动态数组（最常用）

```c++
#include <vector>
vector<int> v;                // 空vector
vector<int> v(5, 10);         // 5个元素，值均为10
vector<int> v = {1, 2, 3};    // 初始化列表
```

**常用操作**

| 操作                     | 说明                  | 时间复杂度 |
| :----------------------- | :-------------------- | :--------- |
| `v.push_back(x)`         | 尾部添加元素          | O(1)       |
| `v.pop_back()`           | 尾部删除元素          | O(1)       |
| `v.insert(it, x)`        | 在迭代器位置插入      | O(n)       |
| `v.erase(it)`            | 删除迭代器位置元素    | O(n)       |
| `v[i]` / `v.at(i)`       | 下标访问 / 带边界检查 | O(1)       |
| `v.front()` / `v.back()` | 首/尾元素             | O(1)       |
| `v.size()`               | 元素个数              | O(1)       |
| `v.capacity()`           | 当前容量              | O(1)       |
| `v.reserve(n)`           | 预分配容量            | O(n)       |
| `v.shrink_to_fit()`      | 释放多余容量          | O(n)       |

>  **注意**：`v.at(i)` 越界会抛出 `std::out_of_range` 异常，`v[i]` 则不会检查。

**遍历方式**

```c++
// 范围for（推荐）
for (int x : v) cout << x << " ";

// 迭代器
for (auto it = v.begin(); it != v.end(); ++it) cout << *it << " ";

// 下标
for (size_t i = 0; i < v.size(); ++i) cout << v[i] << " ";
```

------





### deque —— 双端队列

```c++
#include <deque>
deque<int> dq = {1, 2, 3};
```

| 操作                                   | 说明      |
| :------------------------------------- | :-------- |
| `dq.push_front(x)` / `dq.push_back(x)` | 头/尾添加 |
| `dq.pop_front()` / `dq.pop_back()`     | 头/尾删除 |
| `dq[i]` / `dq.at(i)`                   | 随机访问  |

> **适用场景**：需要在两端频繁插入/删除时使用。

------





### list —— 双向链表

```c++
#include <list>
list<int> lst = {1, 2, 3, 4, 5};
```

**特有操作**

| 操作                                     | 说明                         |
| :--------------------------------------- | :--------------------------- |
| `lst.push_front(x)` / `lst.push_back(x)` | 头/尾添加                    |
| `lst.pop_front()` / `lst.pop_back()`     | 头/尾删除                    |
| `lst.remove(x)`                          | 删除所有值为x的元素          |
| `lst.unique()`                           | 删除连续重复元素（需先排序） |
| `lst.sort()`                             | 自身排序（不能用std::sort）  |
| `lst.merge(lst2)`                        | 合并两个有序list             |
| `lst.splice(it, lst2)`                   | 将lst2移动到lst的it位置      |

> **注意**：list 没有 `[]` 运算符，不支持随机访问。

------





### forward_list —— 单向链表

```C++
#include <forward_list>
forward_list<int> flst = {1,2,3,4,5};
```

## 核心特性

1. 底层单链表，节点只存**后继指针**，内存开销比 `list` 更小；
2. **无尾操作接口**：不存在 `push_back` / `pop_back` / `back()`；
3. 迭代器仅能**向后移动**，不支持反向遍历、无 `rbegin()`/`rend()`；
4. 所有插入 / 删除操作，都依赖**前驱位置**（`insert_after` / `erase_after`）。

## 特有操作对照表

表格

| 操作                           | 说明                                        |
| ------------------------------ | ------------------------------------------- |
| `flst.push_front(x)`           | 头部插入元素（唯一原生添加接口）            |
| `flst.pop_front()`             | 删除头部元素                                |
| `flst.emplace_front(args...)`  | 头部原位构造，减少拷贝                      |
| `flst.insert_after(it, x)`     | 在迭代器 `it` **后面**插入元素              |
| `flst.erase_after(it)`         | 删除迭代器 `it` **后面**的元素              |
| `flst.remove(x)`               | 删除链表中**所有**值等于 x 的元素           |
| `flst.unique()`                | 删除连续重复元素（需先排序）                |
| `flst.sort()`                  | 链表自身排序，不能使用全局 `std::sort`      |
| `flst.merge(flst2)`            | 合并两个**有序**单向链表，合并后 flst2 清空 |
| `flst.splice_after(it, flst2)` | 将 flst2 全部节点移动到 `it` 迭代器后方     |
| `flst.clear()`                 | 清空整个链表                                |







## 关联容器（有序）

> 有序表的所有操作时间复杂度都是 $O(logN)$ N 为有序表含有的记录数

### set / multiset —— 有序集合

```c++
#include <set>
set<int> s = {3, 1, 4, 1, 5};    // 自动排序去重 → {1, 3, 4, 5}
multiset<int> ms = {1, 1, 2, 3}; // 允许重复
set<string, CompareByLength> s;  // 提供比较器
```

**常用操作**

| 操作                               | 说明                                   |
| :--------------------------------- | :------------------------------------- |
| `s.insert(x)`                      | 插入元素                               |
| `s.erase(x)` / `s.erase(it)`       | 按值删除 / 按迭代器删除                |
| `s.find(x)`                        | 查找，返回迭代器，找不到返回 `end()`   |
| `s.count(x)`                       | 元素个数（set为0/1，multiset可能>1）   |
| `s.lower_bound(x)`                 | 第一个 **≥ x** 的元素                  |
| `s.upper_bound(x)`                 | 第一个 **> x** 的元素                  |
| `s.equal_range(x)`                 | 返回 `{lower_bound, upper_bound}` 对   |
| `s.size()`                         | 返回容器中元素个数                     |
| `s.empty()`                        | 判断容器是否为空                       |
| `s.clear()`                        | 清空所有元素                           |
| `s.begin()`                        | 返回指向第一个元素的迭代器             |
| `s.end()`                          | 返回指向最后一个元素之后位置的迭代器   |
| `s.rbegin()`                       | 返回指向最后一个元素的反向迭代器       |
| `s.rend()`                         | 返回指向第一个元素之前位置的反向迭代器 |
| `s.swap(t)`                        | 交换两个容器的内容                     |
| `s.emplace(args...)`               | 就地构造元素（C++11）                  |
| `s.extract(it)` / `s.extract(key)` | 提取节点（C++17）                      |
| `s.merge(t)`                       | 合并另一个容器（C++17）                |



### map / multimap —— 有序键值对

```c++
#include <map>
map<string, int> m;
m["apple"] = 5;                 // 通过[]插入/修改
m.insert({"banana", 3});        // insert方式
m.emplace("cherry", 7);         // C++11原地构造（更高效）
```



> **特别注意**：`m["key"]` 如果key不存在，会**默认插入**一个值（int为0，string为空字符串），容易引发bug。建议查找时先用 `find()`。

**安全访问方式**

```c++
auto it = m.find("apple");
if (it != m.end()) {
    cout << it->first << " → " << it->second;
}
```



**遍历（按键升序自动排序）**

```c++
for (const auto& [key, value] : m) {
    cout << key << ": " << value << endl;
}
```



------

## 无序关联容器（哈希表）

### unordered_set

```c++
#include <unordered_set>

unordered_set<int> us = {5, 2, 8, 1};
us.insert(7);                     // 插入单个元素；已存在则不生效
us.emplace(9);                    // 原地构造元素，比insert开销更小
us.insert({3,6});                 // 批量初始化列表插入（一次性插入多个元素）
us.erase(2);                      // 按值删除，返回删除个数(0/1)
auto us_it = us.find(8);          // 查找元素，找不到返回us.end()
if (us_it != us.end()) {
    int val = *us_it;
    us.erase(us_it);              // 通过迭代器删除
}
us.count(5);                      // 判断元素是否存在，存在返回1，不存在返回0
us.empty();                       // 判断容器是否为空
us.size();                        // 获取当前元素数量
us.clear();                       // 清空所有元素
us.reserve(100);                  // 预留空间，减少哈希扩容rehash
us.rehash(128);                   // 主动设置哈希桶数量
// 遍历
for (auto x : us);
for (auto iter = us.begin(); iter != us.end(); ++iter);
```

###  unordered_map

```C++
#include <unordered_map>

unordered_map<string, int> um;
um.insert({"banana", 3}) 等于 um.insert(pair<string,int>("banana", 3));
um.emplace("pear", 6);           // 原地构造键值对，效率更高
um["apple"] = 5;
um.erase("apple");               // 按key删除，返回删除数量0/1
um.find("banana");	             // 按key查找
um.erase(it);                    // 按迭代器删除
um.empty();                      // 判断容器是否为空
um.size();                       // 获取键值对数量
um.clear();                      // 清空全部
um.count("apple")) 				 // 读取：count判断是否存在
um.reserve(200);                 // 预留空间
um.rehash(256);                  // 设置哈希桶数量
// find获取迭代器，迭代器 -> first取key，-> second取value
auto it = um.find("orange");
if(it != um.end()){
    string k=it -> first;
    int v=it -> second;
}
// at：key不存在直接抛异常，适合强校验场景
int val = um.at("apple");
// 遍历
for (auto& p : um);
for (auto iter = um.begin(); iter != um.end(); ++iter);
```



**自定义类型作为key的示例**

```c++
struct Person {
    string name;
    int age;
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

struct PersonHash {
    size_t operator()(const Person& p) const {
        return hash<string>()(p.name) ^ (hash<int>()(p.age) << 1);
    }
};

unordered_set<Person, PersonHash> people;
```

------





## 容器适配器

### stack —— 栈（LIFO）

```c++
#include <stack>
stack<int> st;
st.push(10);    // 元素压入栈顶
st.emplace(20); // 原地构造元素压栈，开销更小
st.top();       // 获取栈顶元素（不删除）
st.pop();       // 删除栈顶元素，无返回值
st.size();      // 获取元素数量
st.empty();     // 判断栈是否为空，空返回true
st.swap(st2);   // 和另一个栈交换全部元素
```

### queue —— 队列（FIFO）

```c++
#include <queue>
queue<int> q;
q.push(1);        // 入队
q.push(2);
q.pop();          // 出队（无返回值）
int front = q.front();  // 队头
int back = q.back();    // 队尾
```

### priority_queue —— 优先队列（堆）

```c++
#include <queue>
priority_queue<int, vector<int>, less<int>> maxHeap;
priority_queue<int> pq;              // 省略写法，默认大根堆（最大值优先）
pq.push(5); pq.push(1); pq.push(10);
cout << pq.top();                    // 输出 10

// 小根堆
priority_queue<int, vector<int>, greater<int>> minHeap;
// 插入元素
minHeap.push(5);
minHeap.push(1);
minHeap.push(10);
cout << minHeap.top() << endl; // 输出 1，最小值在堆顶
```

>  **底层**：默认使用 `vector` 作为底层容器，基于堆算法实现。

------

## 常用算法速查

### 排序

| 算法                             | 说明                             |
| :------------------------------- | :------------------------------- |
| `sort(beg, end)`                 | 升序排序                         |
| `sort(beg, end, greater<int>())` | 降序排序                         |
| `sort(beg, end, lambda)`         | 自定义排序                       |
| `stable_sort(beg, end)`          | 稳定排序（相等元素保持相对顺序） |
| `partial_sort(beg, mid, end)`    | 部分排序，前mid-beg个元素有序    |

### 查找

| 算法                           | 说明                              |
| :----------------------------- | :-------------------------------- |
| `find(beg, end, val)`          | 线性查找，返回迭代器              |
| `binary_search(beg, end, val)` | 二分查找（**需有序**），返回bool  |
| `lower_bound(beg, end, val)`   | 第一个 ≥ val 的位置（需有序）     |
| `upper_bound(beg, end, val)`   | 第一个 > val 的位置（需有序）     |
| `equal_range(beg, end, val)`   | 返回 `{lower_bound, upper_bound}` |

### 统计与最值

| 算法                       | 说明                   |
| :------------------------- | :--------------------- |
| `count(beg, end, val)`     | 统计val出现次数        |
| `count_if(beg, end, pred)` | 统计满足条件的元素个数 |
| `max_element(beg, end)`    | 返回最大值迭代器       |
| `min_element(beg, end)`    | 返回最小值迭代器       |
| `minmax_element(beg, end)` | 同时返回最小和最大值   |

### 修改与操作

| 算法                          | 说明                                  |
| :---------------------------- | :------------------------------------ |
| `copy(beg, end, dest)`        | 拷贝                                  |
| `reverse(beg, end)`           | 反转                                  |
| `unique(beg, end)`            | 去重（**需先排序**），返回新逻辑结尾  |
| `remove(beg, end, val)`       | 移除指定值（返回新结尾，需配合erase） |
| `replace(beg, end, old, new)` | 替换旧值为新值                        |
| `fill(beg, end, val)`         | 填充                                  |
| `swap(a, b)`                  | 交换两个容器的内容                    |

### 数值运算

| 算法                                    | 说明           |
| :-------------------------------------- | :------------- |
| `accumulate(beg, end, init)`            | 累加           |
| `accumulate(beg, end, init, op)`        | 自定义累加操作 |
| `inner_product(beg1, end1, beg2, init)` | 内积           |
| `adjacent_difference(beg, end, dest)`   | 相邻差         |
| `partial_sum(beg, end, dest)`           | 前缀和         |

### 集合运算（需有序）

| 算法                       | 说明             |
| :------------------------- | :--------------- |
| `set_union`                | 并集             |
| `set_intersection`         | 交集             |
| `set_difference`           | 差集             |
| `set_symmetric_difference` | 对称差集         |
| `includes`                 | 判断是否包含子集 |

------

## 迭代器分类

| 迭代器类型         | 支持操作                    | 对应容器                   |
| :----------------- | :-------------------------- | :------------------------- |
| **输入迭代器**     | 只读，单次遍历              | `istream_iterator`         |
| **输出迭代器**     | 只写，单次遍历              | `ostream_iterator`         |
| **前向迭代器**     | 读写，多次遍历              | `forward_list`             |
| **双向迭代器**     | 支持 `++` 和 `--`           | `list`, `set`, `map`       |
| **随机访问迭代器** | 支持 `+`, `-`, `[]`, `<` 等 | `vector`, `deque`, `array` |

**常用迭代器操作**

```c++
vector<int> v = {1, 2, 3};

// 正向
for (auto it = v.begin(); it != v.end(); ++it) {}

// 反向
for (auto rit = v.rbegin(); rit != v.rend(); ++rit) {}

// 只读（C++11）
for (auto cit = v.cbegin(); cit != v.cend(); ++cit) {}

// 插入迭代器
back_inserter(v)    // 调用 push_back
front_inserter(v)   // 调用 push_front（需list/deque）
inserter(v, it)     // 调用 insert
```



------

## Lambda 表达式速记

```c++
// 基本语法
[capture](parameters) -> return_type { body }

// 捕获方式
[=]   // 值捕获所有外部变量（只读）
[&]   // 引用捕获所有外部变量（可修改）
[x]   // 值捕获 x
[&x]  // 引用捕获 x
[=, &x] // 值捕获所有，引用捕获 x
[&, x]  // 引用捕获所有，值捕获 x

// 示例
int threshold = 3;
auto it = find_if(v.begin(), v.end(), [threshold](int x) {
    return x > threshold;
});

// 修改外部变量
int count = 0;
for_each(v.begin(), v.end(), [&count](int x) {
    if (x % 2 == 0) count++;
});

// C++14 泛型lambda
auto add = [](auto a, auto b) { return a + b; };
```

------

## 容器选择指南

| 使用场景                       | 推荐容器                              |
| :----------------------------- | :------------------------------------ |
| 需要频繁随机访问，只在尾部增删 | **`vector`**                          |
| 需要频繁在头尾增删             | **`deque`**                           |
| 需要频繁在中间插入/删除        | **`list`**                            |
| 需要快速查找，且元素需要有序   | **`set` / `map`**                     |
| 需要快速查找，不关心顺序       | **`unordered_set` / `unordered_map`** |
| 需要存储键值对                 | **`map` / `unordered_map`**           |
| 需要 FIFO（先进先出）          | **`queue`**                           |
| 需要 LIFO（后进先出）          | **`stack`**                           |
| 需要动态获取最值               | **`priority_queue`**                  |
| 存储固定大小数据               | **`array`**                           |
| 需要去重                       | **`set` / `unordered_set`**           |
| 需要统计频次                   | **`map` / `unordered_map`**           |

------

## 常见陷阱与注意事项

### 1. 迭代器失效问题

| 容器                              | 插入/删除后迭代器失效情况           |
| :-------------------------------- | :---------------------------------- |
| `vector` / `deque`                | 插入/删除可能导致**所有**迭代器失效 |
| `list`                            | 仅被删除元素的迭代器失效            |
| `set` / `map`                     | 仅被删除元素的迭代器失效            |
| `unordered_set` / `unordered_map` | rehash时所有迭代器失效              |

### 2. map 的 `[]` 操作符陷阱

```c++
map<string, int> m;
int val = m["apple"];   // ⚠️ 如果"apple"不存在，会默认插入 {"apple", 0}！
// 正确做法
auto it = m.find("apple");
if (it != m.end()) { val = it->second; }
```

### 3. vector 扩容性能

```c++             
vector<int> v;
for (int i = 0; i < 100000; ++i) {
    v.push_back(i);     // 可能多次扩容，效率低
}

// 优化：预先分配容量
vector<int> v;
v.reserve(100000);      // 一次性分配好
for (int i = 0; i < 100000; ++i) v.push_back(i);
```

### 4. 自定义类型作为 key

- **`set` / `map`**：需要重载 `operator<` 或提供比较仿函数
- **`unordered_set` / `unordered_map`**：需要提供 `hash` 函数和 `operator==`

### 5. 多线程安全

> **STL容器不是线程安全的！** 多线程同时读写同一容器需要外部加锁（如 `mutex`）。

------

## 附录：常用头文件速查

| 头文件            | 内容                              |
| :---------------- | :-------------------------------- |
| `<vector>`        | vector                            |
| `<deque>`         | deque                             |
| `<list>`          | list                              |
| `<set>`           | set, multiset                     |
| `<map>`           | map, multimap                     |
| `<unordered_set>` | unordered_set, unordered_multiset |
| `<unordered_map>` | unordered_map, unordered_multimap |
| `<queue>`         | queue, priority_queue             |
| `<stack>`         | stack                             |
| `<algorithm>`     | sort, find, copy, ...             |
| `<numeric>`       | accumulate, inner_product, ...    |
| `<iterator>`      | 迭代器相关                        |
| `<functional>`    | greater, less, bind, ...          |





# 补充



## 二元结构体

### pair 基础说明

头文件：`#include <utility>` **pair 不属于容器，是标准二元结构体**，专门用来保存**一对数据 `(first, second)`**。 `map/multimap` 内部元素本质就是 `pair<const Key, Value>`。

```C++
#include <utility>
#include <vector>
using namespace std;

// 1. 定义与初始化
pair<int, int> p1;
pair<int, int> p2{3, 5};
auto p3 = make_pair(10, 20);        // C++11之前写法
pair<string, double> p4{"score", 95.5};

// 2. 成员访问
cout << p2.first << " " << p2.second;  // first 第一个元素，second 第二个

// 3. C++17 结构化绑定（快速解包）
auto [a, b] = p2;
cout << a << " " << b;
```

### 常用操作

1. **比较运算符（内置重载）** `== != < > <= >=` 全部支持。 比较规则：**先比较 first；first 相等再比较 second**

```C++
pair<int,int> x{2, 10}, y{3, 1};
if (x < y) { ... } // true，因为2 < 3
```

可以直接放入 `sort`、`set`、`map` 的 key。

2. **存储多个 pair：vector<pair<>>（刷题高频）**

```C++
// 存储逆序对(i,j)、区间、坐标
vector<pair<int, int>> vec;
vec.emplace_back(0, 3);
vec.emplace_back(1, 2);

// 遍历
for(auto &pr : vec)
{
    int i = pr.first;
    int j = pr.second;
}
```





