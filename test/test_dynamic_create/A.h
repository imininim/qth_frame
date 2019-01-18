/**********************
派生类A
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
#include "interface.h"

class SubClassA: public baseClass
{
	DECLARE_DYNAMIC_CREATE(SubClassA, baseClass)	//声明该类可以自动创建
	DECLARE_MESSAGE_MAP(SubClassA, baseClass)		//声明该类可以处理消息
public:
	virtual void print();  //  print   "hello, this is SubClassA"
	
	SubClassA() {std::cout << "SubClassA 构造" << std::endl;}
	~SubClassA() {std::cout << "SubClassA 析构" << std::endl;}

	//消息声明
	int one(void* p, int len, ESendMethor);
	int two(void* p, int len, ESendMethor);
	
	int onEvent(void* p, int len, ESendMethor);

	void direct() {std::cout << "SubClassA 直接调用" << std::endl;}
};




#endif
