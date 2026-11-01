# 嵌入式技术知识库

基于 Docusaurus 的嵌入式系统技术资料库。文档内容按技术领域组织，包含语言基础、Linux、构建系统、嵌入式软件、硬件接口、FPGA、通信协议、图像处理以及数据结构与算法。

## 本地运行

环境需要 Node.js 20 或更新版本。

```bash
npm install
npm start
```

开发服务器启动时会检测 WSL 的 IPv4 地址，并让 Windows 浏览器自动打开 `http://<WSL IPv4>:3000`。如果无法自动检测地址，可手动运行 `SITE_IP=<WSL IPv4> npm start`。WSL 的 IPv4 地址可通过 `hostname -I` 查看。打开根地址即自动进入知识索引，不再显示独立首页。

生成静态站点使用 `npm run build`，本地预览构建结果使用 `npm run serve`；预览服务器同样绑定检测到的 WSL IPv4。

## 内容目录

正式技术文档统一位于仓库根目录 `docs/`，Docusaurus 直接从该目录读取 Markdown 文档。页面层级与显示顺序在 `sidebars.js` 中逐页显式维护；新增页面时须在对应目录项中登记。知识页仅收录书面技术概念、原理、接口、示例和技术限制。

仓库目录职责和文档变更同步规则见[知识库初始化与目录结构](docs/初始化说明.md)。CAN、FPGA 等可运行示例位于 `examples/`；正式网站内容只以 `docs/` 为准。

语言文档分为 `C语言/` 和 `C++语言/` 子目录；uC/OS-III 按内核机制组织在 `实时操作系统/uC-OS-III/`。资料审查及归档范围记录在 [record.md](record.md)，正式文章按知识点拆分，不沿用混合笔记的主题边界。

OpenCV 知识位于 `docs/图像处理/OpenCV/openCV.md`，包含图像与视频 I/O、Mat 所有权、滤波与变换、特征、直方图、绘制及运动分析，示例以 OpenCV 4.6 C++ 接口为准。

Linux 用户态内容位于 `docs/Linux应用/`，线程控制单独成页；设备模型、字符设备、设备树、中断、内核同步与 GPIO、Pinctrl、I2C、Input、USB、LCD、V4L2 子系统位于 `docs/Linux驱动/`。Qt、ROS 2 与软件分层归入 `docs/应用框架/`，原 `Linux与框架/` 目录已拆分。驱动 API 示例注明目标内核版本，旧板级源码分析保留相应平台条件。
