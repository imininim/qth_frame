/********************************************************************
	created:	2012/08/02
	created:	2:8:2012   10:31
	filename: 	http_util.h
	file path:	
	file base:	http_util
	file ext:	h
	author:		cg
	
	purpose:	如有问题与我联系 QQ: 17448470
*********************************************************************/

#pragma once

#include <assert.h>
#include <string>
#include <map>
#include <list>
#include <iterator>
using namespace std;

//测试程序
/*
	void test()
	{
		char sz[1024];
		memset(sz, 0, sizeof(sz));
		FILE* fp = fopen("e:\\HTTP_UTIL_resp.txt", "rb");
		int len = fread(sz, 1, 1024, fp);
		fclose(fp);

		CHttpRequestParser ps;
		ps.parse(sz);
		CHttpResponseMaker make;
		string strContent = "ok";
		string strResp;
		make.make_string(strContent, strResp);
	}
*/

enum HttpMethodType{
	HTTP_UTIL_METHOD_NONE,
	HTTP_UTIL_METHOD_GET,
	HTTP_UTIL_METHOD_POST,
	HTTP_UTIL_METHOD_RESP
};


enum HttpParamType{
	HTTP_UTIL_PARAM_ALL,			//所有类型的参数
	HTTP_UTIL_PARAM_HEADPARAM,		//只获取HEADPARAM
	HTTP_UTIL_PARAM_CONTENT			//只获取CONTENT
};

/////////////////////////////////////////////////////////////////////////////
//构造http回应包
class CHttpResponseMaker
{
public:
	CHttpResponseMaker(){}
	virtual ~CHttpResponseMaker(){}
public:
	static int		make(const char* szContent, int nContentLen, char* szBuffer, int nBufferSize);
	static void		make_string(const string& strContent, string& strResp);
	static void		make_404_error(string& strResp);
	static void		make_302_error(const string& strLocation, const string& strMoveTo, string& strResp);
protected:
private:
};

inline int CHttpResponseMaker::make(const char* szContent, int nContentLen, char* szBuffer, int nBufferSize)
{
	int nRealContentLen = 0;
	if(nContentLen > 0)
		nRealContentLen = nContentLen + strlen("\r\n\r\n");

	sprintf(szBuffer, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\nContent-Type: text/html;charset=utf-8\r\nConnection: Keep-Alive\r\n\r\n",
			nRealContentLen);
	int nHeadLen = strlen(szBuffer);
	if(nContentLen > 0)
	{
		memcpy(szBuffer+nHeadLen, szContent, nContentLen);
		szBuffer[nHeadLen + nContentLen] = 0;
		strcat(szBuffer, "\r\n\r\n");
	}
	return strlen(szBuffer);
}

inline void CHttpResponseMaker::make_string(const string& strContent, string& strResp)
{
	char* szBuffer = new char[4096+strContent.size()];
	make(strContent.c_str(), strContent.size(), szBuffer, sizeof(szBuffer));
	strResp = szBuffer;
	delete []szBuffer;
}

