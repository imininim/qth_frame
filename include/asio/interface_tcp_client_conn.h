/*******************************
function:	tcp tcp 客户端接口
date:		2017/01/13
version:	v1.0
tips:		提供对外部使用的TCP客户端接口类
********************************/
#ifndef	INTERFACE_TCP_CLIENT_CONN_H_
#define INTERFACE_TCP_CLIENT_CONN_H_

BEGIN_NAMESPACE
		
	class ITcpClientConnect
	{
	public:
		virtual ~ITcpClientConnect(){}
		
		//连接
		virtual bool Connect(const std::string& strIP, unsigned short nPort)			= 0;
		//关闭连接
		virtual bool Close(void)														= 0;
		//阻塞发送
		virtual long Send(const char* buff, size_t len)									= 0;
		//阻塞接收
		virtual long Recv(char* buff, size_t nRead)										= 0;
		//获取连接对端地址
		virtual bool GetPeerAddress(std::string& strIP, unsigned short& nPort)			= 0; 
		//获取本端地址
		virtual bool GetLocalAddress(std::string& strIP, unsigned short& nPort)			= 0;
		//获取最后一次套接字操作的错误信息
		virtual  const int GetErrorCode() const											= 0;
		virtual  const std::string& GetErrorInfo() const								= 0;
		//连接是否有效
		virtual bool isOpen() const														= 0;
		//设置阻塞模式, false , 阻塞操作直到返回为止,  true 立即返回, 并且伴随一个返回的错误
		virtual bool setSocketMode(bool mode)											= 0;

		/*
			函数名: TimeoutRecv
			功能:	在套接字为非阻塞模式下运行，阻塞程序，直到有数据到来或者超时
			参数:	
					buff		读取数据的buff
					nRead		buff的长度
					time_out	超时时间,毫秒数 (1s = 1000毫秒)
					outvalue	检查超时间隔,默认是30毫秒检查一次
			返回值:

		*/
		virtual long TimeoutRecv(char* buff, size_t nRead, size_t time_out, size_t outvalue=30)= 0;
	};
	
END_NAMESPACE
#endif
