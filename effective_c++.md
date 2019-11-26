# effective_C++2e
1.  尽量不用#define而用const和inline
原因：
	1. 比如#define ASPECT 1.76，编译器报错会直指1.76，因为ASPECT在预处理时就被去掉。这样就有可能需要时间去寻找。 const double  ASPECT=1.76效果会更好。定义常量指针需要将指针类型和指针都为const，定义类成员要为静态。
	2.  

	
	#define max(a,b) ((a)>(b)?(a):(b))
	int a=5,b=0;
	max(++a,b);//a的值增加两次
	max(++a,b+10);//a的值增加一次

	可以转为inline,
	template<typename T>
	inline max(const T &a,const T &b){
		return a>b?a:b;
	}
2. 尽量用<iostream>而不用<stdio.h>
原因:
	1. scanf和printf不是类型安全以及没有扩展性;
	2. << >>可以重载;
3.  尽量用new delete不用malloc free
原因是 malloc和free不知道构造函数;
4. c++内存管理，正确的得到和有效的使用;
5. 如果new的时候使用了[]，那么delete的时候也要用[]，最好杜绝对数组类型使用typedef。不然可能在用delete时产生混乱;
6. 删除空指针是安全的，因为它什么也没做。所以在析构函数里可以简单的delete掉每个声明的指针，不用担心是否被new过。但是不要绝对，不用delete一个传递给我的指针。除非类成员最初用了delete，否则不用在析构函数里调用delete;
7. 预先准备好内存不够的情况。set_new_handler;
