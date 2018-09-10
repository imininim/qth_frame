/*
	上传下载服务器
*/
#include <iostream>
#include "frameWork.h"

int main(int argc, char* argv[])
{
	using namespace QTH_NAME_SPACE;

	std::cout << "请输入监听端口号: ";
	char buf[256] = {0};
	std::cin.getline(buf, 256);
	std::cout << std::endl;

	CMyFrameWork frame("FileStore", atoi(buf));
	frame.main();
	getchar();
	return 0;
}
