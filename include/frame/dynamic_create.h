/**************************************
author:			qutianhao
function:		动态创建机制
date:			2018/07/25
tips:	
	提供以类名称创建类的机制
	类似于如下:
	A* pA = new "A"      其中 "A" 是一个字符串，可以来自与网络、用户输入或者一个本地文件
***************************************/
#ifndef DYNAMIC_CREATE_H_
#define DYNAMIC_CREATE_H_
BEGIN_NAMESPACE

	
	template<class T>
	class RunTimeClassEx
	{
	public:
		char* m_pClassName;					//类名称	
		RunTimeClassEx* m_pNext;			//指向下一个类节点
		
		T* (*m_pCreateFn)();				//指向创建出这个类对象的函数
		T* CreateObject();	

		static RunTimeClassEx* s_pFirst;	//指向首类节点
		static T* CreateObject(const char* strObj);
	};
		
	
	template<class T> RunTimeClassEx<T>* RunTimeClassEx<T>::s_pFirst = NULL;
	
	template<class T>
	T* RunTimeClassEx<T>::CreateObject()
	{
		if (!m_pCreateFn) return NULL;
		return (*m_pCreateFn)();
	}
	
	template<class T>
	T* RunTimeClassEx<T>::CreateObject(const char* strObj)
	{
		typedef RunTimeClassEx<T>  ClassInfo;
		for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
		{
			if (strObj && it->m_pClassName && strcmp(strObj, it->m_pClassName) == 0)
				return it->CreateObject();
		}

		return NULL;
	}


	
	template<class RTC>
	struct initRunTimeClassEx
	{
		initRunTimeClassEx(RTC* pNewClass)
		{
			if (RTC::s_pFirst != NULL)
				pNewClass->m_pNext = RTC::s_pFirst;
				
			RTC::s_pFirst = pNewClass;
		}
	};
END_NAMESPACE


/*******************************
动态创建声明
params:
	className		当前类的名称
	baseClassName	继承的抽象类名称
********************************/
#define DECLARE_DYNAMIC_CREATE(className, baseClassName) \
	public:\
	static QTH_NAME_SPACE::RunTimeClassEx<baseClassName> class##className;\
	static baseClassName* CreateObject();

#define IMPLEMENT_DYNAMIC_CREATE(className, baseClassName)\
	static char str##className[] = #className;\
	QTH_NAME_SPACE::RunTimeClassEx<baseClassName> className::class##className = {str##className, NULL, &className::CreateObject};\
	static QTH_NAME_SPACE::initRunTimeClassEx<QTH_NAME_SPACE::RunTimeClassEx<baseClassName> > init_##className(&className::class##className);\
	baseClassName* className::CreateObject()\
	{return new className;}

#endif
