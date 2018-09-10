/**********************
派生类A
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
#include "interface.h"

//动态创建头文件
#include "utility/utility.h"
#include "frame/dynamic_create.h"

class SubClassA: public baseClass
{
public:
	virtual void print();  //  print   "hello, this is SubClassA"
	
	DECLARE_DYNAMIC_CREATE(SubClassA,baseClass)
};


#endif
