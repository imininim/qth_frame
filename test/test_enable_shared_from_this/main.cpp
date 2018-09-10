/*
	测试 enable_shared_from_this
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 服务头文件
#include "utility/memory.h"

class A: public QTH_NAME_SPACE::enable_shared_from_this<A>
{
	public:
	A(int _i):i(_i){}
	

	void invoke_foo()
	{
		shared_from_this()->foo(); //获取类A的智能指针，  如果调用shared_from_this 时不存在A的智能指针， 则返回一个空的智能指针对象

		/*备注：
		 *		1.你不可以在构造函数中调用 shared_from_this ,  因为这时候尚未有A 的智能指针存在
		 *	
		 *
		 * */
	}



	void foo()
	{
		cout << "A::i = " << i << endl;
	}
  
	  int i;
};

void testMain()
{
	using namespace QTH_NAME_SPACE;

	shared_ptr<A> p(new A(1) );
	p->invoke_foo();

	shared_ptr<A> p2 = p->shared_from_this();
	p2->foo();

	//修改一下值
	p2->i = 3;

	p->invoke_foo();
	p2->foo();
}


int main(int argc, char* argv[])
{
	testMain();
	getchar();
	return 0;
}
