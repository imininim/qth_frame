/********************************
function:	boost 异步IO 包装 
date:		2016/12/19
version:	v1.0
tips:		
封装 boost io_service的操作
遵循 一个线程， 一个IO事件对象， 内部一个IO对象集合
提供一个 GetIOService 操作， 采用轮询的方式返回IO对象集合中的一个IO事件对象
*********************************/
#ifndef		QTH_ASIO_H_
#define		QTH_ASIO_H_
#include "boost/asio/io_service.hpp"
#include "utility/memory.h"
BEGIN_NAMESPACE
class CIOServices
{
	
public:
	typedef QTH_NAME_SPACE::shared_ptr<boost::asio::io_service>		IoServerPtr;
	typedef std::vector<IoServerPtr>						IoServerPtrs;
	
	typedef QTH_NAME_SPACE::shared_ptr<boost::asio::io_service::work> IoWorkPtr;
	typedef std::vector<IoWorkPtr>							IoWorkPtrs;
	
	explicit CIOServices(size_t numService);
	void start(void);
	void stop(void);
	boost::asio::io_service& GetIOService(void);
private:
	IoServerPtrs m_ios;
	IoWorkPtrs m_works;
	std::size_t m_next;
};

END_NAMESPACE
#endif

