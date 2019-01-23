/*
	tcp服务器, 消息回显服务器
	
*/
#include <iostream>

//step1 动态库公用头文件
#include "utility/utility.h"
//step2 服务头文件
#include "asio/frame_work.h"

#include "collect.h"
#include "common/property.h"
using namespace QTH_NAME_SPACE;

class CMyFrameWork: public CFrameWork
{
private:
	CCollect m_collect;
public:
	/*
		构造参数：
		1. 服务名称，  唯一的标识了当前服务
		2. 服务器使用端口
		3. 处理 监听、建立连接、消息收发的线程数量
	*/
	CMyFrameWork(const std::string& strName, unsigned short nPort, int threadNum = 3): CFrameWork(strName,nPort, threadNum)
	{

	}

	~CMyFrameWork()
	{
		
	}

	virtual void OnInitialize(void)
	{
		using namespace std;
		cout << "服务器初始化成功! 正在等待用户连接..." << endl;
		
	}
	virtual void OnUninitalize(void)
	{
		using namespace std;
		cout << "服务器关闭." << endl;
	}
	virtual void OnLogin(ConnectPtr pTCPHandle)
	{
		using namespace std;
		std::cout << "有用户登录!" << endl;
		cout << ">";
	}
	virtual void OnRecvMsg(ConnectPtr pTCPHandle, const char* pData, size_t len)
	{
		if (len >= sizeof(long))
		{
			const long* pTime = (const long*)pData;
			m_collect.doCollect(*pTime);

			//消息回显
			pTCPHandle->SendMsg(pData, len);
		}
		
	}

	virtual void OnCommond(const char* pData, size_t len)
	{
		if (!pData || len == 0) return;

		//解析参数
		CmdParse cmd;
		cmd.parse(std::string(pData));
		
		//用户输入了答应信息的命令行
		if (cmd["act"] == "dump")
		{
			m_collect.dump();  //打印
		}
		else if (cmd["act"] == "clear")
		{
			m_collect.reset(); //清理数据
		}
		else
		{
			std::cout << "命令行" << pData << "未找到对应的处理!" << std::endl;
		}
	}

	virtual void OnError(ConnectPtr pTCPHandle, int errCode, const std::string& msg)
	{
		using namespace std;
		cout << "连接发生错误, 错误码:" << errCode << " 原因:" << msg.c_str() << endl;
		cout << ">";

	}
	
	virtual void OnTimer(std::size_t nIDEvent, void* params)
	{
		

	}
	
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
		frame.main(true);
	}
	
	getchar();
	return 0;
}
