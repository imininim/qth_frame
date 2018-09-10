/****************************
Author:			屈天豪
date:			2016/9/10
Version:		v2.0
Descript:		日志包装类
Others:			日志的配置文件默认为 "./log4cpp.logConfig" , 如果没有找到，则默认输出目的地为 std::cout， 否则为配置文件中所指定的文件路径。
*****************************/
#ifndef LOGGER_H_
#define LOGGER_H_
#include "utility/single.h"


#include <stdarg.h>
#include<vector>

namespace log4cpp
{
	class Category;
};



BEGIN_NAMESPACE
class CLogger;
extern QTH_UTILITY_EXPORT CLogger g_logger;

//日志路径设置
#define LOG_PATH_INIT(path) \
	QTH_NAME_SPACE::Singleton<QTH_NAME_SPACE::CLogger, 999>::Instance().LoadConfig(path);

//日志输出宏
#define LOG_FATAL(msg, ...)				__LOGGER__(Fatal, msg, ##__VA_ARGS__)
#define LOG_ALERT(msg, ...)				__LOGGER__(Alert, msg, ##__VA_ARGS__)
#define LOG_CRIT(msg, ...)				__LOGGER__(Crit, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)				__LOGGER__(Error, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...)				__LOGGER__(Warn, msg, ##__VA_ARGS__)
#define LOG_NOTICE(msg, ...)			__LOGGER__(Notice, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)				__LOGGER__(Info, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...)				__LOGGER__(Debug, msg, ##__VA_ARGS__)

//内部系统日志输出，用于动态库错误和调试信息，外部不要使用
#define _LOG_FATAL(obj, msg, ...)		__INTER_LOGGER__(obj, Fatal, msg, ##__VA_ARGS__)
#define _LOG_ALERT(obj, msg, ...)		__INTER_LOGGER__(obj, Alert, msg, ##__VA_ARGS__)
#define _LOG_CRIT(obj,  msg, ...)		__INTER_LOGGER__(obj, Crit, msg, ##__VA_ARGS__)
#define _LOG_ERROR(obj, msg, ...)		__INTER_LOGGER__(obj, Error, msg, ##__VA_ARGS__)
#define _LOG_WARN(obj,  msg, ...)		__INTER_LOGGER__(obj, Warn, msg, ##__VA_ARGS__)
#define _LOG_NOTICE(obj,msg, ...)		__INTER_LOGGER__(obj, Notice, msg, ##__VA_ARGS__)
#define _LOG_INFO(obj,  msg, ...)		__INTER_LOGGER__(obj, Info, msg, ##__VA_ARGS__)
#define _LOG_DEBUG(obj, msg, ...)		__INTER_LOGGER__(obj, Debug, msg, ##__VA_ARGS__)


#define __LOGGER__(out_level, msg, ...) \
	QTH_NAME_SPACE::Singleton<QTH_NAME_SPACE::CLogger, 999>::Instance().out_level(msg, ##__VA_ARGS__);

#define __INTER_LOGGER__(obj, out_level, msg, ...) \
	obj.out_level(msg, ##__VA_ARGS__);


#define _LOG_PATH_INIT(path)\
	QTH_NAME_SPACE::g_logger.LoadConfig(path);

class  QTH_UTILITY_EXPORT CLogger
{
public:	//输出不同级别的日志
	virtual void Fatal(const char* pszFormat, ...);
	virtual void Alert(const char* pszFormat, ...);
	virtual void Crit(const char* pszFormat, ...);
	virtual void Error(const char* pszFormat, ...);
	virtual void Warn(const char* pszFormat, ...);
	virtual void Notice(const char* pszFormat, ...);
	virtual void Info(const char* pszFormat, ...);
	virtual void Debug(const char* pszFormat, ...);

	int LoadConfig(const char* pszConfig);

public:
	CLogger(const char* pszConfig = NULL);
	virtual ~CLogger(void);
protected:
	CLogger(const CLogger&);
	void operator=(const CLogger&);
private:
	std::vector<log4cpp::Category*> m_vCatogory;
};


END_NAMESPACE
#endif

