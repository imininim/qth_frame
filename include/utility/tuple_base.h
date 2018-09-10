/********************************
function:	class tuple_base
date:		2016/09/20
tips:		
			元组， 用于存储不同类型类，并给与其顺序的定义
			此实现tuple 简单功能， 提供 get<类型>, get<位置> 两种访问方式
			增强tuple 功能， 提供 序列化,反序列化和动态大小计算的能力
*********************************/
#ifndef tuple_base_H_
#define tuple_base_H_
#include "utility/typetraits.h"

BEGIN_NAMESPACE

/************************************************************************/
/*	
	function:	可序列化对象
				继承该接口的对象必须实现所有的虚函数， 这些虚函数提供给 tuple_base, 用于序列化为字串
*/
/************************************************************************/
class Serializable : public size_info
{
public:
	/*
		name:		size()
		function:	可序列化对象的大小
		return:		返回序列化的对象大小
	*/
	virtual size_t size() const							= 0;

	/*
		name:		serialize(void*, int)
		function:	将对象内容序列化成一个字串
		params:		
			pBuffer		存储序列化内容的buffer
			len			pBuffer指向内存的大小
		return		
					实际序列化字节数

	*/
	virtual size_t serialize(void* pBuffer, int len)		= 0;

	/*
		name:		unserialize(void*, int)
		function:	以缓冲区中的数据初始化对象
		return:
					实际反序列化字节数
	*/
	virtual size_t unserialize(void* pBuffer, int len)	= 0;
};

/*
	用户数据类型的包装类
*/
template<class T>
struct Holder
{
	typedef		typename TypeTraits<T>::ReferenceType			ReturnType;
	typedef		const typename TypeTraits<T>::ReferenceType		ConstReturnType;

	Holder():value_(  ){}
	Holder(typename TypeTraits<T>::ParamterType t): value_(t){}

	ReturnType operator()()					{ return value_;}
	ConstReturnType operator()() const		{ return value_;}

	size_t size() const						{ return size_of<T>(value_);}
	
	//序列化
	size_t serialize(void* pBuffer, int len)
	{
		return serialize_help(pBuffer, len, Int2Type<size_type<T>::result>() );
	}

	//反序列化
	size_t unserialize(void* pBuffer, int len)
	{
		return unserialize_help(pBuffer, len, Int2Type<size_type<T>::result>() );
	}

private:
	//继承于 size_info 的序列化处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<0>)
	{
		return value_.serialize(pBuffer, len);
	}
	//字符串 std::string序列化处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<1>)
	{
		size_t data_len = value_.size() + sizeof(char);//存储的数据长度
		assert(sizeof(size_t) + data_len <= (size_t)len);
		
		size_t* pIntBuffer = (size_t*)pBuffer;
		*pIntBuffer = data_len; //先存储数据的长度
		++pIntBuffer;

		memcpy(pIntBuffer, value_.c_str(), data_len); 
		return sizeof(size_t) + data_len;
	}
	//其他类型处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<2>)
	{
		assert(sizeof(T) <= len);
		memcpy(pBuffer, &value_,sizeof(T));
		return sizeof(T);
	}

	//继承于 size_info 的反序列化处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<0>)
	{
		return value_.unserialize(pBuffer, len);
	}
	//字符串 std::string反序列化处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<1>)
	{
		size_t* pIntBuffer = (size_t*)pBuffer;
		size_t s = *pIntBuffer; //先读取长度
		++pIntBuffer;

		//读取实际的数据
		value_.assign((const char*)pIntBuffer, s);
		return sizeof(size_t) + s;
	}
	//其他类型处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<2>)
	{
		assert(sizeof(value_) <= len);
		memcpy(&value_, pBuffer, sizeof(value_));
		return sizeof(value_);
	}

	T value_;
};

/*
	特化版， 针对数组类型提供包装
*/
template<class T, int tLen>
struct Holder< T[tLen] >
{
	typedef		T											ValueType;
	typedef		typename TypeTraits<ValueType>::RemoveConst	NoConstValueType;

