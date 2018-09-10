/********************************
author:		qutianhao
function:	ģ�����
date:		2017/12/27
tips:
	����ʱ������Ϣʶ�� �����Ϣ��, ��̬��������Ϣ���ͻ���
*********************************/
#ifndef RUNNING_CLASS_INFO_H_
#define RUNNING_CLASS_INFO_H_
#include "frame/macro.h"

class Module;
class IModuleManager;

//��Ϣ���ݵķ�ʽö��
enum ESendMethor
{
	SEND_BY_CLIENT,			//�û�����������Ϣ
	SEND_BY_INTERNAL,		//�ڲ����ݵ���Ϣ

	//Ԥ����Ϣ���ݷ�ʽ�����¿�����ȡ�ã��ڲ�ͬģ�����Բ�ͬ�ķ�ʽ������Ϣ����
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
	//������Ϣ�ṹ��,�ڽ�list
	struct QTH_UTILITY_EXPORT RunTimeClass
	{
		char* m_pClassName;				//��������
		M_TYPE m_id;					//ģ���ʶ��
		int m_objSize;					//����Ĵ�С
		Module* (*m_pCreateFn)();		//ָ�򴴽���������͵�ָ��
		RunTimeClass* m_pBase;			//ָ��������͵Ļ�������Ϣ
		RunTimeClass* m_pNext;			//ָ����һ��������Ϣ��ָ��
		
		static RunTimeClass* s_pFirst;	//ȫ��ָ�룬ָ���׸����͵���Ϣ

		Module* CreateObject();
	};
	
	//��ʼ��ÿ��������Ϣ�ṹ�������������������ӹ���
	struct QTH_UTILITY_EXPORT initRunTimeClass
	{
		initRunTimeClass(RunTimeClass* pNewClass);
	};
		
	typedef int (Module::*MSG_DEAL)(void*,int,ESendMethor); 
	//��Ϣ��Ϣ�ṹ
	struct QTH_UTILITY_EXPORT Message
	{
		Message(): msgID(0),m_pfnMsg(NULL)
		{}

		MSG_TYPE msgID;								//��ϢID
		MSG_DEAL m_pfnMsg;							//��Ϣ������
	};
		
	//��Ϣӳ���
	struct QTH_UTILITY_EXPORT MessageMap
	{
		MessageMap* m_pBaseMap;			//ָ��������Ϣӳ���
		Message* m_pMessage;	
	};
		
	//��Ϣ����
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

		//��ȡ������ģ������,��ģ������Ϊ������
		std::string GetModuleName() const;
		void		SetModuleName(const char* pszName);
		//��ȡ������ģ��ID ��ģ��IDΪ������
		M_TYPE GetModuleID() const;
		void   SetModuleID(M_TYPE id);

		//���ģ�鷢����Ϣ
		int SendMsg(MSG_TYPE msgID, void* pData, int len, ESendMethor methor = SEND_BY_INTERNAL);
				
		//��ȡģ�������
		void SetManager(IModuleManager* pManager);
		IModuleManager* GetManager(void);

		//���л�/�����л��ӿ�
		virtual size_t serialize(void* pData, size_t len);
		virtual size_t unserialize(void* pData, size_t len);

		virtual QTH_NAME_SPACE::RunTimeClass* GetRuntimeClass() const;		// ��ȡRTTI��Ϣ
		virtual QTH_NAME_SPACE::MessageMap* GetMessageMap() const;			// ��ȡ��Ϣӳ����Ϣ								
	public:
		static QTH_NAME_SPACE::RunTimeClass classModule;
		static QTH_NAME_SPACE::Message s_messages[QTH_MAX_MESSAGE_NUM];
		static QTH_NAME_SPACE::MessageMap s_mapMessage;

	private:
		IModuleManager* m_pManager; //ģ�������������
	};

/***************************************
RTTI ����
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
RTTI ʵ�֣� ���ҽ��ú����������������Ϣ��������

�� QTH_IMPLEMENT_DYNMIC  ֻ������ӵ�ж�̬��������
��������
*************************************************/
#define QTH_IMPLEMENT_DYNMIC(class_name, base_class_name) \
	_QTH_IMPLEMENT_RUNTIME_CLASS(class_name, 0, base_class_name, class_name::CreateObject)

#define QTH_IMPLEMENT_DYNMIC_WITH_ID(class_name, base_class_name, class_id)\
	_QTH_IMPLEMENT_RUNTIME_CLASS(class_name, class_id, base_class_name, class_name::CreateObject)

/*************************************************
��Ϣ��������
*************************************************/
#define QTH_DECLARE_MESSAGE()\
	public:\
	static QTH_NAME_SPACE::Message s_messages[QTH_MAX_MESSAGE_NUM];\
	static QTH_NAME_SPACE::MessageMap s_mapMessage;\
	virtual QTH_NAME_SPACE::MessageMap* GetMessageMap() const;

/*************************************************
��Ϣ����ʵ��
*************************************************/
#define QTH_IMPLEMENT_MESSAGE(class_name, base_class)\
	QTH_NAME_SPACE::MessageMap* class_name::GetMessageMap() const\
	{ return &class_name::s_mapMessage;}\
	QTH_NAME_SPACE::MessageMap class_name::s_mapMessage = {&base_class::s_mapMessage, (QTH_NAME_SPACE::Message*)(&class_name::s_messages)}; \
	QTH_NAME_SPACE::Message class_name::s_messages[QTH_MAX_MESSAGE_NUM];

#define QTH_ON_MESSAGE(class_name, id, memberFn)\
	static QTH_NAME_SPACE::pushMessage class_name##id((QTH_NAME_SPACE::Message*)(&class_name::s_messages), (MSG_TYPE)id, (QTH_NAME_SPACE::MSG_DEAL)memberFn);


/*************************
��̬������RTTI����Ϣ���ͻ����ۺ�����
**************************/
#define QTH_MODULE_DECLARE(class_name)\
	QTH_DECLARE_DYNMIC(class_name)\
	QTH_DECLARE_MESSAGE()

/*************************
��̬������RTTI����Ϣ���ͻ����ۺ�ʵ��
**************************/
#define QTH_MODULE_IMPLEMENT(class_name, base_name)\
	QTH_IMPLEMENT_DYNMIC(class_name, base_name)\
	QTH_IMPLEMENT_MESSAGE(class_name, base_name)

#define QTH_MODULE_IMPLEMENT_WITH_ID(class_name, base_name, class_id)\
	QTH_IMPLEMENT_DYNMIC_WITH_ID(class_name, base_name, class_id)\
	QTH_IMPLEMENT_MESSAGE(class_name, base_name)
#endif
