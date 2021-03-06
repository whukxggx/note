* ##### new int[n]是创造长度为n的数组，new int(n)为创建一个int型数，并且以n初始化;
* ##### (不知道是否过时)目前唯一能够让template的运用具有可移植的方式，就是在头文件a中以inline function实现template function;
* ##### string class 被设计为安全易用的组建，其接口几乎不言自明，并且能够对许多可能的错误作检验。
* ##### STL的设计目标是将不同的算法和数据结构结合在一起，并获得最佳效率。所以stl的使用并不非常便利，也不检验很多可能的逻辑错误。要运用STL强大的框架和优异的效率，必须通晓其概念并小心应用。
* ##### nullptr用来表示一个pointer指向所谓的novalue。注意nullptr可以自动转换为各种pointer类型，但是不会转换为任何整数类型。因此可以避免null pointer被解释为一个整数值的误解。


	void f(int);
	void f(void *);
	f(0);//calls f(int)
	f(NULL);//calls f(int) if NULL is 0, ambiguous otherwise
	f(nullptr);//calls f(void*)
* ##### auto可以根据数据初值自动推导其类型，因此必须要有一个初始化操作，即必须直接赋值。
* ##### 一致性初始化(Uniform Initialization)。


	int values[] {1,2,3};
	std::vector<int> v{2,5,42,3,23,,1,3};
	std::vector<std::string> cities{
	"Berlin","Beijing","London","Braunschweig"
	};
	std::complex<double> c{4.0,3.0};//equivalent to c(4.0,3.0)
初值列(Initializer List)会造成所谓的value initialization，即某个local变量属于某种基础类型也会被初始化为0(或nullptr，如果它是个指针)。

	int i;//i has undefined value
	int j{};//j is initialized by 0
	int *p; //p has undefined value
	int *q{};//q is initialized by nullptr
	//但是窄化也就是精度降低或造成数值变动，对大括号是不成立的
	int x1(5.3);//x1=5;
	int x2=5.3;//x2=5;
	int x3{5.0};//error
	int x4={5.3};//error
	char c1{7};//ok
	char c2{99999};//error
判断是否窄化时，C++11用以避免许多不兼容性的做法是，依赖初值设定的实际值，而不是只依赖类型，如果一个值可被标的类型(target value)精确描述，其间的转换就不算窄化。浮点数转换至整数永远是一种窄化。

	class P
	{ 
		public:
			P(int a,int b){}
			explicit P(int a,int b,int c){}
	};
	P x(77,5);//ok
	P y{77,5};//ok
	P z{77,5,42};//ok
	P v={77,5};//ok  (implicited type conversion allowed)
	P w={77,5,42};//error,due to explicit (no implicit type conversion allowed);
	void fp(const P&);
	fp({47,11});//ok  隐式转换为P
	fp({47,11,3});//error
	fp(P{47,11});//ok
	fp(P{47,11,3});//ok

* ##### Range-Based for循环(其他编程语言的foreach循环),可以逐一迭代某个给定的区间，数组，集合(range,array,or collection)内的每一个元素
  for(decl:coll){
  	statement
  }
  其中decl是给定之coll集合中的每个元素的声明。
  当元素在for循环中被初始化为decl，不得有任何显式类型转换。
* ##### move语义。
  用以避免非必要拷贝和临时对象。
  代码说明：
	

	insert(const T&x);//for lvalues: copies the value
	insert(T&& x);// for rvalues: move the value
	X x;
	coll.insert(x);//insert copys of x//x is still used
	..........
	coll.insert(x+x);//moves contents temporary rvalue
	.........
	coll.insert(std::move(x));//moves contents of x into coll