inline void CHttpResponseMaker::make_404_error(string& strResp)
{
	string strContent;
	strContent += "<html>\r\n";
	strContent += "<head><title>404 Not Found</title></head>\r\n";
	strContent += "<body bgcolor=\"white\">\r\n";
	strContent += "<center><h1>404 Not Found</h1></center>\r\n";
	strContent += "<hr><center>http_util</center>\r\n";
	strContent += "</body>\r\n";
	strContent += "</html>\r\n";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	strContent += "<!-- The padding to disable MSIE's friendly error page -->";
	
	char szTemp[100];
	sprintf(szTemp, "Content-Length: %d\r\n", (int)strContent.size());
	
	strResp = "HTTP/1.1 404 Not Found\r\n";
	strResp += "Server: http_util\r\n";
	strResp += "Content-Type: text/html; charset=UTF-8\r\n";
	strResp += szTemp;
	strResp += "Connection: keep-alive\r\n";
	strResp += "\r\n";
	strResp += strContent;
}
inline void CHttpResponseMaker::make_302_error(const string& strLocation, const string& strMoveTo, string& strResp)
{
	string strContent;
	strContent += "<html><head><title>Object moved</title></head><body>\r\n";
	strContent += "<h2>Object moved to <a href=\"";
	strContent += strMoveTo;
	strContent += "\">here</a>.</h2>\r\n";
	strContent += "</body></html>\r\n";

	char szTemp[100];
	sprintf(szTemp, "Content-Length: %d\r\n", (int)strContent.size());
	
	strResp = "HTTP/1.1 302 Found\r\n";
	strResp += "Server: http_util\r\n";
	strResp += "Content-Type: text/html; charset=UTF-8\r\n";
	strResp += szTemp;
	strResp += "Connection: keep-alive\r\n";
	strResp += "Location: ";
	strResp += strLocation + "\r\n";
	strResp += "\r\n";
	strResp += strContent;
}


/////////////////////////////////////////////////////////////////////////////
//构造参数字符串
class CHttpParamStringMaker
{
public:
	CHttpParamStringMaker(){}
	virtual ~CHttpParamStringMaker(){}

public:
	void	add_param(const string& strKey, const string& strValue);
	void	add_param(const string& strKey, int nValue);
	void	set_paramlines(const string& strLines);
	string  get_params();
protected:
private:
	typedef struct 
	{
		string strKey;
		string strValue;
	}HttpGetMakerParam;
	list<HttpGetMakerParam>	m_params;
	string m_strParamLines;
};
inline void CHttpParamStringMaker::add_param(const string& strKey, const string& strValue)
{
	HttpGetMakerParam param;
	param.strKey = strKey;
	param.strValue = strValue;
	m_params.push_back(param);
}
inline void CHttpParamStringMaker::add_param(const string& strKey, int nValue)
{
	char szValue[100];
	sprintf(szValue, "%u", nValue);
	add_param(strKey, szValue);
}
inline void CHttpParamStringMaker::set_paramlines(const string& strParamLines)
{
	m_strParamLines = strParamLines;
}
inline string CHttpParamStringMaker::get_params()
{
	if(!m_strParamLines.empty())
		return m_strParamLines;

	char szParams[4096] = {0};
	list<HttpGetMakerParam>::iterator it;
	for(it = m_params.begin(); it != m_params.end(); it++)
	{
		HttpGetMakerParam param = *it;

		char szTmp[1024];
		sprintf(szTmp, "%s=%s&", param.strKey.c_str(), param.strValue.c_str());
		strcat(szParams, szTmp);
	}
	if(szParams[strlen(szParams) -1] == '&')
		szParams[strlen(szParams) -1] = 0;
	return szParams;
}

/////////////////////////////////////////////////////////////////////////////
//构造请求包
class CHttpMaker	: public CHttpParamStringMaker
{
public:
	CHttpMaker(){}
	virtual ~CHttpMaker(){}

public:
	int		make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize);
	void	make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest);

