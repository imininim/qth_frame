/*********************
function:	weak_ptr
date:		2016/10/11
author:		屈天豪
tips:		弱引用指针， 搭配shared_ptr使用
**************************/

#ifndef QTH_WEAK_PTR_H_
#define QTH_WEAK_PTR_H_

BEGIN_NAMESPACE

//短式声明
class sp_count;

template<class T>
struct Deleter;


template<class T, template<class> class Del>
class shared_ptr;

template<class T, template<class> class Del>
class weak_ptr;


//////////////////////////////////////////////////////////////////
template<class T, template<class> class Del>
void swap(weak_ptr<T,Del>& a, weak_ptr<T,Del>& b)
{
	a.swap(b);
}

template<class T, template<class> class Del = Deleter>
class weak_ptr
{
	template<class Y, template<class> class D> friend class weak_ptr;
	template<class Y, template<class> class D> friend class shared_ptr;
public:

	//构造/析构
	weak_ptr(void):pointte_(0), pcount_(new sp_count(0))
	{}

	~weak_ptr()
	{
		pcount_->sub_weak_ref();
	}

	//拷贝构造/拷贝赋值
	weak_ptr(const weak_ptr& rhs): pointte_(rhs.pointte_), pcount_(rhs.pcount_) 
	{
		assert(pcount_);
		pcount_->add_weak_ref();
	}

	template<class Y, template<class> class D>
	weak_ptr(const weak_ptr<Y,D>& rhs): pointte_(rhs.pointte_), pcount_(rhs.pcount_)
	{
		pcount_->add_weak_ref();
	}

	weak_ptr& operator=(const weak_ptr& rhs)
	{
		if (this == &rhs)
			return *this;

		weak_ptr p(rhs);
		p.swap(*this);
		return *this;
	}

	template<class Y, template<class> class D>
	weak_ptr& operator=(const weak_ptr<Y,D>& rhs)
	{
		if ((this->pointee_ == rhs.pointee_) && (this->pcount_ == rhs.pcount_) )
			return *this;

		this->pcount_->sub_weak_ref();

		this->pointee_ = rhs.pointee_;
		this->pcount_ = rhs.pcount_;
		this->pcount_->add_weak_ref();
		return *this;
	}

	template<class Y, template<class>class D>
	weak_ptr(const shared_ptr<Y,D>& rhs): pointte_(rhs.pointee_), pcount_(rhs.pcount_)
	{ pcount_->add_weak_ref();}

	//相等性和不等性
	const inline bool operator==(const weak_ptr& rhs) const
	{return rhs.get() == this->pointte_;}

	const inline bool operator!=(const weak_ptr& rhs) const
	{return rhs.get() != this->pointte_;}

	template<class Y, template<class> class D>
	const inline bool operator==(const weak_ptr<Y,D>& rhs) const
	{return rhs.get() == this->pointte_;}

	template<class Y, template<class> class D>
	const inline bool operator!=(const weak_ptr<Y,D>& rhs) const
	{return rhs.get() != this->pointte_;}

	//和原始指针的相等性
	const friend inline bool operator==(const weak_ptr& rhs, const T* lhs)
	{ return rhs.get() == lhs;	}
	
	const friend inline bool operator==(const T* lhs, const weak_ptr& rhs)
	{ return lhs == rhs.get();}
	
	const friend inline bool operator!=(const weak_ptr& rhs, const T* lhs)
	{ return rhs.get() != lhs;	}

	const friend inline bool operator!=(const T* lhs, const weak_ptr& rhs)
	{ return lhs != rhs.get();}	

	//智能指针与任意原始指针的相等性
	template<class Y>
	const friend inline bool operator==(const weak_ptr& rhs, const Y* lhs)
	{ return rhs.get() == lhs;	}

	template<class Y>
	const friend inline bool operator==(const Y* lhs, const weak_ptr& rhs)
	{ return lhs == rhs.get();}
	
	template<class Y>
	const friend inline bool operator!=(const weak_ptr& rhs, const Y* lhs)
	{ return rhs.get() != lhs;	}

	template<class Y>
	const friend inline bool operator!=(const Y* lhs, const weak_ptr& rhs)
	{ return lhs != rhs.get();}	
	
	const inline long use_count() const
	{
		return pcount_->usecount();
	}

	const inline long weak_count() const
	{
		return pcount_->weakcount();
	}

	inline bool expired() const
	{
		return pcount_->usecount() == 0;
	}

	shared_ptr<T, Del> lock() const
	{
		return shared_ptr<T,Del>(*this);
	}

	void swap(weak_ptr& p)
	{
		using std::swap;
		swap(p.pointte_, pointte_);
		swap(p.pcount_, pcount_);
	}

private:
	T* pointte_;
	sp_count* pcount_;
};


END_NAMESPACE



#endif
