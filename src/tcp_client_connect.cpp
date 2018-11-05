#include "stdafx.h"
#include "asio/tcp_client_connect.h"
#include "boost/thread.hpp"

using namespace ::boost::asio::ip;
BEGIN_NAMESPACE

::boost::asio::io_service CTcpClientConnect::s_ios;

CTcpClientConnect::CTcpClientConnect(): m_socket(s_ios)
{
	m_errCode = 0;
}

CTcpClientConnect::~CTcpClientConnect()
{
	Close();
}


bool CTcpClientConnect::Connect(const std::string& strIP, unsigned short nPort)
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
		
	return true;
}

bool CTcpClientConnect::Close(void)
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

long CTcpClientConnect::Send(const char* buff, size_t len)
{
	if (buff == NULL || len == 0) return -1;

	boost::system::error_code ec;
	long nWrite = boost::asio::write(m_socket,boost::asio::buffer(buff,len), ec);
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return -1;
	}

	return nWrite;
}

long CTcpClientConnect::Recv(char* buff, size_t nRead)
{
	if (buff == NULL || nRead == 0) return 0;

	boost::system::error_code ec;
	long nRet = m_socket.read_some(boost::asio::buffer(buff, nRead), ec);
	if (ec && ec.value()==10035) //10035表示在非阻塞套接字下没有读取数据并立即返回的情况
		return 0;
	else if(ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return -1;
	}

	return nRet;
}


long CTcpClientConnect::TimeoutRecv(char* buff, size_t nRead, size_t time_out, size_t outvalue/* = 30*/)
{
	if (buff == NULL || nRead == 0) return 0;

	size_t time = 0;
	long nRet = 0;

	while (time < time_out) //只要当前尚未超时
	{
		nRet = Recv(buff, nRead);
		if (nRet == 0)
		{
			time += outvalue;
			boost::this_thread::sleep(boost::posix_time::millisec(outvalue));
		}
		else
			break;
	}

	return nRet;
}


bool CTcpClientConnect::GetPeerAddress(std::string& strIP, unsigned short& nPort)
{
	if (isOpen())
	{
		boost::system::error_code ec;
		boost::asio::ip::tcp::endpoint ep = m_socket.remote_endpoint(ec);

		if (ec)
		{
			m_errCode = ec.value();
			m_errMsg = ec.message();
			return false;
		}
		else
		{
			strIP = ep.address().to_string();
			nPort = ep.port();
			return true;
		}
	}

	return false;
}
	
bool CTcpClientConnect::GetLocalAddress(std::string& strIP, unsigned short& nPort)
{
	if (isOpen())
	{
		boost::system::error_code ec;
		boost::asio::ip::tcp::endpoint ep = m_socket.local_endpoint(ec);
		if (ec)
		{
			m_errCode = ec.value();
			m_errMsg = ec.message();
			return false;
		}
		else
		{
			strIP = ep.address().to_string();
			nPort = ep.port();
			return true;
		}
	}

	return false;
}

const int CTcpClientConnect::GetErrorCode() const
{
	return m_errCode;
}

const std::string& CTcpClientConnect::GetErrorInfo() const
{
	return m_errMsg;
}
	
bool CTcpClientConnect::isOpen() const
{
	return m_socket.is_open();
}

bool CTcpClientConnect::setSocketMode(bool mode)
{

	boost::system::error_code ec;
	m_socket.non_blocking(mode, ec);
	if (ec)
	{
		m_errCode = ec.value();
		m_errMsg = ec.message();
		return false;
	}

	return true;

}

END_NAMESPACE




