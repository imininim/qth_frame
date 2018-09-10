/********************************
function:	可调用对象实现
date:		2016/09/13
tips:		
		包装函数指针、仿函数和成员指针，并可以一致的处理他们, 最大支持11个参数
*********************************/
#ifndef FUNCTOR_H_
#define FUNCTOR_H_
#include "utility/typelist.h"
#include "utility/SmallObj.h"
BEGIN_NAMESPACE

/*可调用对象指向实现的接口*/
template<class R, class TList>
class FunctorImp;

/*
	特化版：
		无参数 的可调用对象的实现接口
*/
template<class R>
class FunctorImp<R, NullType>
{
public:
	typedef R ResultType;

	virtual R operator()() = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}

};

/*
	特化版：
		1个参数 的可调用对象的实现接口
*/
template<class R, class P1>
class FunctorImp<R, TYPELIST_1(P1)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;

	virtual R operator()(Param1) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	特化版：
		2个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2>
class FunctorImp<R, TYPELIST_2(P1, P2)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;

	virtual R operator()(Param1,Param2) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};


/*
	特化版：
		3个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3>
class FunctorImp<R, TYPELIST_3(P1, P2, P3)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;

	virtual R operator()(Param1,Param2,Param3) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	特化版：
		4个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4>
class FunctorImp<R, TYPELIST_4(P1, P2, P3, P4)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;

	virtual R operator()(Param1,Param2,Param3, Param4) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};


/*
	特化版：
		5个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5>
class FunctorImp<R, TYPELIST_5(P1, P2, P3, P4, P5)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	特化版：
		6个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6>
class FunctorImp<R, TYPELIST_6(P1, P2, P3, P4, P5, P6)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};


/*
	特化版：
		7个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class FunctorImp<R, TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;
	typedef P7		Param7;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6, Param7) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	特化版：
		8个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
class FunctorImp<R, TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;
	typedef P7		Param7;
	typedef P8		Param8;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6, Param7, Param8) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};


/*
	特化版：
		9个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
class FunctorImp<R, TYPELIST_9(P1, P2, P3, P4, P5, P6, P7, P8, P9)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;
	typedef P7		Param7;
	typedef P8		Param8;
	typedef P9		Param9;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6, Param7, Param8, Param9) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};


/*
	特化版：
		10个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
class FunctorImp<R, TYPELIST_10(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;
	typedef P7		Param7;
	typedef P8		Param8;
	typedef P9		Param9;
	typedef P10		Param10;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	特化版：
		11个参数 的可调用对象的实现接口
*/
template<class R, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
class FunctorImp<R, TYPELIST_11(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>
{
public:
	typedef R		ResultType;
	typedef P1		Param1;
	typedef P2		Param2;
	typedef P3		Param3;
	typedef P4		Param4;
	typedef P5		Param5;
	typedef P6		Param6;
	typedef P7		Param7;
	typedef P8		Param8;
	typedef P9		Param9;
	typedef P10		Param10;
	typedef P11		Param11;

	virtual R operator()(Param1,Param2,Param3, Param4, Param5, Param6, Param7, Param8, Param9, Param10, Param11) = 0;
	virtual FunctorImp* Clone() const = 0;
	virtual ~FunctorImp(){}
};

/*
	FunctorHandle 针对一般函数、仿函数 的包装处理
	继承自 FunctorImp
*/
template<class ParentFunctor, class Fun>
class FunctorHandle : public ParentFunctor::Impl, public SmallObject<>       
{
	typedef ParentFunctor  Base;
	Fun _f;			//可调用对象
public:
	typedef typename Base::ResultType		ResultType;
	typedef typename Base::Param1			Param1;
	typedef typename Base::Param2			Param2;
	typedef typename Base::Param3			Param3;
	typedef typename Base::Param4			Param4;
	typedef typename Base::Param5			Param5;
	typedef typename Base::Param6			Param6;
	typedef typename Base::Param7			Param7;
	typedef typename Base::Param8			Param8;
	typedef typename Base::Param9			Param9;
	typedef typename Base::Param10			Param10;
	typedef typename Base::Param11			Param11;

	//构造， 接受一个一般函数指针、仿函数对象
	FunctorHandle(const Fun& fun): _f(fun)		{}
	virtual FunctorHandle* Clone(void) const	{return new FunctorHandle(*this);}
	virtual ~FunctorHandle(){}

	ResultType operator()()				{ return _f(); }
	ResultType operator()(Param1 p1)	{ return _f(p1);}

	ResultType operator()(Param1 p1, Param2 p2)
	{ return _f(p1, p2); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3)
	{ return _f(p1, p2, p3); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{ return _f(p1, p2, p3, p4); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
	{ return _f(p1, p2, p3, p4, p5); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6)
	{ return _f(p1, p2, p3, p4, p5, p6); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7)
	{ return _f(p1, p2, p3, p4, p5, p6, p7); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8)
	{ return _f(p1, p2, p3, p4, p5, p6, p7, p8); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param8 p9)
	{ return _f(p1, p2, p3, p4, p5, p6, p7, p8, p9); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param8 p9,
							Param10 p10)
	{ return _f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param8 p9,
		Param10 p10, Param11 p11)
	{ return _f(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
};

/*
	MemFunHandle 成员函数指针的包装处理
	继承自 FunctorImp
*/
template<class ParentFunctor, class PointToObj, class MemFun>
class MemFunHandle:public ParentFunctor::Impl, public SmallObject<>
{
	typedef ParentFunctor  Base;

	PointToObj  _obj;								//调用成员函数的对象
	MemFun	 _memf;									//成员函数可调用对象

public:
	typedef typename Base::ResultType		ResultType;
	typedef typename Base::Param1			Param1;
	typedef typename Base::Param2			Param2;
	typedef typename Base::Param3			Param3;
	typedef typename Base::Param4			Param4;
	typedef typename Base::Param5			Param5;
	typedef typename Base::Param6			Param6;
	typedef typename Base::Param7			Param7;
	typedef typename Base::Param8			Param8;
	typedef typename Base::Param9			Param9;
	typedef typename Base::Param10			Param10;
	typedef typename Base::Param11			Param11;

	MemFunHandle(const PointToObj& obj, const MemFun& fun): _obj(obj), _memf(fun) {}
	virtual MemFunHandle* Clone(void) const { return new MemFunHandle(*this);}
	virtual ~MemFunHandle(){}

	ResultType operator()() 
	{ return ((*_obj).*_memf)(); }

	ResultType operator()(Param1 p1) 
	{ return ((*_obj).*_memf)(p1); }

	ResultType operator()(Param1 p1, Param2 p2) 
	{ return ((*_obj).*_memf)(p1,p2); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3) 
	{ return ((*_obj).*_memf)(p1,p2,p3); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6, p7); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6, p7, p8); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9)
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6, p7, p8, p9); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6, p7, p8, p9, p10); }

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10, Param11 p11) 
	{ return ((*_obj).*_memf)(p1,p2,p3, p4, p5, p6, p7, p8, p9, p10, p11); }
};


