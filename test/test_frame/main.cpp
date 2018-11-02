/***************************
	测试 模块化,动态创建， 消息自动派发

****************************/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
//模块管理器头文件
#include "frame/module_manager.h"

#include "common/property.h"

int main(int argc, char* argv[])
{
	{
		using namespace QTH_NAME_SPACE;
				
		IModuleManager* pManager = new ModuleManager(NULL);
		if (!pManager) return -1;
		
		pManager->Init("../../doc/modules.xml");
		//打印搜集的模块信息
		pManager->print();

		////根据用户输入 进行消息派发
		char pszCmd[256] = {};
		cout << "请输入协议(模块标识,消息号): ";
		while (std::cin.getline(pszCmd, 256))
		{
			std::string strCmd(pszCmd), first_str,second_str;
			CmdParse cmd;

			M_TYPE moduleID = 0;
			MSG_TYPE msgID = 0;

			if (cmd.parse(strCmd))
			{
				moduleID = atoi(cmd["module"].c_str());  
				msgID = atoi(cmd["msg"].c_str());  

				//分派消息
				pManager->dispatchMessage(moduleID, msgID, NULL, 0); 

				//回过头来看看这条协议的完整形式
				std::string test = cmd.reverse();
			}
			else 
			{
				cout << "协议格式错误, 要求格式为:module=xxx msg=xxx" << endl;
			}
			

			//继续接受下一条协议
			memset(pszCmd, 0, 256);
			cout << endl << "请输入协议(模块标识,消息号): ";
		}//end while

		SAFE_DELETE(pManager);
	}
	
	getchar();
	return 0;
}

