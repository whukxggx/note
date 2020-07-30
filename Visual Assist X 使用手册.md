| 快捷键       | 说明                                             | 备注                       |
| ------------ | ------------------------------------------------ | -------------------------- |
| Alt+G        | 快速跳转，如由声明跳转到实现，由实现跳转到声明。 | 光标要在标识符处           |
| Alt+G        | .h与.cpp文件的快速切换                           |                            |
| Alt+Shift+F  | 查找标识符所有被引用的位置                       | 光标要在标识符处           |
| Alt+Shift+O  | 查找整个Solution下的某个文件                     | O指open，打开指定的文件    |
| Alt+Shift+S  | 查找标识符                                       | S指Symbol                  |
| Alt+Shift+R  | 重命名标识符                                     | 光标要在标识符处,R指ReName |
| Ctrl+Shift+V | 选择剪切板的内容进行粘贴                         |                            |

跳转
        Alt + G           函数 定义 和 声明 的 相互跳转。 
        F12               跳转到所调用函数或变量的定义处
        Alt + O           在.h与.cpp文件中实现相互转换。.h与.cpp文件的快速切换
        Alt + M           列出当前文件所有的函数。输入函数名的一部分，查看某个函数
        Ctrl + Tab        切换标签
        Alt + 向上方向键    跳转到上一个类或函数
        Alt + 向下方向键    跳转到下一个类或函数
        Ctrl + F3   　　    选中一个单词时，跳到下个单词（F3下一个，shift + F3 上一个）
        Ctrl + F2   　　    某行加书签 再按一次测去掉书签
        F2    　　　　      跳到一个书签，再按一下测跳到下一个书签
        Shift + F2 　　     跳到上一个书签
        Ctrl + Home   　　  跳到文档最前面
        Ctrl + End   　　　 跳到文档最后面
        Home    　　        跳到一行最前面
        End    　　　　     跳到一行最后面
        
        Ctrl + ->  　　     跳到一个单词最后  
        Ctrl + <-  　　     跳到一个单词最前   
        Ctrl + Shift + ->   选择光标后的一个单词 
        Ctrl + Shift + <-   选择光标前的一个单词
        Shift + ->   　　　 选择后的一个字母，每按一次增先一个字母
        Shift + <-    　　  选择前的一个字母，每按一次增先一个字母
        
        Alt + <-  　　 　　 退回到上一视图
        Alt + ->   　　 　　前进到下一视图 （这个快捷键是自定义的）
        Ctrl + }   　　 　　如当时光标在"{" 处，则跳到配对的 "}"。如当时光标在"}" 处，则跳到配对的 "{"。

 

调试运行     
        F9                   设置/删除 断点
        Ctrl + Shift + F9    删除所有断点
        F5                   启动调试
        Shift + F5           停止调试
        Ctrl + F5   　　　   运行程序，注意不是调试
        F10    　　　　 　 　单步调试。逐过程
        F11    　　 　　　　 单步进入某个函数调试。逐语句
        Shift + F11   　　　 从某个函数跳出        


查找    
        
        Ctrl + F            查找
        Ctrl + Shift + F    在文件中查找关键字，当前工程，当前文档，打开的文档，自定义等        
        F3                  查找下一个
        Shift + F3          查找上一个
        Alt + Shift + O     查找文件。打开文件名搜索对话框。O指open，打开指定的文件
        Alt + Shift + S     查找符号。FindSynbolDialog 打开查找符号对话框。S指Symbol
        Alt + Shift + F     查找所有引用。Find References 
        Alt + Shift + R     重命名标识符。光标要在标识符处，R指ReName
                            改掉一个类名或是其他东西的命名时，引用的地方同时被改成新的名字。
        Ctrl +  I   　　    输入，增量搜索（实时查找）


​    
​    
替换     
​        Ctrl + H           替换
​        Ctrl + Shift + H   在文件中替换
​        Alt + F12          查找符号(列出所有查找结果)
​        Alt + Shift + R    选定内容改名，用得不多，但很实用（改过名的都知道）

​        

代码操作     
        Ctrl + /   　　　　 加注释（这个快捷键是自定义的）
        Ctrl + K + C        注释
        Ctrl + K + U        取消注释
        Ctrl + Shift + ]    选择匹配的括号与其中的代码
        Ctrl + Shift + U    转换为大写
        Ctrl + U            转换为小写
        Ctrl + K + D        设置文档格式
        Tab                 增加缩进
        Shift + Tab         减少缩进
        Ctrl + M + M        折叠代码，例如将光标放在#region、if、方法头的位置，将会折叠它们
        Ctrl + Shift + 空格 当光标在函数参数时，则提示函数参数列表
        Ctrl + B   　　　　 加条件断点
        Ctrl + L   　　　　 删除一行
        
        Shift + F9   　　　// 快速查看变量
        Alt + 3    　　　　// 查看变量
        Alt + 7    　　　　// 查看堆栈
        Alt + 6    　　　　// 查看内存

 

项目的操作     
        Alt + Enter         打开选定项的属性对话框
        Ctrl + Shift + N    新建项目
        Alt + Shift + N     新建网站
        Ctrl + N            新建文件
        Ctrl + Shift + O    打开项目
        Alt + Shift + O     打开网站
        Ctrl + O            打开文件


视图调用     
        F4                属性
        Ctrl + Alt + X    工具箱
        Ctrl + Alt + L    解决方案资源管理器
        Ctrl + Alt + S    服务器资源管理器