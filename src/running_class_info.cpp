#include "stdafx.h"
#include "frame/interface_module_manager.h"
#include "frame/running_class_info.h"
#include "utility/logger.h"
#include <assert.h>
using namespace QTH_NAME_SPACE;

// 初始化RunTimeClass静态成员
RunTimeClass* RunTimeClass::	s_pFirst = NULL;

// 初始化Module静态成员
static char strObject[QTH_MAX_CLASS_NAME_NUM] = "Module";
struct RunTimeClass Module::classModule = {strObject, 0, sizeof(Module), NULL, NULL, NULL};
static initRunTimeClass g_initRunTimeClass(&Module::classModule);

QTH_NAME_SPACE::Message Module::s_messages[QTH_MAX_MESSAGE_NUM];
QTH_NAME_SPACE::MessageMap Module::s_mapMessage = {NULL, (QTH_NAME_SPACE::Message*)&(Module::s_mapMessage)};

initRunTimeClass::initRunTimeClass(RunTimeClass* pNewClass)
{
	//将类别连接到类别链中， s_pFirst为指向第一个类别信息的指针
	pNewClass->m_pNext = RunTimeClass::s_pFirst;
	RunTimeClass::s_pFirst = pNewClass;
}


Module::Module(): m_pManager(NULL)
{

}

Module::~Module()
{

}

std::string Module::GetModuleName() const
{
	RunTimeClass* pClass = GetRuntimeClass();
	assert(pClass != NULL);

	return std::string(pClass->m_pClassName);
}

void Module::SetModuleName(const char* pszName)
{
	size_t len = strlen(pszName) + 1;
	if (len < QTH_MAX_CLASS_NAME_NUM)
	{
		RunTimeClass* pClass = GetRuntimeClass();
		assert(pClass != NULL);
		//strcpy_s(pClass->m_pClassName, QTH_MAX_CLASS_NAME_NUM, pszName);
		memset(pClass->m_pClassName, 0, QTH_MAX_CLASS_NAME_NUM);
		strncpy(pClass->m_pClassName, pszName, len);

	}
	else
	{
		std::cout << "[模块] 设置模块名错误， 原因: 模块名 " << pszName << ",过长!" << std::endl;
	}
}

M_TYPE Module::GetModuleID() const
{
	RunTimeClass* pClass = GetRuntimeClass();
	assert(pClass != NULL);
	
	return pClass->m_id;
}

void Module::SetModuleID(M_TYPE id)
{
	RunTimeClass* pClass = GetRuntimeClass();
	assert(pClass != NULL);

	pClass->m_id = id;
}

int Module::SendMsg(MSG_TYPE msgID, void* pData, int len, ESendMethor methor)
{
	QTH_NAME_SPACE::MessageMap* pMessageMap = GetMessageMap();
	assert(pMessageMap != NULL);

	QTH_NAME_SPACE::MSG_DEAL pFn = pMessageMap->m_pMessage[msgID].m_pfnMsg;
	if (!pFn)
		return INT_MAX;
	
	return (this->*pFn)(pData, len, methor);
}

size_t Module::serialize(void* pData, size_t len)
{
	return 0;
}

size_t Module::unserialize(void* pData, size_t len)
{
	return 0;
}

void Module::SetManager(IModuleManager* pManager)
{
	m_pManager = pManager;
}

IModuleManager* Module::GetManager(void)
{
	return m_pManager;
}

RunTimeClass* Module::GetRuntimeClass() const
{
	return &Module::classModule;
}

QTH_NAME_SPACE::MessageMap* Module::GetMessageMap() const
{
	return &Module::s_mapMessage;
}

Module* QTH_NAME_SPACE::RunTimeClass::CreateObject()
{
	if (m_pCreateFn == NULL)
		return NULL;
	
	return (*m_pCreateFn)();
}

QTH_NAME_SPACE::pushMessage::pushMessage(Message* pMessages, MSG_TYPE msgID, MSG_DEAL pfnMsg)
{
	pMessages[msgID].msgID = msgID;
	pMessages[msgID].m_pfnMsg = pfnMsg;
}
