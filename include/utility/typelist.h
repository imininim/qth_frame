/********************************
author:		qutianhao
function:	类型列表
date:		2016/8/24
tips:
*********************************/
#ifndef TYPELIST_H_
#define TYPELIST_H_
#include "utility/premacro.h"
#include "utility/compile.h"

/********************************
function:	NullType 和 EmptyType
*********************************/

//NullType 作为TypeList 边界符， 当达到TypeList的边界时，使编译停止下来
struct NullType;

//EmptyType 作为 TypeList 特殊边界符，处理越界操作，编译器遇到他不会停止编译
struct EmptyType
{};

//检测 NullType 和 Emptype

template<class T>
struct NullTypeCheck
{
	enum {isNull = false};
	enum {isEmpty = false};
};

template<>
struct NullTypeCheck<NullType>
{
	enum {isNull = true};
	enum {isEmpty = false};
};


template<>
struct NullTypeCheck<EmptyType>
{
	enum {isNull = false};
	enum {isEmpty = true};
};


/********************************
		类型列表定义
*********************************/
template<class T, class U>
struct TypeList
{
	typedef	 T		Head;
	typedef	 U		Tail;
};


#define TYPELIST_0()													NullType
#define TYPELIST_1(p1)													TypeList<p1, TYPELIST_0()>
#define TYPELIST_2(p1, p2)												TypeList<p1, TYPELIST_1(p2)>
#define TYPELIST_3(p1, p2, p3)											TypeList<p1, TYPELIST_2(p2, p3)>
#define TYPELIST_4(p1, p2, p3, p4)										TypeList<p1, TYPELIST_3(p2, p3, p4)>
#define TYPELIST_5(p1, p2, p3, p4, p5)									TypeList<p1, TYPELIST_4(p2, p3, p4, p5)>
#define TYPELIST_6(p1, p2, p3, p4, p5, p6)								TypeList<p1, TYPELIST_5(p2, p3, p4, p5, p6)>
#define TYPELIST_7(p1, p2, p3, p4, p5, p6, p7)							TypeList<p1, TYPELIST_6(p2, p3, p4, p5, p6, p7)>
#define TYPELIST_8(p1, p2, p3, p4, p5, p6, p7, p8)						TypeList<p1, TYPELIST_7(p2, p3, p4, p5, p6, p7, p8)>
#define TYPELIST_9(p1, p2, p3, p4, p5, p6, p7, p8, p9)					TypeList<p1, TYPELIST_8(p2, p3, p4, p5, p6, p7, p8, p9)>


#define TYPELIST_10(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)			TypeList<p1, TYPELIST_9(p2, p3, p4, p5, p6, p7, p8, p9, p10)>
#define TYPELIST_11(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)		TypeList<p1, TYPELIST_10(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)>
#define TYPELIST_12(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)	TypeList<p1, TYPELIST_11(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)>

#define TYPELIST_13(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)	\
		TypeList<p1, TYPELIST_12(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)>

#define TYPELIST_14(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)	\
	TypeList<p1, TYPELIST_13(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)>

#define TYPELIST_15(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15)	\
	TypeList<p1, TYPELIST_14(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15)>

#define TYPELIST_16(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)	\
	TypeList<p1, TYPELIST_15(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)>

#define TYPELIST_17(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17)	\
	TypeList<p1, TYPELIST_16(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17)>

#define TYPELIST_18(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18)	\
	TypeList<p1, TYPELIST_17(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18)>

#define TYPELIST_19(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19)	\
	TypeList<p1, TYPELIST_18(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19)>

#define TYPELIST_20(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20)	\
	TypeList<p1, TYPELIST_19(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20)>

#define TYPELIST_21(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21)	\
	TypeList<p1, TYPELIST_20(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21)>

#define TYPELIST_22(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22)	\
	TypeList<p1, TYPELIST_21(p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22)>