public:
	int		GET_make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize);
	void	GET_make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest);

	int		POST_make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize);
	void	POST_make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest);
};
inline int CHttpMaker::make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize)
{
	return GET_make(strHost, nPort, strUri, szBuffer, nBufferSize);
}
inline void CHttpMaker::make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest)
{
	GET_make_string(strHost, nPort, strUri, strRequest);
}
inline int CHttpMaker::GET_make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize)
{
	char szPort[100] = {0};
	if(nPort != 80)
	{
		sprintf(szPort, ":%d", nPort);
	}
	string strParams = get_params();
	sprintf(szBuffer, "GET %s?%s HTTP/1.1\r\nHost: %s%s\r\nConnection: Keep-Alive\r\nAccept: */*\r\nUser-Agent: http_util\r\n\r\n",
		strUri.c_str(), strParams.c_str(), strHost.c_str(), szPort);
	return strlen(szBuffer);
}
inline void CHttpMaker::GET_make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest)
{
	char szBuffer[4096];
	GET_make(strHost, nPort, strUri, szBuffer, sizeof(szBuffer));
	strRequest = szBuffer;
}
inline int CHttpMaker::POST_make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize)
{
	char szPort[100] = {0};
	if(nPort != 80)
	{
		sprintf(szPort, ":%d", nPort);
	}
	string strParams = get_params();
	string strContentType = "application/x-www-form-urlencoded; charset=UTF-8";//二进制用application/octet-stream
	sprintf(szBuffer, "POST %s HTTP/1.1\r\nHost: %s%s\r\nContent-type: %s\r\nContent-Length: %d\r\nConnection: Keep-Alive\r\n\r\n%s",
		strUri.c_str(), strHost.c_str(), szPort, strContentType.c_str(), (int)strParams.size(), strParams.c_str());
	return strlen(szBuffer);
}
inline void CHttpMaker::POST_make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest)
{
	char szBuffer[4096];
	POST_make(strHost, nPort, strUri, szBuffer, sizeof(szBuffer));
	strRequest = szBuffer;
}

/////////////////////////////////////////////////////////////////////////////
//构造GET请求包
class CHttpGetMaker	: public CHttpMaker
{
public:
	CHttpGetMaker(){}
	virtual ~CHttpGetMaker(){}

public:
	int make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize)
	{
		return GET_make(strHost, nPort, strUri, szBuffer, nBufferSize);
	}
	void make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest)
	{
		GET_make_string(strHost, nPort, strUri, strRequest);
	}
};

/////////////////////////////////////////////////////////////////////////////
//构造POST请求包
class CHttpPostMaker	: public CHttpMaker
{
public:
	CHttpPostMaker(){}
	virtual ~CHttpPostMaker(){}
	
public:
	int make(const string& strHost, unsigned short nPort, const string& strUri, char* szBuffer, int nBufferSize)
	{
		return POST_make(strHost, nPort, strUri, szBuffer, nBufferSize);
	}
	void make_string(const string& strHost, unsigned short nPort, const string& strUri, string& strRequest)
	{
		POST_make_string(strHost, nPort, strUri, strRequest);
	}
};

/////////////////////////////////////////////////////////////////////////////
//分析Http的数据包长度，支持GET\POST\RESP
class CHttpLengthAnaly
{
public:
	CHttpLengthAnaly(){}
	virtual ~CHttpLengthAnaly(){}
public:
	//获取长度，错误数据返回-1，数据不完整返回0，接收完全返回>0
	static int		get_length(const char* szData, int nDataLen);

	//获取长度，错误数据返回-1，数据不完整返回0，接收完全返回>0
	//nContentPos返回内容区域的位置
	static int		get_length_ex(const char* szData, int nDataLen, int& nContentPos, int& nContentLen);
};
inline int CHttpLengthAnaly::get_length(const char* szData, int nDataLen)
{
	int nContentPos;
	int nContentLen;
	return get_length_ex(szData, nDataLen, nContentPos, nContentLen);
}
inline int CHttpLengthAnaly::get_length_ex(const char* szData, int nDataLen, int& nContentPos, int& nContentLen)
{
	bool bGetType = false;
	bool bPostType = false;
	bool bRespType = false;
	if(memcmp(szData, "GET ", 4) == 0)
	{
		bGetType = true;
	}
	else if(memcmp(szData, "POST ", 5) == 0)
	{
		bPostType = true;
	}
	else if(memcmp(szData, "HTTP/", 5) == 0)
	{
		bRespType = true;
	}
	else
	{
		return -1;
	}

	//根据http头结束符判断
	char* pHeadEnd = strstr((char*)szData, "\r\n\r\n");
	if(!pHeadEnd)
		return 0;

	nContentPos = 0;
	nContentLen = 0;
	int nHeadLen = pHeadEnd+4-szData;
	if(bPostType || bRespType)
	{
		char* pContentLen = strstr((char*)szData, "Content-Length: ");
		if(pContentLen)
		{
			pContentLen += strlen("Content-Length: ");
			char* pContentLenEnd = strstr(pContentLen, "\r\n");
			if(pContentLenEnd)
			{
				char szTmp[30];
				memset(szTmp, 0, sizeof(szTmp));
				memcpy(szTmp, pContentLen, pContentLenEnd-pContentLen);
				nContentLen = atoi(szTmp);

				//内容的相对位置
				nContentPos = pHeadEnd-szData + strlen("\r\n\r\n");
			}
		}
	}
	if(nDataLen < nHeadLen+nContentLen)
		return 0;

	return nHeadLen+nContentLen;
}


