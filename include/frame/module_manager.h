/********************************
author:		qutianhao
function:	模块管理器实现
date:		2018/04/24
tips:
	实现模块管理器， 将各个模块聚集起来，统一进行管理
*********************************/
#ifndef MODULE_MANAGER_H_
#define MODULE_MANAGER_H_
#include "utility/nocopyable.h"
#include "frame/interface_module_manager.h"
#include "frame/running_class_info.h"
BEGIN_NAMESPACE

class QTH_UTILITY_EXPORT ModuleManager: public Nocopyable, public IModuleManager
{
	//序列化头
	struct SerailizeHead
	{
		SerailizeHead(M_TYPE no, size_t s = 0): m_id(no), size(s){}

		M_TYPE m_id; //模块ID
		size_t size;//模块存储数据
	};

public:
	ModuleManager(IUser* pUser);
	virtual ~ModuleManager();

	virtual bool Init(const char* pszConfig);
	virtual void UnInit(void);

	//根据模块号获取指定模块
	virtual Module* GetModuleByID(M_TYPE moduleID);
	virtual Module* GetModuleByName(const char* pszName);

	//序列化、反序列化
	virtual size_t serialize(void* pData, size_t len);
	virtual size_t unserialize(void* pData, size_t len);

	//消息派发,将一条消息派发给指定的模块
	virtual bool dispatchMessage(M_TYPE moduleID,MSG_TYPE msgID, void* p, int len);
	//通用消息派发,将一条消息派发给所有的模块
	virtual void execMessageAll(MSG_TYPE msgID, void* p, int len);

	//模块信息打印
	virtual void print();

	virtual IUser* GetUser();

	//错误信息
	virtual const std::string& GetErrorInfo() const;
private:
	Module* m_modules[QTH_MAX_PROTOCOL_NUM];		//管理的模块信息
	IUser* m_pUser;									//所属用户

	std::string m_errStr;
	//读取模块配置信息, 模块信息的配置文件无论调用多少次，都只会读取一次
	static void doReadXml(const char* pszConfig, std::map<std::string, std::pair<M_TYPE, std::string> >& out_map);

	typedef std::map<std::string, std::pair<M_TYPE, std::string> >	ModuleConfigMap;  //<类名， <对应的模块ID，对应的模块名> >
	typedef		ModuleConfigMap::iterator	Iterator;
	
	static ModuleConfigMap s_modulesConfig;		//配置信息
};

END_NAMESPACE
#endif
