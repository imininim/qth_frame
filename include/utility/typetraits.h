/********************************
function:	traits 特性
tips:
		判断类型 T 是否具有某种特性
		萃取出类型T 的某种特性
*********************************/


#ifndef TYPETRAITS_H_
#define TYPETRAITS_H_
#include "utility/typelist.h"

BEGIN_NAMESPACE

/*返回值过滤 void*/
template<class T>
struct return_type
{
	typedef typename Select<is_same_type<T,void>::value , EmptyType, T>::Result type;
};

//大小比较的预处理
template<int a, int b>
struct preMin
{
	enum {result = a < b ? a : b};
};

template<int a, int b>
struct preMax
{
	enum {result = a > b ? a : b};
};

template<class T>
class TypeTraits
{
private:

	//这里根据是否存在 long long 这种类型加以区分
#if LONGLONG_EXISTS
	typedef  TYPELIST_5(unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long)		UnsignedInts;
	typedef	 TYPELIST_6(char, wchar_t, short, int, long, long long)											SignedInts;
	typedef	 TYPELIST_3(float, double, long double)															Floats;
#else
	typedef  TYPELIST_4(unsigned char, unsigned short, unsigned int, unsigned long)							UnsignedInts;
	typedef	 TYPELIST_5(char, wchar_t, short, int, long)													SignedInts;
	typedef	 TYPELIST_3(float, double, long double)															Floats;
#endif
	/*成员指针*/
	template<class U>
	struct MemPointerTraits
	{
		enum {result = false};
	};

	template<class U, class V>
	struct MemPointerTraits<U V::*>
	{
		enum {result = true};
	};

	/*const 限定类型*/
	template<class U>
	struct ConstTraits
	{
		enum {result = false};
		typedef		U			ConstType;
		typedef 	U			RemoveConstType;
	};

	template<class U>
	struct ConstTraits<const U>
	{
		enum {result = true};
		typedef		const U		ConstType;
		typedef 	U			RemoveConstType;
	};

	///*引用类型
	template<class U>
	struct ReferenceTraits
	{
		enum {result = false};
		typedef 	U	RemoveRefType;
		typedef		U&	ReferenceType;
		typedef		U&	ConReferenceType;
				
	};

	//数组的引用转化为数组指针的引用
	template<class U, int nLen>
	struct ReferenceTraits<U[nLen]>
	{
		enum {result = false};
		typedef		U*			RemoveRefType;
		typedef		U*			ReferenceType;
		typedef		const U*	ConReferenceType;
	};

	template<class U>
	struct ReferenceTraits<U&>
	{
		enum {result = true};
		typedef 	U 			RemoveRefType;
		typedef		U&			ReferenceType;
		typedef		const U&	ConReferenceType;
	};
	
	//常引用
	template<class U>
	struct ConstRefTraits
	{
		enum {result = false};
		typedef 	U		ConstRefType;
		typedef 	U		RemoveConstRefType;
	};

	template<class U>
	struct ConstRefTraits<const U&>
	{
		enum {result = true};
		typedef 	const U& 	ConstRefType;
		typedef 	U			RemoveConstRefType;
	};


	/*指针类型*/
	template<class U>
	struct PointerTraits
	{
		enum {result = false};
		typedef NullType	PointerType;
	};

	template<class U>
	struct PointerTraits<U*>
	{
		enum {result = true};
		typedef typename ConstTraits<U*>::ConstType	PointerType;
	};

	/*c++字符串对象类型*/
	template<class U, class no_use = int>
	struct StrTraits
	{
		enum {result = false};
		typedef NullType	StrType;
	};
		
	template<class no_use>
	struct StrTraits<std::string, no_use>
	{
		enum {result = true};
		typedef std::string	StrType;
	};

	template<class no_use>
	struct StrTraits<const std::string, no_use>
	{
		enum {result = true};
		typedef const std::string	StrType;
	};

	/*EmptyType*/
	template<class U, class no_use = int>
	struct EmptyTraits
	{
		enum {result = false};
	};

	template<class no_use>
	struct EmptyTraits<EmptyType, no_use>
	{
		enum {result = true};
	};
	
	/*void*/
	template<class U>
	struct VoidTraits
	{
		enum {result = is_same_type<U,void>::value };
	};
	
