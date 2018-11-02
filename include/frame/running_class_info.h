/********************************
author:		qutianhao
function:	模块基类
date:		2017/12/27
tips:
	运行时类型信息识别， 类别信息链, 动态创建，消息推送机制
*********************************/
#ifndef RUNNING_CLASS_INFO_H_
#define RUNNING_CLASS_INFO_H_
#include "frame/macro.h"

class Module;
class IModuleManager;

//消息传递的方式枚举
enum ESendMethor
{
	SEND_BY_CLIENT,			//用户传递来的消息
	SEND_BY_INTERNAL,		//内部传递的消息

	//预留消息传递方式，以下可以自取用，在不同模块中以不同的方式进行消息传递
	SEND_BY_CUSTOMER_1,		
	SEND_BY_CUSTOMER_2,		
	SEND_BY_CUSTOMER_3,		
	SEND_BY_CUSTOMER_4,		
	SEND_BY_CUSTOMER_5,		
	SEND_BY_CUSTOMER_6,		
	SEND_BY_CUSTOMER_7,		
	SEND_BY_CUSTOMER_8,		
	SEND_BY_CUSTOMER_9,		
};

BEGIN_NAMESPACE
	//类型信息结构体,内建list
	struct QTH_UTILITY_EXPORT RunTimeClass
	{
		char* m_pClassName;				//类型名称
		M_TYPE m_id;					//模块标识符
		int m_objSize;					//对象的大小
		Module* (*m_pCreateFn)();		//指向创建出这个类型的指针
		RunTimeClass* m_pBase;			//指向这个类型的基类型信息
		RunTimeClass* m_pNext;			//指向下一个类型信息的指针
		
		static RunTimeClass* s_pFirst;	//全局指针，指向首个类型的信息

		Module* CreateObject();
	};
	
	//初始化每个类型信息结构体以完成类型链表的连接工作
	struct QTH_UTILITY_EXPORT initRunTimeClass
	{
		initRunTimeClass(RunTimeClass* pNewClass);
	};
		
	typedef int (Module::*MSG_DEAL)(void*,int,ESendMethor); 
	//消息信息结构
	struct QTH_UTILITY_EXPORT Message
	{
		Message(): msgID(0),m_pfnMsg(NULL)
		{}

		MSG_TYPE msgID;								//消息ID
		MSG_DEAL m_pfnMsg;							//消息处理函数
	};
		
	//消息映射表
	struct QTH_UTILITY_EXPORT MessageMap
	{
		MessageMap* m_pBaseMap;			//指向基类的消息映射表
		Message* m_pMessage;	
	};
		
	//消息链接
	struct QTH_UTILITY_EXPORT pushMessage
	{
		pushMessage(Message* pMessages, MSG_TYPE msgID, MSG_DEAL pfnMsg);
	};

END_NAMESPACE
		
	class QTH_UTILITY_EXPORT Module
	{
	public:
		Module();
		virtual ~Module() = 0;
		//获取模块名称,即实际指向类的类名称
		std::string GetModuleName() const;
		//获取、设置模块ID 该模块ID. 标识该类
		M_TYPE GetModuleID() const;
		void   SetModuleID(M_TYPE id);

		//向该模块发送消息
		int SendMsg(MSG_TYPE msgID, void* pData, int len, ESendMethor methor = SEND_BY_INTERNAL);
				
		//获取模块管理器
		void SetManager(IModuleManager* pManager);
		IModuleManager* GetManager(void);

		//动态类型识别
		Module* isKindOf(const QTH_NAME_SPACE::RunTimeClass* pClass);


		//序列化/反序列化接口
		virtual size_t serialize(void* pData, size_t len);
		virtual size_t unserialize(void* pData, size_t len);

		//类型信息识别

		virtual QTH_NAME_SPACE::RunTimeClass* GetRuntimeClass() const;		// 获取RTTI信息
		virtual QTH_NAME_SPACE::MessageMap* GetMessageMap() const;			// 获取消息映射信息								
	public:
		static QTH_NAME_SPACE::RunTimeClass classModule;
		static QTH_NAME_SPACE::Message s_messages[QTH_MAX_MESSAGE_NUM];
		static QTH_NAME_SPACE::MessageMap s_mapMessage;

	private:
		IModuleManager* m_pManager; //模块管理器的引用
	};

