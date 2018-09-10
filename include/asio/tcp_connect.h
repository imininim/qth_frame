/*******************************
function:	tcp 连接 包装
date:		2016/12/20
version:	v1.0
tips:		提供tcp 连接的包装类
********************************/
#include "boost/asio.hpp"
#include "asio/frame_work.h"

BEGIN_NAMESPACE
	class ITcpServer;
	
class CTcpConnect
	: public ITcpConnect,
	  public enable_shared_from_this<ITcpConnect, ConnectDeleter>		
{
	enum {
			max_buffer_len	= 1024 * 64,		//最大接收缓冲区
			max_send_len	= 1024 * 64,		//最大发送缓冲区
		};			

	friend class CTcpServer;
public:
	CTcpConnect(ITcpServer* pServer,::boost::asio::io_service* pIo, int max_buffer_memory_obj);
	virtual ~CTcpConnect();

	virtual bool Connect(const std::string& strIP, unsigned short nPort);
	virtual bool Close(void);
	
	//异步发送
	virtual void SendMsg(const char* pszData, size_t nLen);
	virtual void GetPeerAddress(std::string& strIP, unsigned short& nPort);
	//获取本端地址
	virtual void GetLocalAddress(std::string& strIP, unsigned short& nPort);
	//设置处理器
	virtual void SetProcessor(IProcessor* pProcessor );
	//获取错误信息
	virtual const int GetErrorCode() const;
	virtual const std::string& GetErrorInfo() const;
	//连接有效性
	virtual bool isOpen() const;	

	//获取接收缓冲区
	char* GetBuffer();

	//设置错误码和错误信息
	void SetError(int code, const std::string& info);

	//设置/获取 连接类型
	virtual void SetConnectType(ETcpConnectType t);
	virtual const ETcpConnectType GetConnectType(void) const;

	virtual void SetData(void* p);
	virtual void* GetData();

	::boost::asio::ip::tcp::socket& GetSock();
private:
	//通常的成员初始化
	void memInit();
	
	void handle_read(ConnectPtr pConnect, const ::boost::system::error_code& ec, std::size_t nRead);
	void handle_write(ConnectPtr pConnect, const ::boost::system::error_code& ec);

	void handle_error(ConnectPtr pConnect, const ::boost::system::error_code& ec);

	void doRecvMsg(ConnectPtr pConnect, char* pBuff, size_t len);
	void start_recv(ConnectPtr pConnect, bool first = false);
	void doGetPeerAddress(void);
	void doGetLocalAddress(void);
	void doSetSockOpt();
private:
	::boost::asio::io_service* m_ios;
	::boost::asio::ip::tcp::socket m_socket;
	ITcpServer*		m_server;							//所属服务器
	IProcessor*	m_processors;							//消息处理器

	int m_errCode;
	std::string m_errMsg;

	SmallObjAllocator m_recvAlloctor;					//用户区内存分配器
	Loki::Mutex m_Allocklock;							//内存分配锁
	char* m_pBuffer;									//接收临时buffer		
	
	std::string m_remoteIP;								//连接对端IP地址
	unsigned short m_remotePort;						//连接对端端口
	std::string m_localIP;								//连接本端IP地址
	unsigned short m_localPort;							//连接本端的端口

	ETcpConnectType m_connectType;						//服务器连接类型
	void* m_pParams;									//用户设置的附加数据

};
END_NAMESPACE

