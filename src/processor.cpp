#include "stdafx.h"
#include "utility/processor.h"
#include "boost/bind.hpp"
#include <assert.h>


BEGIN_NAMESPACE


CProcessor::CProcessor( const char* pszIdentifier ): 
	m_strIdentifier(pszIdentifier), m_eState(STOPED)
{

}

CProcessor::~CProcessor( void )
{

}

const char* CProcessor::GetIdentifier( void ) const
{
	return m_strIdentifier.c_str();
}

bool CProcessor::PostTask( const FTask& task, ETaskPriority pri /*= NORMAL*/ )
{
	if (m_eState > PAUSE)
		return false;

	if (pri == HIGHT)
		m_hight.push(task);
	else if (pri == NORMAL)
		m_normal.push(task);
	else if (pri == LOW)
		m_low.push(task);
	else
		return false;

	if (m_eState == RUNNING)
		m_runCond.notify_one();

	return true;
}

bool CProcessor::SendTask(const FTask& task, size_t nWaitTime/* = -1*/)
{
	if (m_eState > RUNNING)
		return false;
	
	//发送任务的线程和处理器处理线程属于同一个线程，直接处理任务
	if (boost::this_thread::get_id() == m_threadID)
	{
		task();
		return true;
	}

	//入系统任务队列
	SSysTask* pSysTask = new SSysTask;
	pSysTask->eState = WAITING;
	pSysTask->task = task;
	m_sysQueue.push(pSysTask);

	//通告处理器线程，有任务可以执行
	m_runCond.notify_one();

	boost::unique_lock<boost::mutex> lock(pSysTask->stateMutex);
	if (pSysTask->eState != DEALED)			//如果任务尚未被处理， 则等待处理器线程处理任务
	{
		if (nWaitTime == (size_t)-1)
		{
			pSysTask->cond.wait(lock);
		}
		else
		{
			//等待指定时间， 然后返回
			pSysTask->cond.wait_for(lock, boost::chrono::seconds(nWaitTime));
		}

	}

	if (pSysTask->eState ==DEALED)
	{
		//处理线程处理完成任务
		lock.unlock();
		lock.release();
		delete pSysTask;
		return true;
	}

	//设置这个任务为超时状态， 这样处理线程就会将这个任务给抛弃掉(delete)
	pSysTask->eState = TIMEOUT;
	return false;
}

void CProcessor::Run( boost::promise<bool>& runPromise )
{
	assert(m_eState != RUNNING);
	m_threadID = boost::this_thread::get_id();
	m_eState = RUNNING;
	runPromise.set_value(true);
	boost::unique_lock<boost::mutex> lock(m_runMutex);
	while (m_eState != STOPED)
	{
		if (!RunOneSys())
		{
			RunAll();
		}

		m_runCond.wait_for(lock, boost::chrono::milliseconds(50));
	}
}

void CProcessor::InternalDealSysTask()
{
	assert(!m_sysQueue.empty() );
	SSysTask* pSysTask = m_sysQueue.front();
	m_sysQueue.pop();

	pSysTask->stateMutex.lock();

	if (pSysTask->eState == WAITING)
	{
		pSysTask->eState =DEALING;
		pSysTask->task();
		pSysTask->eState = DEALED;
		pSysTask->stateMutex.unlock();
		pSysTask->cond.notify_one();
	}
	else
	{
		//超时
		pSysTask->stateMutex.unlock();
		delete pSysTask;
	}

}

void CProcessor::InternalDealTask( RWQueue<FTask>& que )
{
	size_t size = que.size();
	for (size_t i= 0; i < size; ++i)
	{
		FTask& task = que.front();
		task();
		que.pop();	
	}
}

void CProcessor::Stop( void )
{
	if (m_eState < STOPPING)
	{
		m_eState = STOPPING;
		m_normal.push(boost::bind(&CProcessor::InternalStop, this) );
	}
}

void CProcessor::InternalStop( void )
{
	m_eState = STOPED;
}

bool CProcessor::RunOneSys( void )
{
	if (!m_sysQueue.empty())
	{
		InternalDealSysTask();
		return true;
	}

	return false;
}

bool CProcessor::RunAll( void )
{
	if (!m_hight.empty())
	{
		InternalDealTask(m_hight);
	}
	else if(!m_normal.empty())
	{
		InternalDealTask(m_normal);
	}
	else if (!m_low.empty())
	{
		InternalDealTask(m_low);
	}
	else
	{
		return false;
	}


	return true;
}



















END_NAMESPACE