	/*类类型检测*/
	struct classTraits
	{
		enum {result = is_class<T>::value };
	};

	//数组特性
	template<class U>
	struct arrayTraits
	{
		enum {result = false, len = 0};
		typedef		U			type;
		typedef		U			const_type;
		typedef		U			original_type;
		typedef		U			char_type;
		typedef		U			con_char_type;
	};

	template<class U, int i>
	struct arrayTraits<U[i]>
	{
		enum {result = true, len = i};
		typedef		U*			type;			//数组提取的指针类型
		typedef		const U*	const_type;		//数组提取的常指针类型
		typedef		U			original_type;	//数组的原始类型
	};

	/*函数指针特性*/
	template<class Fun>
	struct FunTraits
	{
		enum {isFun = false};

		typedef		NullType		ResultType;
		typedef		NullType		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R>
	struct FunTraits<R(*)()>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		NullType		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1>
	struct FunTraits<R(*)(P1)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2>
	struct FunTraits<R(*)(P1, P2)>
	{
		enum {isFun = true};

		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3>
	struct FunTraits<R(*)(P1, P2, P3)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4>
	struct FunTraits<R(*)(P1, P2, P3, P4)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6, P7)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6, P7, P8)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		P10		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
	struct FunTraits<R(*)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>
	{
		enum {isFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		P10		Param10;
		typedef		P11		Param11;
	};

	/*成员函数指针特性*/
	template<class Fun>
	struct MemFunTraits
	{
		enum {isMemFun = false};
		typedef		NullType		ResultType;
		typedef		NullType		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S>
	struct MemFunTraits<R (S::*)()>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		NullType		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1>
	struct MemFunTraits<R (S::*)(P1)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		NullType		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2>
	struct MemFunTraits<R (S::*)(P1,P2)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		NullType		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3>
	struct MemFunTraits<R (S::*)(P1,P2,P3)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		NullType		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4>
	struct MemFunTraits<R (S::*)(P1,P2,P3, P4)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		NullType		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		NullType		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		NullType		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6,class P7>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6,P7)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		NullType		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6,class P7,class P8>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6,P7,P8)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		NullType		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6,class P7,class P8,class P9>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		NullType		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6,class P7,class P8,class P9, class P10>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		P10		Param10;
		typedef		NullType		Param11;
	};

	template<class R, class S, class P1,class P2,class P3, class P4,class P5, class P6,class P7,class P8,class P9, class P10,class P11>
	struct MemFunTraits<R (S::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11)>
	{
		enum {isMemFun = true};
		typedef		R		ResultType;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
		typedef		P10		Param10;
		typedef		P11		Param11;
	};

	//用于帮助萃取 返回值类型
	template<class U, bool b>
	struct interval_result_type_helper
	{
		typedef 	NullType 			result_type;
	};

	template<class U>
	struct interval_result_type_helper<U, true>
	{
		typedef 	typename U::result_type 		result_type;
	};


