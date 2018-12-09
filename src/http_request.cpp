#include "stdafx.h"
#include "asio/http_request.h"
#include "asio/tcp_client_connect.h"

using namespace std;

BEGIN_NAMESPACE

CHttpRequest::CHttpRequest() {
	SetHeader("Accept-Language", "cn");
	SetHeader("Accept", "*/*");
	SetHeader("Connection", "Keep-Alive");
	SetHeader("User-Agent", "Mozilla/4.0");
	SetHeader("Cache-Control", "no-cache");
}

CHttpRequest::~CHttpRequest() {}

std::string CHttpRequest::Host2Ip(const std::string &host) {
	struct hostent *p = gethostbyname(host.c_str());
	for (int i = 0; p->h_addr_list[i]; i++) {
		return inet_ntoa(*(struct in_addr *)p->h_addr_list[i]);
	}
	return "";
}

int CHttpRequest::GetPort(const std::string &url) const {
	int port = 80;
	const char *tmp = url.c_str();
	size_t offset = 0;
	if (url.substr(0, 5) == "https") {
		port = 443;
		offset += 6;
	} else if (url.substr(0, 4) == "http") {
		offset += 5;
	}
	
	size_t port_begin = INF, port_end = INF;
	while (offset < url.size()) {
		if (port_begin == INF && *(tmp+offset) == ':') {
			port_begin = offset;
		} else if (port_begin != INF && *(tmp+offset) == '/') {
			port_end = offset;
			break;
		}
		++offset;
	}
	if (port_begin != INF && port_end != INF) {
		port = atoi(std::string(tmp+port_begin+1, port_end-port_begin).c_str());
	}
	return port;
}

std::string CHttpRequest::GetUri(const std::string &url) const {
	std::string uri = "/";
	const char *tmp = url.c_str();
	size_t offset = 0;
	if (url.substr(0, 5) == "https") {
		offset += 8;
	} else if (url.substr(0, 4) == "http") {
		offset += 7;
	}
	while (offset < url.size()) {
		if (*(tmp+offset) == '/') {
			break;
		}
		++offset;
	}
	if (offset < url.size()) {
		uri = url.substr(offset);
	}
	return uri;
}
std::string CHttpRequest::GetHost(const std::string &url) const {
	const char *tmp = url.c_str();
	size_t offset = 0;
	if (url.substr(0, 5) == "https") {
		offset += 6;
	} else if (url.substr(0, 4) == "http") {
		offset += 5;
	}
	
	size_t host_begin = INF, host_end = INF;
	while (offset < url.size()) {
		if (host_begin == INF && *(tmp+offset) != '/') {
			host_begin = offset;
		} else if (host_begin != INF && (*(tmp+offset) == ':' || *(tmp+offset) == '/')) {
			host_end = offset;
			break;
		}
		++offset;
	}
	if (offset >= url.size()) {
		host_end = url.size();
	}
	return std::string(tmp+host_begin, host_end-host_begin);
}
std::string CHttpRequest::HttpRequestExec(const std::string &url, const std::string &data) {
	std::string request;
	if (data != "") {
		request += "POST ";
	} else {
		request += "GET ";
	}

	request += GetUri(url)+" HTTP/1.1\r\n";
	for (std::map<std::string, std::string>::const_iterator it = m_header.begin(); it != m_header.end(); ++it) {
		request += it->first+": "+it->second+"\r\n";
	}
	request += "\r\n";

	if (data != "") {
		request += data;
	}

	cerr << "request:" << endl;
	cerr << request << endl;

	if (!m_tcpClientPtr) {
		//m_tcpClientPtr.reset(new CTcpClientConnect());
		m_tcpClientPtr.reset(new SimpleTcpClientConnect());
	}
	//if (!m_tcpClientPtr->Connect("127.0.0.1", 8888)) {
	if (!m_tcpClientPtr->Connect(Host2Ip(GetHost(url)), (unsigned short)GetPort(url))) {
		//_LOG_DEBUG(g_logger, "[连接管理器]客户端连接... %p, sizeof(CTcpClientConnect)=%u", p, sizeof(CTcpClientConnect));
		cerr << "content err" << endl;
		cerr << m_tcpClientPtr->GetErrorInfo() << endl;
		cerr << m_tcpClientPtr->GetErrorCode() << endl;
		return "";
	}

	m_tcpClientPtr->Send(request.c_str(), request.size());

	cerr <<"send ok" << endl;
	std::string response;
	long ret = m_tcpClientPtr->Recv(response);
	cerr << "get resp:" << endl;
	return response;
}


std::string CHttpRequest::Post(const std::string &url, const std::string &data) {
	return HttpRequestExec(url, data);
}

std::string CHttpRequest::Get(const std::string &url) {
	return HttpRequestExec(url, "");
}

void CHttpRequest::SetHeader(const std::string &key, const std::string &value) {
	m_header[key] = value;
}

void CHttpRequest::SetParam(const std::string &key, const std::string &value) {
	m_params[key] = value;
}


void CHttpRequest::Debug(const std::string &url) const {
	std::cerr << "Host: " << GetHost(url) << std::endl;
	std::cerr << "Ip: " << Host2Ip(GetHost(url)) << std::endl;
	std::cerr << "port: " << GetPort(url) << std::endl;
	std::cerr << "Uri: " << GetUri(url) << std::endl;
}
END_NAMESPACE