声明于<utility>的std::move()自身并不做任何moving工作，它只是将其实参转成一个所谓的rvalue reference,被声明为X&&的类型。这种新类型主张rvalue(不具名的临时对象只能出现于赋值操作的右边)可被改动内容。这份契约的含义是，这是个不再需要的临时对象，你可以偷其内容或资源。
* ##### 右值引用：移动语义和完美转发c++11新特性解析和应用
对上方c++标准库的再补充
函数返回临时变量的好处就是不需要声明变量，也不需要知道生命周期。
gcc似乎直接内置移动语义或者对某些构造函数在某些情况下的省略？写的示例代码并没有调用移动构造函数。
c++98/03的语言和库中已经有移动语义：
在某些情况下拷贝构造函数的省略。(copy constructor elision in some contexts)
智能指针的拷贝。(auto_ptr "copy")
链表的拼接。("list::splice")
容器内的置换。(swap on containers)

* 左值，右值，右值引用。
	在c++98中，可以取地址的，有名字的就是左值。不能取地址的，沒有名字的就是右值。
	c++11中，右值有两种，将亡值和纯右值(c++98中的)。
	将亡值是c++新增的跟右值引用相关的表达式，这样的表达式通常是将要被移动的对象。比如返回右值引用T&&的函数返回值、std::move的返回值、或者转换为T&&的类型转换函数的返回值。


	T && a = ReturnRValue();//声明了一个名为a的右值引用。//假设ReturnRValue返回一个右值。
	//右值引用不能绑定到任何的左值。
	int c;
	int && d=c;//error!
	T &e = ReturnRValue();//error
	const T &e=ReturnRValue();//ok
	//常量左值引用在c++98中是个万能的引用类型，可以接受非常量左值、常量左值，右值对其进行初始化。
	//在c++98中，常可以使用常量左值减少临时对象的开销。
在gcc8.3版本中，似乎c++11新特性解析与应用中的测试都会被优化，因此关于移动语义的测试的构造函数很多都没被调用。因此再用g++编译的时候需要参数-fno-elide-constructors;
* 关于 -fno-elide-constructors:
	c++标准允许一种编译器实现省略创建一个只是为了初始化另一个同类型对象的临时对象。指定这个参数将关闭这种优化，强制g++在所有情况下调用构造函数。当然，除了如上方一些情况下的测试功能用例，基本不可能调用这个参数。"如果你的代码依赖于拷贝构造函数的副作用，那么你的代码就写的很烂。你编写的构造函数必须保证这样的优化是安全的。"(g++中被称为RVO/NRVO,Return Value Opyimization,Named Return Value optimization)
* 通常情况下，程序员会为声明了移动构造函数的类声明一个常量左值为参数的拷贝构造函数，以保证在移动构造不成时，可以使用拷贝构造。
* 有常量右值引用，const T &7 crvalueref=ReturnRValue();但是其并没有任何用处。
* 标准库在< type_traits>头文件中提供了三个模板类：is_rvalue_reference,is_lvalue_reference,is_reference	
* std::move,强制转化为右值。在头文件< utility>中,实际上它并不能移动任何东西，它唯一的功能是将一个左值强制转化为右值引用，继而我们可以通过右值引用使用该值。基本等同于static< T&&>(lvalue);
* 在c++11中，拷贝构造/赋值和移动构造/赋值函数必须同时提供或者同时不提供，这样程序员才能保证同时具有拷贝和移动语义。只声明其中一种，类都仅能实现一种语义。(因为只声明其中一种编译器便不会生成默认版本的构造函数)
* 只有移动语义构造函数的类型表明该类型的变量所拥有的资源只能被移动。比如unique_ptr,ifstream
* 在< type_traits>中，有一些辅助的模板类来判断一个类型是否是可移动的，比如is_move_construcible,is_trivially_move_constructible,is_nothrow_move_constructible;
* 高性能的置换函数(swap)

	
	template<typename T>
	void swap(T&a,T& b){
		t tmp(move(a));
		a=move(b);
		b=move(tmp);
	}
	//如果T是可移动的，那么整个过程代码都只会按照移动语义进行指针交换，不会有资源的释放和申请。如果T是不可移动但是可拷贝的，那么拷贝语义会被用来进行置换。跟普通的置换相同。
