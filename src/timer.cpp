#include "stdafx.h"
#include "asio/timer.h"
#include "utility/logger.h"
#include "boost/bind.hpp"
BEGIN_NAMESPACE

CTimer::CTimer(boost::asio::io_service& io, IProcessor* pProcessor)
	:m_ios(io), m_pProcessor(pProcessor)
{
	assert(pProcessor);
	m_alloc.Initialize(BlockSize(sizeof(STimerObj) ), PageSize(4096) );
}

CTimer::~CTimer()
{
	for (TimerMap::iterator it = m_ts.begin(); it != m_ts.end(); ++it)
	{
		STimerObj* pTimer = it->second;
		if (pTimer)
		{
			pTimer->~STimerObj();
			m_alloc.Deallocate(pTimer);
		}
	}

	m_ts.clear();
}

bool CTimer::SetTimer( std::size_t nIDEvent, std::size_t nElaspse,
					  const FCallBack& callback,int nRunTimes )
{
	STimerObj* pTimer = findTimer(nIDEvent);
	if (!pTimer)
	{
		pTimer = (STimerObj*)m_alloc.Allocate();
		if (!pTimer)
		{
			_LOG_ERROR(g_logger, "内存分配失败, 无法设置定时器 %u", nIDEvent);
			return false;
		}
		else
		{
			//新建立的timer需要重新构造
			new (pTimer) STimerObj(m_ios, nIDEvent, nElaspse);
			insertTimer(pTimer->nIDEvent, pTimer);
		}		
	}
		
	
	pTimer->nRunTimes = nRunTimes;
	pTimer->nElaspse = nElaspse;
	pTimer->callback = callback;
	
	pTimer->t.async_wait(QTH_NAME_SPACE::bind(&CTimer::run, this, pTimer, QTH_NAME_SPACE::placeholders::_1) );
	return true;
}

void CTimer::KillTimer( std::size_t nIDEvent )
{
	STimerObj* pTimer = findTimer(nIDEvent);
	if (pTimer)
	{
		removeTimer(nIDEvent);
		pTimer->nRunTimes = 0; //设置运行次数为0，等待下一次定时器运行时将其释放
	}
	 
	
}

void CTimer::run(STimerObj* p, const boost::system::error_code& e)
{
	assert(p); //定时器回调过程中，这个定时器一定存在
	
	if (e)
		return;
		

	if (p->nRunTimes < 0)
	{
		//无限次运行
		m_pProcessor->PostTask(p->callback, IProcessor::HIGHT);
	}
	else if (p->nRunTimes == 0)
	{
		//运行了指定次数后删除该定时器
		doKillTimer(p);
		return;
	}
	else
	{
		m_pProcessor->PostTask(p->callback,IProcessor::HIGHT);
		--p->nRunTimes; //减少指定运行次数
	}

	//继续等待下次回调
	p->t.expires_from_now(boost::posix_time::milliseconds(p->nElaspse));
	p->t.async_wait( QTH_NAME_SPACE::bind(&CTimer::run, this, p, QTH_NAME_SPACE::placeholders::_1) );
}

void CTimer::SetProcessor( IProcessor* pProcessor )
{
	m_pProcessor->PostTask(QTH_NAME_SPACE::bind(&CTimer::doSetProcessor, this, pProcessor), IProcessor::HIGHT);
}

void CTimer::doSetProcessor( IProcessor* p )
{
	m_pProcessor = p;
}

IProcessor* CTimer::GetProcessor( void )
{
	return m_pProcessor;
}

void CTimer::doKillTimer( STimerObj* p )
{
	assert(p);
	p->~STimerObj();
	m_alloc.Deallocate(p);
}

CTimer::STimerObj* CTimer::findTimer(std::size_t nIDEvent)
{
	m_mutex.lock();
	TimerMap::iterator it = m_ts.find(nIDEvent);
	CTimer::STimerObj* pTimer = (it != m_ts.end()) ? it->second : NULL;
	m_mutex.unlock();

	return pTimer;
}

void CTimer::insertTimer(std::size_t nIDEvent, STimerObj* pTimer)
{
	m_mutex.lock();
	m_ts.insert(std::make_pair(nIDEvent, pTimer));
	m_mutex.unlock();
}

void CTimer::removeTimer(std::size_t nIDEvent)
{
	m_mutex.lock();
	m_ts.erase(nIDEvent);
	m_mutex.unlock();
}

END_NAMESPACE
