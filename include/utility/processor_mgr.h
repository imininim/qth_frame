/*******************************
function:	线程处理器管理器
date:		2016/12/17
version:	v1.0
tips:		提供对于线程和任务的管理功能
			
			提供分配接口， 可以获取一个线程处理器， 该处理器自动绑定到一个额外分配的线程上，所有任务的处理都是在这个额外的线程上处理
			提供释放接口， 可以释放一个线程处理器， 停止该处理器的运行，并且回收线程资源
使用
		CProcessorMgr::getInstance().AllocProcessor("用户自定义处理器标识符"); //分配一个新的处理器， 他会开启一个新的线程
*******************************************/
#ifndef PROCESSOR_MGR_H_
#define PROCESSOR_MGR_H_
#include "utility/single.h"
#include "utility/nocopyable.h"
#include "utility/interface_processor.h"
namespace  boost
{
	class thread;
	class mutex;
	template<class> class promise;
}

BEGIN_NAMESPACE
//短式声明
class CProcessor;


class QTH_UTILITY_EXPORT CProcessorMgr: 
	public Singleton<CProcessorMgr, 0, QTH_NAME_SPACE::CreateUsingNew<CProcessorMgr>, ::Loki::ClassLevelLockable>, private Nocopyable 
{
	friend class Singleton<CProcessorMgr>;
public:
	CProcessorMgr(void);
	~CProcessorMgr(void);

	//分配一个处理器
	IProcessor* AllocProcessor(const char* pszIdentifier);
	//释放一个处理器
	void FreeProcessor(IProcessor* pProcessor);
	//释放一个处理器，通过处理器标示
	void FreeProcessor(const char* pszIdentifier);

	//关闭所有处理器，同时释放所有的线程资源
	void shutDown(void);

private:
	typedef		struct STRUCT_PROC_INFO
	{
		size_t nRefCount;			//处理器被引用次数
		CProcessor* pProcessor;		//处理器
		boost::thread* pThread;		//处理器在这个线程上进行处理

	}SProcInfo;

	void StartProcessor(const SProcInfo& procInfo, boost::promise<bool>& runPromise);

private:
	typedef	std::map<std::string, SProcInfo>	ProcessorMap;
	typedef	ProcessorMap::iterator				ProIter;

	boost::mutex* m_pMutex;
	ProcessorMap m_mapProcessors;
};

END_NAMESPACE
#endif