/***************************************
RTTI 声明
***************************************/
#define QTH_DECLARE_DYNMIC(class_name) \
	public:\
	static QTH_NAME_SPACE::RunTimeClass class##class_name;\
	virtual QTH_NAME_SPACE::RunTimeClass* GetRuntimeClass() const;\
	static Module* CreateObject();

#define QTH_RUNCLASS(class_name) (&class_name::class##class_name)

#define _QTH_IMPLEMENT_RUNTIME_CLASS(class_name, class_id, base_class_name, pfnNew)\
	static char str##class_name[QTH_MAX_CLASS_NAME_NUM] = #class_name;\
	QTH_NAME_SPACE::RunTimeClass class_name::class##class_name = {str##class_name, class_id, sizeof(class_name), pfnNew, QTH_RUNCLASS(base_class_name), NULL};\
	static QTH_NAME_SPACE::initRunTimeClass init_##class_name(&class_name::class##class_name);\
	QTH_NAME_SPACE::RunTimeClass* class_name::GetRuntimeClass() const\
	{return &class_name::class##class_name;}\
	Module* class_name::CreateObject()\
	{ return new class_name;}

/*************************************************
RTTI 实现， 并且将该宏声明的类的类型信息串联起来

宏 QTH_IMPLEMENT_DYNMIC  只声明类拥有动态创建能力
后排美国
*************************************************/
#define QTH_IMPLEMENT_DYNMIC(class_name, base_class_name) \
	_QTH_IMPLEMENT_RUNTIME_CLASS(class_name, 0, base_class_name, class_name::CreateObject)

#define QTH_IMPLEMENT_DYNMIC_WITH_ID(class_name, base_class_name, class_id)\
	_QTH_IMPLEMENT_RUNTIME_CLASS(class_name, class_id, base_class_name, class_name::CreateObject)

/*************************************************
消息推送声明
*************************************************/
#define QTH_DECLARE_MESSAGE()\
	public:\
	static QTH_NAME_SPACE::Message s_messages[QTH_MAX_MESSAGE_NUM];\
	static QTH_NAME_SPACE::MessageMap s_mapMessage;\
	virtual QTH_NAME_SPACE::MessageMap* GetMessageMap() const;

/*************************************************
消息推送实现
*************************************************/
#define QTH_IMPLEMENT_MESSAGE(class_name, base_class)\
	QTH_NAME_SPACE::MessageMap* class_name::GetMessageMap() const\
	{ return &class_name::s_mapMessage;}\
	QTH_NAME_SPACE::MessageMap class_name::s_mapMessage = {&base_class::s_mapMessage, (QTH_NAME_SPACE::Message*)(&class_name::s_messages)}; \
	QTH_NAME_SPACE::Message class_name::s_messages[QTH_MAX_MESSAGE_NUM];

#define QTH_ON_MESSAGE(class_name, id, memberFn)\
	static QTH_NAME_SPACE::pushMessage class_name##id((QTH_NAME_SPACE::Message*)(&class_name::s_messages), (MSG_TYPE)id, (QTH_NAME_SPACE::MSG_DEAL)memberFn);


/*************************
动态创建，RTTI，消息推送机制综合声明
**************************/
#define QTH_MODULE_DECLARE(class_name)\
	QTH_DECLARE_DYNMIC(class_name)\
	QTH_DECLARE_MESSAGE()

/*************************
动态创建，RTTI，消息推送机制综合实现
**************************/
#define QTH_MODULE_IMPLEMENT(class_name, base_name)\
	QTH_IMPLEMENT_DYNMIC(class_name, base_name)\
	QTH_IMPLEMENT_MESSAGE(class_name, base_name)

#define QTH_MODULE_IMPLEMENT_WITH_ID(class_name, base_name, class_id)\
	QTH_IMPLEMENT_DYNMIC_WITH_ID(class_name, base_name, class_id)\
	QTH_IMPLEMENT_MESSAGE(class_name, base_name)
#endif
