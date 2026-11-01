---
title: Qt
---

# QT



## 调用驱动库

以蓝牙为例：

​    在工程的.pro文件添加：  `QT += bluetooth`      





## 信号槽  

**信号槽就 Qt 专门用来让“一个对象”通知“另一个对象”发生了某件事的“通信工具”**

```
QMetaObject::Connection connect(
    const QObject *sender,                       // 谁发出的信号（发送者对象指针，比如按钮对象）
    const char *signal,                          // 要发送哪个信号（函数名，用 SIGNAL() 宏包裹，比如 SIGNAL(clicked(bool))）
    const QObject *receiver,                     // 谁来处理信号（接收者对象指针，比如窗口对象）
    const char *method,                          // 收到后执行哪个槽函数（用 SLOT() 宏包裹，比如 SLOT(close())）
    Qt::ConnectionType type = Qt::AutoConnection // 连接方式（决定同线程/跨线程调用方式，默认自动选择）
);
```





## UI 界面添加控件

1. 打开项目里的 `widget.ui` 文件，会自动弹出 UI 设计窗口
2. 左侧控件栏找到 **PushButton**，拖拽到窗口空白处
3. 右侧属性面板修改 2 个关键参数：
   - `objectName`：改为 `btn`（和代码里名字一致）
   - `text`：改为 `xxx`（按钮显示文字）
4. 按 `Ctrl+S` 保存 UI 文件





## 绑定按钮点击事件，生成点击槽函数

1. 在 UI 界面，右键刚创建的 `btn` 按钮
2. 选择【转到槽】，弹窗里勾选 `clicked()`，点击 OK
3. 自动跳转到 `widget.cpp`，生成如下空函数：

```
void Widget::on_btn_scan_clicked()
{
	//写按键按下的业务逻辑
}
```





## 控件

### LineEdit（单行输入框）

`objectName`：lineEdit_send

`placeholderText`：输入要发送的内容

`ui->lineEdit_send->text()`：读取输入框文本

`inputText.isEmpty()`：查看输入框是否为空





## 生成可执行.exe文件

打开`Qt 6.11.1 MSVC 2022 64-bit` 命令提示符

`windeployqt --bluetooth BLE.exe`
