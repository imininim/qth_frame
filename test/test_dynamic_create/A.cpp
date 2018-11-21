#include "A.h"
#include <iostream>
using namespace std;

IMPLEMENT_DYNAMIC_CREATE(SubClassA, baseClass, 1)
IMPLEMENT_MESSAGE_MAP(SubClassA,baseClass)
ON_MESSAGE(SubClassA,1, &SubClassA::one)

void SubClassA::print()
{
	cout << "hello, this is SubClassA!" << endl;
}

int SubClassA::one(void* p, int len, ESendMethor)
{
	cout << "SubClassA::one" << endl;
	return 0;
}
