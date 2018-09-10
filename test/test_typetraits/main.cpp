/*
	测试 类型特性
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 traits头文件
#include"utility/typetraits.h"
class B{
public:
	typedef int		result_type;

	void operator()(){}
};		
union C{};						//代表联合
enum D{};						//代表枚举
class A{};						//代表类

class base{};					//代表基类
class Deriver: public base{};	//代表子类

void foo(){}

template<class T, int i = 0>
struct macro_test
{
	macro_test(const T& t):m_t(t)
	{
		cout << "begin: " << m_t << endl;
	}

	~macro_test()
	{
		cout << "end:" << m_t << endl;
	}

	const T& m_t;
};

struct sizeA
{
	int i;
};

struct sizeArray
{
	sizeA a[3];
};

//可序列化大小
struct customSize: public QTH_NAME_SPACE::size_info
{
	virtual size_t size() const
	{
		return 5;
	}
};

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	printType<B>("class"); 
	printType<D>("enum");
	printType<int>("int");
	printType<double>("double");
	printType<void(*)()>("function");
	printType<int&>("ref");
	printType<int*>("pointer");
	printType<char*>("char*");
	printType<const char*>("const char*");
	printType<std::string>("string");
	printType<const std::string>("const string");
	//去除修饰
	TypeTraits<const B>::RemoveConst b;      	//这里没有进行初始化     	 	b 的类型应该是  	B, 	而不是	const B
	TypeTraits<B&>::RemoveRef b2;				//这里没有进行初始化，  	 	b2 的类型应该是  	B  	而不是  B&
	//TypeTraits<const B&>::RemoveConst b3;   	//error 这里必须要进行初始化，	b3 的类型是 		B&  而不是	const B&
	TypeTraits<const B&>::RemoveModified b4;	//这里没有进行初始化， 			b4 的类型应该是 	B  	而不是 	const B&

	(void)b;
	(void)b2;
	(void)b4;

	//检测内置类型
	cout << "class A 是否存在内置类型 result_type ? " << has_interval_type<const A&>::value << endl;
	cout <<	"class B 是否存在内置类型 result_type ? " << has_interval_type<B>::value << endl;

	//检测继承
	cout << "base 是 Deriver 的基类? " << is_base_of<base, Deriver>::value << endl;
	cout << "base 是 A 的基类? " <<  is_base_of<base, A>::value << endl;
	
	// size_of
	std::string sizeStr("ac");
	sizeA a;
	sizeArray arr;
	customSize customsize;
	char c;

	cout << "std::string 大小: " << size_of(sizeStr) << endl;
	cout << "sizeA	  大小:"	 << size_of(a) << endl;
	cout << "suzeArray大小:"	<< size_of(arr) << endl;
	cout << "customsize大小:"	<< size_of(customsize) << endl;
	cout << "char  大小:"		<< size_of(c) << endl;

	getchar();
	return 0;
}
