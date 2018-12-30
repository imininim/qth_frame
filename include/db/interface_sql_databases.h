/*******************************
function:	连接关系型数据库的接口
date:		2018/12/07
version:	v1.0
tips:		提供sql 数据库客户端接口
********************************/

#ifndef __interface_sql_databases_h__
#define __interface_sql_databases_h__

BEGIN_NAMESPACE
class ISQLDataBases {
public:
	virtual ~ISQLDataBases() {}

	//初始化数据库账号密码
	virtual void Init(const std::string &host, const std::string &username, const std::string &passwd, int port, const std::string &database) = 0;
	//连接数据库
	virtual bool Connect()                                                                                      = 0;
	//是否已连接
	virtual bool IsConnect()                                                                                    = 0;
	//插入数据
	virtual bool Insert(const std::string &query)                                                               = 0;
	//获取数据
	virtual bool Select(const std::string &query, std::vector<std::vector<std::string> > &data)                 = 0;
};

END_NAMESPACE
#endif
