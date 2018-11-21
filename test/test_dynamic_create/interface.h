/**********************
动态创建的基类
***********************/
#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "utility/utility.h"
#include "frame/dynamic_create.h"

//定义任意一个基类
class baseClass
{
public:
	virtual void print()				 = 0;

	DECLARE_BASE(baseClass)
};


#endif
