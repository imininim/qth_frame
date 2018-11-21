/***************************
	动态创建机制测试

原始需求如下:
	clsas A{}
	class B : public A{}
	class C : public A{}
	
	A* p = NULL;
	if (xxx)  p = new B;
	else      p = new C;

本例子改良如下:
	A* p = NULL;
	std::string str;
	cin >> str;

	p = Create(str);   //如果str=="A" 则 new A，   如果str=="B"则new B  , 否则返回NULL
	
****************************/
//基类接口
#include "interface.h"


using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	
	typedef RunTimeClassEx<baseClass>  ClassInfo;

	std::vector<baseClass*> moduleMap;
	moduleMap.resize(UCHAR_MAX);
	for (int i = 0; i < UCHAR_MAX; ++i)
		moduleMap[i] = NULL;

	//遍历所有继承于baseClass，并且声明了动态创建的类
	//注意，这里没有实际包含 A.h 和 B.h, 但是创建出来的类是 class SubClassA 和 class SubClassB
	for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
	{
		cout << "类名:" << it->m_pClassName << endl;
		cout << "ID:" << (unsigned int)it->classID << endl;
		cout << "创建地址:" << it->m_pCreateFn << endl;
		cout << "---------------------" << endl;

		baseClass* pNewClass = it->CreateObject(); //创建其派生对象
		if (pNewClass)
		{
			pNewClass->print(); //执行器派生类的print
			moduleMap[it->classID] = pNewClass;
		}
		
		cout << endl;
	}

	//创建指定的基类,实现   A* pA = new "A" 这样的操作
	{
		cout << "通过字符串创建类 SubClassB:" << endl; 
		baseClass* pSubClassB = ClassInfo::CreateObject("SubClassB");
		if (pSubClassB)
		{
			pSubClassB->print();
			delete pSubClassB;
		}
			
	}

	//消息派发
	{
		cout << "module=1 msg=1:" << endl;

		//获取消息列表
		const baseClass::MSG_DEAL* pFunc = moduleMap[1]->GetMessageMap();
		//获取指令1
		const baseClass::MSG_DEAL pDeal1 = pFunc[1];
		if (pDeal1) //执行指令1
		{
			(moduleMap[1]->*pDeal1)(NULL,0,SEND_BY_CLIENT);
		}
		
	}

	for (int i = 0; i < UCHAR_MAX; ++i)
		SAFE_DELETE(moduleMap[i]);

	getchar();
	return 0;
}

