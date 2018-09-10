/****************************
Author:			屈天豪
date:			2016/7/22
Version:		v1.0
Descript:		dll导出文件定义
Others:	
History:
*****************************/

#ifndef XDN_EXPORT_H_
#define XDN_EXPORT_H_

#if defined (WIN32) || defined(WIN64)
	#ifndef XDN_UTILITY_IS_DLL
		#define  XDN_UTILITY_IS_DLL 1
	#endif//XDN_UTILITY_IS_DLL

#if defined (XDN_UTILITY_IS_DLL) && (XDN_UTILITY_IS_DLL == 1)
	#ifdef XDN_UTILITY_BUILD_DLL
		#define QTH_UTILITY_EXPORT __declspec(dllexport)
	#else
		#define QTH_UTILITY_EXPORT __declspec(dllimport)
	#endif//XDN_UTILITY_BUILD_DLL
#else
	#define QTH_UTILITY_EXPORT
#endif//XDN_UTILITY_IS_DLL

#else
	#define QTH_UTILITY_EXPORT
#endif

#endif
