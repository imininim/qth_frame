/*******************************
function:	tcp 服务 接口类
date:		2016/12/21
version:	v1.0
tips:		提供tcp服务的对外接口
********************************/
#ifndef INTERFACE_TCP_SERVER_H_
#define INTERFACE_TCP_SERVER_H_
#include "asio/tcp_connect_manager.h"

BEGIN_NAMESPACE
class CFrameWork;
class IProcessor;
class ITimer;
template<class T>
struct function;

class ITcpServer
{
public:
	//服务器状态
	typedef enum ENUM_STATE_SERVER
	{
		STOPED,			//停止		
		INITED,			//已经初始化,未运行
		RUNNING,		//运行状态
	}EServerState;

	//系统错误
	typedef		QTH_NAME_SPACE::function<void(int, std::string)>		FSysErrorHandle;

	virtual ~ITcpServer(){};
	virtual CFrameWork* GetFrame()	 = 0;
	
	//获取服务器状态
	virtual const EServerState& GetState()  const = 0;
	//获取服务器标识名称
	virtual const std::string& GetSvrName() const = 0;
	//获取服务器的主处理线程
	virtual IProcessor* GetLogicalProcessor() = 0;
	//获取服务器定时器模块
	virtual ITimer*		GetTimer()			  = 0;
	
	/*
		name:		SetTimer
		function:	设置一个定时器
		params:		
			nIDEvent	定时器ID， 唯一标识一个定时器
			nElaspse	定时间隔（毫秒）
			nRunTimes	定时器回调次数， -1 表示无限次数
	*/
	virtual bool SetTimer(std::size_t nIDEvent, std::size_t nElaspse,void* pParam = NULL, int nRunTimes = -1) = 0;
	
	
	/*
		name:		KillTimer
		function:	清除一个定时器
		params:
			nIDEvent	定时器ID， 唯一标识一个定时器
	*/
	virtual void KillTimer(std::size_t nIDEvent)										 = 0;

	/*
		name:		ConnectServer
		function:	连接其他服务
		params:
				strIP		服务器IP地址
				nPort		服务器端口
		return:
				返回指定连接的指针， 有可能为空, 判断连接建立成功的例子如下:
				ConnectPtr p = ConnectServer("127.0.0.1", 11234);
				if (p && p->isOpen())
					连接成功...
				else if(p == NULL)
					内存分配失败...
				else
					cout << "错误码: " << p->GetErrorCode() << "错误信息:" << p->GetErrorInfo().c_str() << endl;


	*/
	virtual ConnectPtr ConnectServer(const std::string& strIP, unsigned short nPort)	= 0;
	/*
		这些接口用户不应该使用
	*/
	virtual bool init()				 = 0;
	virtual void uinit()			 = 0;
	virtual void run()				 = 0;
		
};
END_NAMESPACE
#endif
