/*
	测试 智能指针
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 智能指针， 弱引用指针头文件
#include "utility/memory.h"


class A
{
public:
	A() { cout << "A 构造" << endl;}
	virtual ~A() {cout << "A 析构" << endl;}
	A(const A& other) {cout << "A 拷贝" << endl;}
	virtual void foo()	{ cout << "fooA" << endl; }
};

class B: public A
{
public:
	B() { cout << "B 构造" << endl;}
	virtual ~B() {cout << "B 析构" << endl;}
	B(const B& other) {cout << "B 拷贝" << endl;}
	virtual void fooB()	{ cout << "fooB" << endl; }
};

//自定义析构
template<class T>
struct MyDeleter
{
	static void Delete(T* p)
	{
		cout << "自定义析构方式" << endl;
		delete p;
	}
};

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	shared_ptr<int> pI(new int(3));
	cout << "pi = " << *pI.get() << endl;
	cout << "pi count = " << pI.usecount() << endl;
	
	{
	//copy
	shared_ptr<int> pI2;
	pI2 = pI;
	cout << "----after copy-----------" << endl;
	cout << "pi = " << *pI.get() << endl;
	cout << "pi count = " << pI.usecount() << endl;
	cout << "pi2 = " << *pI2.get() << endl;
	cout << "pi2 count = " << pI2.usecount() << endl;

	//modify value
		*(pI2.get()) = 5;
		cout << "----after modify value-----------" << endl;
		cout << "pi = " << *pI.get() << endl;
		cout << "pi count = " << pI.usecount() << endl;
		cout << "pi2 = " << *pI2.get() << endl;
		cout << "pi2 count = " << pI2.usecount() << endl;

		int *pInt = NULL;

		//相等性，不等性
		if (pI == pI2);
		if (pI != pI2);
		if (pI == pInt);
		if (pI != pInt);
		if (pInt == pI);
		if (pInt != pI);
		
		//判断空
		if (pI);
		if (!pI);
		
		pI2.reset(new int(4));
		cout << "----after reset-----------" << endl;
		cout << "pi = " << *pI.get() << endl;
		cout << "pi count = " << pI.usecount() << endl;
		cout << "pi2 = " << *pI2.get() << endl;
		cout << "pi2 count = " << pI2.usecount() << endl;
		
	}
	
	
	cout << "----after 析构 -----------" << endl;
	cout << "pi = " << *pI.get() << endl;
	cout << "pi count = " << pI.usecount() << endl;

	//多态
	{
		shared_ptr<B> pB(new B);
		shared_ptr<A> pA(new B);
		shared_ptr<A> pB2(pB);
		
		shared_ptr<A> pB3;
		pB3 = pB;
		pA = pB3;
		
		A* pAA = NULL;
		B* pBB = NULL;

		//普通指针相等性
		if (pAA == pBB);
		if (pBB == pAA);
		if (pAA != pBB);
		if (pBB != pAA);

		//智能指针相等性
		if (pA == pB);
		if (pB == pA);
		if (pA != pB);
		if (pB != pA);

		//智能指针和普通指针的相等性
		if (pAA == pA);
		if (pA == pAA);
		if (pAA != pB);
		if (pB != pAA);		
	}
	
	{
		// const  and   non-const
		shared_ptr<double> pSD(new double);
		shared_ptr<const double> pConstSD(pSD);		//ok    允许从 非const 转化为const
		//shared_ptr<double> pNormalSD(pConstSD); //error,   无法从const 转化为非const
	}
	

	//自定义析构方式
	{
		shared_ptr<double, MyDeleter> pTest(new double);
	}

	/*
		线程安全保障
		tips:
			shared_ptr 确保其进行 拷贝，析构时 引用计数的线程安全性。  即你在多线程下，对一个shared_ptr 进行拷贝，能够正常进行引用计数
	*/

	getchar();
	return 0;
}