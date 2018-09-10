/*******************************
function:	单线程处理器
date:		2016/12/16
version:	v1.0
tips:		提供单个线程的处理
********************************/
#ifndef PROCESSOR_H_
#define PROCESSOR_H_
#include "utility/interface_processor.h"
#include "utility/rwqueue.h"
#include "utility/functional.h"

#include "boost/thread.hpp"


BEGIN_NAMESPACE

class CProcessor: public IProcessor
{
public:

	CProcessor(const char* pszIdentifier);
	~CProcessor(void);

	virtual const char* GetIdentifier(void) const;
	virtual bool PostTask(const FTask& task, ETaskPriority pri = NORMAL);
	virtual bool SendTask(const FTask& task, size_t nWaitTime = -1);
	virtual bool RunOneSys(void);
	virtual bool RunAll(void);
	//开始运行处理线程
	void Run(boost::promise<bool>& runPromise);
	//向处理线程投递一个结束运行的任务， 调用Stop 之前的任务处理完成后会停止处理器运行
	void Stop(void);
	//获取指定队列中任务的数量
	size_t GetQueueSize(ETaskPriority pri) const;
	//获取处理器所运行的线程ID
	boost::thread::id GetThreadID(void) const;
private:
	void InternalDealSysTask();		//处理一个系统任务
	void InternalDealTask(RWQueue<FTask>& que);	
	void InternalStop(void);
private:
	//处理器状态
	typedef		enum ENUM_STATE
	{
		RUNNING,		//处理器运行中
		PAUSE,			//处理器暂停运行
		STOPPING,		//正在停止
		STOPED,			//停止
	}EState;

	//任务状态
	typedef		enum ENUM_SYSFLAG
	{
		WAITING,		//等待处理
		DEALING,		//正在处理
		DEALED,			//处理完毕
		TIMEOUT,		//处理超时
	}ETaskState;
	
	// 阻塞执行的任务
	typedef		struct STRUCT_SYSTASK
	{
		FTask task;							
		ETaskState eState;
		boost::mutex stateMutex;
		boost::condition_variable cond;
	} SSysTask;


	std::string m_strIdentifier;			//处理器标识
	boost::thread::id  m_threadID;			//处理器运行所在线程ID
	volatile EState m_eState;				//处理器运行状态
	
	RWQueue<SSysTask*>  m_sysQueue;			// SendTask 处理队列
	RWQueue<FTask> m_hight;					//任务处理队列
	RWQueue<FTask> m_normal;	
	RWQueue<FTask> m_low;

	boost::condition_variable m_runCond;
	boost::mutex m_runMutex;


};

END_NAMESPACE
#endif

