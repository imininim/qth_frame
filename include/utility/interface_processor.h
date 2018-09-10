/*******************************
function:	单线程处理器接口
date:		2016/12/14
version:	v1.0
tips:		提供对外部使用的线程处理接口
********************************/
#ifndef INTERFACEPROCESSOR_H_
#define INTERFACEPROCESSOR_H_
BEGIN_NAMESPACE

//短式声明
template<class R>
struct function;

class IProcessor
{
public:
	//投递任务类型
	typedef function<void(void)> FTask;		

	//线程投递任务的优先级， 级别越高的，越先处理
	typedef enum ENUM_TASK_PRORITY
	{
		LOW,
		NORMAL,
		HIGHT
	}ETaskPriority;

public:
	virtual ~IProcessor(){}


	//获取处理器的标识符, 唯一的标识了一个线程处理器
	virtual const char* GetIdentifier(void) const = 0;

	//非阻塞的形式投递任务给线程处理
	virtual bool PostTask(const FTask& task, ETaskPriority pri = NORMAL) = 0;

	//投递任务给线程处理， 该线程会优先处理此任务，并在处理任务完成后才返回
	virtual bool SendTask(const FTask& task, size_t nWaitTime = -1) = 0;

	//处理系统队列中的一个任务
	virtual bool RunOneSys(void) = 0;

	//处理非系统队列中的所有任务
	virtual bool RunAll(void) = 0;
};

END_NAMESPACE
#endif