	typedef		ValueType*			ReturnType;
	typedef		const ValueType*	ConstReturnType;

	Holder()
	{
		initObj(Int2Type<TypeTraits<T>::isClass>());
	}
	
	Holder(const T* t)
	{
		for (int i = 0; i < tLen; ++i)
			value_[i] = t[i];
	}
		
	ReturnType operator()()						{ return value_;}
	ConstReturnType operator()() const			{ return value_;} 

	size_t size() const
	{
		size_t s = 0;
		for (int i = 0; i < tLen; ++i)
			s += size_of<ValueType>(value_[i]);

		return s; 
	}

	//序列化
	size_t serialize(void* pBuffer, int len)
	{
		return serialize_help(pBuffer, len, Int2Type<size_type<T>::result>() );
	}

	//反序列化
	size_t unserialize(void* pBuffer, int len)
	{
		return unserialize_help(pBuffer, len, Int2Type<size_type<T>::result>() );
	}

private:
	//非对象类型初始化
	void initObj(Int2Type<false>)
	{
		for (int i = 0; i < tLen; ++i)
			value_[i] = T();
	}
	//对象类型初始化交由各自的构造函数
	void initObj(Int2Type<true>)	{}

	//继承于 size_info 的序列化处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<0>)
	{
		char* pCharBuffer = (char*)pBuffer;

		size_t s = 0, total_size = 0;
		for (int i = 0; i < tLen; ++i)
		{
			s = value_[i].serialize(pCharBuffer, len);
			pCharBuffer += s;
			len -= s;

			total_size += s;
		}
		return total_size;
	}
	//字符串 std::string序列化处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<1>)
	{
		char* pCharBuffer = (char*)pBuffer;
		
		size_t data_len = 0, total_size = 0;
		for (int i = 0; i < tLen; ++i)
		{
			size_t* pIntBuffer = (size_t*)pCharBuffer;
			data_len = value_[i].size() + sizeof(char);
			assert(sizeof(size_t) + data_len <= (size_t)len);
			*pIntBuffer = data_len; //存储数据长度
			++pIntBuffer;
			
			//存储数据
			memcpy(pIntBuffer, value_[i].c_str(), data_len);

			pCharBuffer += data_len + sizeof(size_t);
			len -= data_len + sizeof(size_t);

			total_size += data_len + sizeof(size_t);
		}

		return total_size;
	}
	//其他类型处理
	size_t serialize_help(void* pBuffer, int len, Int2Type<2>)
	{
		size_t data_len = sizeof(T) * tLen;

		assert(data_len <= (size_t)len);
		memcpy(pBuffer, value_, data_len);
		return data_len;
	}

	//继承于 size_info 的反序列化处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<0>)
	{
		char* pCharBuffer = (char*)pBuffer;

		size_t s = 0, total_size = 0;
		for (int i = 0; i < tLen; ++i)
		{
			s = value_[i].unserialize(pCharBuffer, len);
			pCharBuffer += s;
			len -= s;

			total_size += s;
		}
		return total_size;
	}

	//字符串 std::string反序列化处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<1>)
	{
		char* pCharBuffer = (char*)pBuffer;
		size_t data_len = 0, total_len = 0;

		for (int i = 0; i < tLen; ++i)
		{
			size_t* pIntBuffer = (size_t*)pCharBuffer;
			data_len = *pIntBuffer; //读取长度信息
			++pIntBuffer;

			value_[i].assign((const char*)pIntBuffer, data_len);

			pCharBuffer += sizeof(size_t) + data_len;
			total_len += sizeof(size_t) + data_len;
			len -= sizeof(size_t) + data_len;
		}
		return total_len;
	}
	//其他类型处理
	size_t unserialize_help(void* pBuffer, int len, Int2Type<2>)
	{
		size_t data_len = sizeof(T) * tLen;
		assert(data_len <= (size_t)len);

		memcpy(value_, pBuffer, data_len);
		return data_len;
	}

