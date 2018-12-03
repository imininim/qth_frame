/*******************************
function:	http 客户端连接 包装
date:		2018/11/15
version:	v1.0
tips:		提供http 客户端连接的简便方式
********************************/

#ifndef INTERFACE_HTTP_REQUEST_H_
#define INTERFACE_HTTP_REQUEST_H_

BEGIN_NAMESPACE

class IHttpRequest {
public:
	virtual ~IHttpRequest(){}

	//发送 Post请求
	virtual std::string Post(const std::string &uri, const std::string &data) = 0;

	//发送 get请求
	virtual std::string Get(const std::string &uri) = 0;

	//设置http 头
	virtual void SetHeader(const std::string &key, const std::string &value) = 0;

	//设置 http参数
	virtual void SetParam(const std::string &key, const std::string &value) = 0;
};

END_NAMESPACE
#endif

