/**************************************
author:			qutianhao
function:		动态创建机制,运行时类型信息识别机制， 模块划分，消息派发机制
date:			2018/07/25
tips:	
	提供以类名称创建类的机制
	类似于如下:
	A* pA = new "A"      其中 "A" 是一个字符串，可以来自与网络、用户输入或者一个本地文件

	提供对于类模块的统一管理，以及消息的派发,
***************************************/
#ifndef DYNAMIC_CREATE_H_
#define DYNAMIC_CREATE_H_
#include "frame/macro.h"
#include "utility/nocopyable.h"
#include "utility/premacro.h"
#include <string.h>

//错误码
enum EManagerErrCode
{
	MEC_OK								=0,			//成功返回
	MEC_ERROR							=-1,		//执行协议成功，协议处理中返回了一个错误
	MEC_MODULE_NOT_FIND					=-2,		//模块未找到
	MEC_MODULE_NOT_DECLARE_DYN_CREATE	=-3,		//类并没有声明或实现动态创建
	MEC_MODULE_NOT_DEAL_MSG				=-4,		//类并没有声明或实现可以处理消息
	MEC_MODULE_MSG_NOT_REGISTER			=-5,		//该消息ID没有被注册
	MEC_MSG_ID_NOT_EVENT				=-6,		//不属于一个事件编号
};


//消息来源枚举
enum ESendMethor
{
	SEND_BY_CLIENT,		//客户端调用
	SEND_BY_INTERNAL,	//内部调用
	SEND_BY_EVENT,		//事件通告
	SEND_NO_COST,

	//以下为用户自定义消息来源
	SEND_BY_CUSTOM1,
	SEND_BY_CUSTOM2,
	SEND_BY_CUSTOM3,
	SEND_BY_CUSTOM4,
	SEND_BY_CUSTOM5,
};

BEGIN_NAMESPACE
	
