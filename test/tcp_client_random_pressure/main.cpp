/*
	压力测试 回显客户端 tcp_client 
*/
#include <iostream>
#include <string.h>
#include <stdlib.h>

#ifndef WIN32
	#include <unistd.h>
#endif

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
#define BUFF_SIZE 48

//发送线程
void threadWrite(ClientConnectPtr pConnect, char* buff, int len)
{
	if (pConnect && buff)
	{
		pConnect->Send(buff, len);
	}
}

//读取线程
void threadRead(ClientConnectPtr pConnect, char* buff, int len)
{
	memset(buff, 0, BUFF_SIZE);
	long nRead = pConnect->Recv(buff, BUFF_SIZE);
	if (nRead <= 0)
	{
		cout << "套接字错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
	}
	else
	{
		//输出服务器回复的消息
		cout  << buff << endl;
	}
	
}

int main(int argc, char* argv[])
{
	//准备发送的数据域
	std::string strN = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	srand((unsigned)time(NULL));  

	std::vector<IProcessor*> processor;
	
	//分配2个线程进行分别的读写操作
	processor.push_back(CProcessorMgr::Instance().AllocProcessor("Write1"));
	processor.push_back(CProcessorMgr::Instance().AllocProcessor("Read1"));

	//分配一个连接   interface_tcp_client_conn.h 接口说明文件
	ClientConnectPtr pConnect  = CTcpConnectMgr::Instance().AllocConnect();
	if (!pConnect)
	{
		cout << "内存分配出错" << endl;
		getchar();
		return 0;
	}

	if (!pConnect->Connect("127.0.0.1", 11234))
	{
		cout << "连接错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
		getchar();
		return 0;
	}
	
	char *pReadBuf = new char[BUFF_SIZE];		//接收缓冲区
	char *pWriteBuf = new char[BUFF_SIZE];		//发送缓冲区
	int index = 0;								//发送线程索引
	
	while (true)		
	{
#ifdef WIN32
		Sleep(5);
#else
		usleep(5000);
#endif

		memset(pWriteBuf, 0, BUFF_SIZE);

		for (int i = 0; i < BUFF_SIZE - 1; ++i)			//随机生成需要发送的字节的数据
			pWriteBuf[i] = strN[rand()%strN.size()];

		pWriteBuf[BUFF_SIZE - 1] = '\0';
		//随机选择一个发送线程，将数据发送出去
		int sendIndex = processor.size()-2 > 0 ? (++index) % (processor.size()-2) : 0;
		processor[sendIndex]->PostTask(QTH_NAME_SPACE::bind(threadWrite, pConnect, pWriteBuf, BUFF_SIZE));

		//选择读取线程接收数据并输出信息
		processor[processor.size() -1]->PostTask(QTH_NAME_SPACE::bind(threadRead, pConnect, pReadBuf, BUFF_SIZE));
	}
	
	delete[] pReadBuf;
	delete[] pWriteBuf;
	return 0;
}
