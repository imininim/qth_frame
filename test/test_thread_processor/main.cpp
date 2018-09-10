/*
	测试 线程处理器
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 线程处理器头文件
#include "utility/processor_mgr.h"

//这个是为了使用 QTH_NAME_SPACE::bind 
#include "utility/functional.h"


//全局互斥锁
Loki::Mutex g_mutex;


void thread1(int i)
{
	g_mutex.Lock();
	cout << "thread 1 end : " << i  << endl;
	g_mutex.Unlock();
}

void thread2(int i)
{
	g_mutex.Lock();
	cout << "thread 2 end : " << i  << endl;
	g_mutex.Unlock();
}

void thread3(const std::string& str)
{
	cout << "thread 3 end: " << str.c_str() << endl;
}

int main(int argc, char* argv[])
{
	//分配线程1
	QTH_NAME_SPACE::IProcessor* pProcessor1 = QTH_NAME_SPACE::CProcessorMgr::Instance().AllocProcessor("test1");
	assert(pProcessor1);

	//分配线程2
	QTH_NAME_SPACE::IProcessor* pProcessor2 = QTH_NAME_SPACE::CProcessorMgr::Instance().AllocProcessor("test2");
	assert(pProcessor2);

	//投递一个阻塞任务，该任务执行完成后才返回
	pProcessor1->SendTask(QTH_NAME_SPACE::bind(thread3, std::string("阻塞任务")) ); 

	//分别投递1000个任务到线程处理器1 和 2
	for (int i = 1; i <= 1000; ++i)
	{
		pProcessor1->PostTask(QTH_NAME_SPACE::bind(thread1, i) );
		pProcessor2->PostTask(QTH_NAME_SPACE::bind(thread2, i) );	
	}

	//释放处理线程， 会等到所有之前投递的任务完成后，回收线程资源，并且销毁处理器
	QTH_NAME_SPACE::CProcessorMgr::Instance().FreeProcessor(pProcessor2);
	QTH_NAME_SPACE::CProcessorMgr::Instance().FreeProcessor(pProcessor1);
	getchar();
	return 0;
}