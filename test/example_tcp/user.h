/*
	function:	用户类
*/
#ifndef USER_H_
#define USER_H_
#include "utility/utility.h"
#include "utility/single.h"
#include "frame/interface_user.h"

//短式声明
class IModuleManager;
namespace QTH_NAME_SPACE
{
	class ITcpConnect;
}

//用户类
class StoreUser: public IUser
{
public:
	StoreUser(QTH_NAME_SPACE::ITcpConnect* pConnect);
	~StoreUser();
	
	//用户ID
	void SetUserID(unsigned id)						{ m_id = id;}
	virtual unsigned GetUserID()					{ return m_id;}
	//通过用户接口获取用户模块管理器
	virtual IModuleManager* GetManager()			{ return m_pModuleManager; }
	//通过用户接口向连接的对端发送消息
	virtual void SendMsgToClient(const char* pData, size_t len);
	virtual void SendMsgToServer(int no, const char* pData, size_t len){}
private:
	unsigned		m_id;						//用户临时连接ID
	int  m_bAccountLogin;						//账户是状态
	IModuleManager* m_pModuleManager;			//该用户的功能处理类
	QTH_NAME_SPACE::ITcpConnect*	m_pConnect; //针对用户连接的引用

};


#endif
