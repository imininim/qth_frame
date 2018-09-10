/*********************
function:	enable_shared_from_this
date:		2016/10/11
author:		屈天豪
tips:		在一个类对象的内部中获取这个类对象的智能指针
**************************/
#ifndef 	ENABLE_SHARED_FROM_THIS_QTH_H_
#define 	ENABLE_SHARED_FROM_THIS_QTH_H_

BEGIN_NAMESPACE
//短式声明
template<class T, template<class> class Del>
class weak_ptr;

template<class T, template<class> class Del>
class shared_ptr;

template<class T, class U>
class Conversion;

template<class T>
struct Deleter;


template<class T,template<class> class Del = Deleter>
class enable_shared_from_this
{
	template<class Y, template<class> class D> friend class shared_ptr; 
	template<class Y, class D> friend class Conversion; 
protected:
	enable_shared_from_this(){}
	virtual ~enable_shared_from_this(){}

	enable_shared_from_this(const enable_shared_from_this& rhs): wp(rhs.wp)
	{}

	enable_shared_from_this& operator=(const enable_shared_from_this& rhs)
	{
		wp = rhs.wp;
		return *this;
	}

public:
	inline shared_ptr<T,Del> shared_from_this()
	{
		return wp.lock();
	}



private:
	weak_ptr<T,Del> wp;

	inline void enable_shared_from_this_init(const shared_ptr<T,Del>& sp)
	{wp = sp;}
};
END_NAMESPACE
#endif
