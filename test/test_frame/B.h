/*********************
function: 模块 B 

**********************/
#ifndef _B_H
#define _B_H
#include "utility/utility.h"
#include "frame/running_class_info.h"	

using std::cout;
using std::endl;

class B: public Module
{
public:
	B() {cout << "B Create" << endl; };
	~B(){cout << "B Destroy" << endl; };

	int id_one(void*, int, ESendMethor)			{cout << "B_one" << endl; return 0;}
	int id_two(void*, int, ESendMethor)			{cout << "B_two" << endl; return 0;}
	int id_three(void*, int, ESendMethor)		{cout << "B_tree" << endl; return 0;}
	int id_4(void*, int, ESendMethor)			{cout << "B_4" << endl; return 0;}

	int from_A(void*, int, ESendMethor e);
	QTH_MODULE_DECLARE(B)			//声明动态创建机制和
};




#endif
