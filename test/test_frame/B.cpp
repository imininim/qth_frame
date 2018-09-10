#include "B.h"

//QTH_MODULE_IMPLEMENT(B, Module)
QTH_MODULE_IMPLEMENT_WITH_ID(B, Module, 2)
	QTH_ON_MESSAGE(B, 1, &B::id_one)  //响应协议 1
	QTH_ON_MESSAGE(B, 2, &B::id_two)  //响应协议 2
	QTH_ON_MESSAGE(B, 3, &B::id_three) //响应协议 3
	QTH_ON_MESSAGE(B, 4, &B::id_4) //响应协议 4
	QTH_ON_MESSAGE(B, 10, &B::from_A) //响应协议 3

int B::from_A(void*, int, ESendMethor e)
{
	if (e == SEND_BY_CLIENT)
	{
		cout << "模块B 的协议号10 不处理来自外部发送的指令!" << endl;
		return -1;
	}

	cout << "B: 接收到来自模块A的指令!" << endl;
	return 0;
}

