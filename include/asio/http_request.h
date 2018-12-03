/*******************************
function:	http 客户端连接 包装
date:		2018/11/15
version:	v1.0
tips:		提供http 客户端连接的简便方式
********************************/

#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "utility/memory.h"
#include "asio/interface_http_request.h"
#include "asio/interface_tcp_client_conn.h"

BEGIN_NAMESPACE
class CHttpRequest: public IHttpRequest {
public:
	virtual ~CHttpRequest();

	//发送 Post请求
	virtual std::string Post(const std::string &url, const std::string &data);

	//发送 get请求
	virtual std::string Get(const std::string &url);

	//设置http 头
	virtual void SetHeader(const std::string &key, const std::string &value);

	//设置 http参数
	virtual void SetParam(const std::string &key, const std::string &value);

public:
	CHttpRequest();
	//debug 接口
	void Debug(const std::string &url) const;
private:
	int GetPort(const std::string &url) const;
	std::string GetUri(const std::string &url) const;
	std::string GetHost(const std::string &url) const;
	std::string HttpRequestExec(const std::string &url, const std::string &data);
private:
	enum { INF=999999 };
	typedef shared_ptr<ITcpClientConnect> TcpClientPtr;
	TcpClientPtr m_tcpClientPtr;
	//boost::unordered::unordered_map<std::string, std::string> m_header;
	//boost::unordered::unordered_map<std::string, std::string> m_params;
	std::map<std::string, std::string> m_header;
	std::map<std::string, std::string> m_params;
};



END_NAMESPACE
#endif
