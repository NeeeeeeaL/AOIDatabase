## 3DAOI项目数据库部分开发文档

### GitHub部署
1. [GitHub官网](https://github.com)新建远程库AOIDatabase
2. 新建本地库AOIDatabase
   1. 在AOI数据库文件夹根目录下打开 Git Bash, 输入命令:``` git init```新建本地仓库
   2. 输入命令```git add .```将文件夹内所有文件添加到本地仓库中
   3. 输入命令```git commit ```提交所有文件 
3. 建立与远程库的连接
   1. 输入命令```git remote add origin + 远程库的网址（https）例如：https://github.com/NeeeeeeaL/AOIDatabase```建立与远程库的连接
   2. 输入命令```git push -u origin master -f```将本地仓库推送到远端仓库，-f 表示强制推送，可能会使远程修改丢失，多人协作时不可取。

### 数据库操作
