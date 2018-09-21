/*
	测试 md5加密解密
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
using std::string;
//step2 xml 头文件
#include "common/md5.h"

void printMD5(const string& message) {
	cout << "md5(\"" << message << "\") = "
		<< QTH_NAME_SPACE::MD5(message).toStr() << endl;
}

int main(int argc, char* argv[])
{
	printMD5("a");
	printMD5("abc");
	printMD5("message digest");
	printMD5("abcdefghijklmnopqrstuvwxyz");
	printMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	
	getchar();
	return 0;
}
