/**********************
派生类B
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
//接口头文件
#include "interface.h"
//动态创建头文件
#include "utility/utility.h"
#include "frame/dynamic_create.h"

class SubClassB: public baseClass
{
public:
	virtual void print();  //  print   "hello, this is SubClassB"
	
	DECLARE_DYNAMIC_CREATE(SubClassB, baseClass)
};


#endif
