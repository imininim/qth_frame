/***************************
	动态创建机制
****************************/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//动态创建头文件
#include "frame/dynamic_create.h"
//基类接口
#include "interface.h"


int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	
	typedef RunTimeClassEx<baseClass>  ClassInfo;

	//遍历所有继承于baseClass，并且声明了动态创建的类
	//注意，这里没有实际包含 A.h 和 B.h, 但是创建出来的类是 class SubClassA 和 class SubClassB
	for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
	{
		baseClass* pNewClass = it->CreateObject(); //创建其派生对象
		if (pNewClass)
		{
			pNewClass->print(); //执行器派生类的print
			delete pNewClass;
		}
			
	}

	//创建指定的基类,实现   A* pA = new "A" 这样的操作
	{
		baseClass* pSubClassB = ClassInfo::CreateObject("SubClassB");
		if (pSubClassB)
			pSubClassB->print();
	}
	getchar();
	return 0;
}

