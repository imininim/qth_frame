/****************************
Author:			屈天豪
date:			2016/7/22
Version:		v1.0
Descript:		项目公用头文件
Others:	
History:
*****************************/
#ifndef XDN_UTILITY_H_
#define XDN_UTILITY_H_
#include <cstddef>
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <exception>
#include<time.h>
#include <climits>
#include "export.h"

#ifdef _WIN32
//#pragma warning(disable:4512)
#pragma warning(disable:4390)
#pragma warning(disable:4251)
#pragma warning(disable:4290 )
#pragma warning(disable:4996)
#endif

//项目使用的命名空间
#define QTH_NAME_SPACE		qth

#define BEGIN_NAMESPACE namespace QTH_NAME_SPACE{
#define END_NAMESPACE }



#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);\
	void operator=(const TypeName&);

#define SAFE_DELETE(p)\
	if (NULL != p){\
	delete p;\
	p = NULL;}
	
BEGIN_NAMESPACE
	//字符串分割
	void QTH_UTILITY_EXPORT str_split(std::vector<std::string>& vec, const std::string& in, char delimiter = '\n');
	
//任意类型转字符串
	template<class T>
	std::string T_to_string( const T& in_t )
	{
		static std::stringstream stream_T_to_str;
		stream_T_to_str.str("");
		stream_T_to_str.clear();
		
		stream_T_to_str << in_t;
		return stream_T_to_str.str();
	} 

END_NAMESPACE

#endif