	ValueType value_[tLen];
};

/************************************************************************/
//                  散乱继承体系 tuple
/************************************************************************/
template<class TList, template<class> class Unit = Holder>
class tuple_base;

template<class,class>
class tuple_base_tag;

template<class T1, class T2, template<class>class Unit>
class tuple_base<TypeList<T1, T2>, Unit>
	:	public tuple_base<tuple_base_tag<T1,T2>, Unit>,
		public tuple_base<T2, Unit> 
{
	typedef TypeList<T1, T2> TList;

	typedef		typename TypeAtNonStrict<TList, 0>::Result		P1;
	typedef		typename TypeAtNonStrict<TList, 1>::Result		P2;
	typedef		typename TypeAtNonStrict<TList, 2>::Result		P3;
	typedef		typename TypeAtNonStrict<TList, 3>::Result		P4;
	typedef		typename TypeAtNonStrict<TList, 4>::Result		P5;
	typedef		typename TypeAtNonStrict<TList, 5>::Result		P6;
	typedef		typename TypeAtNonStrict<TList, 6>::Result		P7;
	typedef		typename TypeAtNonStrict<TList, 7>::Result		P8;
	typedef		typename TypeAtNonStrict<TList, 8>::Result		P9;
	typedef		typename TypeAtNonStrict<TList, 9>::Result		P10;
	typedef		typename TypeAtNonStrict<TList, 10>::Result		P11;

public:
	typedef tuple_base<T1, Unit> leftBase;
	typedef tuple_base<T2, Unit> RightBase;
	
	/*
		将TypeList 萃取出的类型添加上修饰，  P1 是 P1&, 则 Param1 = P1&,  否则 Param1 = ParamterType 类型
		P2, P3  ...同上
	*/
	typedef typename Select<TypeTraits<P1>::isReference, P1, typename TypeTraits<P1>::ParamterType>::Result  		Param1;
	typedef typename Select<TypeTraits<P2>::isReference, P2, typename TypeTraits<P2>::ParamterType>::Result  		Param2;
	typedef typename Select<TypeTraits<P3>::isReference, P3, typename TypeTraits<P3>::ParamterType>::Result  		Param3;
	typedef typename Select<TypeTraits<P4>::isReference, P4, typename TypeTraits<P4>::ParamterType>::Result  		Param4;
	typedef typename Select<TypeTraits<P5>::isReference, P5, typename TypeTraits<P5>::ParamterType>::Result  		Param5;
	typedef typename Select<TypeTraits<P6>::isReference, P6, typename TypeTraits<P6>::ParamterType>::Result  		Param6;
	typedef typename Select<TypeTraits<P7>::isReference, P7, typename TypeTraits<P7>::ParamterType>::Result  		Param7;
	typedef typename Select<TypeTraits<P8>::isReference, P8, typename TypeTraits<P8>::ParamterType>::Result 		Param8;
	typedef typename Select<TypeTraits<P9>::isReference, P9, typename TypeTraits<P9>::ParamterType>::Result 		Param9;
	typedef typename Select<TypeTraits<P10>::isReference, P10, typename TypeTraits<P10>::ParamterType>::Result		Param10;
	typedef typename Select<TypeTraits<P11>::isReference, P11, typename TypeTraits<P11>::ParamterType>::Result		Param11;

	
	tuple_base(){}
	tuple_base(Param1 p1)
		:tuple_base<tuple_base_tag<T1,T2>, Unit>(p1){}

	tuple_base(Param1 p1, Param2 p2)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2, p3){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4){}
	
	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6, p7){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6, p7, p8){}
	
	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9)
		:tuple_base<tuple_base_tag<T1,T2> , Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6, p7, p8, p9){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10)
		:tuple_base<tuple_base_tag<T1,T2>,Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6, p7, p8, p9, p10){}

	tuple_base(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10,
		Param11 p11)
		:tuple_base<tuple_base_tag<T1,T2> ,Unit>(p1), tuple_base<T2, Unit>(p2,p3,p4,p5, p6, p7, p8, p9, p10, p11){}

};

