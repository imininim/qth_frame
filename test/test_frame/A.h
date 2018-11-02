/*********************
function: 模块A 
structrue:
	本模块由三个部分组成:
	1	模块类 class A;
	
	2	模块化集成宏
		QTH_MODULE_DECLARE(模块名称) 											此宏应该声明在模块类中
		QTH_MODULE_IMPLEMENT_WITH_ID(模块名，基类模块名， 用户自定义模块ID)		此宏应该声明在类外，最好能够声明在.cpp中

	3	模块的消息处理分派宏
		QTH_ON_MESSAGE(模块名，需要处理的消息ID， 处理该消息的函数)				此宏需要声明在QTH_MODULE_IMPLEMENT_WITH_ID之后

tips:
	当你同时具备上述条件 1 和 2 时， 本模块具备动态创建特性， 当 main.cpp执行时自动创建该模块的对象
	当你同时具备上述条件1,2,3   时， 本模块可以处理消息，消息格式由main.cpp定义， 消息可以来自网络，文件读取或者用户输入
**********************/
#ifndef _A_H
#define _A_H
#include "utility/utility.h"
#include "frame/module_manager.h"
using std::cout;
using std::endl;

class A: public Module
{
public:
	A() {cout << "A Create" << endl; };
	~A(){cout << "A Destroy" << endl; };


	int id_one(void*, int, ESendMethor)		{cout << "A_One" << endl; return 0;}
	int id_two(void*, int, ESendMethor)		{cout << "A_Two" << endl; return 0;}
	int id_three(void*, int, ESendMethor)	{cout << "A_Three" << endl; return 0;}
	
	//向其他模块发送协议
	int send_message_to_B(void*, int, ESendMethor);
	
	//类型信息识别
	int getKind(void*, int, ESendMethor);


	int i;	//使得类A的大小为4
	QTH_MODULE_DECLARE(A)			//声明动态创建机制
};

#endif
