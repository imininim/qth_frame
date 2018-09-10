#include "stdafx.h"
#include <assert.h>
#include "asio/asio.h"
#include "boost/thread/thread.hpp"

BEGIN_NAMESPACE
CIOServices::CIOServices( size_t numService ):m_next(0)
{
	assert(numService != 0);
	for (std::size_t i = 0; i < numService; ++i)
	{
		IoServerPtr io_server(new boost::asio::io_service);
		IoWorkPtr io_work(new boost::asio::io_service::work(*io_server) );
		
		m_ios.push_back(io_server);
		m_works.push_back(io_work);
	}
}

void CIOServices::start( void )
{
	std::vector<boost::thread*> threads;
	for (size_t i = 0; i < m_ios.size(); ++i)
	{
		boost::thread* pThread = new boost::thread(boost::bind(&boost::asio::io_service::run, m_ios[i].get()));
		if (pThread)
			threads.push_back(pThread);
	}
	
	for (std::size_t i=  0; i < threads.size(); ++i)
	{
		threads[i]->join();
		SAFE_DELETE(threads[i]);
	}
}

void CIOServices::stop( void )
{
	for (std::size_t i = 0; i < m_ios.size(); ++i)
	{
		m_ios[i]->stop();
	}
}

boost::asio::io_service& CIOServices::GetIOService()
{
	return *(m_ios[(m_next++) % m_ios.size()]);
}


END_NAMESPACE