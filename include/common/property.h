/****************
function: 	字符串 key/value 解析类
author: 	屈天豪
date:		2018/04/27
tips:		
	使用boost hash_map

	用于解析固定格式的字符串，类似以下形式：
	"action=A num=B"
	提取 action作为key, A作为value。用以提供 通过 Key 查找对应的value值, 其中key 和 value 都是 std::string 类型
*****************/
#ifndef PROPERTY_H_
#define PROPERTY_H_

namespace boost
{
	namespace unordered
	{
		template <class K, class T, class H, class P, class A>
		class unordered_map;
	}
}

BEGIN_NAMESPACE

//字符串HASH
struct BKDRHash
{
	size_t operator()(const std::string& str) const
	{
		register size_t hash = 0;
		for (size_t i = 0; i < str.size(); ++i)
			hash = hash * 131 + str[i];   

		return hash;  
	}
};

class QTH_UTILITY_EXPORT CmdParse
{
	public:	
		CmdParse();
		~CmdParse();

		CmdParse(const CmdParse& other);
		CmdParse& operator=(const CmdParse& other);

		//对字符串进行解析
		bool parse(const std::string& str_in);
		//反向解析, 将map["a"]=="b"   组织成  "a=b "的形式
		const std::string reverse() const;
		//key是否存在
		bool check(const std::string& key) const;
		//通过key 获取值
		std::string& operator[](const char* key);
		const std::string& operator[](const char* key) const;
		
		template<class T>
		const std::string& operator[](const T& key);
		template<class T>
		const std::string& operator[](const T& key) const;

		//解析参数数量
		const size_t size()  const;
private:
	typedef boost::unordered::unordered_map<std::string, std::string, BKDRHash, std::equal_to<std::string>, std::allocator<std::pair<const std::string, std::string> > >	KeyValueMap;
	typedef		KeyValueMap*			KeyValueMapPtr;
	KeyValueMapPtr m_kvMap;
};



template<class T>
const std::string& CmdParse::operator[](const T& key)
{
	std::string str = T_to_string(key);
	return operator[](str);
}

template<class T>
const std::string& CmdParse::operator[](const T& key) const
{
	std::string str = T_to_string(key);
	return operator[](str);
}
END_NAMESPACE
#endif
