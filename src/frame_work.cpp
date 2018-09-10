#include "stdafx.h"
#include "asio/frame_work.h"
#include "asio/tcp_server.h"
#include "utility/processor.h"

#include <iostream>
BEGIN_NAMESPACE


CFrameWork::CFrameWork( const std::string& svrName, unsigned short nPort, int numHandleThread /*= 2*/, int max_obj_size)
{
	m_pSvrHandle = new CTcpServer(svrName, this, nPort, numHandleThread, max_obj_size);
	assert(m_pSvrHandle);
}

void CFrameWork::main(bool useCmd/* = false*/)
{
	if (!m_pSvrHandle->init())
		return; //初始化服务器失败
		
	if (useCmd) //以命令行方式启动服务
	{
		boost::thread thCmd(boost::bind(&CFrameWork::doCmd, this) );

		//开始等待连接
		m_pSvrHandle->run();

		//结束运行
		m_pSvrHandle->uinit();

		using namespace std;
		cin.setstate(std::ios::eofbit);
		thCmd.join();
	}
	else	//非命令行启动
	{
		//开始等待连接
		m_pSvrHandle->run();
		//结束运行
		m_pSvrHandle->uinit();
	}		
}

void CFrameWork::doCmd()
{
	using namespace std;
	char buffer[256] = {};
	cout << ">";

	while (cin.getline(buffer, 256))
	{
		if (m_pSvrHandle->GetLogicalProcessor())
		{
			size_t size = (size_t)cin.gcount();
			m_pSvrHandle->GetLogicalProcessor()->SendTask(boost::bind(&CFrameWork::OnCommond, this, buffer,  size) );
		}
		cout << ">";
	}
	
	cin.clear(std::ios::eofbit);
}

CFrameWork::~CFrameWork()
{
	if (m_pSvrHandle)
		delete m_pSvrHandle;
}

END_NAMESPACE
