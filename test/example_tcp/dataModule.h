/*
	function: 用户数据模块
	提供用户账户信息，
*/
#ifndef DATAMODULE_H_
#define DATAMODULE_H_
#include "frame/running_class_info.h"
#include "utility/single.h"

struct SData
{
	SData() {}

	std::string account; 	//账号
	std::string passwd;		//密码
	std::string path;		//目录所在路径
};


//用户数据配置
class CUserDataConfig: public QTH_NAME_SPACE::Singleton<CUserDataConfig>
{
	friend class QTH_NAME_SPACE::Singleton<CUserDataConfig>;
public:
	CUserDataConfig();
	~CUserDataConfig();
	
	//配置文件加载/保存
	void loadConfig();
	void saveConfig();
	//查找用户配置
	SData* findAccount(const std::string& acc);
	bool insertAccount(SData& acc);

private:
	typedef 	std::map<std::string, SData> 	userAccountMap; //<用户名, 密码>
	
	userAccountMap m_accounts;

};

//用户数据
class CUserData: public Module
{
public: //共用操作
	CUserData();
	~CUserData();

	inline void set_account(const std::string& acc)		{ m_strAccount = acc;}
	inline void set_name(const std::string& name)		{ m_name = name;}
	inline void set_pwd(const std::string& pwd)   		{ m_strPwd = pwd;}
	inline const std::string& get_account(void) const 	{ return m_strAccount;}
	inline const std::string& get_name(void) const 		{ return m_name;}
	inline const std::string& get_pwd(void) const 		{ return m_strPwd;}
public:  //协议
	
	//测试协议
	int test(void*, int, ESendMethor);
	//是否已经登录, true 已经登录， false 尚未登录
	int hasLogin(void*, int, ESendMethor);
	//进行登录操作
	int doLogin(void*, int, ESendMethor);
	//新建用户
	int doCreateAccount(void*, int, ESendMethor);
	QTH_MODULE_DECLARE(CUserData)
private:
	std::string m_strAccount;	//用户账户
	std::string m_strPwd;		//用户密码
	std::string m_name;			//用户名
};

#endif
