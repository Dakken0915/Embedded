# 嵌入式技术知识库

基于 Docusaurus 的嵌入式系统技术资料库。文档内容按技术领域组织，包含语言基础、Linux、构建系统、嵌入式软件、硬件接口、FPGA、通信协议、图像处理以及数据结构与算法。

## 本地运行

环境需要 Node.js 20 或更新版本。

```bash
npm install
npm start
```

开发服务器启动时会检测 WSL 的 IPv4 地址，并让 Windows 浏览器自动打开 `http://<WSL IPv4>:3000`。如果无法自动检测地址，可手动运行 `SITE_IP=<WSL IPv4> npm start`。WSL 的 IPv4 地址可通过 `hostname -I` 查看。

生成静态站点使用 `npm run build`，本地预览构建结果使用 `npm run serve`；预览服务器同样监听所有网卡。

## 内容目录

技术文档位于 `docs/技术知识/`，Docusaurus 只从 `docs/` 读取 Markdown 文档。侧栏在 `sidebars.js` 中按文档目录自动组织页面；学习方法、路线规划和工具操作说明不纳入站点文档。
