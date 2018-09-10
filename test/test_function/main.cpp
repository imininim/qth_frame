/*
	测试 function
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 函数对象头文件
#include "utility/function.h"


//测试一般指针
void foo0(int i)
{
	cout << "foo0 = "<<i << endl;
}

int fooF0(void)
{
	return 0;
}

double fooF1(int i, double& d)
{
	return d;
}

//仿函数
struct foo1
{
	void operator()(int i)
	{
		cout << "foo1 = "<<i << endl;
	}
};

struct foo1F0
{
	int operator()()
	{
		return 1;
	}
};


//成员函数
struct foo2
{
	void foo(int& i)
	{
		cout << "foo2 = "<<i << endl;
		++i;   //传引用的方式，看看能不能成功修改值
	}
};

struct foo2F0
{
	int foo(void)
	{
		return 2;
	}
};

//演示策略的使用
void function_test(const QTH_NAME_SPACE::function<int(void)>&  op)
{
	//执行策略前 你会准备需要的参数或者做些其他事情
	cout << op() << endl; //执行策略， 当然，这个策略不需要参数，所以你什么都不传递
	//...
}


int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;

	///////////////////////基本使用
	QTH_NAME_SPACE::function<int(void)> f(&fooF0);
	cout << "一般函数指针: " << f() << endl;

	foo1F0 foo1fo;
	QTH_NAME_SPACE::function<int(void)> f1(foo1fo);
	cout << "仿函数: " << f1() << endl;

	foo2F0 foo2fo;
	QTH_NAME_SPACE::function<int(void)> f2(&foo2F0::foo, &foo2fo);
	cout << "成员函数指针: " << f2() << endl;
	cout << "--------分割-------------" << endl;
	/////////////////////////
	// 作用， 通常被用来形成某种策略
	// 例如
	function_test(f1);		//使用策略1
	function_test(f2);		//使用策略2
	function_test(f);		//使用策略3

	cout << "--------分割-------------" << endl;
	f1 = f;     //一致的对待 函数指针，和仿函数对象
	cout << "after f1 = f:  " << f1() << endl;

	f2 = f1;    //一致的对待仿函数，函数指针
	cout << "after f2 = f1 " << f2() << endl;

	QTH_NAME_SPACE::function<int(void)> f_empty; 
	f2 = f_empty;	//  f2和f_empty模板参数必须一致
			
	if (f2)
		cout << "这是一个可用的 function" << endl;

	if (!f2)
		cout << "这是一个空的 function" << endl;

	getchar();
	return 0;
}