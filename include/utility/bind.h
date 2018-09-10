/********************************
function:	bind
date:		2016/09/21
tips:		搭配 function 一起使用，实现函数绑定功能，类似于 tr1::bind
*********************************/
#ifndef	BIND_H_
#define BIND_H_
BEGIN_NAMESPACE

//短式声明
template<class T>
struct Holder;

template<class TList, template<class> class Unit>
class tuple_base;

template<class R>
struct function;

template<class T>
struct Deleter;

template<class T, template<class> class Del>
class shared_ptr;


/*
	bind 占位符定义
*/
namespace placeholders
{
	extern QTH_UTILITY_EXPORT Int2Type<0>			_1;
	extern QTH_UTILITY_EXPORT Int2Type<1>			_2;
	extern QTH_UTILITY_EXPORT Int2Type<2>			_3;
	extern QTH_UTILITY_EXPORT Int2Type<3>			_4;
	extern QTH_UTILITY_EXPORT Int2Type<4>			_5;
	extern QTH_UTILITY_EXPORT Int2Type<5>			_6;
	extern QTH_UTILITY_EXPORT Int2Type<6>			_7;
	extern QTH_UTILITY_EXPORT Int2Type<7>			_8;
	extern QTH_UTILITY_EXPORT Int2Type<8>			_9;
	extern QTH_UTILITY_EXPORT Int2Type<9>			_10;
	extern QTH_UTILITY_EXPORT Int2Type<10>			_11;
}

/*
	引用包装类
*/
template<class T>
class reference_wrapper
{
public:
	typedef		T		type;

	explicit reference_wrapper(T& t): pointee(&t)	{}
	operator T&() const							{ return *pointee; }
	
	T& get() const								{return *pointee; }
	T* get_pointer() const						{ return pointee; }
private:
	T* pointee;
};

template<class T>
reference_wrapper<T> ref(T& t)
{
	return reference_wrapper<T>(t);
}

template<class T>
reference_wrapper<const T> cref(const T& t)
{
	return reference_wrapper<const T>(t);
}


/*
	绑定参数列表
*/
template<class R, class TList>
class bindList
{
public:
	explicit bindList(const shared_ptr<tuple_base<TList> >& bl): bl_(bl){}

	template<class Fun, class callList>
	R operator()(Fun& f, callList& cl)
	{
		return doOp(f,cl, Int2Type<len>(), Int2Type<TypeTraits<Fun>::isMemFunction>() );
	}

private:	
	/*空参数处理*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, const callList& cl, Int2Type<0>, Int2Type<false>)		
	{ return f();}

	/*1个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<1>, Int2Type<false>)		
	{return f(cl[get<0>(*bl_)] );}

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<1>, Int2Type<true>)		
	{
		return doOpAddressOrObj1(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj1(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)();
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj1(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)();
	}

	/*2个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<2>, Int2Type<false>)		
	{return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)]);}

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<2>, Int2Type<true>)		
	{
		return doOpAddressOrObj2(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj2(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj2(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)]);
	}

	//**3个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<3>, Int2Type<false>)		
	{ return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)]);}

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<3>, Int2Type<true>)		
	{
		return doOpAddressOrObj3(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj3(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj3(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)]);
	}


	/*4个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<4>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)]); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<4>, Int2Type<true>)		
	{
		return doOpAddressOrObj4(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj4(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj4(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)]);
	}

	/*5个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<5>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)]); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<5>, Int2Type<true>)		
	{
		return doOpAddressOrObj5(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj5(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj5(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)]);
	}

	/*6个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<6>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)]); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<6>, Int2Type<true>)		
	{
		return doOpAddressOrObj6(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj6(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj6(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)]);
	}

	/*7个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<7>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)], 
								 cl[get<6>(*bl_)]); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<7>, Int2Type<true>)		
	{
		return doOpAddressOrObj7(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj7(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],cl[get<6>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj7(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],cl[get<6>(*bl_)]);
	}
	
	/*8个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<8>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)], 
								 cl[get<6>(*bl_)], cl[get<7>(*bl_)]); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<8>, Int2Type<true>)		
	{
		return doOpAddressOrObj8(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj8(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj8(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)]);
	}

	/*9个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<9>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)], 
	cl[get<6>(*bl_)],cl[get<7>(*bl_)], cl[get<8>(*bl_)] ); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<9>, Int2Type<true>)		
	{
		return doOpAddressOrObj9(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj9(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj9(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)]);
	}

	/*10个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<10>, Int2Type<false>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)], 
	cl[get<6>(*bl_)],cl[get<7>(*bl_)], cl[get<8>(*bl_)], cl[get<9>(*bl_)] ); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<10>, Int2Type<true>)		
	{
		return doOpAddressOrObj10(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj10(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)],cl[get<8>(*bl_)],cl[get<9>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj10(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)],cl[get<8>(*bl_)],cl[get<9>(*bl_)]);
	}


	/*11个参数*/
	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<11>)		
	{  return f(cl[get<0>(*bl_)], cl[get<1>(*bl_)], cl[get<2>(*bl_)], cl[get<3>(*bl_)], cl[get<4>(*bl_)], cl[get<5>(*bl_)], 
	cl[get<6>(*bl_)],cl[get<7>(*bl_)], cl[get<8>(*bl_)], cl[get<9>(*bl_)], cl[get<10>(*bl_)] ); }

	template<class Fun, class callList>
	inline R doOp(Fun& f, callList& cl, Int2Type<11>, Int2Type<true>)		
	{
		return doOpAddressOrObj11(f, cl, Int2Type<TypeTraits<typename TList::Head>::isPointer>());
	}

	template<class Fun, class callList>
	inline R doOpAddressOrObj11(Fun& f, callList& cl, Int2Type<true>)
	{
		return (*(cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)],cl[get<8>(*bl_)],cl[get<9>(*bl_)],cl[get<10>(*bl_)]);
	}
	template<class Fun, class callList>
	inline R doOpAddressOrObj11(Fun& f, callList& cl, Int2Type<false>)
	{
		return ((cl[get<0>(*bl_)]).*f)(cl[get<1>(*bl_)],cl[get<2>(*bl_)],cl[get<3>(*bl_)],cl[get<4>(*bl_)],cl[get<5>(*bl_)],
			cl[get<6>(*bl_)],cl[get<7>(*bl_)],cl[get<8>(*bl_)],cl[get<8>(*bl_)],cl[get<9>(*bl_)],cl[get<10>(*bl_)]);
	}

	enum {len = Length<TList>::value};								//绑定参数个数
	shared_ptr<tuple_base<TList> > bl_;								//存储绑定参数
};

