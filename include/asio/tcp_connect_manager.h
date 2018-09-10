/*******************************
function:	tcp 连接管理器
date:		2016/01/16
version:	v1.0
tips:		提供tcp连接的分配，回收等管理操作
********************************/
#ifndef TCP_CONNECT_MANAGER_H_
#define TCP_CONNECT_MANAGER_H_
#include "utility/memory.h"
#include "asio/interface_tcp_connect.h"
#include "asio/interface_tcp_client_conn.h"

namespace boost
{
	namespace asio
	{
		class io_service;
	}
}

BEGIN_NAMESPACE
	//短式声明
	class ITcpServer;

	template<class T>
	struct ConnectDeleter;
	//服务端回调连接指针
	typedef		shared_ptr<ITcpConnect,ConnectDeleter>				ConnectPtr;
	
	//客户端连接指针
	typedef		shared_ptr<ITcpClientConnect,ConnectDeleter>		ClientConnectPtr;

	class QTH_UTILITY_EXPORT CTcpConnectMgr: public Singleton<CTcpConnectMgr>
	{
		friend class CreateUsingNew<CTcpConnectMgr>;
		template<class> friend struct ConnectDeleter;
	public:
		~CTcpConnectMgr();	
		//分配一个客户端的连接
		ClientConnectPtr AllocConnect(void);
		//分配一个服务端的连接
		ConnectPtr AllocConnect(ITcpServer* pServer, ::boost::asio::io_service* pIo, int max_buffer_memory_obj);

	private:
		void FreeConnect(ITcpConnect* pConnect);
		void FreeConnect(ITcpClientConnect* pConnect);

		CTcpConnectMgr(void);
	private:
		FixedAllocator m_alloc;			//连接分配器
		::Loki::Mutex m_mutex;
	};
	
	template<class T>
	struct ConnectDeleter
	{
		static void Delete(T* p)
		{
			CTcpConnectMgr::Instance().FreeConnect(p);
		}
	};
	
END_NAMESPACE
#endif