/*******************
类型宏重载
	TYPELIST_0()		==> NullType
	TYPELIST(p1)		==> TYPELIST_1(p1)
	TYPELIST(p1,p2)		==> TYPELIST_2(p1,p2)
	...

********************/
#define TYPELIST(...) INVOKE_ARG_MACRO(TYPELIST_, __VA_ARGS__)

/********************************
function:		计算长度 
tips:
				Length<TYPELIST_2(int,char)>::value     ===>   2 
return:
				Typelist 长度
*********************************/
template<class TList>
struct Length;

template<>
struct Length<NullType>
{
	enum {value = 0};
};

template<class T, class U>
struct Length<TypeList<T, U> >
{
	enum{value = 1 + Length<U>::value };
};

/********************************
function:		索引访问 
tips:
				TypeAt<TYPELIST_2(char, int), 0>::Result	===>     char
return:
				返回指定索引位置的类型
*********************************/
template<class TList, unsigned int i>
struct TypeAt;

template<class Head, class Tail>
struct TypeAt<TypeList<Head,Tail>, 0>
{
	typedef			Head		Result;
};

template<class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i>
{
	typedef typename TypeAt<Tail, i - 1>::Result	Result;
};


/********************************
function:		索引访问(越界检测) 
tips:
				TypeAtNonStrict<TYPELIST_2(char, int), 13, double>::Result	===>  double     
return:
				返回指定索引位置的类型, 如果没有，返回默认类型
*********************************/
template<class TList, unsigned int i, class DefaultType = EmptyType>
struct TypeAtNonStrict
{
	typedef DefaultType			Result;
};

template<class Head, class Tail, class DefaultType>
struct TypeAtNonStrict<TypeList<Head, Tail>, 0,	DefaultType >
{
	typedef		Head	Result;
};

template<class Head, class Tail, unsigned int i, class DefaultType>
struct TypeAtNonStrict<TypeList<Head, Tail>, i, DefaultType>
{
	typedef typename TypeAtNonStrict<Tail, i- 1, DefaultType>::Result		Result;
};

/********************************
function:		类型所在位置 
tips:
			IndexOf<TYPELIST_2(int,char), char>::value   ==>  1 	
return:
			返回指定类型在TypeList中的 位置
*********************************/
template<class TList, class T>
struct IndexOf;

template<class T>
struct IndexOf<NullType, T>
{
	enum {value = -1};
};

template<class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T>
{
	enum {value = 0};
};


template<class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T>
{
private:
	enum {temp = IndexOf<Tail, T>::value};
public:
	enum {value = (temp == -1 ? -1 : 1 + temp)};
};


/********************************
function:		删除类型 
tips:
			Erase<TYPELIST_2(int,char), char>::Result	===>  TYPELIST_1(int)
return:
			删除指定类型， 返回删除后的TypeList
*********************************/
template<class TList, class T>
struct Erase;

template<class T>
struct Erase<NullType, T>
{
	typedef NullType	Result;
};

template<class T, class	Tail>
struct Erase<TypeList<T, Tail>, T>
{
	typedef		Tail	Result;
};

template<class Head, class Tail, class T>
struct Erase<TypeList<Head, Tail>, T>
{
	typedef		TypeList<Head, typename Erase<Tail, T>::Result> 	Result;
};

/********************************
function:		替换 
tips:
				Replace<TYPELIST_2(int,char), char, double>::Result  ===>     TYPELIST_2(int,double)			
return:
				把TypeList 中第一个遇到的指定类型替换成另外一种类型 
*********************************/
template<class TList, class T, class U>
struct Replace;

template<class T, class U>
struct Replace<NullType, T, U>
{
	typedef		NullType	Result;
};

template<class T, class Tail, class U>
struct Replace<TypeList<T, Tail>, T, U>
{
	typedef		TypeList<U, Tail>	Result;
};

template<class Head, class Tail, class T, class U>
struct Replace<TypeList<Head,Tail>, T, U>
{
	typedef TypeList<Head, typename Replace<Tail, T, U>::Result >	Result;
};


#endif

