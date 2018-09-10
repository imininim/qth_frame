/*
	测试 bind
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 bind头文件
#include"utility/functional.h"

using std::endl;
using std::cout;

//全局函数
int foo(int i, int& ri, const std::string& str)
{
	//	输出
	cout << "i = " << i << endl;
	cout << "ri = " << ri << endl;
	cout << "str = " << str.c_str() << endl;
	//修改参数
	i = 1;
	ri = 2;

	return i;
}

//仿函数
struct SFoo
{
	typedef int 	result_type;    //使用 bind 绑定仿函数必须将  operator() 的返回值类型定义为  result_type

	int operator()(int i, int& ri, const std::string& str)
	{

		//	输出
		cout << "i = " << i << endl;
		cout << "ri = " << ri << endl;
		cout << "str = " << str.c_str() << endl;

		//修改参数
		i = 11;
		ri = 22;

		return i;
	}
};

//成员函数
class CFoo
{
public:
	int foo(int i, int& ri, const std::string& str)
	{

		//	输出
		cout << "i = " << i << endl;
		cout << "ri = " << ri << endl;
		cout << "str = " << str.c_str() << endl;

		//修改参数
		i = 111;
		ri = 222;

		return i;
	}
};


int b = 999;
int i = b;
int& ri = b;

void reset()
{
	b = 999;
	i = b;
}

void print()
{
	cout << "modified after : i = " << i << endl;
	cout << "modified after : ri = " << ri << endl;
}

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;

	//一般函数使用
	cout << "--------------------------- 一般函数使用1 ---------------------------------" << endl;
	function<int(int, int&)> f1 = bind(foo, placeholders::_1, placeholders::_2, std::string("一般函数使用"));
	f1(i, ri);	
	print();   	//打印全局变量  i , ri
	reset();	//重新设定 i, ri 为初始值

	cout << "--------------------------- 一般函数使用2 ---------------------------------" << endl;
	function<int(int)> f2 = bind(f1, placeholders::_1, QTH_NAME_SPACE::ref(ri)); //注意： bind 想要传递引用，需要使用 QTH_NAME_SPACE::ref()  包装参数
	f2(i);
	print();
	reset();
	cout << "--------------------------- 一般函数使用3 ---------------------------------" << endl;
	function<int(void)> f3 = bind(f2, i);
	f3();
	print();
	reset();


	cout << "--------------------------- 成员函数1 ---------------------------------" << endl;
	CFoo fo;
	function<int(int,int&)> f4 = bind(&CFoo::foo, &fo, placeholders::_1, placeholders::_2, std::string("成员函数使用"));
	f4(i,ri);
	print();
	reset();

	cout << "--------------------------- 成员函数2  ---------------------------------" << endl;
	function<int(int)> f5 = bind(f4, placeholders::_1, QTH_NAME_SPACE::ref(ri));
	f5(i);
	print();
	reset();


	cout << "--------------------------- 成员函数3  ---------------------------------" << endl;
	function<int(void)> f6 = bind(f5, i);
	f6();
	print();
	reset();

	//仿函数使用
	cout << "--------------------------- 仿函数1  ---------------------------------" << endl;
	function<int(int,int&)> f7 = bind(SFoo(), placeholders::_1, placeholders::_2, std::string("仿函数使用")); 
	f7(i,ri);
	print();
	reset();

	//std::标准仿函数
	function<bool(int,int)>  f8 = bind(std::less<int>(), placeholders::_1, placeholders::_2);
	cout << "标准仿函数: std::less<int> : " << f8(1,3) << endl;
		
	getchar();
	return 0;
}

