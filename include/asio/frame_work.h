/********************************
function:	服务运行启动框架
date:		2016/12/22
version:	v1.0
tips:		
		封装TCP服务器的启动，监听， 数据接收和发送
		继承使用
*********************************/
#ifndef FRAME_WORK_H_
#define FRAME_WORK_H_
#include "utility/nocopyable.h"
#include "asio/interface_tcp_server.h"

BEGIN_NAMESPACE
	class QTH_UTILITY_EXPORT CFrameWork: public Nocopyable
	{
	public:
		/*
			显示构造参数:
				svrName:			服务名称，唯一的表示一个服务器实例
				nPort:				服务器使用的端口号
				numHandleThread:	服务器使用多少个线程来进行数据收发和监听(用户应该根据实际测试情况随时调整)
				
				max_obj_size:		
									最理想状态下， 你每个发送的数据包大小都应该小于此值。
									在内存吃紧的情况下，适量减少此值
		*/
		CFrameWork(const std::string& svrName, unsigned short nPort, int numHandleThread = 1, int max_obj_size = 64);
		virtual ~CFrameWork();

		//开始启用服务器, 启动服务器，阻塞运行，等待用户的连接
		/*
			params: 
				useCmd		是否启用命令行参数，true启用， 

			tips: 一旦使用命令行启动方式， 则不要将其置入后台运行
		*/
		void main(bool useCmd = false);
		
		/*
			name:		OnInitialize
			function:	初始化， tcp服务器初始化后回调此接口
		*/
		virtual void OnInitialize(void) = 0;

		/*
			name:		OnUninitalize
			function:	结束清理， tcp服务器停止运行时回调此接口执行清理操作
		*/
		virtual void OnUninitalize(void) = 0;

		/*
			name:		OnLogin
			function:	用户登录回调此接口
			params:
						pTCPHandle		用户连接指针封装， 详细接口说明查看 interface_tcp_connect.h
						
		*/
		virtual void OnLogin(ConnectPtr pTCPHandle)			= 0;

		/*
			name:		OnRecvMsg
			function:	消息接收
			params:
						pTCPHandle		用户连接指针封装， 详细接口说明查看 interface_tcp_connect.h
						pData			用户发送的数据
						len				用户发送数据大小
		*/
		virtual void OnRecvMsg(ConnectPtr pTCPHandle, const char* pData, size_t len) = 0;

		/*
			name:		OnCommond
			function:	服务器命令行接收
			params:		
						pData		接收的命令行信息
						len			接收的命令行大小
		*/
		virtual void OnCommond(const char* pData, size_t len)	= 0;

		/*
			name:		OnError
			function:	下线处理，用户套接字读取发生错误，连接被关闭时回调此函数
			params:
						pTCPHandle		发生套接字错误的用户连接指针
						errCode			套接字错误码
						msg				套接字错误信息
			tips:
				当发生套接字读取、写入错误时，通常情况意味这连接断开，这个时候你不应该使用
				pTCPHandle->SendMsg 发送信息， 而是应该释放这个连接

				当你不再持有这个连接的引用（从你的数据集合中erase后），该连接自动释放
		*/
		virtual void OnError(ConnectPtr pTCPHandle, int errCode, const std::string& msg) = 0;
		
		/*
			name:		OnTimer
			function:	定时器服务, 到达用户指定的定时器间隔时，回调此接口
			params:
				nIDEvent 用户设定的定时器ID， 用来标识哪个定时器到达指定时间间隔
				params	 用户附加的数据， 用户在调用 m_pSvrHandle->SetTimer 所附加的数据
		*/
		virtual void OnTimer(std::size_t nIDEvent, void* params)		= 0;
	protected:
		void doCmd();

		/*	
				服务器指针   接口说明查看 interface_tcp_server.h
		*/
		ITcpServer* m_pSvrHandle;
	};

END_NAMESPACE
#endif