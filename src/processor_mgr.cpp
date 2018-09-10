#include "stdafx.h"
#include "utility/processor_mgr.h"
#include "utility/processor.h"

BEGIN_NAMESPACE

CProcessorMgr::CProcessorMgr()
{
	m_pMutex = new ::boost::mutex;
}

CProcessorMgr::~CProcessorMgr()
{
	shutDown();

	if (m_pMutex)
	{
		delete m_pMutex;
	}	
}



IProcessor* CProcessorMgr::AllocProcessor( const char* pszIdentifier )
{
	IProcessor* pProcessor = NULL;
	boost::lock_guard<boost::mutex> lock(*m_pMutex);

	ProIter itProInfo = m_mapProcessors.find(pszIdentifier);
	if (itProInfo != m_mapProcessors.end())
	{
		SProcInfo& procInfo = itProInfo->second;
		pProcessor = procInfo.pProcessor;
		++procInfo.nRefCount;
	}
	else
	{
		SProcInfo proInfo;
		proInfo.nRefCount = 1;
		proInfo.pProcessor = new CProcessor(pszIdentifier);
		boost::promise<bool> runPromise;
		proInfo.pThread = new boost::thread(boost::bind(&CProcessorMgr::StartProcessor, this, QTH_NAME_SPACE::ref(proInfo), QTH_NAME_SPACE::ref(runPromise) ) );
		m_mapProcessors.insert(std::make_pair(pszIdentifier, proInfo) );
		pProcessor = proInfo.pProcessor;
		runPromise.get_future().wait(); //等待处理线程开始后，此函数才返回
	}

	return pProcessor;
}

void CProcessorMgr::StartProcessor( const SProcInfo& procInfo, boost::promise<bool>& runPromise )
{
	procInfo.pProcessor->Run(runPromise);
}

void CProcessorMgr::FreeProcessor( IProcessor* pProcessor )
{
	assert(pProcessor);
	boost::lock_guard<boost::mutex> lock(*m_pMutex);
	ProIter itProcInfo = m_mapProcessors.find(pProcessor->GetIdentifier() );
	if (itProcInfo != m_mapProcessors.end())
	{
		SProcInfo& procInfo = itProcInfo->second;
		--procInfo.nRefCount;

		if (procInfo.nRefCount == 0)
		{
			procInfo.pProcessor->Stop();
			procInfo.pThread->join();
			delete procInfo.pProcessor;
			delete procInfo.pThread;
			m_mapProcessors.erase(itProcInfo);
		}
	}
}

void CProcessorMgr::FreeProcessor( const char* pszIdentifier )
{
	assert(pszIdentifier);
	boost::lock_guard<boost::mutex> lock(*m_pMutex);
	ProIter itProcInfo = m_mapProcessors.find(pszIdentifier);
	if (itProcInfo != m_mapProcessors.end())
	{
		SProcInfo& procInfo = itProcInfo->second;
		--procInfo.nRefCount;

		if (procInfo.nRefCount == 0)
		{
			procInfo.pProcessor->Stop();
			procInfo.pThread->join();
			delete procInfo.pProcessor;
			delete procInfo.pThread;
			m_mapProcessors.erase(itProcInfo);
		}
	}
}


void CProcessorMgr::shutDown( void )
{
	boost::lock_guard<boost::mutex> lock(*m_pMutex);
	for (ProIter it = m_mapProcessors.begin(); it != m_mapProcessors.end(); ++it)
	{
		SProcInfo& procInfo = it->second;
		procInfo.pProcessor->Stop();
		procInfo.pThread->join();
		delete procInfo.pProcessor;
		delete procInfo.pThread;
	}

	m_mapProcessors.clear();
}
END_NAMESPACE

