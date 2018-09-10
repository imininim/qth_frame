/*******************************
function:	tcp 客户端连接 包装
date:		2016/12/20
version:	v1.0
tips:		提供tcp 客户端连接的简便方式， 该接口只是为了测试方便而使用
********************************/
#include "asio/interface_tcp_client_conn.h"
#include "boost/asio.hpp"
BEGIN_NAMESPACE
	
class CTcpClientConnect: public ITcpClientConnect
{
public:
	CTcpClientConnect(void);
	virtual ~CTcpClientConnect();

	//连接
	virtual bool Connect(const std::string& strIP, unsigned short nPort);
	//关闭连接
	virtual bool Close(void);
	//阻塞发送
	virtual long Send(const char* buff, size_t len);
	//阻塞接收
	virtual long Recv(char* buff, size_t nRead);
	//获取连接对端地址
	virtual bool GetPeerAddress(std::string& strIP, unsigned short& nPort);
	//获取本端地址
	virtual bool GetLocalAddress(std::string& strIP, unsigned short& nPort);
	//获取最后一次套接字操作的错误信息
	virtual  const int GetErrorCode() const;
	virtual  const std::string& GetErrorInfo() const;
	//连接是否有效
	virtual bool isOpen() const;
	//设置阻塞模式
	virtual bool setSocketMode(bool mode);
private:
	static ::boost::asio::io_service s_ios;
	::boost::asio::ip::tcp::socket m_socket;

	int m_errCode;
	std::string m_errMsg;
};
END_NAMESPACE