public:
	//类型判定
	enum {isArray = arrayTraits<T>::result};						//是否是数组类型
	enum {lenArray = arrayTraits<T>::len};							//数组类型的有效长度

	enum {isVoid = VoidTraits<T>::result};							//是否是 void 类型
	enum {isConst = ConstTraits<T>::result};						//是否是 const 修饰的类型
	enum {isClass = classTraits::result };							//是否是 class, struct ,union 这样的类型
	enum { isPointer = PointerTraits<T>::result};					//是否是 指针类型
	enum { isReference = ReferenceTraits<T>::result };				//是否是引用类型
	enum { isMemPointer = MemPointerTraits<T>::result};				//是否是成员指针
	enum { isConstRef = ConstRefTraits<T>::result };				//是否是常引用
	enum { isStdIntegral = IndexOf<SignedInts, T>::value >= 0};		//有符号基本类型	
	enum { isStdUnsigned = IndexOf<UnsignedInts, T>::value >= 0};	//无符号基本类型
	enum { isStdFloat = IndexOf<Floats, T>::value >= 0};			//浮点类型
	enum { isStdType = isStdIntegral || isStdUnsigned || isStdFloat}; //除 void 外的 基本类型， 包括 有符号，无符号 ，浮点类型 ,char, w_char_t 类型
	enum {isStdArith = isStdIntegral || isStdFloat};				//算数类型，  可进行 +-*/ 的类型
	enum {isEmptyType = EmptyTraits<T>::result};					//侦测是否是  class EmptyType {};
	enum {isFunction = FunTraits<T>::isFun};						//侦测函数类型 
	enum {isMemFunction = MemFunTraits<T>::isMemFun};				//侦测成员函数类型
	enum {isString = StrTraits<T>::result};					//是否是字符串类型 (std::string)
	enum {isEnum = !isArray 	&&
	     		   !isVoid  	&&
				   !isClass 	&& 
				   !isPointer	&&
				   !isReference &&
				   !isMemPointer&&
				   !isStdType   &&
				   !isFunction 	&&
				   !isMemFunction };									//枚举类型

	enum {has_result_type = has_interval_type<T>::value};    	//是否存在内置类型
	
	/*
		const 类型判断
		如果 T 是 const T,  返回  const T
		如果 T 是		T,	返回		T
	*/
	typedef		typename ConstTraits<T>::ConstType		ConstType;

	/*
	 *	const  移除
	 */
	typedef 	typename ConstTraits<T>::RemoveConstType RemoveConst;
		
	//从 数组类型转化成  指针类型,   例 char[5]  ==>  char*
	typedef		typename arrayTraits<T>::type			arrayToPtrType;
	//从 数组类型转化成  常指针类型,   例 char[5]  ==>  const char*
	typedef		typename arrayTraits<T>::const_type		arrayToConPtrType;
	//提取数组的原始类型
	typedef		typename arrayTraits<T>::original_type	arrayOriginalType;

	//指针类型萃取
	typedef		typename PointerTraits<T>::PointerType	PointerType;
	////引用类型萃取
	typedef		typename ReferenceTraits<T>::ReferenceType	ReferenceType;
	//常引用类型
	typedef		typename ReferenceTraits<T>::ConReferenceType	ConReferenceType;

	//引用移除
	typedef 	typename ReferenceTraits<T>::RemoveRefType	RemoveRef;
	
	
	typedef typename Select<isStdArith || isPointer || isMemPointer, T, const T&>::Result		CommonParamType;
	typedef typename Select<isStdArith || isPointer || isMemPointer, T, T&>::Result				NCCommonParamType;

	//参数类型
	typedef typename Select<isArray, arrayToConPtrType, CommonParamType>::Result	ParamterType;
	typedef typename Select<isArray, arrayToPtrType, NCCommonParamType>::Result		NCParamterType;
	/*一般函数指针的返回值类型和参数类型*/
	typedef  typename FunTraits<T>::ResultType		FunResultType;

	/*成员函数指针的返回值类型和参数类型*/
	typedef		typename MemFunTraits<T>::ResultType	MemFunResultType;

	/*移除类型的 const 和 引用修饰*/
	typedef 	typename ConstRefTraits<T>::RemoveConstRefType		RemoveModified;

	//返回值类型萃取
	typedef 	typename interval_result_type_helper<T, has_result_type>::result_type 	result_type;
};

template<class T>
void printType(std::string type = "")
{
	using namespace QTH_NAME_SPACE;
	using std::cout;
	using std::endl;
	cout << "----------类型检测 " << type.c_str() << "--------------" << endl;
	cout << "是否是基本类型	: " << TypeTraits<T>::isStdType << endl;
	cout << "是否是void		：" << TypeTraits<T>::isVoid << endl;
	cout << "是否是class or union：" << TypeTraits<T>::isClass << endl;
	cout << "是否是 枚举: "		<< TypeTraits<T>::isEnum << endl;
	cout << "是否是指针： "		<< TypeTraits<T>::isPointer << endl;
	cout << "是否是成员指针："  << TypeTraits<T>::isMemPointer << endl;
	cout << "是否是函数："		<< TypeTraits<T>::isFunction << endl;
	cout << "是否是成员函数: " 	<< TypeTraits<T>::isMemFunction << endl;
	cout << "是否是引用 : "		<< TypeTraits<T>::isReference << endl;
	cout << "是否是字符串 :"	<< TypeTraits<T>::isString << endl;
	cout << "是否是数组 :"		<< TypeTraits<T>::isArray << endl;
	cout << endl;
}


END_NAMESPACE
#endif
