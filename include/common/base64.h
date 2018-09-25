/****************************
function:		base64编码解码
Author:			屈天豪
date:			2018/09/21
tips:
	封装boost库的base64编码解码方法
*****************************/
#ifndef USE_BASE64_H_
#define USE_BASE64_H_
#include<string>

BEGIN_NAMESPACE
	std::string QTH_UTILITY_EXPORT base64_encode(const std::string& input);
	std::string QTH_UTILITY_EXPORT base64_decode(const std::string& input);
END_NAMESPACE

#endif