/********************************
function:	模块管理器接口
date:		2018/04/24
author:		qutianhao
tips:		
*********************************/
#ifndef INTERFACE_MODULE_MANAGER_H_
#define INTERFACE_MODULE_MANAGER_H_
#include "frame/macro.h"
class Module;
class IUser;

class QTH_UTILITY_EXPORT IModuleManager
{
public:
	IModuleManager(){}
	virtual ~IModuleManager(){}

	//根据模块号获取指定模块
	virtual Module* GetModuleByID(M_TYPE moduleID)							= 0;
	virtual Module* GetModuleByName(const char* pszName) 					= 0;
	
	//模块初始化/反初始化
	virtual bool Init(const char* pszConfig = NULL)							= 0;
	virtual void UnInit(void)												= 0;

	//序列化、反序列化
	virtual size_t serialize(void* pData, size_t len)						= 0;
	virtual size_t unserialize(void* pData, size_t len)						= 0;
		
	//消息派发
	virtual bool dispatchMessage(M_TYPE moduleID,MSG_TYPE msgID, void* p, int len) = 0;
	//模块信息打印
	virtual void print()											{}
	//获取模块所属用户
	virtual IUser* GetUser()												= 0;
	//获取错误信息
	virtual const std::string& GetErrorInfo() const							= 0;

};

#endif