/////////////////////////////////////////////////////////////////////////////
//解析参数
//说明：用于解析如aaa=123&bbb=321&ccc=888之类的字符串
class CHttpParamParser
{
public:
	CHttpParamParser(const char* szHttpParam = NULL, int nLen = 0)
	{
		if(szHttpParam && nLen > 0)
		{
			parse(szHttpParam, nLen);
		}
	}
	virtual ~CHttpParamParser(){}

public:
	bool parse(const char* szHttpParam, int nLen);
	string get_param(const char* szKey);
	int get_param_int(const char* szKey);

private:
	map<string, string> m_mapValues;
};

inline bool CHttpParamParser::parse(const char* szHttpParam, int nLen)
{
	//清理内存
	m_mapValues.clear();
	
	//复制到缓冲
	char* szBuffer = new char[4096+nLen];
	memcpy(szBuffer, szHttpParam, nLen);
	szBuffer[nLen] = 0;
	if(szBuffer[strlen(szBuffer)-1] != '&')
		strcat(szBuffer, "&");

	//处理参数
	char* szParam = szBuffer;
	while(1)
	{
		if(szParam[0] == '\0')
			break;

		if(!((szParam[0] >='a' && szParam[0] <='z') || (szParam[0] >='A' && szParam[0] <='Z')))
		{
			szParam ++;
			continue;
		}

		char* szValue = strchr(szParam, '=');
		if(!szValue)
			break;

		char* szSplit = strchr(szParam, '&');
		if(!szSplit)
			break;

		string strKey;
		string strValue;

		char szTmp[1024];
		//提取key
		memset(szTmp, 0, sizeof(szTmp));
		memcpy(szTmp, szParam, szValue-szParam);
		strKey = szTmp;

		//提取value
		memset(szTmp, 0, sizeof(szTmp));
		memcpy(szTmp, szValue+1, szSplit-szValue-1);
		strValue = szTmp;

		//保存
		m_mapValues.insert(std::make_pair(strKey, strValue));
		//printf("parse http request, key:%s, value:%s \r\n", strKey.c_str(), strValue.c_str());

		//下一个参数
		szParam = szSplit + 1;
	}

	delete []szBuffer;
	return true;
}

inline string CHttpParamParser::get_param(const char* szKey)
{
	map<string, string>::iterator it = m_mapValues.find(szKey);
	if(it != m_mapValues.end())
	{
		return it->second;
	}
	else
	{
		return "";
	}
}
inline int CHttpParamParser::get_param_int(const char* szKey)
{
	return atoi(get_param(szKey).c_str());
}

/////////////////////////////////////////////////////////////////////////////
//分离字符串工具
class CInsertTempZero
{
public:
	CInsertTempZero(char* pSrc)
	{
		m_szOld = *pSrc;
		m_pSrc = pSrc;
		*m_pSrc = 0;
	}
	virtual ~CInsertTempZero()
	{
		*m_pSrc = m_szOld;
	}
protected:
private:
	char* m_pSrc;
	char  m_szOld;
};