//模块默认优先级
#define MODULE_PRIORITY		10

	//类型信息结构
	template<class T>
	class RunTimeClassEx
	{
	public:
		M_TYPE classID;						//类ID
		BYTE priority;						//模块优先级
		std::string m_strClassName;			//类名称	
		RunTimeClassEx* m_pNext;			//指向下一个类节点
		T* (*m_pCreateFn)();				//指向创建出这个类对象的函数

		T* CreateObject();	
		static RunTimeClassEx* s_pFirst;			//指向首类节点
		static T* CreateObject(M_TYPE classID);		//依据类名ID创建类对象
		static T* CreateObject(const char* strObj); //依据类名称创建类对象

		//依据类ID 查找类名称
		static const char*			findRunTimeClass(M_TYPE classID);
		//依据类名称查找类ID, 查找不到返回 QTH_MAX_PROTOCOL_NUM
		static M_TYPE				findRunTimeClass(const char* strObj);
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
			if (strObj && strcmp(strObj, it->m_strClassName.c_str()) == 0)
				return it->CreateObject();
		}

		return NULL;
	}

	template<class T>
	T* RunTimeClassEx<T>::CreateObject(M_TYPE classID)
	{
		typedef RunTimeClassEx<T>  ClassInfo;
		for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
		{
			if (classID == it->classID)
				return it->CreateObject();
		}

		return NULL;
	}

	template<class T>
	const char* RunTimeClassEx<T>::findRunTimeClass(M_TYPE classID)
	{
		typedef RunTimeClassEx<T>  ClassInfo;
		for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
		{
			if (classID == it->classID)
				return it->m_strClassName.c_str();
		}

		return NULL;
	}

	template<class T>
	M_TYPE RunTimeClassEx<T>::findRunTimeClass(const char* strObj)
	{
		typedef RunTimeClassEx<T>  ClassInfo;
		for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
		{
			if (strObj && strcmp(strObj, it->m_strClassName.c_str()) == 0)
				return it->classID;
		}

		return QTH_MAX_PROTOCOL_NUM;
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


//ID到数组索引的转换宏, 将一个ID映射到 [0,size) 区域范围内
#define ID_TO_INDEX(id, size)		((id)%size)

//模块的事件数据结构
struct SModuleEvent
{
	SModuleEvent(M_TYPE _id, BYTE _pri = 0)
	{
		priority = _pri;
		moduleID = _id;
	}

	//相等性
	bool operator==(const SModuleEvent& other)					{return moduleID == other.moduleID;}
	const bool operator==(const SModuleEvent& other)	const	{return moduleID == other.moduleID;}

	//不等性
	bool operator!=(const SModuleEvent& other)					{return moduleID != other.moduleID;}
	const bool operator!=(const SModuleEvent& other)	const	{return moduleID != other.moduleID;}

	//比较, 优先级相等，则按照模块编号排序
	bool operator<(const SModuleEvent& other)					
	{
		return (priority == other.priority) ? moduleID < other.moduleID : (priority < other.priority);
	}

	const bool operator<(const SModuleEvent& other)		const	
	{
		return (priority == other.priority) ? moduleID < other.moduleID : (priority < other.priority);
	}

	BYTE priority;		//执行优先级
	M_TYPE moduleID;	//注册了一个事件的模块ID
};

/******************************
function: 模块管理和消息派发器
模板参数:
T				生成的接口类， 所有模块的公共接口
moduleMaxNum	模块数量
OwnerType		管理器所有者的类型


eventStart		事件编号起始符, 不包括起始符本身
eventEnd		事件编号结束符, 不包括起始符本身
****************************/
template<class T, 
	M_TYPE		moduleMaxNum = QTH_MAX_PROTOCOL_NUM,
	class		OwnerType = void, 
	MSG_TYPE	eventStart = 199,
	MSG_TYPE	eventEnd = QTH_MAX_MESSAGE_NUM
>
class CDataManager: private Nocopyable
{
public:
	enum {
		EVENT_NUM = (eventEnd - eventStart - 1),    //事件数量
		MODULE_NUM = moduleMaxNum,					//模块数量
	};

	explicit CDataManager(OwnerType* pOwner = NULL);
	~CDataManager();
	//获取该模块管理器的所有者
	inline OwnerType* GetOwner(void)			{return m_pOwner;}
			
	//获取指定用户模块的接口
	T* GetModuleByID(M_TYPE moduleID);

	/*
		function: 消息分派,将一条协议快速分派给指定模块的具体处理函数
		params:
			moduleID	模块编号
			msgID		消息编号
			p			协议数据
			len			数据长度

		return:
			0 成功, < 0 调用出错， 错误码见文件开头 EManagerErrCode
	*/
	int dispatchMessage(M_TYPE moduleID, MSG_TYPE msgID, void* p = NULL, int len = 0);
	
	/*
		function: 事件执行, 执行一个事件,所有注册了该事件的模块都会收到通告,模块执行事件的优先级由动态创建宏第4个参数指定
				  默认是10，数值越小，越优先调用,数值相同,按模块编号顺序调用
		params:
			msgID		事件编号
			p			协议数据
			len			数据长度
		
		return:
			0 成功, < 0 调用出错， 错误码见文件开头 EManagerErrCode
	*/
	int allModuleExcEvent(MSG_TYPE msgID, void* p = NULL, int len = 0);
	//管理器信息输出
	static std::string dump(void);
private:
	//将一个模块中注册的事件加入快速查询表中
	bool doRegisterModule(T* pModule);

	OwnerType* m_pOwner;				//模块管理器所有者
	T* m_modules[MODULE_NUM];			//各数据模块指针

	typedef std::vector<SModuleEvent>	EventDataVector;
	EventDataVector  m_moduleEvent[EVENT_NUM];		//每一个事件都有一个vector,存储着登记了这个事件的模块和其执行优先级
};

template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
std::string CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::dump(void)
{
	typedef RunTimeClassEx<T>		ClassInfo;
	std::string str;
	char pszStr[256] = {};

	for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
	{
		str += "------------------------\n";
		sprintf(pszStr, "模块ID: %u\n类名: %s\n优先级: %u\n", it->classID, it->m_strClassName.c_str(), it->priority);
		str += pszStr;
	}

	return str;
}

template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::CDataManager(OwnerType* pOwner /*= NULL*/)
{
	m_pOwner = pOwner;

	for (M_TYPE i = 0; i < MODULE_NUM; ++i)
		m_modules[i] = NULL;

	typedef RunTimeClassEx<T>	ClassInfo;
	for (ClassInfo* it = ClassInfo::s_pFirst; it != NULL; it = it->m_pNext)
	{
		T* pModule = it->CreateObject();
		M_TYPE index = ID_TO_INDEX(it->classID, MODULE_NUM);

		if (pModule && it->classID < MODULE_NUM && m_modules[index] == NULL)
		{
			pModule->SetManager(this);
			m_modules[index] = pModule;

			//注册该模块的所有事件，将其加入快速查询表中
			doRegisterModule(pModule);
		}
		else
		{
			SAFE_DELETE(pModule);
		}
	}

	//依据优先级,调整快速查询表
	for (MSG_TYPE i = 0; i < EVENT_NUM; ++i)
	{
		EventDataVector& refVec = m_moduleEvent[i];
		std::sort(refVec.begin(), refVec.end());
	}
}


template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
bool CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::doRegisterModule(T* pModule)
{
	//获取该模块的运行时类型信息
	const RunTimeClassEx<T>* pRunTimeClass = pModule->GetRuntimeClass();
	if (!pRunTimeClass)
		return false;	

	//获取该模块的消息列表
	const typename T::MSG_DEAL* pMessageMap = pModule->GetMessageMap();
	if (!pMessageMap)
		return false;
	
	//遍历所有事件槽
	for (MSG_TYPE i = eventStart + 1; i < eventEnd; ++i)
	{
		if (pMessageMap[i] != NULL) //用户注册了该事件,则加入快速查询表
		{
			SModuleEvent e(pRunTimeClass->classID, pRunTimeClass->priority);

			MSG_TYPE et = ID_TO_INDEX(i, EVENT_NUM);
			m_moduleEvent[et].push_back(e);
		}
	}

	return true;
}

template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::~CDataManager()
{
	for (M_TYPE i = 0; i < MODULE_NUM; ++i)
		SAFE_DELETE(m_modules[i]);
}

 
template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
T* CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::GetModuleByID(M_TYPE moduleID)
{
	return m_modules[ID_TO_INDEX(moduleID, MODULE_NUM)];
}

template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
int CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::dispatchMessage(M_TYPE moduleID, MSG_TYPE msgID, void* p, int len)
{
	T* pModule = GetModuleByID(moduleID);
	if (!pModule) 
		return MEC_MODULE_NOT_FIND;

	return pModule->SendMsg(msgID, p, len, SEND_BY_CLIENT);
}

template<class T, M_TYPE moduleMaxNum, class OwnerType, MSG_TYPE eventStart, MSG_TYPE eventEnd>
int CDataManager<T, moduleMaxNum, OwnerType, eventStart, eventEnd>::allModuleExcEvent(MSG_TYPE msgID, void* p /*= NULL*/, int len /*= 0*/)
{
	if (msgID <= eventStart || msgID >= eventEnd)
		return MEC_MSG_ID_NOT_EVENT;  //该接口只执行事件,不执行消息

	EventDataVector& eventModule = m_moduleEvent[ID_TO_INDEX(msgID, EVENT_NUM)];
	//遍历所有已经实现了该事件的模块,执行该模块
	for (MSG_TYPE i = 0; i < eventModule.size(); ++i)
	{
		SModuleEvent& refData = eventModule[i];
		m_modules[ID_TO_INDEX(refData.moduleID, MODULE_NUM) ]->SendMsg(msgID, p, len, SEND_BY_EVENT);
	}

	return MEC_OK;
}

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

//基类声明以及SendMsg的实现
#define DECLARE_BASE_EX(baseClassName)\
	public:\
	virtual QTH_NAME_SPACE::RunTimeClassEx<baseClassName>* GetRuntimeClass() const {return NULL;}\
	typedef int (baseClassName::*MSG_DEAL)(void*,int,ESendMethor);\
	virtual const MSG_DEAL* GetMessageMap(void) const {return NULL;}\
	int SendMsg(MSG_TYPE msgID, void* pData = NULL, int len = 0, ESendMethor methor = SEND_BY_INTERNAL)\
	{\
		const baseClassName::MSG_DEAL* pMessageMap = GetMessageMap();\
		if (!pMessageMap)\
		{ return MEC_MODULE_NOT_DEAL_MSG;}\
		const baseClassName::MSG_DEAL pDealFunction = pMessageMap[msgID];\
		if (!pDealFunction)\
		{ return MEC_MODULE_MSG_NOT_REGISTER;}\
		return (this->*pDealFunction)(pData, len, methor);\
	}


//获取运行时类型信息的宏
#define RUN_CLASS_INFO(className)	(&className::class##className)

//动态创建声明
#define DECLARE_DYNAMIC_CREATE(className, baseClassName) \
	public:\
	static QTH_NAME_SPACE::RunTimeClassEx<baseClassName> class##className;\
	static baseClassName* CreateObject();\
	virtual QTH_NAME_SPACE::RunTimeClassEx<baseClassName>* GetRuntimeClass() const {return RUN_CLASS_INFO(className);}

//动态创建实现,使用宏重载的功能，扩展成3参数和4参数两个版本
#define IMPLEMENT_DYNAMIC_CREATE(...)	INVOKE_ARG_MACRO(IMPLEMENT_DYNAMIC_CREATE, ##__VA_ARGS__)

//动态创建实现3参数版，使用默认优先级
#define IMPLEMENT_DYNAMIC_CREATE3(className, baseClassName, moduleID)\
	static char str##className[] = #className;\
	QTH_NAME_SPACE::RunTimeClassEx<baseClassName> className::class##className = {moduleID, MODULE_PRIORITY,str##className, NULL, &className::CreateObject};\
	static QTH_NAME_SPACE::initRunTimeClassEx<QTH_NAME_SPACE::RunTimeClassEx<baseClassName> > init_##className(RUN_CLASS_INFO(className));\
	baseClassName* className::CreateObject()\
	{return new className;}

//动态创建实现4参数版，使用参数优先级
#define IMPLEMENT_DYNAMIC_CREATE4(className, baseClassName, moduleID, priority)\
	static char str##className[] = #className;\
	QTH_NAME_SPACE::RunTimeClassEx<baseClassName> className::class##className = {moduleID, priority,str##className, NULL, &className::CreateObject};\
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
