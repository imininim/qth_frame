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
	
	//套接字设置为非阻塞模式
	if (!pConnect->setSocketMode(true))
	{
		cout << "套接字模式错误: " << pConnect->GetErrorCode() << " , " << pConnect->GetErrorInfo().c_str() << endl;
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
		if (pConnect->Send(Buffer, nLen) >= 0)	//发送消息
		{
			//接收消息
			memset(Buffer, 0, 256);
			
			//超时间隔设置为500毫秒，如果在超时之前接收到服务器的数据则打印出来
			int nRet = pConnect->TimeoutRecv(Buffer,256, 500);
			if (nRet > 0)			//成功读取了数据
				cout << Buffer << endl;		
			else if (nRet == 0)		//超时
				cout << "接收消息超时!" << endl;
			else					//套接字错误,打印错误信息
			{
				cout <<  pConnect->GetErrorCode() << pConnect->GetErrorInfo().c_str() << endl;
				break;
			}						
		}
		else
		{
			cout <<  pConnect->GetErrorCode() << pConnect->GetErrorInfo().c_str() << endl;
			break;
		}
				
		
	}
	return 0;
}