* 关于异常和移动语义。可能移动语义还没完成，一个异常却抛出来了，这会导致一些指针成为悬挂指针。**因此程序员应该尽量编写不抛出异常的移动构造函数，通过为其添加一个noexcept关键字，可以保证移动构造函数抛出来的异常会直接调用terminate程序终止运行。同时还有一个std::move_if_noexcept替代模板函数，该函数在没有noexcept关键字修饰时返回一个左值引用从而使变量可以使用拷贝语义，有noexcept时返回一个右值引用从而使用移动语义。(move_if_noexcept牺牲了性能)
* 完美转发,是指在函数模板中，完全依照模板的参数的类型，将参数传递给函数模板中调用的另外一个函数。c++11通过引用折叠并结合新的模板推导规则来完成完美转发。
* Raw String
* noexcept关键字：指明某个函数无法或不打算抛出异常。比如

	void foo() noexcept;//如果有异常在foo()内被处理，或foo抛出异常，程序会被终止。
* c++异常明细是被检验于运行期而不是编译期，所以它无法对程序员保证每个异常都被处理。而且运行期检验会令编译器产生额外的代码且妨碍优化。甚至可以指明在某种条件下函数不抛出异常。
	
	
	void swap(type&x,type& y)noexcept(noexcept(x.swap(y))){
		x.swap(y);
	}
	//这里哦，noexcept(...)中可以指定一个boolean条件，如果符合就不抛出异常，	//换句话说，指明noexcept而不带条件其实就是noexcept(true)的一个简洁形式。	noexcept(x.swap(y))会在小括号内的表达式不可抛出异常时产生true
* constexpr可用来让表达式核定于编译期。

	constexpr int square(int x){
		return x*x;
	}
	float a[square(9)];//ok in c++11
* 模板可拥有那种得以接受个数不定之template实参的参数。

	void print(){}//必须提供一个非模板的重载函数才能结束下面那个递归动作
	template<typename T,typename... Types>
	void print(const T& firstArg,const Types&... args){
		std::cout<<firstArg<<std::endl;
		print(args...);
	}
* Alias Template:
 template< typename T>
 using Vec=std::vector< T,MyAlloc< T>>;
 Vec<int> coll;


 * Lambda
 

	[] lambda introducer
	在lambda introducer 和lambda body之间，你可以指明参数或mutable，或一份异常明细或attribute specifier以及返回类型，只要其中一个出现了，参数所需要的小括号就必须出现。

	auto l = [](const std::string &s){
		std::cout<<s<<std::endl;
	}
	l("hello lambda");//prints"hello lambda"
	如果要指定一个返回类型，可以采用新式c++语法。
	[] () ->double {return 42;};//返回42.0
* capture(传递外部作用域内未被传递为实参的数据。)  
 在lambda introducer内，你可以指明一个capture来处理外部作用域内未被传递为实参的数据。[=] 为by value方式，[&]为by reference方式。也可以个别指明访问对象是by value还是by reference;
 auto qqq=[x,&y]{
 };
或者[=,&y]，意思是y以by reference传递，其他所有实参以bny value传递。
也可以声明lambda为mutable,这样可以在by value传递的时候改变改变内部的值。auto f = [id] () mutable {};
* lambda的类型 可以使用std::function<> class template
	
	
	std::function<int(int,int)> returnLambda(){
		return [] (int x,inty){
			return x*y;
		};
	}

* decltype关键字  
  decltype可让编译器找出表达式类型。
  
		std::map< std::string,float> coll;
		decltype(coll)::value_type elem;
* 新的函数声明语法。

		template< typename T1,typename T2>	
		auto add(T1 x,T2 y)->decltype(x+y);