/////////////////////////////////////////////////////////////////////////////
//HTTP协议解析器
class CHttpParser
{
public:
	CHttpParser(const char* szHttpReq = NULL, int nDataLen = 0, int nExtraParamType = HTTP_UTIL_PARAM_ALL)
		: m_pszContent(NULL)
		, m_nContentLen(0)
		, m_nHttpMethod(HTTP_UTIL_METHOD_NONE)
		, m_pszUri(NULL)
		, m_pszActParam(NULL)
		, m_nExtraParamType(HTTP_UTIL_PARAM_ALL)
	{
		if(szHttpReq != NULL)
		{
			if(nDataLen > 0)
			{
				parse(szHttpReq, nDataLen, nExtraParamType);
			}
			else
			{
				assert(0);
			}
		}
		m_szFirstLine[0] = 0;
	}

	virtual ~CHttpParser(){}

public:
	//错误数据返回-1，数据不完整返回0，接收完全返回>0
	int parse(const char* szHttpReq, int nDataLen, int nExtraParamType = HTTP_UTIL_PARAM_ALL);

	//获取各个头域参数值
	string get_head_field(const string& strFieldName);

	//获取cookie
	string get_cookie();

	//获取参数项的数值
	string get_param(const char* szKey);
	int get_param_int(const char* szKey);

	//获取参数字符串，如 "aaa=123&bbb=321&ccc=888"
	string		get_param_string();
	
	//获取uri内容，如 "/update/mytest"
	string		get_uri();

	//获取object，比如http://127/aa/bb/cc?fff=999 中的cc
	string		get_object();
	
	//获取类型
	int			get_http_method();

protected:
	//解析头域
	bool parseField(const char* szHttpReq, int nTotalLen);
	//解析第一行，提取method和headparam
	bool parseFirstLine();
	//解析method
	bool parseMethod(const char* szFirstLine, const char* szMethod, int nMethodType);

private:
	map<string, string>	m_mapFields;
	char				m_szFirstLine[4096];
	const char*			m_pszContent;
	int					m_nContentLen;
	int					m_nHttpMethod;
	
	const char*			m_pszUri;
	const char*			m_pszActParam;

	CHttpParamParser	m_paramParser;		//参数解析器

	int					m_nExtraParamType;
};

inline int CHttpParser::parse(const char* szHttpReq, int nDataLen, int nExtraParamType)
{
	int nContentPos = 0;

	m_nExtraParamType = nExtraParamType;

	//判断是否接收完成
	int nTotalLen = CHttpLengthAnaly::get_length_ex(szHttpReq, nDataLen, nContentPos, m_nContentLen);
	if(nTotalLen <= 0)
		return nTotalLen;
	
	//分离头域字段
	if(!parseField(szHttpReq, nTotalLen))
		return -1;

	//分析第一行的信息
	if(!parseFirstLine())
		return -1;

	//分析参数详细信息
	if(m_pszActParam)
	{
		if(nExtraParamType == HTTP_UTIL_PARAM_ALL || nExtraParamType == HTTP_UTIL_PARAM_HEADPARAM)
		{
			m_paramParser.parse(m_pszActParam, strlen(m_pszActParam));
		}
	}
	else if(m_pszContent)
	{
		if(nExtraParamType == HTTP_UTIL_PARAM_ALL || nExtraParamType == HTTP_UTIL_PARAM_CONTENT)
		{
			m_paramParser.parse(m_pszContent, m_nContentLen);
		}
	}

	return nTotalLen;
}

