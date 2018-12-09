/*******************************
function:	连接关系型数据库的接口
date:		2018/12/07
version:	v1.0
tips:		提供 mysql 数据库客户端接口
********************************/

#ifndef __mysql_databases_h__
#define __mysql_databases_h__

#include "db/interface_sql_databases.h"
#include "mysql/mysql.h"

BEGIN_NAMESPACE
class Mysql: public ISQLDataBases {
public:
	virtual ~Mysql();
	//初始化数据库账号密码
	virtual void Init(const std::string &host, const std::string &username, const std::string &passwd, int port, const std::string &database);
	//连接数据库
	virtual bool Connect();
	//是否已连接
	virtual bool IsConnect();
	//插入数据
	virtual bool Insert(const std::string &query);
	//获取数据
	virtual bool Select(const std::string &query, std::vector<std::vector<std::string> > &data);
private:
	std::string m_host;
	std::string m_username;
	std::string m_passwd;
	int m_port;
	std::string m_database;
	MYSQL m_mysql;
};

END_NAMESPACE
#endif
