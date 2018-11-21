#include "B.h"
#include <iostream>
using namespace std;
IMPLEMENT_DYNAMIC_CREATE(SubClassB, baseClass, 2)

void SubClassB::print()
{
	cout << "hello, this is SubClassB!" << endl;
}
