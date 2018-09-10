/*******************************
function:	定时器
date:		2016/12/30
version:	v1.0
tips:		
********************************/
#ifndef		TIMER_H_
#define		TIMER_H_
#include "asio/interface_timer.h"
#include "boost/asio.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/atomic.hpp"
#include"boost/thread/mutex.hpp"
#include "utility/interface_processor.h"

BEGIN_NAMESPACE
	class CTimer: public ITimer
	{	
	public:
		//定时器数据结构
		struct STimerObj
		{
			STimerObj(boost::asio::io_service& io, std::size_t ID, std::size_t nTime)
				:nIDEvent(ID),t(io, boost::posix_time::milliseconds(nTime) )
			{}

			std::size_t nIDEvent;				//定时器ID
			std::size_t nElaspse;				//定时器延迟
			boost::atomic_int nRunTimes;		//总回调次数， -1 表示无限运行
			boost::asio::deadline_timer t;		//定时器
			FCallBack callback;					//定时器执行的任务
		};

		typedef		std::map<std::size_t, STimerObj*>	TimerMap;		//定时器集合	
	public:
		CTimer(boost::asio::io_service& io, IProcessor* pProcessor);
		virtual ~CTimer();

		/*
			nIDEvent		定时器ID
			nElaspse		毫秒数定时器
			nRunTimes		回调次数
		*/
		virtual bool SetTimer(std::size_t nIDEvent, std::size_t nElaspse, const FCallBack& callback,int nRunTimes);
		virtual void KillTimer(std::size_t nIDEvent);
		virtual void SetProcessor(IProcessor* pProcessor);
		virtual IProcessor* GetProcessor(void);

	private:
		void doKillTimer(STimerObj* p);
		void doSetProcessor(IProcessor* p);
		void run(STimerObj* p, const boost::system::error_code& e);

		STimerObj* findTimer(std::size_t nIDEvent);
		void insertTimer(std::size_t nIDEvent, STimerObj* pTimer);
		void removeTimer(std::size_t nIDEvent);

		boost::asio::io_service& m_ios;
		IProcessor* m_pProcessor;				//定时器所用的
		FixedAllocator m_alloc;					//分配器
		boost::mutex	m_mutex;
		TimerMap m_ts;							//存储所有被设置的定时器
	};


END_NAMESPACE
#endif