inline bool CHttpParser::parseField(const char* szHttpReq, int nTotalLen)
{
	//获取第一行
	char* ptr = strstr((char*)szHttpReq, "\r\n");
	if(!ptr)
		return false;
	

	CInsertTempZero z1(ptr);
	strncpy(m_szFirstLine, szHttpReq, sizeof(m_szFirstLine)-1);
	ptr += strlen("\r\n");
	
	while(1)
	{
		//防止越界
		if(ptr > szHttpReq + nTotalLen - 4)	//4 == strlen("\r\n\r\n")
			break;

		//是否到了文本区域末尾
		if(memcmp(ptr, "\r\n", 2) == 0)
			break;

		//行末
		char* pLineEnd = strstr(ptr, "\r\n");
		if(!pLineEnd)
			break;

		CInsertTempZero zLineEnd(pLineEnd);
		
		//对一行进行分析
		char* p = strstr(ptr, ": ");
		if(!p)
		{
			break;
		}

		//提取头域名和值
		CInsertTempZero zp(p);
		string strFieldName = ptr;

		p += strlen(": ");
		string strValue = p;
		
		m_mapFields.insert(std::make_pair(strFieldName, strValue));
		
		ptr = pLineEnd + strlen("\r\n");
	}

	ptr += strlen("\r\n");

	if(ptr < szHttpReq + nTotalLen)
	{
		m_pszContent = ptr;
	}
	return true;
}

inline bool CHttpParser::parseFirstLine()
{
	if(strlen(m_szFirstLine) < 10)		//第一行不能小于10个字符
		return false;

	char* pBegin = NULL;
	//分析method
	while(1)
	{
		if(parseMethod(m_szFirstLine, "GET ", HTTP_UTIL_METHOD_GET))
		{
			pBegin = m_szFirstLine + 4;
			break;
		}
		if(parseMethod(m_szFirstLine, "POST ", HTTP_UTIL_METHOD_POST))
		{
			pBegin = m_szFirstLine + 5;
			break;
		}
		if(parseMethod(m_szFirstLine, "HTTP/", HTTP_UTIL_METHOD_RESP))
		{
			pBegin = m_szFirstLine + 5;
			break;
		}
	}
	//没找到可支持的method则返回
	if(m_nHttpMethod == HTTP_UTIL_METHOD_NONE)
		return false;

	//提取uri和动作参数
	char* szParam = strchr(pBegin, '?');
	if(szParam)
	{
		*szParam = 0;
		m_pszActParam = szParam+1; 
	}
	m_pszUri = pBegin;
	return true;
}
inline bool CHttpParser::parseMethod(const char* szFirstLine, const char* szMethod, int nMethodType)
{
	int len = strlen(szMethod);
	if(memcmp(m_szFirstLine, szMethod, len) == 0)
	{
		char* pEnd = strstr(m_szFirstLine, " HTTP");
		if(pEnd)
		{
			*pEnd = 0;
			m_nHttpMethod = nMethodType;
			return true;
		}
	}
	return false;
}


inline string CHttpParser::get_head_field(const string& strFieldName)
{
	map<string, string>::iterator it = m_mapFields.find(strFieldName);
	if(it != m_mapFields.end())
	{
		return it->second;
	}
	return "";
}
inline string CHttpParser::get_cookie()
{
	return get_head_field("Cookie");
}

inline string CHttpParser::get_param(const char* szKey)
{
	return m_paramParser.get_param(szKey);
}
inline int CHttpParser::get_param_int(const char* szKey)
{
	return m_paramParser.get_param_int(szKey);
}

inline string CHttpParser::get_param_string()
{
	if(m_pszActParam && (m_nExtraParamType == HTTP_UTIL_PARAM_ALL || m_nExtraParamType == HTTP_UTIL_PARAM_HEADPARAM))
		return m_pszActParam;

	if(m_pszContent && (m_nExtraParamType == HTTP_UTIL_PARAM_ALL || m_nExtraParamType == HTTP_UTIL_PARAM_CONTENT))
		return m_pszContent;

	return "";
}

inline string CHttpParser::get_uri()
{
	if(!m_pszUri)
		return "";

	return m_pszUri;
}
inline string CHttpParser::get_object()
{
	if(!m_pszUri)
		return "";
	
	char* ptr = strrchr((char*)m_pszUri, '/');
	if(!ptr)
		return "";

	return ptr+1;
}

inline int CHttpParser::get_http_method()
{
	return m_nHttpMethod;
}

/////////////////////////////////////////////////////////////////////////////

