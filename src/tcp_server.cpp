#include "stdafx.h"
#include "asio/tcp_server.h"
#include "asio/tcp_connect.h"
#include "utility/processor_mgr.h"
#include "utility/logger.h"
#include "asio/timer.h"
#include "boost/bind.hpp"
BEGIN_NAMESPACE
CTcpServer::CTcpServer( const std::string& svrName, CFrameWork* pFrame, unsigned short nPort,
					   int numThread, int max_memory_pool_obj)
					   : m_ios(numThread),
					   m_pFrame(pFrame),
					   m_acceptor(m_ios.GetIOService() ),
					   m_asignal(m_ios.GetIOService()),
					   m_localUrl(boost::asio::ip::tcp::v4(), nPort), 
					   m_svrName(svrName),
					   m_connectMaxSize(max_memory_pool_obj)
						 
{
	m_pTimer = NULL;
	m_pProcessor = NULL;
	m_eRuning = STOPED;
	assert(m_pFrame != NULL);

	m_asignal.add(SIGINT);
	m_asignal.add(SIGTERM);
#if defined(SIGQUIT)
	m_asignal.add(SIGQUIT);
#endif
}


CTcpServer::~CTcpServer( void )
{
	assert(m_eRuning == STOPED);
	//SAFE_DELETE(m_ios);

}

bool CTcpServer::init()
{
	if (m_eRuning != STOPED)
		return false;
	
	_LOG_DEBUG(g_logger, "[TCP服务]初始化服务器 %s...", GetSvrName().c_str() );
	try
	{
		//初始化主逻辑处理器
		m_pProcessor = CProcessorMgr::Instance().AllocProcessor("logical_handle");
		assert(m_pProcessor);
		_LOG_DEBUG(g_logger, "[TCP服务]分配主逻辑处理器成功!");

		//绑定、监听
		using namespace boost::asio::ip;
		m_acceptor.open(m_localUrl.protocol() );
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
		m_acceptor.bind(m_localUrl);
		m_acceptor.listen();
		m_eRuning = ITcpServer::INITED;

		_LOG_DEBUG(g_logger, "[TCP服务]启动监听服务! 监听端口%u", m_localUrl.port());
		//设置退出信号
		m_asignal.async_wait(boost::bind(&CTcpServer::uinit, this));
		_LOG_DEBUG(g_logger, "[TCP服务]启动信号监听服务!");
		//设置定时器服务
		m_pTimer = new CTimer(m_ios.GetIOService(), m_pProcessor);
		assert(m_pTimer);
		_LOG_DEBUG(g_logger, "[TCP服务]启动定时器服务!");
		//调用框架的初始化
		m_pProcessor->SendTask(boost::bind(&CFrameWork::OnInitialize, GetFrame()) );
	}
	catch(boost::system::system_error& ec)
	{
		CProcessorMgr::Instance().FreeProcessor(m_pProcessor);
		_LOG_ERROR(g_logger, "[TCP服务]初始化服务器 %s 失败, 原因:%s", GetSvrName().c_str(), ec.what() );
		return false;
	}
	catch (std::exception& ec)
	{
		_LOG_ERROR(g_logger, "[TCP服务]初始化服务器 %s 失败, 原因:%s", GetSvrName().c_str() ,ec.what() );
		return false;
	}

	_LOG_DEBUG(g_logger, "[TCP服务]初始化服务器 %s 完成.", GetSvrName().c_str() );
	return true;
	
}

void CTcpServer::uinit()
{
	using namespace std;

	if (m_eRuning < INITED)
		return;
	_LOG_DEBUG(g_logger, "[TCP服务]正在停止服务器 %s...", GetSvrName().c_str() );
	//先通告客户，服务器准备关闭
	m_pProcessor->SendTask(boost::bind(&CFrameWork::OnUninitalize, GetFrame() ) );
	try
	{
		//停止服务运行	
		m_acceptor.close();
		_LOG_DEBUG(g_logger, "[TCP服务]%s: 停止 IO服务!", GetSvrName().c_str() );
		m_ios.stop();

		_LOG_DEBUG(g_logger, "[TCP服务]%s 停止定时器服务!", GetSvrName().c_str() );
		if (m_pTimer)
		{
			delete m_pTimer;
			m_pTimer = NULL;
		}
				
		_LOG_DEBUG(g_logger, "[TCP服务]%s: 停止逻辑处理器!", GetSvrName().c_str() );
		CProcessorMgr::Instance().FreeProcessor(m_pProcessor);
		m_pProcessor = NULL;

		_LOG_DEBUG(g_logger, "[TCP服务]%s: 停止框架运行成功!", GetSvrName().c_str() );
	}
	catch(boost::system::error_code& ec)
	{
		_LOG_ERROR(g_logger,"[TCP服务]停止服务异常: %d:%s", ec.value(), ec.message().c_str());
	}
	catch(std::exception& e)
	{
		_LOG_ERROR(g_logger,"[TCP服务]停止服务异常: %s", e.what());
	}
	
	//设置状态为停止
	m_eRuning = STOPED;
	_LOG_DEBUG(g_logger, "[TCP服务]服务器 %s 已经停止", GetSvrName().c_str() );
}

