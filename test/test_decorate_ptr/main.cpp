/*
	测试 装饰指针
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;

//#define NO_DECORATE_PTR

//step2 装饰指针头文件
#include "utility/decorate_ptr.h"

//可选 提供加锁，解锁
#include "utility/Threads.h"

class A
{
public:
	virtual void foo()	
	{ 
		cout << "fooA" << endl; 
		for(int i = 0; i < 100000000; ++i); //让函数foo 运行一段时间
	}

	int i;

};

class B: public A
{
public:
	virtual void fooB()	
	{ 
		cout << "fooB" << endl; 
		for(int i = 0; i < 300000000; ++i); //让函数foo 运行一段时间
	}
};

struct C
{
	void Lock() { cout << "上锁" << endl;}
	void unLock() {cout << "解锁" << endl;}
};

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;

	//decorate_ptr 功能演示
	{
		/*
			功能： 1 统计一个功能模块被使用的次数以及其总的运行时间
		*/
		decorate_ptr<A> ptr(new A, 1);				//统计类A 被使用的次数, 第二个参数唯一的标识了这个功能模块
		ptr->foo();									//调用class A 的任何一个成员函数或者成员变量都会使得被调用次数+1
		//(*ptr).foo();								// error *操作符被禁止使用


		//此成员函数将模块信息打印到cout 和日志文件中 输出格式为 "[模块统计(id)] 模块累计调用次数 count"
		//注意， 此函数会使计数器+1
		ptr.out_decorate();							

		decorate_ptr<B,std::string> ptrS(new B, "class B"); //以字符串作为标识
		ptrS->foo();
		ptrS.out_decorate();

	}
	cout << "------------------分割线------------" << endl << endl;
	{
		/*
			功能2： 使得一个对象的接口或成员函数在调用之前自动上锁，调用完成后自动解锁				
			params:
				模板参数1：  需要进行上锁的数据
				模板参数2：	 对象标识符，在此功能中固定为int或long 都可以
				模板参数3:	 传递锁的类型, 只要是拥有 Lock 和 unLock的接口都可以
				模板参数4:   固定是 DecoratorLock，表明这是用于上锁解锁的装饰指针
		*/	
		typedef decorate_ptr<A, long, C, DecoratorLock> LockAPtr; // C 为一个提供 Lock  和 unLock, 接口的类

		LockAPtr ptr(new A, 1);
		ptr->foo();

		LockAPtr ptrCopy;
	}

	cout << "------------------分割线------------" << endl << endl;
	////////////////////////////以下为 decorate_ptr 所支持的指针操作	
	//拷贝
	{
		decorate_ptr<A> ptr0(new A);						
		decorate_ptr<A> ptr1;

		cout << "begin---------------------------------" << endl;
		if (ptr0)	{cout << "ptr0 引用了一个对象" << endl;}
		else		{cout << "ptr0 为空" << endl;}

		if (ptr1)	{cout << "ptr1 引用了一个对象" << endl;}
		else		{cout << "ptr1 为空" << endl;}

		ptr0->foo();
		ptr1 = ptr0; 

		cout << "afer copy --------ptr1 = ptr0;--------------" << endl;
		if (ptr0)	{cout << "ptr0 引用了一个对象" << endl;}
		else		{cout << "ptr0 为空" << endl;}

		if (ptr1)	{cout << "ptr1 引用了一个对象" << endl;}
		else		{cout << "ptr1 为空" << endl;}

		//拷贝后， 两个指针指向同一个对象
		ptr0->foo();
		ptr1->foo();
		
		//获取当前引用的人数
		cout << "user num = " << ptr0.use_count() << endl;
	}
		
	//相等性，不等性， 比较性
	{
		decorate_ptr<A> dp1(new A);						
		decorate_ptr<A> dp2(new A);
		A* p = NULL;

		//相等性，不等性
		if (dp1 == dp2);
		if (dp2 != dp1);
		if (p == dp1);
		if (dp1 != p);
				
		//判断空
		if (dp1);
		if (!dp1);
	}

	//多态性
	{
		decorate_ptr<B> pB(new B);
		decorate_ptr<A> pA(new B);
		decorate_ptr<A> pB2(pB);
				
		A* pAA = NULL;
		B* pBB = NULL;

		//普通指针相等性
		if (pAA == pBB);
		if (pBB == pAA);
		if (pAA != pBB);
		if (pBB != pAA);

		//装饰指针相等性
		if (pA == pB);
		if (pB == pA);
		if (pA != pB);
		if (pB != pA);

		//装饰指针和普通指针的相等性
		if (pAA == pA);
		if (pA == pAA);
		if (pAA != pB);
		if (pB != pAA);		
	}
	//
	//常量， 非常量
	{
		decorate_ptr<A> dpA(new A);
		decorate_ptr<const A> dpcA(new A);
			
		decorate_ptr<const A> dpConst(dpA); //ok 允许从 non-const conversion const
		//decorate_ptr<A> dpNonConst(dpcA);	//no 不允许从 const conversion non-const
	}
		
	getchar();
	return 0;
}
