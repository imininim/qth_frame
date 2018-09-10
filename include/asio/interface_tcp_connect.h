/*******************************
function:	tcp 连接 对外接口类
date:		2016/12/20
version:	v1.0
tips:		提供对外部使用的TCP接口类
********************************/
#ifndef		INTERFACE_TCP_CONNECT_H_
#define		INTERFACE_TCP_CONNECT_H_
BEGIN_NAMESPACE

//连接远端类型
enum ETcpConnectType
{
	CONNECT_CLIENT,			//该连接是客户端类型
	CONNECT_SERVER,			//该链接是服务器类型
};

class ITcpConnect
{
public:
	virtual ~ITcpConnect(){}

	//发送消息， 异步非阻塞发送
	virtual void SendMsg(const char* pszData, size_t nLen)									= 0;
	//连接有效
	virtual bool isOpen() const																= 0;
	//获取连接对端地址
	virtual void GetPeerAddress(std::string& strIP, unsigned short& nPort)					= 0; 
	//获取本端地址
	virtual void GetLocalAddress(std::string& strIP, unsigned short& nPort)					= 0;
	//获取最后一次套接字操作的错误信息
	virtual  const int GetErrorCode() const													= 0;
	virtual  const std::string& GetErrorInfo() const										= 0;

	//断开连接
	virtual bool Close()																	= 0;

	//获取/设置 连接类型	
	virtual void SetConnectType(ETcpConnectType t)											= 0;
	virtual const ETcpConnectType GetConnectType(void) const								= 0;
			
	//设置一个连接的附加数据
	virtual void SetData(void* p)															= 0;
	virtual void* GetData()																	= 0;
};


END_NAMESPACE
#endif
