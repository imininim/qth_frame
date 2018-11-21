/**********************
派生类B
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
//接口头文件
#include "interface.h"

class SubClassB: public baseClass
{
public:
	virtual void print();  //  print   "hello, this is SubClassB"
	

	DECLARE_DYNAMIC_CREATE(SubClassB, baseClass)
};


#endif
