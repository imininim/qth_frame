/**********************
派生类B
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
//接口头文件
#include "interface.h"

class SubClassB: public baseClass
{
	DECLARE_DYNAMIC_CREATE(SubClassB, baseClass) //声明该类可以自动创建
	DECLARE_MESSAGE_MAP(SubClassB, baseClass)	//声明该类可以处理消息
public:
	virtual void print();  //  print   "hello, this is SubClassB"
	
	SubClassB() {std::cout << "SubClassB 构造" << std::endl;}
	~SubClassB() {std::cout << "SubClassB 析构" << std::endl;}

	int onEvent(void* p, int len, ESendMethor);
};


#endif
