#include "stdafx.h"
#include "asio/tcp_connect.h"

#include "utility/logger.h"
#include "utility/processor.h"

#include "boost/bind.hpp"


using namespace ::boost::asio::ip;
BEGIN_NAMESPACE

void CTcpConnect::memInit()
{
	m_pBuffer = NULL;
	m_pParams = NULL;
	m_processors = NULL;	
	m_remotePort = 0;
	m_localPort = 0;	
	m_connectType = CONNECT_CLIENT; 
	m_errCode = 0;
}

CTcpConnect::CTcpConnect( ITcpServer* pServer, ::boost::asio::io_service* pIo, int max_buffer_memory_obj)
	:m_ios(pIo), m_socket(*m_ios),
	m_server(pServer),m_recvAlloctor(PageSize(max_buffer_memory_obj*32), MaxObjSize(max_buffer_memory_obj) )
{
	assert(pServer && pIo);
	memInit();
}

CTcpConnect::~CTcpConnect()
{
	Close();
	SAFE_DELETE(m_pBuffer);
}

bool CTcpConnect::Connect( const std::string& strIP, unsigned short nPort )
{
	boost::system::error_code ec;
	tcp::endpoint ep(address_v4::from_string(strIP, ec), nPort);
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return false;
	}

	m_socket.connect(ep, ec);
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return false;
	}

	doSetSockOpt();
	doGetLocalAddress();
	doGetPeerAddress();
	return true;
}

bool CTcpConnect::Close( void )
{
	if (!m_socket.is_open())
		return true;

	boost::system::error_code ec;
	m_socket.shutdown(boost::asio::socket_base::shutdown_both, ec); //停止连接数据的发送和接收
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return false;
	}

	m_socket.close(ec);
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return false;
	}
	return true;
}

void CTcpConnect::SendMsg( const char* pszData, size_t nLen )
{
	if (isOpen() && pszData != NULL && nLen != 0)
	{
		boost::asio::async_write(m_socket, 
			boost::asio::buffer(pszData, nLen), 
			boost::bind(&CTcpConnect::handle_write, this, shared_from_this(),boost::asio::placeholders::error) );
	}
}


void CTcpConnect::GetLocalAddress( std::string& strIP, unsigned short& nPort )
{
	strIP = m_localIP;
	nPort = m_localPort;
}

void CTcpConnect::GetPeerAddress( std::string& strIP, unsigned short& nPort )
{
	strIP = m_remoteIP;
	nPort = m_remotePort;
}

void CTcpConnect::SetProcessor(IProcessor* pProcessor )
{
	m_processors = pProcessor;
}

const int CTcpConnect::GetErrorCode() const
{
	return m_errCode;
}

const std::string& CTcpConnect::GetErrorInfo() const
{
	return m_errMsg;
}

