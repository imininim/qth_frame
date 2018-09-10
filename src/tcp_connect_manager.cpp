#include "stdafx.h"
#include "asio/tcp_connect_manager.h"
#include "asio/tcp_connect.h"
#include "asio/tcp_client_connect.h"

#include "utility/logger.h"
BEGIN_NAMESPACE
CTcpConnectMgr::CTcpConnectMgr( void )
{
	m_alloc.Initialize(BlockSize(sizeof(CTcpConnect) ),  PageSize(sizeof(CTcpConnect)*100) );
}

CTcpConnectMgr::~CTcpConnectMgr()
{
	
}

ClientConnectPtr CTcpConnectMgr::AllocConnect( void )
{
	CTcpClientConnect* p = new CTcpClientConnect;
	ClientConnectPtr pConnect(p);
	_LOG_DEBUG(g_logger, "[连接管理器]客户端连接... %p, sizeof(CTcpClientConnect)=%u", p, sizeof(CTcpClientConnect));
	return pConnect;
}

ConnectPtr  CTcpConnectMgr::AllocConnect(ITcpServer* pServer, ::boost::asio::io_service* pIo, int max_buffer_memory_obj)
{
	m_mutex.Lock();
	CTcpConnect* pConnect = (CTcpConnect*)m_alloc.Allocate();
	m_mutex.Unlock();
	ConnectPtr pShared(pConnect);

	if (pConnect)
		new (pConnect) CTcpConnect(pServer, pIo, max_buffer_memory_obj);
	
	_LOG_DEBUG(g_logger, "[连接管理器]申请服务端连接... %p, sizeof(TcpConnect)=%u", pConnect, sizeof(CTcpConnect));
	return pShared;
}

void CTcpConnectMgr::FreeConnect(ITcpConnect* pConnect)
{
	if (!pConnect)
		return;

	CTcpConnect* p = (CTcpConnect*)pConnect;
	std::string strIP;
	unsigned short nPort = 0;
	p->GetPeerAddress(strIP, nPort);
	_LOG_DEBUG(g_logger, "[连接管理器]释放连接..%p, 地址:%s,端口%u", p, strIP.c_str(), nPort);

	p->~CTcpConnect(); 	
	m_mutex.Lock();
	m_alloc.Deallocate(pConnect);
	m_mutex.Unlock();
}

void CTcpConnectMgr::FreeConnect(ITcpClientConnect* pConnect)
{
	_LOG_DEBUG(g_logger, "[连接管理器]释放客户端连接... %p", pConnect);
	SAFE_DELETE(pConnect);
}

END_NAMESPACE
