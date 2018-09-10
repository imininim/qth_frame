/*
	测试 日志
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//step2 日志头文件
#include "utility/logger.h"

int main(int argc, char* argv[])
{
	/*
		1.  日志的配置文件名称为 log4cpp.logConfig。 使用日志模块会加载此配置文件， 此配置文件记录日志文件生成目录， 单个日志文件的大小，最多保存多少个日志文件等信息
		2.  如果日志模块没有成功加载 日志配置文件， 则将日志信息输出到cout 上
		3.  你可以在 项目根目录/src  找到log4cpp.logConfig 配置文件，里面是已经配置好的，直接拷贝到你的程序运行目录即可使用
	*/
	using namespace QTH_NAME_SPACE;
	LOG_DEBUG("haha %s", "hehe");	//调试信息级别日志输出
	LOG_INFO("haha %s", "hehe");	//参数信息级别日志输出
	LOG_WARN("haha %s", "hehe");	//警告信息级别日志输出
	LOG_ERROR("haha %s", "hehe");	//错误信息级别日志输出
	LOG_FATAL("haha %s", "hehe");	//不可恢复性级别日志输出
	
	//设置配置文件的信息
	LOG_PATH_INIT("../../doc/log4cpp.logConfig");
	LOG_DEBUG("after reload");

	getchar();
	return 0;
}