template<class T1, class T2, template<class> class Unit>
class tuple_base<tuple_base_tag<T1,T2>, Unit>
	: public tuple_base<T1, Unit>
{
public:
	typedef		typename TypeTraits<T1>::ParamterType		Param;
	tuple_base(){}
	tuple_base(Param p1):tuple_base<T1,Unit>(p1)
	{}
};


template<class AtomicType, template<class> class Unit>
class tuple_base : public Unit<AtomicType>
{
	typedef Unit<AtomicType>	LeftBase;
		
public:
	tuple_base(): Unit<AtomicType>(){}
	tuple_base(typename TypeTraits<AtomicType>::ParamterType p1): Unit<AtomicType>(p1)
	{
	}
};

template<template<class> class Unit>
class tuple_base<NullType, Unit>
{
public:
	
	tuple_base(){}
};


/************************************************************************/
/*		       通过类型来访问tuple_base   
		tuple_base<TYPELIST_2(double, char)> d;
		get<double>(d) ==> double
		get<char>(d) ==> char
*/
/************************************************************************/
template<class T, class TList>
typename TypeTraits<T>::ReferenceType get(tuple_base<TList>& obj)
{
	enum {index_type_t = IndexOf<TList, T>::value};
	STATIC_CHECK(index_type_t >= 0,  ERROR_qth_tuple_get_error_type);
	return ((Holder<T>&)obj)();
}

template<class T, class TList>
typename TypeTraits<T>::ReferenceType get(const tuple_base<TList>& obj)
{
	enum {index_type_t = IndexOf<TList, T>::value};
	STATIC_CHECK(index_type_t >= 0,  ERROR_qth_tuple_get_error_type);
	return ((const Holder<T>&)obj)();
}

/************************************************************************/
/* 通过位置访问tuple_base        
	get 方法
	
	tuple_base<TYPELIST_2(double, char)> d;
	get<0>(d) ==> double
	get<1>(d) ==> char
*/
/************************************************************************/
template <class TList, template<class> class Unit>
Unit<typename TList::Head>& FieldHelper(tuple_base<TList, Unit>& obj, Int2Type<0>)
{
	tuple_base<tuple_base_tag<typename TList::Head, typename TList::Tail>,Unit>& l = obj;
	tuple_base<typename TList::Head, Unit>& lb = l;
	return lb;
}

template<int i, class TList, template<class> class Unit>
Unit<typename TypeAt<TList, i>::Result>& FieldHelper(tuple_base<TList, Unit>& obj, Int2Type<i>)
{
	tuple_base<typename TList::Tail, Unit>& rb = obj;
	return FieldHelper(rb, Int2Type<i-1>());
}

template<int i, class TList>
typename TypeTraits<typename TypeAtNonStrict<TList, i>::Result>::ReferenceType get(tuple_base<TList>& obj)
{
	STATIC_CHECK(i < Length<TList>::value, ERROR_tuple_index_error);
	tuple_base<TList, Holder>& base = obj;
	return FieldHelper(base, Int2Type<i>())();
}


template<int i, class TList>
const typename TypeTraits<typename TypeAtNonStrict<TList, i>::Result>::ReferenceType get(const tuple_base<TList>& obj)
{
	STATIC_CHECK(i < Length<TList>::value, ERROR_tuple_index_error);
	tuple_base<TList, Holder>& base = const_cast<tuple_base<TList>& >(obj);
	return FieldHelper(base, Int2Type<i>())();
}



