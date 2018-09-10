
#include <iostream>

#include "utility/utility.h"
#include "asio/frame_work.h"

using namespace QTH_NAME_SPACE;

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
		
	}
	//消息接收
	virtual void OnRecvMsg(ConnectPtr pTCPHandle, const char* pData, size_t len)
	{
		using namespace std;
		cout << "接收到http请求: " << pData << endl;
	}
	//用户登录
	virtual void OnLogin(ConnectPtr pTCPHandle)
	{	
	}
	//套接字错误
	virtual void OnError(ConnectPtr pTCPHandle, int errCode, const std::string& msg)
	{
	}
	//定时器
	virtual void OnTimer(std::size_t nIDEvent, void* params)
	{
	}

	virtual void OnCommond(const char* pData, size_t len){}
};


int main(int argc, char* argv[])
{
	CMyFrameWork fra("httpServer", 80);
	fra.main();

	getchar();
	return 0;
}
