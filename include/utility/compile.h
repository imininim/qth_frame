/********************************
author:		qutianhao
function:	一些编译期间的工具类
date:		2016/8/15
tips:
*********************************/

#ifndef COMPILE_H_
#define COMPILE_H_
struct NullType;

/********************************
function:	Int2Type
tips:
*********************************/
BEGIN_NAMESPACE

template<int i>
struct Int2Type
{
	enum {value = i};
};


/********************************
function:	Type2Type
tips:
*********************************/
template<class T>
struct Type2Type
{
	typedef T	OriginalType;
};


/********************************
function:	Select
tips:
	编译期间2选一
	Select<sizeof(T) > sizeof(U), T, U>::Result
*********************************/
template<bool flags, class T, class U>
struct Select
{
	typedef T	Result;
};

template<class T, class U>
struct Select<false, T, U>
{
	typedef U	Result;
};


/********************************
function:	STATIC_CHECK(expr, msg)
tips:
	编译期静态 assert
	表达式  expr 为  true, 程序可执行编译
	表达式  expr 为  false, 程序不可通过编译， 编译错误信息 msg ， msg 不是一个字符串

useing:
	STATIC_CHECK(expr, 哈哈)
注意: 
	1. 参数 哈哈 不应该是 "哈哈"
*********************************/
template<bool>struct CompileTimeCheck;
template<> struct CompileTimeCheck<true>{};


#define STATIC_CHECK(expr, msg)   \
{   \
	class msg{};   \
	QTH_NAME_SPACE::CompileTimeCheck<(expr)> msg; (void)msg;  \
}

/********************************
function:	is_base_of<base,derived>
tips:
	编译期间继承性检测
useing:
	is_base_of<base,derived>::value   如果 base 是 derived 的基类，返回true, 否则返回 false
*********************************/
template<class T, class U>
class Conversion
{
	typedef		char	Small;
	class		Big		{char dummy[2];};

	static T* MakeT();

	static Small Test(U*);
	static Big	 Test(...);

public:
	enum {exists = sizeof(Test(MakeT())) == sizeof(Small)};
	enum {exists2Way = exists && Conversion<U, T>::exists};
	enum {sameType = false};
};

template<class T>
class Conversion<T,T>
{
public:
	enum {exists = 1, exists2Way = 1, sameType = 1};
};


template<typename base, typename derived>
struct is_base_of
{
	enum {value = Conversion<derived, base>::exists};
};

/********************************
function:	is_same_type<T,U>::value
tips:
	判定类型 T ,U 是否是同一种类型
useing:
	is_base_of<base,derived>::value    ====    true or false
*********************************/
template<class T, class U>
struct is_same_type
{
	enum {value = false};
};

template<class T>
struct is_same_type<T,T>
{
	enum {value = true};
};


/********************************
function:	侦测类类型
tips:
	针对类型 T   有   is_class<T>::value   ,  true 表示是类类型， false  不是类类型
*********************************/
template<class T>
class is_class
{
	typedef 	char 	Small;
	typedef 	struct {char a[2]; } 	Big;

	template<class C>
	static Small test(int C::*);
	
	template<class C>
	static Big   test(...);
public:
	enum {value = sizeof(test<T>(0) ) == sizeof(Small)  };

};

/********************************
function:	侦测 result_type 内置类型
tips:
	针对类 T  有 has_interval_type<T>::value,  true   存在类型 result_type   ,   false 不存在类型 result_type
*********************************/
template<class T>
class has_interval_type
{
	typedef 	char 				Small;
	typedef 	struct{char a[2];}	Big;

	template<class U> static Small test(typename U::result_type const *);
	template<class U> static Big   test(...);
	public:
	enum {value = sizeof(test<T>(0) ) == sizeof(Small) };
};

//类型数据大小计算接口
class size_info
{
public:
	virtual size_t size() const			= 0;
};

//指定类型 T 类型检测, 检测结果 0 继承于 size_info的类
//								1 字符串 std::string
//								2 其他类型
template<class T>
struct size_type
{
	enum {
		////继承于size_info
		is_base   = is_base_of<size_info, T>::value ,		
		
		//字符串类
		is_string = is_same_type<T,std::string>::value			|| \
					is_same_type<T,const std::string>::value	|| \
					is_same_type<T,std::string&>::value			|| \
					is_same_type<T,const std::string&>::value,
		
		//其他所有类型
		is_other,											

		//类型结果
		result = is_base ? 0 : (is_string ? 1 : 2),
		};
};

/********************************
function:	动态类型大小计算
			如果类型 T 是一个继承于 size_info 的类对象， 则调用虚函数size 获取对象的大小
			否则调用 sizeof(T) 计算类型的大小
			如果 T 是std::string, 则返回 字符数据的长度的大小 + 包括nul的字符数量
*********************************/
template<class T>
size_t size_of(const T& p)
{
	return size_of_help(p, Int2Type<size_type<T>::result>());
}

template<class T>
size_t size_of_help(const T& p, Int2Type<0>)	//继承于size_info的大小处理
{
	return p.size();
}

template<class T>
size_t size_of_help(const T& p, Int2Type<1>)	//std::string的大小处理
{
	return sizeof(size_t) + p.size() + sizeof(char);
}

template<class T>
size_t size_of_help(const T& p, Int2Type<2>)	
{
	return sizeof(p);
}
END_NAMESPACE

#endif