/*
	调用参数表
*/
template<class TList>
struct CallList
{
public:
	explicit CallList(tuple_base<TList>& cl): cl_(cl){}

	template<class T>
	T& operator[](T& t)
	{
		return t;
	}

	template<int i>
	typename TypeAt<TList, i>::Result& operator[](Int2Type<i>)
	{
		return get<i>(cl_);
	}

private:
	tuple_base<TList>& cl_;			//调用参数列表
};

/* 绑定仿函数对象*/
template<class Func, class R, class BL>
struct binder
{
	binder(){}
	binder(const Func& f, const BL& bl): bl_(bl), f_(f){}
	/*空参数*/
	R operator()()
	{
		tuple_base<TYPELIST_0()> tpl;
		CallList<TYPELIST_0()> list(tpl);
		return bl_(f_, list);
	}

	/*单参数*/
	template<class P1>
	R operator()(P1& p1)
	{  
		tuple_base<TYPELIST_1(P1&)> tu(p1); 
		CallList<TYPELIST_1(P1&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*2个参数*/
	template<class P1, class P2>
	R operator()(P1& p1, P2& p2)
	{ 
		tuple_base<TYPELIST_2(P1&,P2&)> tu(p1,p2); 
		CallList<TYPELIST_2(P1&,P2&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*3个参数*/
	template<class P1, class P2, class P3>
	R operator()(P1& p1, P2& p2, P3& p3)
	{ 
		tuple_base<TYPELIST_3(P1&,P2&,P3&)> tu(p1,p2,p3); 
		CallList<TYPELIST_3(P1&,P2&,P3&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*4个参数*/
	template<class P1, class P2, class P3, class P4>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4)
	{ 
		tuple_base<TYPELIST_4(P1&,P2&,P3&,P4&)> tu(p1,p2,p3,p4); 
		CallList<TYPELIST_4(P1&,P2&,P3&,P4&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*5个参数*/
	template<class P1, class P2, class P3, class P4, class P5>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5)
	{ 
		tuple_base<TYPELIST_5(P1&,P2&,P3&,P4&,P5&)> tu(p1,p2,p3,p4,p5); 
		CallList<TYPELIST_5(P1&,P2&,P3&,P4&,P5&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*6个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6)
	{ 
		tuple_base<TYPELIST_6(P1&,P2&,P3&,P4&,P5&,P6&)> tu(p1,p2,p3,p4,p5,p6); 
		CallList<TYPELIST_6(P1&,P2&,P3&,P4&,P5&,P6&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*7个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7)
	{ 
		tuple_base<TYPELIST_7(P1&,P2&,P3&,P4&,P5&,P6&,P7&)> tu(p1,p2,p3,p4,p5,p6,p7); 
		CallList<TYPELIST_7(P1&,P2&,P3&,P4&,P5&,P6&,P7&)> cl(tu);
		return	bl_(f_, cl);
	}


	///*8个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8)
	{ 
		tuple_base<TYPELIST_8(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&)> tu(p1,p2,p3,p4,p5,p6,p7,p8); 
		CallList<TYPELIST_8(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*9个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8, P9& p9)
	{ 
		tuple_base<TYPELIST_9(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&)> tu(p1,p2,p3,p4,p5,p6,p7,p8,p9); 
		CallList<TYPELIST_9(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*10个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8, P9& p9, P10& p10)
	{ 
		tuple_base<TYPELIST_10(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&,P10&)> tu(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10); 
		CallList<TYPELIST_10(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&,P10&)> cl(tu);
		return	bl_(f_, cl);
	}

	///*11个参数*/
	template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
	R operator()(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8, P9& p9, P10& p10, P11& p11)
	{ 
		tuple_base<TYPELIST_11(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&,P10&,P11&)> tu(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11); 
		CallList<TYPELIST_11(P1&,P2&,P3&,P4&,P5&,P6&,P7&,P8&,P9&,P10&,P11&)> cl(tu);
		return	bl_(f_, cl);
	}

private:
	BL bl_;				//持有一个绑定参数列表
	Func f_;				//可调用对象
};



template<class T>
struct ResultTraits
{
private:
	typedef 	typename TypeTraits<T>::result_type																						Tmp1;
	typedef 	typename Select<functionTraits<T>::isObjFun, 	typename functionTraits<T>::result_type,		Tmp1>::Result			Tmp2;
	typedef 	typename Select<TypeTraits<T>::isMemFunction,	typename TypeTraits<T>::MemFunResultType,		Tmp2>::Result			Tmp3;
	typedef 	typename Select<TypeTraits<T>::isFunction,		typename TypeTraits<T>::FunResultType,			Tmp3>::Result 			Tmp4;

public:
	typedef 	Tmp4		Result;
	
};

/*
	bind 接口
*/
template<class Fun>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_0()> >
	bind(Fun fun)
{
	typedef		TYPELIST_0()										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>  BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>());
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

/*bind一般函数*/

template<class Fun, class P1>
binder<Fun,typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_1(P1)> >
	bind(Fun fun, const P1& p1)
{
	typedef		TYPELIST_1(P1)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList> BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_2(P1,P2)> >
	bind(Fun fun, const P1& p1, const P2& p2)
{
	typedef		TYPELIST_2(P1,P2)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList> BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2));
	BL bindlist(pTu);
	return binder<Fun,typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}
	
template<class Fun, class P1, class P2, class P3>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_3(P1,P2,P3)> >
	bind(Fun fun, const P1& p1, const P2& p2, const P3& p3)
{
	typedef		TYPELIST_3(P1,P2,P3)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
	
}


template<class Fun, class P1, class P2, class P3, class P4>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_4(P1,P2,P3,P4)> >
	bind(Fun fun, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
{
	typedef		TYPELIST_4(P1,P2,P3,P4)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;
	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_5(P1,P2,P3,P4,P5)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	typedef		TYPELIST_5(P1,P2,P3,P4,P5)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}


template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_6(P1,P2,P3,P4,P5,P6)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	typedef		TYPELIST_6(P1,P2,P3,P4,P5,P6)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6, class P7>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_7(P1,P2,P3,P4,P5,P6,P7)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
{
	typedef		TYPELIST_7(P1,P2,P3,P4,P5,P6,P7)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6,p7));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6, class P7, class P8>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_8(P1,P2,P3,P4,P5,P6,P7,P8)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
{
	typedef		TYPELIST_8(P1,P2,P3,P4,P5,P6,P7,P8)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6,p7,p8));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6, class P7, class P8, class P9>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
{
	typedef		TYPELIST_9(P1,P2,P3,P4,P5,P6,P7,P8,P9)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6,p7,p8,p9));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6, class P7, class P8, class P9, class P10>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_10(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
{
	typedef		TYPELIST_10(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6,p7,p8,p9, p10));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}

template<class Fun, class P1, class P2, class P3, class P4, class P5,class P6, class P7, class P8, class P9, class P10,class P11>
binder<Fun, typename ResultTraits<Fun>::Result, bindList<typename ResultTraits<Fun>::Result, TYPELIST_11(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11)> >
	bind(Fun fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)
{
	typedef		TYPELIST_11(P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11)										TList;
	typedef		bindList<typename ResultTraits<Fun>::Result,TList>			BL;

	shared_ptr<tuple_base<TList> > pTu(new tuple_base<TList>(p1,p2,p3,p4,p5,p6,p7,p8,p9, p10, p11));
	BL bindlist(pTu);
	return binder<Fun, typename ResultTraits<Fun>::Result, BL>(fun, bindlist);
}


END_NAMESPACE
#endif
