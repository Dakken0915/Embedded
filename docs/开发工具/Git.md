## GIT

```C
.gitignore     /* git 提交忽略清单 */
```



```shell
# 设置 HTTP 代理
git config --global http.proxy http://127.0.0.1:7890
git config --global http.proxy http://172.18.128.1:7890

# 设置 HTTPS 代理
git config --global https.proxy http://127.0.0.1:7890
git config --global https.proxy http://172.18.128.1:7890

# 验证
git config --list

# 取消 HTTP 代理
git config --global --unset http.proxy

# 取消 HTTPS 代理
git config --global --unset https.proxy

# 查看状态
git status                  # 查看文件状态：哪些修改、新增、未跟踪
git diff                    # 查看未 add 的代码改动（红色删除，绿色新增）
git diff --staged           # 查看已经 git add 放入暂存区的改动
git log                     # 查看提交历史（q 退出日志界面）
git log --oneline           # 简洁单行版提交记录

# 提交三部曲（本地保存 + 上传远端）
git add .                   # 将当前目录全部改动加入暂存区
git add 文件名.c             # 只把单个文件加入暂存
git commit -m "注释文本"     # 暂存内容打包提交到本地仓库
git push                    # 把本地提交推送到远程仓库
git push -u origin main     # 首次推送分支，绑定远端

# 拉取远程代码（同步服务器最新代码）
git pull                    # 拉取远程代码并自动合并到本地分支
git fetch                   # 只拉远程更新，不自动合并
git merge origin/main       # 将远程分支合并到本地

# 分支操作
git branch                  # 查看本地所有分支，*代表当前分支
git branch dev              # 创建名为 dev 的新分支
git checkout dev            # 切换到 dev 分支
git switch dev              # 切换分支
git checkout -b dev         # 创建并直接切换到 dev 分支
git branch -d dev           # 删除本地分支（必须先切到别的分支）
git push origin --delete dev # 删除远程分支

# 撤销/回滚（重要，谨慎使用）
git restore 文件名           # 放弃未 add 的文件修改，恢复成上次 commit 版本
git reset HEAD 文件名        # 已经 git add，但不想提交该文件，从暂存区撤出来（保留本地修改）
git reset --hard HEAD       # 丢弃本地所有未提交改动，回退到最近一次提交
git reset --hard HEAD~1     # 回退到上一个commit，丢弃最近一次提交

# 仓库初始化与克隆
git init                    # 在当前文件夹新建git仓库
git clone https://xxx.git   # 克隆远程仓库到本地

# 配置
git config --global user.name "你的名字"
git config --global user.email "邮箱@xxx.com"
git config --list           # 查看git全部配置

# 冲突处理（pull遇到冲突）
#1.打开冲突文件，找到<<<<<<< HEAD标记，手动修改代码
#2.git add 冲突文件
#3.git commit -m "resolve conflict"
#4.git push

# .gitignore 示例（项目根目录新建.gitignore文件）
#*.o
#*.log
#build/
#node_modules/

# 日常开发完整流程
git pull                # 先拉远端最新代码
git status              # 看文件改动
git diff                # 核对代码修改
git add .               # 加入暂存
git commit -m "fix:xxx" # 本地提交
git push                # 上传服务器


git reset --hard HEAD
```



