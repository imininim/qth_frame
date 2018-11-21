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
#include "frame/macro.h"

//消息来源枚举
enum ESendMethor
{
	SEND_BY_CLIENT,		//客户端调用
	SEND_BYINTERNAL,	//内部调用
};

BEGIN_NAMESPACE

	//类型信息结构
	template<class T>
	class RunTimeClassEx
	{
	public:
		M_TYPE classID;						//类ID
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
	
	//完成类型信息的连接工作
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

	//消息映射表结构体
	template<class Func>
	struct MessageMap
	{
		MessageMap()	{for (MSG_TYPE i = 0; i < QTH_MAX_MESSAGE_NUM; ++i) message[i] = NULL;}
		~MessageMap(){}

		Func message[QTH_MAX_MESSAGE_NUM];
	};
	
	//消息连接
	struct pushMessage
	{
		template<class Func>
		pushMessage(MessageMap<Func>* pMap, MSG_TYPE msgID, Func pfnMsg)
		{ pMap->message[msgID] = pfnMsg;}
	};

END_NAMESPACE


/*******************************
动态创建声明
params:
	className		当前类的名称
	baseClassName	继承的抽象类名称
********************************/
//基类声明操作
#define DECLARE_BASE(baseClassName)\
	public:\
	virtual QTH_NAME_SPACE::RunTimeClassEx<baseClassName>* GetRuntimeClass() const {return NULL;}\
	typedef int (baseClassName::*MSG_DEAL)(void*,int,ESendMethor);\
	virtual const MSG_DEAL* GetMessageMap(void) const {return NULL;}

//获取运行时类型信息的宏
#define RUN_CLASS_INFO(className)	(&className::class##className)

//动态创建声明
#define DECLARE_DYNAMIC_CREATE(className, baseClassName) \
	public:\
	static QTH_NAME_SPACE::RunTimeClassEx<baseClassName> class##className;\
	static baseClassName* CreateObject();\
	virtual QTH_NAME_SPACE::RunTimeClassEx<baseClassName>* GetRuntimeClass() const {return RUN_CLASS_INFO(className);}

//动态创建实现
#define IMPLEMENT_DYNAMIC_CREATE(className, baseClassName, moduleID)\
	static char str##className[] = #className;\
	QTH_NAME_SPACE::RunTimeClassEx<baseClassName> className::class##className = {moduleID, str##className, NULL, &className::CreateObject};\
	static QTH_NAME_SPACE::initRunTimeClassEx<QTH_NAME_SPACE::RunTimeClassEx<baseClassName> > init_##className(RUN_CLASS_INFO(className));\
	baseClassName* className::CreateObject()\
	{return new className;}

//声明消息映射
#define DECLARE_MESSAGE_MAP(className, baseClassName)\
	public:\
	typedef int (className::*MSG_DEAL)(void*,int,ESendMethor);\
	virtual const baseClassName::MSG_DEAL* GetMessageMap(void) const;\
	static QTH_NAME_SPACE::MessageMap<className::MSG_DEAL> s_messageMap;

//实现消息映射表
#define IMPLEMENT_MESSAGE_MAP(className, baseClassName)\
	QTH_NAME_SPACE::MessageMap<className::MSG_DEAL> className::s_messageMap;\
	const baseClassName::MSG_DEAL* className::GetMessageMap(void) const\
{\
	QTH_NAME_SPACE::MessageMap<className::MSG_DEAL>* pMap = &className::s_messageMap;\
	return reinterpret_cast<const baseClassName::MSG_DEAL*>(pMap->message);\
}

//消息声明
#define ON_MESSAGE(className, msgID, func)\
	static QTH_NAME_SPACE::pushMessage className##msgID((&className::s_messageMap),msgID, func);

#endif
