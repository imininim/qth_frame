/**********************
派生类A
***********************/
#ifndef MODULEA_H_
#define MODULEA_H_
#include "interface.h"

class SubClassA: public baseClass
{
public:
	virtual void print();  //  print   "hello, this is SubClassA"
	
	int one(void* p, int len, ESendMethor);

	DECLARE_DYNAMIC_CREATE(SubClassA, baseClass)
	DECLARE_MESSAGE_MAP(SubClassA, baseClass)
};




#endif