void CTcpConnect::start_recv(ConnectPtr pConnect, bool first)
{
	if (isOpen())
	{
		if (first) //第一次调用接收时，设置其套接字选项
			doSetSockOpt();
				
		m_socket.async_read_some(boost::asio::buffer(GetBuffer(), max_buffer_len),
			boost::bind(&CTcpConnect::handle_read, this, pConnect,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
}

boost::asio::ip::tcp::socket& CTcpConnect::GetSock()
{
	return m_socket;
}

void CTcpConnect::handle_read(ConnectPtr pConnect, const ::boost::system::error_code& ec, std::size_t nRead)
{
	if (ec)
	{
		handle_error(pConnect, ec);
	}
	else
	{
		if (nRead != 0 && m_processors)
		{
			//分配用户数据buffer
			m_Allocklock.Lock();
			char* pTempBuffer = (char*)m_recvAlloctor.Allocate(nRead, false);
			m_Allocklock.Unlock();

			if (pTempBuffer != NULL)
			{
				memcpy(pTempBuffer, GetBuffer(), nRead);
				//通告用户，有数据到来
				m_processors->PostTask(boost::bind(&CTcpConnect::doRecvMsg, this, pConnect, pTempBuffer, nRead));
			}
			else
			{
				_LOG_ERROR(g_logger, "[tcp连接]内存分配失败， 此数据将无法传达:  %s", GetBuffer());
			}
		}
			
		//处理完一次消息接收后，继续等待下一个消息
		start_recv(pConnect);
	}
}



void CTcpConnect::handle_write(ConnectPtr pConnect, const boost::system::error_code& ec )
{
	if (ec)
	{
		handle_error(pConnect, ec);
	}
	
}

void CTcpConnect::handle_error(ConnectPtr pConnect, const ::boost::system::error_code& ec)
{
	Close();

	//其他套接字错误
	if (m_processors)
	{
		m_processors->PostTask(boost::bind(&CFrameWork::OnError, m_server->GetFrame(),
		pConnect, ec.value(), ec.message() ));
	}
}

void CTcpConnect::doRecvMsg(ConnectPtr pConnect, char* pBuff, size_t len)
{
	if (pBuff && len != 0)
	{
		m_server->GetFrame()->OnRecvMsg(pConnect,pBuff, len);

		m_Allocklock.Lock();
		m_recvAlloctor.Deallocate(pBuff,len);
		m_Allocklock.Unlock();
	}
	
}

void CTcpConnect::doGetPeerAddress( void )
{
	if (isOpen())
	{
		boost::system::error_code ec;
		boost::asio::ip::tcp::endpoint ep = m_socket.remote_endpoint(ec);

		if (ec)
		{
			_LOG_ERROR(g_logger, "[tcp连接]对端地址获取失败: [%d]%s", ec.value(), ec.message().c_str());
		}
		else
		{
			m_remoteIP = ep.address().to_string();
			m_remotePort = ep.port();
		}
	}
}

void CTcpConnect::doGetLocalAddress(void)
{
	if (isOpen())
	{
		boost::system::error_code ec;
		boost::asio::ip::tcp::endpoint ep = m_socket.local_endpoint(ec);
		if (ec)
		{
			_LOG_ERROR(g_logger, "[tcp连接]本端地址获取失败: [%d]%s", ec.value(), ec.message().c_str());
		}
		else
		{
			m_localIP = ep.address().to_string();
			m_localPort = ep.port();
		}
	}
}

bool CTcpConnect::isOpen() const
{
	return m_socket.is_open();
}

void CTcpConnect::doSetSockOpt()
{
	if (isOpen())
	{
		//设置tcp接收缓冲区大小
		boost::system::error_code  ec;
		boost::asio::socket_base::receive_buffer_size optionR(CTcpConnect::max_buffer_len);
		GetSock().set_option(optionR,ec);
		if (ec) 
			_LOG_ERROR(g_logger, "[TCP连接] 设置接收缓冲区的大小失败,原因 %s", ec.message().c_str());

		boost::asio::socket_base::send_buffer_size optionW(CTcpConnect::max_send_len);
		GetSock().set_option(optionW,ec);
		if (ec) 
			_LOG_ERROR(g_logger, "[TCP连接] 设置发送缓冲区的大小失败,原因 %s", ec.message().c_str());
	}
	
}

char* CTcpConnect::GetBuffer()
{
	if (!m_pBuffer)
		m_pBuffer = new char[max_buffer_len];

	return m_pBuffer;
}

void CTcpConnect::SetError(int code, const std::string& info)
{
	m_errCode = code;
	m_errMsg = info;
}
void CTcpConnect::SetConnectType(ETcpConnectType t)
{
	m_connectType = t;
}

void CTcpConnect::SetData(void* p)
{
	m_pParams = p;
}

void* CTcpConnect::GetData()
{
	return m_pParams;
}

const ETcpConnectType CTcpConnect::GetConnectType(void) const
{
	return m_connectType;
}

END_NAMESPACE