/**********************************
function:	tuple_base 可序列化大小
	tuple_size 方法 
	tuple_base<TYPELIST_2(double, char)> d;
	size_t s = tuple_size(d); ==>>  sizeof(double) + sizeof(char)

tips:
	可序列化大小 并不是其对象的整个大小
	它取决于数据的类型，如果对象是固定长度，则其与 sizeof() 的结果相等
						如果对象不是定长，  则取决于 Serializable 接口的 size函数
						如果对象是std::string, 则等于 sizeof(size_t) + 包括nul的字符长度
**********************************/
template<class TList, template<class> class Unit>
size_t tuple_size_helper(tuple_base<TList, Unit>& obj, Int2Type<0>)
{
	return 0;
}

template<int i, class TList, template<class> class Unit>
size_t tuple_size_helper(tuple_base<TList, Unit>& obj, Int2Type<i>)
{
	tuple_base<tuple_base_tag<typename TList::Head, typename TList::Tail>,Unit>& all = obj;
	tuple_base<typename TList::Head, Unit>& lb = all;
	tuple_base<typename TList::Tail, Unit>& rb = obj;
	return lb.size() + tuple_size_helper(rb, Int2Type<i - 1>());
}

template<class TList>
size_t tuple_size(const tuple_base<TList>& obj)
{
	tuple_base<TList, Holder>& base = const_cast<tuple_base<TList, Holder>&>(obj);
	return tuple_size_helper(base, Int2Type< Length<TList>::value>());
}

/**********************************
function:	tuple_base 序列化, 反序列化
	tuple_serialize 方法 
	
example:
tuple_base<TYPELIST_2(double, char)> d;
	char buf[256] = {0}; //序列化容器
	tuple_serialize(d, buf, 256);
	...
**********************************/
template<class TList, template<class> class Unit>
size_t tuple_serialize_helper(tuple_base<TList, Unit>& obj, void* pBuffer, int len, Int2Type<0>)
{return 0;}

template<int i, class TList, template<class> class Unit>
size_t tuple_serialize_helper(tuple_base<TList, Unit>& obj, void* pBuffer, int len, Int2Type<i>)
{
	tuple_base<tuple_base_tag<typename TList::Head, typename TList::Tail>,Unit>& l = obj;
	tuple_base<typename TList::Head, Unit>& lb = l;
	tuple_base<typename TList::Tail, Unit>& rb = obj;
	
	char* pCharBuffer = (char*)pBuffer;
	size_t s = lb.serialize(pCharBuffer, len);

	return s + tuple_serialize_helper(rb, pCharBuffer + s , len - s, Int2Type<i - 1>() );
}

template<class TList>
size_t tuple_serialize(const tuple_base<TList>& obj, void* pBuffer, int len)
{
	tuple_base<TList, Holder>& base = const_cast<tuple_base<TList, Holder>&>(obj);
	return tuple_serialize_helper(base, pBuffer, len, Int2Type< Length<TList>::value>() );
}


template<class TList, template<class> class Unit>
size_t tuple_unserialize_helper(tuple_base<TList, Unit>& obj, void* pBuffer, int len, Int2Type<0>)
{ return 0;}

template<int i, class TList, template<class> class Unit>
size_t tuple_unserialize_helper(tuple_base<TList, Unit>& obj, void* pBuffer, int len, Int2Type<i>)
{
	tuple_base<tuple_base_tag<typename TList::Head, typename TList::Tail>,Unit>& l = obj;
	tuple_base<typename TList::Head, Unit>& lb = l;
	tuple_base<typename TList::Tail, Unit>& rb = obj;

	char* pCharBuffer = (char*)pBuffer;
	size_t s = lb.unserialize(pCharBuffer, len);

	return  s + tuple_unserialize_helper(rb, pCharBuffer + s, len - s, Int2Type<i - 1>() );
}

template<class TList>
size_t tuple_unserialize(tuple_base<TList>& obj, void* pBuffer, int len)
{
	tuple_base<TList, Holder>& base = obj;
	return tuple_unserialize_helper(base, pBuffer, len, Int2Type< Length<TList>::value>() );
}

END_NAMESPACE
#endif


