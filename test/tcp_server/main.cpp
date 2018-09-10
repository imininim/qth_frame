/*
	tcp 回显服务器 测试 
*/
#include <iostream>

//step1 动态库公用头文件
#include "utility/utility.h"

//step2 服务头文件
#include "asio/frame_work.h"


#include "utility/logger.h"

using namespace QTH_NAME_SPACE;

class CMyFrameWork: public CFrameWork
{
public:
	/*
		构造参数：
		1. 服务名称，  唯一的标识了当前服务
		2. 服务器使用端口
		3. 处理 监听、建立连接、消息收发的线程数量
	*/
	CMyFrameWork(const std::string& strName, unsigned short nPort, int threadNum = 2): CFrameWork(strName,nPort, threadNum)
	{

	}

	~CMyFrameWork()
	{
		
	}

	virtual void OnInitialize(void)
	{
		char buf[256] = {0};
		std::cout << "是否需要连接其他服务器?(n/y): ";
		std::cin.getline(buf, 256);

		if (strcmp("y", buf) == 0)
		{
		std::string strIp;
		unsigned short nPort = 0;

		memset(buf, 0, 256);
		std::cout << "请输入需要连接服务器的IP: ";
		std::cin.getline(buf, 256);
		strIp = buf;

		memset(buf, 0, 256);
		std::cout << "请输入需要连接服务器的端口: ";
		std::cin.getline(buf, 256);
		nPort = atoi(buf);

		pServer = m_pSvrHandle->ConnectServer(strIp, nPort);
		if (pServer && pServer->isOpen())
		{
		pServer->SendMsg("hello, this server!", strlen("hello, this server!")+1);
		std::cout << "连接服务器:" << strIp.c_str() << " 端口:" << nPort << "成功!" << std::endl;
		}		
		else if (pServer == NULL)
		std::cout << "服务器连接错误， 内存分配失败!" << std::endl;
		else
		std::cout << "连接错误 " << pServer->GetErrorCode() << " 原因:" << pServer->GetErrorInfo().c_str() << std::endl;

		}

		for (int i = 1; i <= 10; ++i)
		{
			m_pSvrHandle->SetTimer(i, i * 100); //设定10个定时器，每个定时器间隔为其序号*100毫秒
		}
	}
	virtual void OnUninitalize(void)
	{
	}
	virtual void OnLogin(ConnectPtr pTCPHandle)
	{
	}
	virtual void OnRecvMsg(ConnectPtr pTCPHandle, const char* pData, size_t len)
	{
		if (pTCPHandle)
			pTCPHandle->SendMsg(pData, len); //消息回显
	}

	virtual void OnCommond(const char* pData, size_t len)
	{
		//显示终端命令
		using namespace std;
		cout << pData << endl;
	}

	virtual void OnError(ConnectPtr pTCPHandle, int errCode, const std::string& msg)
	{
		

	}
	
	virtual void OnTimer(std::size_t nIDEvent, void* params)
	{
		using namespace std;
		cout << "定时器" << nIDEvent << endl;

		//定时器执行次数+1
		++m_timerNum[nIDEvent];
		if (m_timerNum[nIDEvent] >= 10) 
			m_pSvrHandle->KillTimer(nIDEvent);

	}
	std::list<ConnectPtr> m_list;
	ConnectPtr pServer;
	std::map<int,int> m_timerNum;
};

int main(int argc, char* argv[])
{
	//设置服务器日志信息，没有如下文件，则默认cout
	_LOG_PATH_INIT("../../doc/log4cpp.logConfig");

	//回显服务器操作
	std::cout << "请输入监听端口号: ";
	char buf[256] = {0};
	std::cin.getline(buf, 256);
	std::cout << std::endl;

	{
		CMyFrameWork frame("test", atoi(buf));
		frame.main();
	}
	
	getchar();
	return 0;
}
