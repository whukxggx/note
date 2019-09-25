1. git status 当前文件状态。加上-s或--short ，更紧凑。（左边右边M啥的没记住。）
2. git diff  查看尚未暂存的文件更新了哪些部分。
3. git diff -staged | git diff --cached 查看已暂存的将要添加到下次提交的内容。
4. git rm 将文件从已跟踪文件清单中移除，并连带将目录中的文件删除。
5. git rm --cached filename  将文件从git仓库中删除（从暂存区域移除），但不从当前目录删除。
	git rm  log/\*.log 删除log/目录下以.log结尾的文件。