/*
	Functor, 可调用对象封装
*/
template<class R = void, class TList = NullType>
class Functor
{
public:
	typedef FunctorImp<R, TList>											Impl;
	typedef R																ResultType;
	typedef TList															ParamList;
	typedef typename TypeAtNonStrict<TList, 0, NullType>::Result			Param1;
	typedef typename TypeAtNonStrict<TList, 1, NullType>::Result			Param2;
	typedef typename TypeAtNonStrict<TList, 2, NullType>::Result			Param3;
	typedef typename TypeAtNonStrict<TList, 3, NullType>::Result			Param4;
	typedef typename TypeAtNonStrict<TList, 4, NullType>::Result			Param5;
	typedef typename TypeAtNonStrict<TList, 5, NullType>::Result			Param6;
	typedef typename TypeAtNonStrict<TList, 6, NullType>::Result			Param7;
	typedef typename TypeAtNonStrict<TList, 7, NullType>::Result			Param8;
	typedef typename TypeAtNonStrict<TList, 8, NullType>::Result			Param9;
	typedef typename TypeAtNonStrict<TList, 9, NullType>::Result			Param10;
	typedef typename TypeAtNonStrict<TList, 10, NullType>::Result			Param11;
	typedef		Param1														Head;
	typedef		typename Erase<TList, Head>::Result							Tail;

	Functor():m_pImpl(0){}
	
	Functor(const Functor& rhs): m_pImpl( (rhs.m_pImpl)->Clone() ){}
	Functor& operator=(const Functor& rhs)
	{
		if (rhs.m_pImpl.get() == NULL)
		{
			m_pImpl.reset(0);
			return *this;
		}
		
		Functor copy_(rhs);
		Impl* p = m_pImpl.release();
		m_pImpl.reset(copy_.m_pImpl.release() );
		copy_.m_pImpl.reset(p);
		return *this;
	}

	template<class R2, class TList2>
	Functor(const Functor<R2,TList2>& rhs)
	{
		//不允许这种配接
		STATIC_CHECK(false, ERROR_qth_function_template_parameter_type_is_inconsistent);
	}

	//接受一般函数指针，仿函数
	template<class Fun>
	Functor(Fun fun): m_pImpl(new FunctorHandle<Functor, Fun>(fun)){}
	//接受成员函数
	template<class Fun, class ObjPtr>
	Functor(Fun fun, ObjPtr obj): m_pImpl(new MemFunHandle<Functor, ObjPtr, Fun>(obj, fun)){};

	bool empty(void) const
	{
		return m_pImpl.get() == NULL;
	}

	Functor(std::auto_ptr<Impl>& p): m_pImpl(p){}

	ResultType operator()() const	{ return (*m_pImpl)();}
	ResultType operator()(Param1 p1) const	{ return (*m_pImpl)(p1);}
	ResultType operator()(Param1 p1, Param2 p2) const	{ return (*m_pImpl)(p1, p2);}
	ResultType operator()(Param1 p1, Param2 p2, Param3 p3) const	{ return (*m_pImpl)(p1, p2, p3);}
	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const	{ return (*m_pImpl)(p1, p2, p3, p4);}
	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const	{ return (*m_pImpl)(p1, p2, p3, p4, p5);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6, p7);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6, p7, p8);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6, p7, p8, p9);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6, p7, p8, p9, p10);}

	ResultType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7, Param8 p8, Param9 p9, Param10 p10, Param11 p11) const
	{ return (*m_pImpl)(p1,p2,p3,p4,p5,p6, p7, p8, p9, p10, p11);}

private:
	std::auto_ptr<Impl> m_pImpl;
};

END_NAMESPACE
#endif

