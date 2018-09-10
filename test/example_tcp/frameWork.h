/*
	tcp 服务器处理
*/
#ifndef TEST_FRAMEWORK_H_
#define TEST_FRAMEWORK_H_

//用户
#include "user.h"

//tcp服务器框架头文件
#include "asio/frame_work.h"

//日志
#include "utility/logger.h"

//tuple
#include "common/property.h"

#include "frame/module_manager.h"

using namespace QTH_NAME_SPACE;

//序列化模块最大字节数
#define SERIALIZE_SIZE 1024 * 64

class CMyFrameWork: public CFrameWork
{
public:
	CMyFrameWork(const std::string& strName, unsigned short nPort, int threadNum = 3): CFrameWork(strName,nPort, threadNum)
	{}
	//初始化
	virtual void OnInitialize(void)
	{
		
	}
	//服务停止
	virtual void OnUninitalize(void)
	{
		for (UserIterator it = m_userMap.begin(); it != m_userMap.end(); ++it)
		{
			StoreUser*  pUser = (StoreUser*)it->second->GetData();
			if (pUser)
			{
				//释放用户
				SAFE_DELETE(pUser);
			}

		}

		m_userMap.clear();
	}
	//消息接收
	virtual void OnRecvMsg(ConnectPtr pTCPHandle, const char* pData, size_t len)
	{
		if (!pTCPHandle && pData && len!=0)
			FreeUser(pTCPHandle);

		StoreUser*  pUser = (StoreUser*)pTCPHandle->GetData();
		if (pUser)
		{			
			std::string strReturn;
			//读取并解析消息 <模块号,协议号,实际的数据>
			CmdParse cmd;
			if (!cmd.parse(std::string(pData)) )
			{
				strReturn = "[错误的指令格式]" + std::string(pData);
				pTCPHandle->SendMsg(strReturn.c_str(), strReturn.size());
				return;
			}
										
			//根据消息方式，进行消息派发
			M_TYPE moduleID = atoi(cmd["module"].c_str());
			MSG_TYPE msgID = atoi(cmd["msg"].c_str());
			
			//派发协议到各个模块
			if (!pUser->GetManager()->dispatchMessage(moduleID, msgID, (void*)&cmd, sizeof(cmd)))
			{
				strReturn = pUser->GetManager()->GetErrorInfo();
				pTCPHandle->SendMsg(strReturn.c_str(), strReturn.size());
			}
				
				
		}
		else
			FreeUser(pTCPHandle);
	}
	//用户登录
	virtual void OnLogin(ConnectPtr pTCPHandle)
	{	
		//创建用户数据管理
		StoreUser*  pUser = new StoreUser(pTCPHandle.get());
		pTCPHandle->SetData(pUser);

		if (!pUser || !pUser->GetManager()->Init("./modules.xml"))
		{
			FreeUser(pTCPHandle);
			return;
		}
					
		//用户信息插入连接列表
		m_userMap.insert(std::make_pair(pUser->GetUserID(), pTCPHandle));
		LOG_DEBUG("用户登录, uniqueID=%u!", pUser->GetUserID())
						
	}
	//套接字错误
	virtual void OnError(ConnectPtr pTCPHandle, int errCode, const std::string& msg)
	{
		//LOG_ERROR("[FrameWork] 发生套接字错误, errCode = %d, errStr=%s", errCode, msg.c_str());
		FreeUser(pTCPHandle);
	}
	//定时器
	virtual void OnTimer(std::size_t nIDEvent, void* params)
	{
	}

	virtual void OnCommond(const char* pData, size_t len){}
private:
	//查找用户
	ConnectPtr find(unsigned uniqueID)
	{
		UserMap::iterator it = m_userMap.find(uniqueID);
		if (it == m_userMap.end())
			return ConnectPtr(NULL);

		return it->second;
	}
	
	//释放用户
	void FreeUser(ConnectPtr pTCPHandle)
	{
		if (!pTCPHandle)
			return;
		
		StoreUser*  pUser = (StoreUser*)pTCPHandle->GetData();
		if (pUser)
		{
			//从用户登录列表中删除
			unsigned loginID = pUser->GetUserID();
			m_userMap.erase(loginID);

			//释放用户
			SAFE_DELETE(pUser);
			LOG_DEBUG("用户 uniqueID=%u 下线!", loginID);
		}
		
		if (pTCPHandle->isOpen()) 
				pTCPHandle->Close();
	}

	//临时用户ID生成
	static const unsigned int uniqueID()
	{
		static unsigned int unique_id = 0;
		return ++unique_id;
	}

	typedef std::map<unsigned, ConnectPtr> UserMap;
	typedef UserMap::iterator				UserIterator;

	UserMap m_userMap; //<用户标识， 用户模块管理器>
};

#endif