* 带领域的Enumeratiion
c++11允许我们定义scoped enumeration；
enum class Salutation : char {mr,ms,co,none};
重点在于在enum之后指明关键字calss。优点有：1.绝不会隐式转换到int；2.如果数值（mr）不在enumeration被声明的作用域中，必须改为Salutation::mr.3.可以定义低层次类型，这里是char，默认是int 4.可以提前声明。
* 新的基础类型。 char16_t和char32_t,long long,unsigned long long,std::nullptr_t;
* 非类型模板参数  bitset< 32> flags32; bitset< 50> f;ags50;//两个不同的类型。
* 模板参数默认值
* 关键字typename
* 成员模板  
* 成员模板的一个特殊形式是template 构造函数。通常用于对象被复制时给予隐式类型转换的能力。但是这个构造函数并不压制copy构造函数的隐式声明，如果类型吻合是会调用隐式的copy构造函数。
* 嵌套式(nested) class template
* 基础类型的明确初始化(explicit initialization for fundamental type)如果使用一个明确的构造函数调用，但不给实参，基础类型会被设定为初值为0;
* main()定义式  唯一正确且可移植的只有两种。


	int main(){}
	int main(int argc,char *argv[]){}//argv也可定义为char **，返回类型必须是int
	//c++是隐式的定义了return 0;的。
* namespace  


	//三种使用
	std::cout  
	using std::cout;//之后可以直接使用cout;  
	using namespace std;//在复杂的代码中，这种方式可能导致意外的名称冲突。更糟糕的是，由于某些晦涩的重载规则甚至导致不一样的行为。所以如果场合不够明确不要使用。
* 头文件  
  不再有.h;.hpp;.hxx如果是c标准头文件，加上c,比如< cstdlib>//was < stdlib.h>
* 差错和异常
* callable object 可以是一个函数，一个指向成员函数的对象，一个函数对象，一个lambda；可使用std::bind()和std::async()调用。
* 并发和多线程。
* pair和tuple  
关于pair的初始化和一些操作在图片文件夹中已截图


	std::pair<int,float> p;//initialize p.first and p.second with zero
	//copy构造函数同时存在两个版本，版本1接受相同类型的pair，版本2是个member template,在构造过程中需要隐式类型转换时被调用。如果pair对象被复制，调用的是被隐式合成的那个copy构造函数。
	//如果pair<>用到了某个类型而该类型只有一个nonconstant copy构造函数，将不再编译成功。

	//便捷函数make_pair()无需写出类型就能生成一个pair对象。
	//std::pair<int,char> (42,'@')
	//std::make_pair(42,'@')
	//在c++11中，make_pair声明如下:
	namespace std{
		template<typename T1,typename T2>
		pair<T1,T2> make_pair(T1 &&x,T2 &&y);//因为要考虑移动语义，所以将原本的常量引用转换为了这个。这样如果可能就会用move语义，否则使用copy语义。
		//另外使用make_pair并不会花费任何执行时间，编译器会做优化。
	}
	void f(std::pair<int,float>);
	//也可使用初值列 f({42,3.1});
	//但是使用std::pair< int,float>有一个明显的优势是有绝对明确的属性，比如
	//std::pair< int,float>(42,7.3)和std::make_pair(42,7.3)，第二个的7.3是float类型;
	std::string s,t;
	...
	auto p=std::make_pair(std::move(s),std::move(t));
	...//s and t are no longer used
	//如果使用reference semantic,必须使用ref(),如果使用const reference 类型，必须使用cref()，都定义在< functional>中
	int i=0;
	auto p=std::make_pair(std::ref(i),std::ref(i));
	++p.first;
	++p.second;//i=2;
	//tuple内的tie接口提取pair中的value
	std::pair< char,char> p =std::make_pair('x','y');
	char c;
	std::tie(std::ignore,c) = p; //extract second value into c
	//这里的实际是p赋值给一个tuple，后者的第二个value是一个reference，指向c
	//一般来讲必须返回两个value的汉俗都使用pair对象。c++标准库的map和multimap容器的元素类型便是pair
