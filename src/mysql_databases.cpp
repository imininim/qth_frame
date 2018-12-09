#include "stdafx.h"
#include "db/mysql_databases.h"


BEGIN_NAMESPACE
Mysql::~Mysql() {

}
//初始化数据库账号密码
void Mysql::Init(const std::string &host, const std::string &username, const std::string &passwd, int port, const std::string &database) {
	m_host = host;
	m_username = username;
	m_passwd = passwd;
	m_port = port;
	m_database = database;
}
//连接数据库
bool Mysql::Connect() {
	mysql_library_init(0, NULL, NULL);
	mysql_init(&m_mysql);
	mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "utf8");

	if (!mysql_real_connect(&m_mysql, m_host.c_str(), m_username.c_str(), m_passwd.c_str(), m_database.c_str(), m_port, NULL, 0)) {
		return false;
	}
	return true;
}
//是否已连接
bool Mysql::IsConnect() {
	return true;
}
//插入数据
bool Mysql::Insert(const std::string &query) {
	if (!mysql_query(&m_mysql, query.c_str())) {
		return false;
	}
	return true;
}
//获取数据
bool Mysql::Select(const std::string &query, std::vector<std::vector<std::string> > &data) {
	if (!mysql_query(&m_mysql, query.c_str())) {
		return false;
	}
 
	MYSQL_RES *result = mysql_store_result(&m_mysql);
 
	// 行列数
	int row = mysql_num_rows(result);
	int field = mysql_num_fields(result);
	MYSQL_ROW line = mysql_fetch_row(result);
 
	std::vector<std::string> tmp;
	tmp.reserve(field);
	while (line) {
		tmp.clear();
		for (int i = 0; i < field; ++i) {
			if (line[i]) {
				tmp.push_back(line[i]);
			}
			else {
				tmp.push_back("");
			}
		}
		line = mysql_fetch_row(result);
		data.push_back(tmp);
	}
	return true;
}

END_NAMESPACE
