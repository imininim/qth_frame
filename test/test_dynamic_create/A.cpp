#include "A.h"
#include <iostream>
using namespace std;

IMPLEMENT_DYNAMIC_CREATE(SubClassA, baseClass)

void SubClassA::print()
{
	cout << "hello, this is SubClassA!" << endl;
}
