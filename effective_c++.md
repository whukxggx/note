# effective_C++2e
1. 尽量不用#define而用const和inline
原因：
a.比如#define ASPECT 1.76，编译器报错会直指1.76，因为ASPECT在预处理时就被去掉。这样就有可能需要时间去寻找。 const double  ASPECT=1.76效果会更好。定义常量指针需要将指针类型和指针都为const，定义类成员要为静态。
b.
	#define max(a,b) ((a)>(b)?(a):(b))
	int a=5,b=0;
	max(++a,b);//a的值增加两次
	max(++a,b+10);//a的值增加一次

可以转为inline,
	template<typename T>
	inline max(const T &a,const T &b){
		return a>b?a:b;
	}

2.尽量用<iostream>而不用<stdio.h>
原因:
a.scanf和printf不是类型安全以及没有扩展性。
b.<< >>可以重载.
3.尽量用new delete不用malloc free
a. malloc和free不知道构造函数。

