#include "stdafx.h"
#include "frame/module_manager.h"
#include "xml/tinyxml2.h"
BEGIN_NAMESPACE
	//静态成员初始
ModuleManager::ModuleConfigMap ModuleManager::s_modulesConfig;

ModuleManager::ModuleManager(IUser* pUser)
{
	for (M_TYPE i = 0; i < QTH_MAX_PROTOCOL_NUM; ++i)
		m_modules[i] = NULL;	

	m_pUser = pUser;
}

bool ModuleManager::Init(const char* pszConfig)
{
	//读取模块配置信息
	doReadXml(pszConfig, s_modulesConfig);

	bool success_all = true; //是否成功初始化了所有的模块
	RunTimeClass* pClass = NULL;

	for (pClass = QTH_NAME_SPACE::RunTimeClass::s_pFirst; pClass != NULL; pClass = pClass->m_pNext)
	{
		if (strcmp(pClass->m_pClassName, "Module") == 0 ) 
			continue;

		if (!pClass->m_pBase || strcmp(pClass->m_pBase->m_pClassName, "Module") != 0)
		{
			m_errStr = std::string(pClass->m_pClassName) + "不是直接继承于Module, 不允许该模块的创建!";
			continue;
		}

		Module* pObject = pClass->CreateObject();
		if (pObject && s_modulesConfig.size() != 0) //使用配置中的模块ID
		{
			ModuleConfigMap::iterator it = s_modulesConfig.find(pClass->m_pClassName);
			if (it == s_modulesConfig.end()) //找不到配置
			{
				m_errStr = "类名为" + std::string(pClass->m_pClassName) + "的模块配置项没找到，请检查配置!";
				SAFE_DELETE(pObject);
				success_all = false;
				continue;
			}

			pObject->SetModuleID(it->second.first);
			
		}
		else if(!pObject)	//设置出错
		{
			m_errStr = "类名为" + std::string(pClass->m_pClassName) + "模块ID=" + T_to_string(pClass->m_pCreateFn) + "设置模块出错!";

			SAFE_DELETE(pObject);
			success_all = false;
			continue;
		}
		
		//设置模块
		pObject->SetManager(this);
		m_modules[pObject->GetModuleID()] = pObject;

	}//end (pClass = QTH_NAME_SPACE::RunTimeClass::s_pFirst; pClass != NULL; pClass = pClass->m_pNext)

	return success_all;
}

void ModuleManager::UnInit(void)
{
	for (M_TYPE i = 0; i < QTH_MAX_PROTOCOL_NUM; ++i)
		SAFE_DELETE(m_modules[i]);
}

ModuleManager::~ModuleManager(void)
{
	UnInit();
}


size_t ModuleManager::serialize(void* pData, size_t len)
{
	assert(pData && len!=0);
	
	std::vector<M_TYPE> modules; //所有需要存储的模块编号
	for (M_TYPE i = 0; i < QTH_MAX_PROTOCOL_NUM; ++i)
	{
		if (m_modules[i] != NULL)
			modules.push_back(i);
	}
	
	size_t total_size = 0; //总序列化大小
	BYTE* pByte = (BYTE*)pData;
	//存储1个字节的模块总数
	*pByte = modules.size(); 
	pByte += 1;
	total_size += 1;

	//将所有的模块存储
	for (std::vector<M_TYPE>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		//序列化模块头
		SerailizeHead* pHead = (SerailizeHead*)pByte;
		pHead->m_id = *it;
		pByte += sizeof(SerailizeHead);
		
		//序列化模块数据
		pHead->size = m_modules[*it]->serialize(pByte, len - total_size - sizeof(SerailizeHead));
		pByte += pHead->size;

		total_size += sizeof(SerailizeHead) + pHead->size; //该模块大小 = 模块头大小 + 模块数据大小
	}

	return total_size;
}

