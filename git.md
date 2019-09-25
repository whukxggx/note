1. git status 当前文件状态。加上-s或--short ，更紧凑。（左边右边M啥的没记住。）
2. git diff  查看尚未暂存的文件更新了哪些部分。
3. git diff -staged | git diff --cached 查看已暂存的将要添加到下次提交的内容。
4. git rm 将文件从已跟踪文件清单中移除，并连带将目录中的文件删除。
5. git rm --cached filename  将文件从git仓库中删除（从暂存区域移除），但不从当前目录删除。
	git rm  log/\*.log 删除log/目录下以.log结尾的文件。
6. git remote -v 查看origin使用的是ssh还是https，如果使用的是https，每次都需要输入用户名和密码，可以改成ssh方式。
	git remote set-url origin git@github.com:username/repo_name
需要知道的是本地要生成sshkey并且在github端保留。
	ssh-keygen -t rsa -C "youremail@exam.com"然后一路回车，一般在～/.ssh目录下查看 cat id_rsa.pub
