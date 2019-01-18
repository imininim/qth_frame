#include "B.h"
#include <iostream>
using namespace std;
IMPLEMENT_DYNAMIC_CREATE(SubClassB, baseClass, MODULE_B)  //实现该类的自动创建，并设定其为模块A
IMPLEMENT_MESSAGE_MAP(SubClassB,baseClass)				 //实现消息处理表
	
	ON_MESSAGE(SubClassB,EVENT_HELLO_WORLD, &SubClassB::onEvent)  //注册一个事件

void SubClassB::print()
{
	cout << "hello, this is SubClassB!" << endl;
}

int SubClassB::onEvent(void* p, int len, ESendMethor)
{
	cout << "SubClassB::onEvent hello world!" << endl;
	return 0;
}
