/*
	测试 单例
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 单例头文件
#include "utility/single.h"

/////////////////////////////////////////     方法1
struct A: public QTH_NAME_SPACE::Singleton<A>
{
	friend class QTH_NAME_SPACE::Singleton<A>;


	A(){ cout << "A::A" << endl;}
	~A(){ cout << "A::~A" << endl;}
	int i;
};
//////////////////////////////////////////////////  方法2
struct B
{	
	B(){ cout << "B::B" << endl;}
	~B(){ cout << "B::~B" << endl;}
	int i;
};

typedef QTH_NAME_SPACE::Singleton<B,1>   SingleB; //第二个模版参数指定生存周期， 默认是0， 因此， B的单例生存周期比A长，  A 在 B 之前被释放

////////////////////////////////////////////////// 方法3
//自定义构造、删除器
template<class T>
class CreateMethod
{
public:
	static T* Create()				
	{
		cout << "通过构造器构造" << endl;
		return new T;
	}
	static void Destroy(T* p)		
	{
		cout << "通过删除器销毁" << endl;
		delete p;
	}
};

struct C: public QTH_NAME_SPACE::Singleton<C, 0, CreateMethod>
{
	friend class QTH_NAME_SPACE::Singleton<C, 0, CreateMethod>;
	int i;
};


//////////////////////////////////////////////////////////////////错误情况1
struct D: public QTH_NAME_SPACE::Singleton<D, 1> 
{
	~D(){}
	int i;
};

struct E: public QTH_NAME_SPACE::Singleton<E, 2>
{
	~E()
	{
		//E 的寿命高于 D, 在 E析构的时候，D必然已经被析构， 这时候去使用D 会抛 SingletonException, 此异常 public std::exception
		D::Instance().i = 4;
	}

	int i;
};

int main(int argc, char* argv[])
{
	A::Instance().i = 0;
	SingleB::Instance().i = 2;
	C::Instance().i = 3;
	//错误1 演示
	/*D::Instance().i = 3;
	E::Instance().i = 4;*/
	//getchar();
	return 0;
}
