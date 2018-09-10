#include "user.h"
#include "frame/module_manager.h"
#include "asio/interface_tcp_connect.h"

using namespace QTH_NAME_SPACE;

StoreUser::StoreUser(ITcpConnect* pConnect)
{
	assert(pConnect != NULL);
	m_pConnect = pConnect;
	m_pModuleManager = new ModuleManager(this);
	assert(m_pModuleManager != NULL);

	m_id = 0;
	//m_bAccountLogin = IUser::STATE_LOGOUT;
}

StoreUser::~StoreUser()
{
	SAFE_DELETE(m_pModuleManager);
}
void StoreUser::SendMsgToClient(const char* pData, size_t len)
{ 
	if (m_pConnect)
		m_pConnect->SendMsg(pData, len);
}

