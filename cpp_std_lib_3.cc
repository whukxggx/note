#include<iostream>
using namespace std;

/*
void printElements(const T& colls){
	for(const auto &elem:colls)//如果要修改其中的值就要用auto &elem
		std::cout<<elem<<std::endl;
}
*/

//c++11新特性关于移动语义
//似乎编译器自动优化了，只调用了construct和desturct
class hasPtrMem{
public:
	hasPtrMem():d(new int(3)){
		cout<<"construct :"<<++n_cstr<<endl;
	}
	hasPtrMem(const hasPtrMem &h):d(new int(*h.d)){
		cout<<"copy construct :"<<++n_cptr<<endl;
	}
	//移动构造函数
	//接受一个"右值引用"的参数
	hasPtrMem(hasPtrMem&& h):d(h.d){
		h.d=nullptr;
		cout<<"move construct :"<<++n_mvtr<<endl;
	}
	~hasPtrMem(){
		delete d;
		cout<<"destruct:"<<++n_dstr<<endl;
	}
	int *d;
	static int n_cstr;
	static int n_cptr;
	static int n_mvtr;
	static int n_dstr;
};
int hasPtrMem::n_cstr=0;
int hasPtrMem::n_cptr=0;
int hasPtrMem::n_mvtr=0;
int hasPtrMem::n_dstr=0;

hasPtrMem getMem(){
	hasPtrMem h;
	cout<<"resource from "<<__func__ <<":"<<hex<<h.d<<endl;
	return h;
}
int main(){
	//3.1.4
	//for(int i:{2321,312,3,12,3,12,3,1,2,3})
	//	std::cout<<i<<std::endl;
	hasPtrMem a=getMem();
	cout<<"resource from"<<__func__<<":"<<hex<<a.d<<endl;
}
