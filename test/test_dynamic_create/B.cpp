﻿#include "B.h"
#include <iostream>
using namespace std;

IMPLEMENT_DYNAMIC_CREATE(SubClassB, baseClass)

void SubClassB::print()
{
	cout << "hello, this is SubClassB!" << endl;
}
