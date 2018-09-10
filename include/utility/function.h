/********************************
function:	可调用对象包装
date:		2016/09/18
tips:		
		对于 functor.h 的包装， 实现tr1/function, boost/function 的使用方式
use:
     //一般函数
		void free(int i);
		function<void(int)> f(&free);    f(1) ==> 调用 free(1);

	//成员函数
		void class A::free(int i);
		A* pA = new A();
		function<void(int)> memf(&A::free, pA);
		memf(2)			==>		pA->free(2);
*********************************/
#ifndef function_H_
#define function_H_
#include "utility/functor.h"
#include "utility/typetraits.h"
BEGIN_NAMESPACE

	template<class R = void()>
	struct function;

	/* 空参数包装*/
	template<class R>
	struct function<R()>: public Functor<R>
	{
		typedef Functor<R> FBase;
		typedef		R			result_type;

		//构造
		function(): FBase(){}

		//拷贝构造
		function(const function& func): FBase()
		{
			if (!func.empty())
					FBase::operator=(func);
		}

		template<class Func>
		function(const Func& func): FBase(func){}
		template<class Func, class Host>
		function(const Func& func, const Host& host): FBase(func, host){}

		operator bool() const		{ return !FBase::empty();}
	};
	
#define		FUNCTION_BODY \
	function(): FBase(){}									\
	function(const function& func):FBase()					\
	{														\
		if (!func.empty())									\
			FBase::operator=(func);							\
	}														\
	template<class Func>									\
	function(Func func): FBase(func){}						\
															\
	template<class Func, class Host>						\
	function(const Func& func, const Host& host): FBase(func, host){}	\
	operator bool() const		{ return !FBase::empty();}
	
///////////////////////////////////////////////////////////////////////////////////
//以下，对于不同参数个数进行分别特
	
	/*
		针对 void(void)
	*/
	template<>
	struct function<> : public Functor<>
	{
		typedef Functor<>			FBase;
		typedef		void			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1) 
	*/
	template<class R, class P1>
	struct function<R(P1)>: public Functor<R, TYPELIST_1(P1)>
	{
		typedef Functor<R, TYPELIST_1(P1)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2) 
	*/
	template<class R, class P1, class P2>
	struct function<R(P1, P2)>: public Functor<R, TYPELIST_2(P1, P2)>
	{
		typedef Functor<R, TYPELIST_2(P1, P2)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};


	/*
		针对 R(P1, P2, P3) 
	*/
	template<class R, class P1, class P2, class P3>
	struct function<R(P1, P2, P3)>: public Functor<R, TYPELIST_3(P1, P2, P3)>
	{
		typedef Functor<R, TYPELIST_3(P1, P2, P3)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4) 
	*/
	template<class R, class P1, class P2, class P3, class P4>
	struct function<R(P1, P2, P3, P4)>: public Functor<R, TYPELIST_4(P1, P2, P3, P4)>
	{
		typedef Functor<R, TYPELIST_4(P1, P2, P3, P4)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5>
	struct function<R(P1, P2, P3, P4, P5)>: public Functor<R, TYPELIST_5(P1, P2, P3, P4, P5)>
	{
		typedef Functor<R, TYPELIST_5(P1, P2, P3, P4, P5)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5, P6) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
	struct function<R(P1, P2, P3, P4, P5, P6)>: public Functor<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
	{
		typedef Functor<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5, P6, P7) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	struct function<R(P1, P2, P3, P4, P5, P6, P7)>: public Functor<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
	{
		typedef Functor<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5, P6, P7, P8) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	struct function<R(P1, P2, P3, P4, P5, P6, P7, P8)>: public Functor<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
	{
		typedef Functor<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5, P6, P7, P8, P9) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
	struct function<R(P1, P2, P3, P4, P5, P6, P7, P8, P9)>: public Functor<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
	{
		typedef Functor<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*
		针对 R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
	struct function<R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>: public Functor<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
	{
		typedef Functor<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

		/*
		针对 R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10,P11) 
	*/
	template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
	struct function<R(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>: public Functor<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>
	{
		typedef Functor<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>  FBase;
		typedef		R			result_type;
		FUNCTION_BODY
	};

	/*function 参数类型萃取*/
	template<class F>
	struct functionTraits
	{
		enum {isObjFun = false};
		typedef		NullType	result_type;
	};

	template<>
	struct functionTraits< function<> >
	{
		enum {isObjFun = true};
		typedef		void	result_type;
	};

	template<class R>
	struct functionTraits<function<R(void)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
	};

	template<class R, class P1>
	struct functionTraits<function<R(P1)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
	};

	template<class R, class P1,class P2>
	struct functionTraits<function<R(P1,P2)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
	};

	template<class R, class P1,class P2, class P3>
	struct functionTraits<function<R(P1,P2,P3)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
	};

	template<class R, class P1,class P2, class P3, class P4>
	struct functionTraits<function<R(P1,P2,P3,P4)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5>
	struct functionTraits<function<R(P1,P2,P3,P4,P5)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6, class P7>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6,P7)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6, class P7,class P8>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6,P7,P8)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6, class P7,class P8,class P9>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6,P7,P8,P9)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P9		Param9;
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6, class P7,class P8,class P9,class P10>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
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
	};

	template<class R, class P1,class P2, class P3, class P4, class P5, class P6, class P7,class P8,class P9,class P10, class P11>
	struct functionTraits<function<R(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11)> >
	{
		enum {isObjFun = true};
		typedef		R		result_type;
		typedef		P1		Param1;
		typedef		P2		Param2;
		typedef		P3		Param3;
		typedef		P4		Param4;
		typedef		P5		Param5;
		typedef		P6		Param6;
		typedef		P7		Param7;
		typedef		P8		Param8;
		typedef		P10		Param10;
		typedef		P11		Param11;
	};

END_NAMESPACE

#endif