关于tuple  
tuple扩展了pair的概念，拥有任意数量的元素。	即异质元素列


	tuple< string,int,int,complex<double>> t;
	tuple< int,float,string> t1(41,4.1,"nice");
	cout <<  get<0>(t1)<<" ";
	cout <<  get<0>(t1)<<" ";
	cout <<  get<0>(t1)<<" ";
	cout << endl;
	auto t2=make_tuple(42,44,"nice");
	get<1>(t1)=get<1>(t2);
	//tuple的元素也可以时reference，比如 string s; tuple<string&> t(s);
	//tuple不允许迭代元素，
	int i;
	get<i>(t1);//error: i is no compile-time value
	//make_tuple可类比为make_pair,自动推断元素。同样可以使用ref和cref.
	string s;
	auto x=make_tuple(ref(s));
	get<0>(x) = "my value";//modifies x ,由于x指向s，因此也改变了s
	auto y=make_tuple(s);
	get<0>(y)="my value";//Modifies y but not s
	//tie()可以建立一个内含reference的tuple。同时std::ignore允许我们建立一个忽略tuple的某些元素。
	
	tuple< int,float,string> t1(41,4.1,"nice");
	int i; float f; string s;
	tie(i,f,s)=t;//其实就是将t内的值赋给i f s
	//或者忽略某些元素
	tie(i,std::ignore,s)=t;
	//tuple的构造函数是explicit的。因此用初值列初始化时：
	tuple<int ,double> t(42,4.2);//ok
	tuple<int,double> t{42,4.2};//ok
	tuple<int,double> t={42,4.2};//error
	//这种做法对与pair<>和容器(除了array<>)是可行的。
	//即对于tuple必须明确的转换为一个tuple
	tuple_size<tupletype>::value 获取元素个数
	tuple_element<idx,tupletype>::type可获取第idx个元素的类型。
	tuple_cat()可将多个tuple串接成一个tuple
	//tuple可以和pair互换。
* smart pointer  
c++11中提供两大类型的pointer：shared_ptr和unique_ptr,抛弃auto_ptr,除非需要编译一些老旧代码否则不该再使用它。


	shared_ptr<string> pNico(new string("ds"));
	shared_ptr<string> pNico{new string("ds")};
	shared_ptr<string> pNico=make_shared<string>("ds");
	//但是不可以直接将new string赋给pNico;因为接受单一pointer的构造函数是explicit。
	shared_ptr<string> pNico;
	pNico.reset(new string("ds"));
	(*pNico)[0]='N';
	shared_ptr<string> pJutta(new string("Jutta"));
	pJutta->replace(0,1,"J");//替换substring
	//但凡容器，总是为传入的元素创建自己的拷贝。所以如果push_back进vector智能指针，被复制的就是那些pointer，所以容器内含多个"指向同一对象的"reference;
	//可以定义自己的deleter
	shared_ptr<string> pNico(new string("nico"),
							[](string *p){
								cout << "delete"<<*p<<endl;
								delete p;
							});
	//传递一个lambda作为第二实参，如果pNico这样声明,当其最后一个拥有者被销毁时，会调用这个lambda函数。
	//如果使用new [] 定义，必须定义自己的deleter.
	shared_ptr<int> p(new int[10],
						[](int *p){
							delete [] p;
						}
						)
	//也可以使用为unique_ptr提供的辅助函数。
	shared_ptr<int> p(new int[10],std::default_delete<int[]>());
	另外shared_ptr不提供operator []
	//当然，由于shared_ptr只提供了*和->,不提供指针运算和[],因此要访问内存必须使用get()获得被shared_ptr包覆的内部指针。
	smp.get()[i]=i*42;
	(&*smp)[i]=i*42;
weak_ptr


* deque是一种双向开口的连续线性空间。
* deque与vector的差异主要体现在两个方面，一是deque允许常数时间内对投起头端进行元素的插入或移除操作，二是deque没有所谓的容量观念，因为它是动态地以分段连续空间组合而成，随和可以增加一段新的空间并链接起来。即不会出现vector那样因空间不足而重新配置一块更大空间，然后复制元素，再释放旧空间的事。
* deque提供的random access iterator不是普通指针，效率很低，因此尽量用vector.对deque的排序操作，为了提高效率，可先完整复制到vector排序再复制回去。
* deque在逻辑上是连续的。
*
