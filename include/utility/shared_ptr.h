#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

BEGIN_NAMESPACE

	//短式声明
template<class T, template<class>class Del>
class weak_ptr;

class sp_count;

template<int i>
struct Int2Type;

template<typename base, typename derived>
struct is_base_of;

template<class T, template<class>class Del>
class enable_shared_from_this;
////////////////////////////////////////


////////////////////////////////////////////////




template<class T>
struct Deleter
{
	static void Delete(T* p) {delete p;}
};

template<class T, template<class> class Del = Deleter>
class shared_ptr
{
	//if (sp) 处理
	struct Tester
	{
		Tester(int){}
		void dummy(){}
	};
	typedef void(Tester::*test_pfun)();
	typedef		test_pfun		pointer_conversion_type;

	template<class Y, template<class>class D> friend class shared_ptr;
	template<class Y, template<class>class D> friend class weak_ptr;
public:
	//构造、析构
	shared_ptr(void):pointee_(NULL), pcount_(new sp_count() ){}

	explicit shared_ptr(T* p) :pointee_(p), pcount_(new sp_count() )		
	{
		assert(p);
		is_init_shared_from_this(Int2Type<is_base_of<enable_shared_from_this<T,Del>, T >::value >());	
	}

	//不是继承自 shared_from_this
	void is_init_shared_from_this(Int2Type<false>){}
	//继承自 shared_from_this
	void is_init_shared_from_this(Int2Type<true>)
	{
		pointee_->enable_shared_from_this_init(*this);
	}

	//接受一个weak_ptr
	template<class Y, template<class>class D>
	shared_ptr(const weak_ptr<Y,D>& rhs)
	{
		pointee_ = rhs.pointte_;
		pcount_ = rhs.pcount_;
		pcount_->add_shared_ref();
	}
	
	~shared_ptr()
	{
		if (unique())
		{
			Del<T>::Delete(pointee_);
		}
		pcount_->sub_shared_ref();
	}

	//拷贝构造、拷贝赋值
	shared_ptr(const shared_ptr& rhs)
		:pointee_(rhs.pointee_), pcount_(rhs.pcount_)
	{pcount_->add_shared_ref();}

	template<class Y, template<class> class D>
	shared_ptr(const shared_ptr<Y,D>& rhs): pointee_(rhs.pointee_), pcount_(rhs.pcount_)
	{ pcount_->add_shared_ref(); }

	shared_ptr& operator=(const shared_ptr& rhs)
	{
		if (this == &rhs)
			return *this;

		if (unique())
		{
			Del<T>::Delete(pointee_);
		}
		pcount_->sub_shared_ref();

		pointee_ = rhs.pointee_;
		pcount_ = rhs.pcount_;
		pcount_->add_shared_ref();
		return *this;
	}

	template<class Y, template<class> class D>
	shared_ptr& operator=(const shared_ptr<Y,D>& rhs)
	{
		if ((pointee_ == rhs.pointee_) && (pcount_ == rhs.pcount_) )
			return *this;

		if (unique())
		{
			Del<T>::Delete(pointee_);
		}
		pcount_->sub_shared_ref();

		pointee_ = rhs.pointee_;
		pcount_ = rhs.pcount_;
		pcount_->add_shared_ref();
		return *this;
	}
		
	//相等性和不等性
	const inline bool operator==(const shared_ptr& rhs) const
	{return rhs.get() == pointee_;}

	const inline bool operator!=(const shared_ptr& rhs) const
	{return rhs.get() != pointee_;}

	template<class Y, template<class> class D>
	const inline bool operator==(const shared_ptr<Y,D>& rhs) const
	{return rhs.get() == pointee_;}

	template<class Y, template<class> class D>
	const inline bool operator!=(const shared_ptr<Y,D>& rhs) const
	{return rhs.get() != pointee_;}

	//和原始指针的相等性
	const friend inline bool operator==(const shared_ptr& rhs, const T* lhs)
	{ return rhs.get() == lhs;	}
	
	const friend inline bool operator==(const T* lhs, const shared_ptr& rhs)
	{ return lhs == rhs.get();}
	
	const friend inline bool operator!=(const shared_ptr& rhs, const T* lhs)
	{ return rhs.get() != lhs;	}

	const friend inline bool operator!=(const T* lhs, const shared_ptr& rhs)
	{ return lhs != rhs.get();}	

	//智能指针与任意原始指针的相等性
	template<class Y>
	const friend inline bool operator==(const shared_ptr& rhs, const Y* lhs)
	{ return rhs.get() == lhs;	}

	template<class Y>
	const friend inline bool operator==(const Y* lhs, const shared_ptr& rhs)
	{ return lhs == rhs.get();}
	
	template<class Y>
	const friend inline bool operator!=(const shared_ptr& rhs, const Y* lhs)
	{ return rhs.get() != lhs;	}

	template<class Y>
	const friend inline bool operator!=(const Y* lhs, const shared_ptr& rhs)
	{ return lhs != rhs.get();}	
	

	//if(!sp)  验证
	const inline bool operator!() const 
	{return (pointee_ == NULL);}

	//if(sp) 验证
	operator pointer_conversion_type() const
	{ return !(*this) ? 0 : &Tester::dummy; }
			
	//公用函数 , reset, unique(), usecount()
	inline const T* get(void) const
	{ return pointee_;}

	inline T* get(void)
	{ return pointee_;}

	inline const long usecount(void) const
	{return pcount_->usecount();}

	inline void reset(T* p)
	{
		shared_ptr pTmp(p);
		pTmp.Swap(*this);
	}

	inline bool unique(void)
	{return pcount_->usecount() == 1;}

	//提领
	inline T* operator->()				{return pointee_;}
	inline const T* operator->() const	{return pointee_;}

	inline T& operator*()				{return *pointee_;}
	inline const T& operator*() const	{return *pointee_;}

private:
	
	void Swap(shared_ptr& rhs)
	{
		using std::swap;
		swap(pointee_, rhs.pointee_);
		swap(pcount_, rhs.pcount_);
	}


	T* pointee_;
	sp_count* pcount_;
};

template<class T, template<class> class D>
void swap(shared_ptr<T,D>& a, shared_ptr<T,D>& b)
{
	a.Swap(b);
}

END_NAMESPACE
#endif

