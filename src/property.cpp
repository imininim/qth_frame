#include "stdafx.h"
#include "common/property.h"
#include "boost/unordered_map.hpp"

BEGIN_NAMESPACE
CmdParse::CmdParse(): m_kvMap(new KeyValueMap())
{
	assert(m_kvMap);
}

CmdParse::~CmdParse()
{
	SAFE_DELETE(m_kvMap);
}

CmdParse::CmdParse(const CmdParse& other): m_kvMap(new KeyValueMap())
{
	assert(m_kvMap);
	*m_kvMap = *(other.m_kvMap);
}

CmdParse& CmdParse::operator=(const CmdParse& other)
{
	if (this == &other)
		return *this;

	*m_kvMap = *(other.m_kvMap);
	return *this;
}

bool CmdParse::parse(const std::string& str_in)
{
	assert(m_kvMap);
	m_kvMap->clear();
	std::vector<std::string> keyValuePairs;
	str_split(keyValuePairs, str_in, ' '); //以空格分割的key=value字符串对

	size_t n = keyValuePairs.size();
	if (!n) return false;

	m_kvMap->clear();
	for (std::vector<std::string>::iterator it = keyValuePairs.begin(); it != keyValuePairs.end(); ++it)
	{
		std::vector<std::string> temp;
		str_split(temp, *it, '=');
		if (temp.size() != 2 || temp[0].empty() || temp[1].empty() ) 
			continue;

		m_kvMap->insert(std::make_pair(temp[0], temp[1]) );
	}
	
	if (m_kvMap->empty())
		return false;

	return true;
}

bool CmdParse::check(const std::string& key) const
{
	KeyValueMap::const_iterator it = m_kvMap->find(key);
	if (it == m_kvMap->end())
		return false;

	return true;
}
	
std::string& CmdParse::operator[](const char* key)
{
	assert(m_kvMap);
	return (*m_kvMap)[std::string(key)];
}

const std::string& CmdParse::operator[](const char* key) const
{
	assert(m_kvMap);
	return (*m_kvMap)[std::string(key)];
}

const size_t CmdParse::size() const
{
	assert(m_kvMap);
	return m_kvMap->size();
}

const std::string CmdParse::reverse() const
{
	std::string strReverse;

	for (KeyValueMap::const_iterator it = m_kvMap->begin(); it != m_kvMap->end(); ++it)
		strReverse += it->first + "=" + it->second + " ";

	return strReverse.substr(0, strReverse.size() - 1); 
}

END_NAMESPACE
