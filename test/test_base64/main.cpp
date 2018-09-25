/*
	测试 base64编码解码
*/

//step1 动态库公用头文件
#include "utility/utility.h"
using std::cout;
using std::endl;
using std::string;
//base64编码解码
#include "common/base64.h"

void printBase64(const string& message) {
	std::string encd = QTH_NAME_SPACE::base64_encode(message).c_str();
	cout << "base64=" << encd.c_str() << endl;
	cout << "src=" << QTH_NAME_SPACE::base64_decode(encd).c_str() << endl << endl;
}


int main(int argc, char* argv[])
{
	printBase64("");
	printBase64("abc");
	printBase64("message digest");
	printBase64("abcdefghijklmnopqrstuvwxyz");
	printBase64("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	getchar();
	return 0;
}
