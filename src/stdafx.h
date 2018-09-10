/****************************
Author:			屈天豪
date:			2016/7/22
Version:		v1.0
Descript:		预编译头文件
Others:	
History:
*****************************/
#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS	//去除4996警告

#ifdef _WIN32								//限定在windows操作平台
	#include "targetver.h"
	#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
	#include <windows.h>					// Windows 头文件:
#else	
	#define BOOL	bool
#endif

#define XDN_UTILITY_NO_LIB			//自动链接 dll
#define XDN_UTILITY_BUILD_DLL		//编译导出/导入宏
#include "utility/utility.h"




#endif