size_t ModuleManager::unserialize(void* pData, size_t len)
{
	assert(pData && len!=0);
	size_t total_size = 0; //总序列化大小
	BYTE* pByte = (BYTE*)pData;
		
	//读取模块总数
	int modules_num = *pByte;
	pByte += 1;
	total_size += 1;

	//读取所有的模块数据
	while (modules_num-- > 0)
	{
		SerailizeHead* pHead = (SerailizeHead*)pByte;

		if (m_modules[pHead->m_id] != NULL)
		{
			size_t size = m_modules[pHead->m_id]->unserialize(pByte, len - total_size - sizeof(SerailizeHead) );
			if (size != pHead->size)
			{
				m_errStr = "序列化读取错误,模块=" + T_to_string((int)pHead->m_id) + ",需要读取=" + T_to_string(pHead->size) + "实际读取=" + T_to_string(size);
				return 0; //发现模块读取数据有误， 立马返回
			}
				
		}

		pByte += sizeof(SerailizeHead) + pHead->size;
		total_size +=sizeof(SerailizeHead) + pHead->size;
	}

	return total_size;
}

void ModuleManager::doReadXml(const char* pszConfig, std::map<std::string, std::pair<M_TYPE, std::string> >& out_map)
{
		if (!pszConfig || !out_map.empty()) return;

	XMLDocument doc;
	XMLError errCode = doc.LoadFile(pszConfig);
	if (errCode != XML_SUCCESS)
	{
		std::cout << "[模块管理器]加载模块配置错误: " << doc.ErrorName() << std::endl;
		return;
	}

	//获取根节点
	XMLElement* root = doc.RootElement();
	if (!root)
	{
		std::cout << "[模块管理器] 配置错误，不存在根节点." << std::endl;
		return;
	}


	std::string key;
	std::pair<M_TYPE, std::string> val;

	XMLElement* moduleNode = root->FirstChildElement("Module");
	while (moduleNode)
	{
		key = moduleNode->Attribute("className");
		val.first = moduleNode->UnsignedAttribute("moduleID");
		val.second = moduleNode->Attribute("moduleName");

		if (key.empty())				//类的名称为空
		{
			moduleNode = moduleNode->NextSiblingElement("Module");
			continue;

		}
		else if (val.second.empty())	//模块名称未设置
		{
			val.second = key; //模块名称设置为类的名称
		}

		out_map.insert(std::make_pair(key, val));

		moduleNode = moduleNode->NextSiblingElement("Module");
	}
}

 IUser* ModuleManager::GetUser()
 {
	 return m_pUser;
 }

void ModuleManager::print()
{
	using std::cout;
	using std::endl;

	//输出每个模块的类型信息 RTTI
	RunTimeClass* pClass = NULL;
	for (pClass = RunTimeClass::s_pFirst; pClass != NULL; pClass = pClass->m_pNext)
	{
		if (strcmp(pClass->m_pClassName, "Module") == 0 )
			continue;

		cout << "-----模块 "		<<  pClass->m_pClassName << "---------------" << std::endl; 
		cout << "	模块id: "		<< (unsigned int)pClass->m_id << endl;
		cout << "	模块大小: "		<< pClass->m_objSize << endl;
		cout << "	基类模块: "		<< pClass->m_pBase->m_pClassName << endl;
		cout << "	创建地址: "		<< pClass->m_pCreateFn << endl;
		cout <<  endl << endl;
	}
}

const std::string& ModuleManager::GetErrorInfo() const
{
	return m_errStr;
}

bool ModuleManager::dispatchMessage(M_TYPE moduleID,MSG_TYPE msgID, void* p, int len)
{
	if (m_modules[moduleID] == NULL)
	{
		m_errStr = "模块" + T_to_string((int)moduleID) + "尚未被创建，此消息无法到达!";
		return false;
	}
	
	int result = m_modules[moduleID]->SendMsg(msgID, p, len, SEND_BY_CLIENT);
	if(result == INT_MAX)
	{
		m_errStr = "模块" + T_to_string((int)moduleID) + "无法处理消息ID=" + T_to_string((int)msgID);
		return false;
	}

	return true;
}

Module* ModuleManager::GetModuleByName(const char* pszName)
{
	for (M_TYPE i = 0; i < QTH_MAX_PROTOCOL_NUM; ++i)
	{
		if (m_modules[i] && (strcmp(m_modules[i]->GetModuleName().c_str(), pszName) == 0) )
			return m_modules[i];
	}

	return NULL;
}

Module* ModuleManager::GetModuleByID(M_TYPE moduleID)
{
	return m_modules[moduleID];
}

END_NAMESPACE
