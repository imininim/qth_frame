/********************************
function:	用户接口
date:		2018/04/26
author:		qutianhao
tips:		
*********************************/
#ifndef INTERFACE_USER_H_
#define INTERFACE_USER_H_
#include <cstddef>
//模块接口
class IModuleManager;

class IUser
{

public:
	IUser(){}
	virtual ~IUser() {}
	
	virtual unsigned GetUserID()										= 0; 
	//通过用户接口获取用户模块管理器
	virtual IModuleManager* GetManager()								= 0;
	//设置用户模块管理器
	virtual void resetManager(IModuleManager* pManager)					= 0;
	//通过用户接口向连接的对端发送消息
	virtual void SendMsgToClient(const char* pData, size_t len)			= 0;
};

#endif
