/***************************
function: 类的自动创建，管理， 模块划分 以及消息处理 测试
****************************/
//基类接口
#include "interface.h"

//直接使用子类对象才需要包含其头文件
#include "A.h"


using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;

	{
		//静态函数，看看能够创建的类对象有哪些
		std::string classInfo = ClassManager::dump();
		cout << classInfo.c_str() << endl;

		//创建一个这样的对象,这个对象无法被拷贝，接口之中所有的模块都会被自动创建 
		ClassManager m;
		
		//可以直接获取各个模块,并且无耦合的调用各个模块的接口
		baseClass* pA = m.GetModuleByID(MODULE_A);
		if (!pA) return 0;

		pA->print(); //调用模块A的虚函数

		baseClass* pB = m.GetModuleByID(MODULE_B);
		if (!pB) return 0;

		pB->print();//调用模块B的虚函数

		//也可以直接转化为子类的指针进行调用,但是需要包含 SubClassA所在的头文件，使得模块之间产生耦合
		SubClassA* pSubClassA = (SubClassA*)pA;
		pSubClassA->direct();

		//如果你觉得直接的指针强行转化不安全，你可以先验证一下 pA 是否指向一个 SubClassA 对象
		if (pA->GetRuntimeClass() == RUN_CLASS_INFO(SubClassA))
			cout << "pA 是一个指向 SubClassA 对象的指针!" << endl;
		else
			cout << "PA 不是一个指向 SubClassA 对象的指针" << endl;
		
		//给模块A 发送消息1
		pA->SendMsg(1);
		//给模块A 发送消息2 并携带参数，   参数是void* 类型，可以传入任何结构,只要在消息处理的时候使用相同的结构就行
		std::string str = "模块A消息2参数";
		pA->SendMsg(2, &str, sizeof(str));

		//尝试给模块B 发送消息，因为模块B没有声明可以处理消息，所以应该是返回失败
		int errCode = pB->SendMsg(1);
		if (errCode < MEC_OK)
		{
			/*
				MEC_OK  ==  0,  表示调用成功, 其他 errCode < 0 都是表示调用失败, errCode > 0 是使用者自定义的返回值
			*/

			std::cout << "模块B 执行消息1错误，错误码=" << errCode << endl;
			
		}

		//执行事件,所有的模块都会执行
		m.allModuleExcEvent(EVENT_HELLO_WORLD);
	}
			
	getchar();
	return 0;
}

