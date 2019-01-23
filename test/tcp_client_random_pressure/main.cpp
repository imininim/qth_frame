/*
	tcp压力测试 
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>

#ifndef WIN32
	#include <unistd.h>
#endif

#include <time.h>
//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
using std::cin;
//step2 tcp客户端头文件
#include "asio/tcp_connect_manager.h"

//线程处理器
#include "utility/processor_mgr.h"
//函数对象
#include "utility/functional.h"

using namespace QTH_NAME_SPACE;

//每次发送，接收字节数量
#define BUFF_SIZE 16
Loki::Mutex g_lock;
FixedAllocator g_allocator(BlockSize(BUFF_SIZE), PageSize(BUFF_SIZE * 4096) );


//发送线程
void threadWrite(ClientConnectPtr pConnect)
{
	if (pConnect)
	{
		char buff[BUFF_SIZE] = {};
		time_t* pTime = (time_t*)buff;
		*pTime = time(NULL); //将当前时间发送过去
		pConnect->Send(buff, BUFF_SIZE);
	}
}

//读取线程
void threadRead(ClientConnectPtr pConnect)
{
	char buff[BUFF_SIZE] = {};
	long nRead = pConnect->Recv(buff, BUFF_SIZE);
	if (nRead <= 0)
	{
		cout << "套接字错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
	}
	else
	{
		//..正常收到数据
		
	}
}

int main(int argc, char* argv[])
{
	//准备发送的数据域
	std::string strN = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	srand((unsigned)time(NULL));  

	std::vector<IProcessor*> processor;
	
	std::string strIP = "127.0.0.1";
	short port = 7001;
	size_t nThread = 10; //启用线程数量

	
	if (argc >= 4)		//包括连接IP 端口 启用线程数量
	{
		strIP = argv[1];
		port = atoi(argv[2]);
		nThread = atol(argv[3]);
	}
	else if(argc >= 3) //包括连接IP和端口
	{
		strIP = argv[1];
		port = atoi(argv[2]);
	}
	else if (argc >= 2) //端口
	{
		port = atoi(argv[1]);
	}
			
	std::string strRead = "Read";
	std::string strWrite = "Write";

	//分配线程进行分别的读写操作
	for (size_t i = 0; i < nThread; ++i)
	{
		std::string strTemp;
		if (i % 2 ==0)
			strTemp = strRead + QTH_NAME_SPACE::T_to_string(i);
		else
			strTemp = strWrite + QTH_NAME_SPACE::T_to_string(i);
		
		//分配线程
		processor.push_back(CProcessorMgr::Instance().AllocProcessor(strTemp.c_str()));
	}

	
	//分配一个连接   interface_tcp_client_conn.h 接口说明文件
	ClientConnectPtr pConnect  = CTcpConnectMgr::Instance().AllocConnect();
	if (!pConnect)
	{
		cout << "内存分配出错" << endl;
		getchar();
		return 0;
	}
		
	cout << "正在向IP:" << strIP.c_str() << " 端口:" << port << "请求连接!" << endl;
	if (!pConnect->Connect(strIP.c_str(), port))
	{
		cout << "连接错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
		getchar();
		return 0;
	}
			
		
	int index = 0;								//发送线程索引
	
	while (true)		
	{
#ifdef WIN32
		Sleep(1); //每毫秒发送一次, 每秒钟发送1000次， 具体发送的数据包数依据发送线程的数量而定
#else
		usleep(1000);
#endif
		
		for (size_t i =0; i < nThread; ++i)
		{
			if (i % 2 == 0)
			{
				//读
				processor[i]->PostTask(QTH_NAME_SPACE::bind(threadRead, pConnect));
			}
			else
			{
				//写
				processor[i]->PostTask(QTH_NAME_SPACE::bind(threadWrite, pConnect));
			}
		}
	}
	
	//停止所有线程处理
	for (size_t i = 0; i < processor.size(); ++i)
		CProcessorMgr::Instance().FreeProcessor(processor[i]);
			
	return 0;
}
