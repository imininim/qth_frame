/*******************************
function:	tcp 服务类
date:		2016/12/21
version:	v1.0
tips:		封装boost tcp服务
********************************/
#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_
#include "asio/asio.h"
#include "asio/interface_tcp_server.h"
#include "boost/asio.hpp"

BEGIN_NAMESPACE
	class IProcessor;
	class CIOServices;
	class CTcpConnect;
	class ITimer;
	class CFrameWork;

	class CTcpServer: public ITcpServer
	{
	public:
		CTcpServer(const std::string& svrName, CFrameWork* pFrame, unsigned short nPort, int numThread, int max_memory_pool_obj);
		virtual ~CTcpServer(void);

		virtual bool init();
		virtual void uinit();
		virtual void run();
		
		virtual CFrameWork* GetFrame();
		virtual const EServerState& GetState()  const;
		virtual const std::string& GetSvrName() const;
		virtual IProcessor* GetLogicalProcessor();
		virtual ITimer*		GetTimer();

		//设置定时器
		virtual bool SetTimer(std::size_t nIDEvent, std::size_t nElaspse, void* pParam = NULL, int nRunTimes = -1);
		virtual void KillTimer(std::size_t nIDEvent);

		//执行主动连接, 返回连接的指针， 有可能为NULL
		virtual ConnectPtr ConnectServer(const std::string& strIP, unsigned short nPort);
	private:
		//等待接收客户端的连接
		void doStartAccept(void);
		void doHandleAccept(const boost::system::error_code& e , ConnectPtr pConnect);
		CIOServices m_ios;
		CFrameWork* m_pFrame;		 
		IProcessor* m_pProcessor;					//事件通知处理器
		boost::asio::ip::tcp::acceptor m_acceptor;	//连接接收器
		boost::asio::signal_set m_asignal;			//信号
		ITimer*		m_pTimer;						//定时器
		EServerState m_eRuning;
		boost::asio::ip::tcp::endpoint m_localUrl;	//服务地址
		std::string m_svrName;						//服务器名称
		std::size_t m_connectMaxSize;				//连接最大能够享受到内存池特性的buffer大小
	};
		
END_NAMESPACE
#endif
