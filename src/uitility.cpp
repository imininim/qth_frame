// xdn_uitility.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//windows 平台下的自动链接动态库
#ifdef _WIN32
	#ifndef XDN_UTILITY_NO_LIB
			#define XDN_UTILITY_NO_LIB
			#pragma comment(lib, "qthserver.lib")
	#endif
#endif

BEGIN_NAMESPACE

void QTH_UTILITY_EXPORT str_split(std::vector<std::string>& vec, const std::string& in, char delimiter/* = '\n'*/)
{
	size_t pos =0, lpos = 0;	

	do{
		lpos = in.find(delimiter, pos);
		if (lpos != std::string::npos)
		{
			vec.push_back(in.substr(pos, lpos - pos));
			pos = lpos + 1;
		}
		else
		{
			vec.push_back(in.substr(pos));
		}
					
	}while(lpos != std::string::npos);
	
}

END_NAMESPACE
