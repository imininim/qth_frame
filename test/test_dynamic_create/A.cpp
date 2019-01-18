#include "A.h"
#include <iostream>
using namespace std;

IMPLEMENT_DYNAMIC_CREATE(SubClassA, baseClass, MODULE_A) //实现该类的自动创建，并设定其为模块A
IMPLEMENT_MESSAGE_MAP(SubClassA,baseClass)				 //实现消息处理表

	ON_MESSAGE(SubClassA,1, &SubClassA::one)  //注册消息1， 绑定其处理函数为 one
	ON_MESSAGE(SubClassA,2, &SubClassA::two)  //注册消息2， 绑定其处理函数为 two

	ON_MESSAGE(SubClassA,EVENT_HELLO_WORLD, &SubClassA::onEvent)  //注册一个事件， 绑定其处理函数为onEvent


void SubClassA::print()
{
	cout << "hello, this is SubClassA!" << endl;
}

int SubClassA::one(void* p, int len, ESendMethor)
{
	cout << "SubClassA::one" << endl;
	return MEC_OK; //协议执行成功
}

int SubClassA::two(void* p, int len, ESendMethor)
{
	if (!p || sizeof(std::string) != len)
		return MEC_ERROR; //协议校验错误

	std::string& str = *(std::string*)p;

	cout << "SubClassA::two say: " << str.c_str() << endl;
	return MEC_OK; 
}

int SubClassA::onEvent(void* p, int len, ESendMethor)
{
	cout << "SubClassA::onEvent hello world!" << endl;
	return 0;
}
