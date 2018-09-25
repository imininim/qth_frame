/*
	上传下载测试客户端
*/
#include <iostream>
//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
using std::cin;
//step2 连接头文件
#include "asio/tcp_connect_manager.h"

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;
	//分配一个连接   interface_tcp_client_conn.h 接口说明文件
	ClientConnectPtr pConnect  = CTcpConnectMgr::Instance().AllocConnect();
	if (!pConnect)
	{
		cout << "内存分配出错" << endl;
		getchar();
		return 0;
	}
	
	char pszIP[256] = {};
	short port = 0;
	cout << "请输入连接的IP地址: ";
	cin.getline(pszIP, 256);
	cout << "请输入端口: ";
	cin >> port;
	cout << "开始连接服务器 " << pszIP << ":" << T_to_string(port).c_str() << endl;
	if (!pConnect->Connect(pszIP, port))
	{
		cout << "连接错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
		getchar();
		return 0;
	}
		
	cin.ignore(INT_MAX, '\n');
	cout << "服务器连接成功!" << endl;
	char userCount[256] = {"guest"};	//用户账户
	char Buffer[256] = {};			
	while (true)
	{
		memset(Buffer, 0, 256);
		cout << userCount << ">";
		cin.getline(Buffer, 256);				//接收用户指令
		if (strcmp(Buffer, "q") == 0)
			break; //断开连接

		std::size_t nLen = (std::size_t)cin.gcount();
		if (pConnect->Send(Buffer, nLen))	//发送消息
		{
			//接收消息
			memset(Buffer, 0, 256);
			pConnect->Recv(Buffer, 256);
			cout << Buffer << endl;				//接收到消息直接打印出来

			if (!pConnect->isOpen())
			{
				cout << pConnect->GetErrorInfo().c_str() << endl;
				getchar();
				break;
			}
		}
		else
		{
			cout << "发送消息失败，对方已经关闭!" << endl;
			break;
		}
				
		
	}
	return 0;
}
