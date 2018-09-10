#include "utility/utility.h"
#include "dataModule.h"
#include "frame/interface_user.h"
#include "utility/logger.h"
#include "common/property.h"
#include "frame/module_manager.h"
#include "xml/tinyxml2.h"

//#include <dir.h>
using namespace QTH_NAME_SPACE;

QTH_MODULE_IMPLEMENT(CUserData, Module)
	QTH_ON_MESSAGE(CUserData, 0, &CUserData::test)				//测试协议
	//QTH_ON_MESSAGE(CUserData, 1, &CUserData::hasLogin)			//登录检测协议
	QTH_ON_MESSAGE(CUserData, 2, &CUserData::doCreateAccount)  	//新建用户
	QTH_ON_MESSAGE(CUserData, 3, &CUserData::doLogin)			//登录
CUserData::CUserData()
{
	//加载配置，配置只会加载一次
	CUserDataConfig::Instance().loadConfig();
	
}

CUserData::~CUserData()
{
	
}

//测试协议 0
int CUserData::test(void*, int, ESendMethor)
{
	std::cout << "这是一条测试协议!" << std::endl;
	
	std::string str = "你好服务器，这是来自 模块 CUserData 的一条问候!";
	IUser* pUser = GetManager()->GetUser();
	if (pUser)
		pUser->SendMsgToClient(str.c_str(), str.size());

	return 0;
}

//是否已经登录?
int CUserData::hasLogin(void*, int, ESendMethor methor)
{
	if (methor == SEND_BY_CLIENT)
		return 0; //这个消息只能内部使用，客户端调用的话直接返回


	if (m_strAccount.empty() || m_strPwd.empty())
		return 0; //账户数据为空，尚未登录

	return 1; //已经登录
}

//登录操作
int CUserData::doLogin(void* p, int len, ESendMethor methor)
{
	if (!p || len == 0)
		return -1;
	 
	CmdParse& cmd  = *(CmdParse*)p;
	
	 std::string strAccount = cmd["account"];
	 std::string strPwd = cmd["passwd"];
	 
	std::string strSendMsg;
	SData* pAccountInfo = CUserDataConfig::Instance().findAccount(strAccount);
	if(pAccountInfo && strAccount == pAccountInfo->account && strPwd == pAccountInfo->passwd)
	{
		strSendMsg = "密码正确，登录成功!";
		
		set_account(strAccount);
		set_pwd(strPwd);
	}
	else if(pAccountInfo)
		strSendMsg = "密码错误,登录失败!";
	else
		strSendMsg = "无效的账号, 登录失败!";


	GetManager()->GetUser()->SendMsgToClient(strSendMsg.c_str(), strSendMsg.size());	
	return 0;
}

int CUserData::doCreateAccount(void* p, int len, ESendMethor methor)
{
	if (!p || len == 0) return -1;

	std::string sendCmd;
	CmdParse& cmd  = *(CmdParse*)p;

	std::string strAccount = cmd["account"];
	std::string strPwd = cmd["passwd"];

	SData* pAccountInfo = CUserDataConfig::Instance().findAccount(strAccount);
	if (pAccountInfo)
	{
		sendCmd = "账户已经存在，无法重复创建!";
	}
	else
	{
		SData temp;
		temp.account = strAccount;
		temp.passwd = strPwd;
		if (!CUserDataConfig::Instance().insertAccount(temp))
		{
			CUserDataConfig::Instance().saveConfig();
			set_account(strAccount);
			set_pwd(strPwd);
			sendCmd = "创建账户成功";
		}
		else
		{
			sendCmd = "创建账户失败，无法创建该账户的目录!";
		}
			

	}
	GetManager()->GetUser()->SendMsgToClient(sendCmd.c_str(), sendCmd.size() );
	return 0;
}

CUserDataConfig::CUserDataConfig()
{

}

CUserDataConfig::~CUserDataConfig()
{
	saveConfig();
}
	
//账号文件加载
void CUserDataConfig::loadConfig()
{
	if (!m_accounts.empty()) return; //不重复加载

	QTH_NAME_SPACE::XMLDocument doc;
	XMLError errCode = doc.LoadFile("./userAccount.xml");
	 if (errCode != XML_SUCCESS)
	 {
		 LOG_ERROR("[配置加载] 加载配置文件 userAccount.xml 失败!");
		 return;
	 }

	 //获取根节点
	 XMLElement* root = doc.RootElement();
	 if (!root)
	 {
		 LOG_ERROR("[配置加载] 加载配置文件失败, 无法获取到根节点!")
		 return;
	 }
	
 	XMLElement* item = root->FirstChildElement("item");
	while (item)
	{
		SData temp;
		temp.account = std::string(item->Attribute("account"));
		temp.passwd = std::string(item->Attribute("passwd"));
		temp.path = std::string(item->Attribute("path"));

		if (!temp.account.empty() && !temp.passwd.empty() && !temp.path.empty()) 
			m_accounts.insert(std::make_pair(temp.account, temp));

		item = item->NextSiblingElement("item");
	}	
}

//账号文件存储
void CUserDataConfig::saveConfig()
{
	QTH_NAME_SPACE::XMLDocument doc;
 	 XMLError errCode = doc.LoadFile("./userAccount.xml");
	 if (errCode != XML_SUCCESS)
	 {
		 LOG_ERROR("[配置加载] 加载配置文件 userAccount.xml 失败!");
		 return;
	 }
	
	doc.Clear();
	
	XMLElement* root = doc.NewElement("root");

	for (userAccountMap::iterator it = m_accounts.begin(); it != m_accounts.end(); ++it)
	{
		XMLElement* item = doc.NewElement("item");
		if (item)
		{
			item->SetAttribute("account", it->second.account.c_str());
			item->SetAttribute("passwd", it->second.passwd.c_str());
			item->SetAttribute("path", it->second.path.c_str());
			
			root->InsertEndChild(item);
		}
	}
	
	doc.InsertEndChild(root);
	doc.SaveFile("./userAccount.xml");
}

SData* CUserDataConfig::findAccount(const std::string& acc)
{
	userAccountMap::iterator it = m_accounts.find(acc);
	if (it == m_accounts.end())
		return NULL;

	return &it->second;
}

bool CUserDataConfig::insertAccount(SData& acc)
{
	if (acc.account.empty() || acc.passwd.empty())
		return false;
	
//	char pathBuffer[256] = {};
//	getcwd(pathBuffer, sizeof(pathBuffer));
//	acc.path = std::string(pathBuffer);
//	acc.path += "/";
//	acc.path += acc.account;
//	
//	//创建目录
//	/*int dir_exist = 0;
//	if (_access(acc.path.c_str(), 0) == -1)
//		dir_exist = _mkdir(acc.path.c_str());
//	
//	if (dir_exist != 0)
//		return false;
//*/
	m_accounts.insert(std::make_pair(acc.account, acc));
	return true;
}
