#include <iostream>
using namespace std;


class XNode {
public:
	virtual void f() {
		cout << a << endl;
		cout << b << endl;
		cout << c << endl;
	}
	int a;
	int b;
	int c;
};

int main() {
	int *arr = new int[100];
	for (int i = 0; i < 100; ++i) {
		arr[i] = i + 1;
	}
	XNode *p = new(arr)XNode;
	p->f();
	system("PAUSE");
	return 0;
}

//�ֱ�Ϊ
//	XNode *p = new(arr)XNode();������ӹ��캯��