void CTcpServer::run()
{
	if (m_eRuning != INITED)
		return;

	doStartAccept();
	_LOG_DEBUG(g_logger, "[TCP服务]服务器 %s 启动完成! 开始等待连接...", GetSvrName().c_str());
	m_ios.start();
}

const std::string& CTcpServer::GetSvrName() const
{
	return m_svrName;
}


CFrameWork* CTcpServer::GetFrame()
{
	return m_pFrame;
}

const CTcpServer::EServerState& CTcpServer::GetState() const
{
	return m_eRuning;
}

void CTcpServer::doStartAccept( void )
{	
	ConnectPtr pTemp = CTcpConnectMgr::Instance().AllocConnect(this, &(m_ios.GetIOService()), m_connectMaxSize);
	if (!pTemp)
	{
		_LOG_ERROR(g_logger, "[TCP服务]建立新的连接失败，原因：分配内存失败");
		return;
	}

	m_acceptor.async_accept( ((CTcpConnect*)pTemp.get())->GetSock(), boost::bind(&CTcpServer::doHandleAccept,
		this, boost::asio::placeholders::error,
		pTemp) );
}

void CTcpServer::doHandleAccept( const boost::system::error_code& e , ConnectPtr pConnect)
{
	if (e)
	{
		if (e.value() == 995)
			return;				//服务器IO对象正常关闭，不应该通告客户

		if (e.value() == 125) 
			return;				//操作取消

		_LOG_ERROR(g_logger, "[TCP服务]监听错误%d 原因: %s", e.value(), e.message().c_str() );
	}
	else
	{
		CTcpConnect* pTmp = (CTcpConnect*)pConnect.get();
		if (pTmp->GetBuffer() != NULL)   
		{
			//获取对端的地址和本端地址
			pTmp->doGetPeerAddress();
			pTmp->doGetLocalAddress();

			//设置消息接收,发送,错误 的处理器
			pTmp->SetProcessor(GetLogicalProcessor());
			//通告客户登录(直到登录完成，然后才能开始接受消息)
			m_pProcessor->SendTask(boost::bind(&CFrameWork::OnLogin, GetFrame(), pConnect));
			//开始接收消息
			pTmp->start_recv(pConnect, true);
		}
		else
		{
			_LOG_ERROR(g_logger, "[TCP服务] 侦测到了一个新的连接,但是分配读取缓冲区失败,连接无效!");
		}
	}

	//继续监听下一个连接
	doStartAccept();
}

IProcessor* CTcpServer::GetLogicalProcessor()
{
	return m_pProcessor;
}

ITimer* CTcpServer::GetTimer()
{
	return m_pTimer;
}

bool CTcpServer::SetTimer( std::size_t nIDEvent, std::size_t nElaspse, void* pParam, int nRunTimes /*= -1*/)
{
	return m_pTimer->SetTimer(nIDEvent, nElaspse, 
		QTH_NAME_SPACE::bind(&CFrameWork::OnTimer, GetFrame(), nIDEvent, pParam) , nRunTimes);
}

void CTcpServer::KillTimer( std::size_t nIDEvent )
{
	m_pTimer->KillTimer(nIDEvent);
}

ConnectPtr CTcpServer::ConnectServer(const std::string& strIP, unsigned short nPort)
{
	ConnectPtr pTemp = CTcpConnectMgr::Instance().AllocConnect(this, &(m_ios.GetIOService()), m_connectMaxSize);
	if (!pTemp)
		return pTemp; //空指针 直接返回

	CTcpConnect* pConnect = (CTcpConnect*)pTemp.get();
	if (pConnect->GetBuffer() == NULL)
	{
		pConnect->SetError(-1, "连接接收缓冲区内存分配失败!");
		return pTemp;	
	}
		

	//设置消息接收，发送，错误的处理器
	pConnect->SetProcessor(GetLogicalProcessor()); 
	//设置连接类型为服务器 到 服务器的连接
	pConnect->SetConnectType(CONNECT_SERVER);

	if (!pConnect->Connect(strIP, nPort)) 
		return pTemp;

	//开启接收消息处理
	pConnect->start_recv(pTemp);
	return pTemp;
}

END_NAMESPACE


