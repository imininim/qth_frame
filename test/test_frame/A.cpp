#include "A.h"

//QTH_MODULE_IMPLEMENT(A, Module)
QTH_MODULE_IMPLEMENT_WITH_ID(A, Module, 1)
	QTH_ON_MESSAGE(A, 1, &A::id_one)  //响应协议 1
	QTH_ON_MESSAGE(A, 2, &A::id_two)  //响应协议 2
	QTH_ON_MESSAGE(A, 3, &A::id_three) //响应协议 3
	QTH_ON_MESSAGE(A, 4, &A::send_message_to_B) //A响应协议4，发送协议10给模块B
	QTH_ON_MESSAGE(A, 5, &A::getKind) //响应协议5

int A::send_message_to_B(void*, int, ESendMethor)
{
	Module* pB = GetManager()->GetModuleByID(2);
	if (!pB)		//不存在模块
		return -1;

	pB->SendMsg(10, NULL, 0, SEND_BY_INTERNAL); //发送消息10, 发送方式为内部发送
	return 0;
	/*
		也可以 B* p = (B*)pB;
		p->from_A(...);
		这种方式会产生耦合， 最好不用
	*/
}

//通过类型名，判断是否是这个类型
int A::getKind(void*, int, ESendMethor)
{
	A* pA = (A*)isKindOf(QTH_RUNCLASS(A));
	if (pA)
	{
		cout << "该模块实际所指对象为A" << endl;
	}
	else
	{
		cout << "该模块实际所指对象不为A" << endl;
	}
	return 0;
}
