/********************************
author:		qutianhao
function:	装饰品， 包装一个类，使得能够在开始使用被包装类之前【调用成员函数之前】，
			和使用完被包装类之后【调用成员函数之后】能够做一些特别处理

date:		2017/12/15
tips:
*********************************/
#ifndef DECORATE_PTR_H
#define DECORATE_PTR_H
#include "utility/typetraits.h"

//当用户没有使用日志信息的时候
#ifndef LOG_DEBUG
#define LOG_DEBUG(msg, id, count, time)	std::cout << "[统计模块(" << (id) << ")] 模块累计调用次数" << (count) << "运行时间:" << (time) << std::endl;
#endif

struct SDecoratePolulation
{

	SDecoratePolulation(void): count(0), time(0){}
	~SDecoratePolulation(void){}

	long count;		//计数器
	clock_t time;	//运行时间(毫秒)
};


/******************************************************
	class:			装饰品， 用于记录一个被装饰对象 的被调用次数和运行时间信息
	template param:
		T			被装饰的对象类型
		Identify	对象唯一标识符号类型
		StoreType	输出的额外信息类型
*******************************************************/
template<class T, class Identify, class StoreType>
struct DecoratorCout
{
	//开始装饰
	DecoratorCout(T* p, const Identify& id, StoreType& store): m_pInstance(p), m_id(id), m_store(store)
	{
		m_time = clock();  //记录该对象被创建的时间,这个时间同时也是开始操作被装饰对象的时间
	}

	//结束装饰
	~DecoratorCout()
		{ 
			++m_store.count;					//增加被装饰对象的调用次数
			m_store.time += clock() - m_time;	//增加累计运行时间
		} 
		
	//输出装饰总执行结果
	void out_put()
	{
		enum {value = QTH_NAME_SPACE::TypeTraits<Identify>::isString ? 1 : 
			QTH_NAME_SPACE::TypeTraits<Identify>::isStdArith ? 2 : 3};

		put_out(QTH_NAME_SPACE::Int2Type<value>() ,m_id, m_store); 
	}

	//返回被装饰的对象
	T* operator->()				{ return m_pInstance;}
	const T* operator->() const {return m_pInstance; }
	
private:
	//输出字符串id
	void put_out(QTH_NAME_SPACE::Int2Type<1>, const Identify& id, const StoreType& store)
	{  LOG_DEBUG("[统计模块(%s)] 模块累计调用次数%ld,累计运行时间%ld msec", id.c_str(), store.count, store.time); }

	//输出整型id
	void put_out(QTH_NAME_SPACE::Int2Type<2>, const Identify& id, const StoreType& store)
	{  LOG_DEBUG("[统计模块(%ld)] 模块累计调用次数%ld,累计运行时间%ld msec", id, store.count, store.time); }

	//其他类型id不管,让其产生编译错误
	void put_out(QTH_NAME_SPACE::Int2Type<3>)
	{ STATIC_CHECK(false, ERROR_Identify_type_error); }

	T* m_pInstance;			//被装饰对象的指针
	const Identify& m_id;	//被装饰对象的标识符
	StoreType& m_store;		//用于存储被装饰对象的运行时间和调用次数信息
	clock_t m_time;			//该对象的生存时间
};

/******************************************************
	class:			装饰品， 用于在使用被装饰对象之前加锁，
					使用完成之后解锁， 使得对于被装饰对象的操作具有原子性
	template param:
		...
*******************************************************/
template<class T, class Identify, class StoreType>
struct DecoratorLock
{
public:
	DecoratorLock(T* p, const Identify& id, StoreType& store): m_pInstance(p), m_id(id), m_store(store)
	{
		m_store.Lock();
	}

	~DecoratorLock()
	{
		m_store.unLock();
	}

	//为了满足decorate_ptr 对于 装饰品 的接口要求， 需要提供此函数
	void out_put() 
	{}

	//返回被装饰的对象
	T* operator->()				{ return m_pInstance;}
	const T* operator->() const {return m_pInstance; }
private:
	T* m_pInstance;
	const Identify& m_id;
	StoreType& m_store;
};

/*
	class: 
		装饰指针， 包装一个类，使得每次调用被包装类的成员或接口之前以及之后可以执行一些其他的操作
	template params:
		T				
		Identify		
		StoreType		
		Decorator		需要附加的功能, 即 在调用被包装类的成员或接口之前以及之后需要执行的操作
	tips:
		1.该类  重载  operator->  使得可以当做指针来使用，  支持指针的一切操作
		2.该类	与指针的不同之处在于 无法使用 *p 这样的操作来直接获取被包装的类对象， 
				而是提供了 get 操作来获取被包装对象的指针，ref操作获取其引用
		
		3.该类  包装的指针为弱引用， 即该类不会自动释放其所包装的指针，用户需要手动使用 delete 来释放
*/
BEGIN_NAMESPACE
template<class T,						//被包装的类
class Identify = int,					//用户自定的对象标识符类型， 唯一的标识着这个对象【不是标识整个类】
class StoreType = SDecoratePolulation,	//用户自定义的存储结构， 用于存储装饰品的额外信息
template<class,class,class>class Decorator = DecoratorCout>	//装饰对象，
class decorate_ptr
{	
	//if (sp) 处理
	struct Tester
	{
		Tester(int){}
		void dummy(){}
	};
	typedef void(Tester::*test_pfun)();
	typedef		test_pfun		pointer_conversion_type;
	
	template<class Y, class I, class S, template<class,class,class>class D> friend class decorate_ptr;
public:
	//构造
	decorate_ptr(void):pInstance(NULL), m_id(),m_count(NULL), m_store(NULL) {}
	
	explicit decorate_ptr(T* p, const Identify& id = Identify()): pInstance(p), m_id(id),m_count(NULL), m_store(NULL)
	{ 
		if (p) 
			add_count(m_store, m_count);
	}

	~decorate_ptr()
	{
		sub_count(m_store, m_count); //递减引用计数，计数为0则删除计数器
	}

	//拷贝构造/赋值
	decorate_ptr(const decorate_ptr& other):pInstance(other.pInstance), m_id(other.m_id), m_count(other.m_count), m_store(other.m_store)
	{
		add_count(m_store, m_count);
	}

	template<class Y, class I, class S, template<class,class,class>class D>
	decorate_ptr(const decorate_ptr<Y,I,S,D>& other):
		pInstance(other.pInstance), m_id(other.m_id), m_count(other.m_count), m_store(other.m_store)
	{
		add_count(m_store, m_count);
	}

	decorate_ptr& operator=(decorate_ptr& other)
	{
		if (this == &other)
			return *this;

		decorate_ptr temp(other);
		temp.swap(*this);
		return *this;
	}
	
	template<class Y, class I, class S, template<class,class,class>class D>
	decorate_ptr& operator=(decorate_ptr<Y,I,S,D>& other)
	{
		if ((pInstance == other.pInstance) && (m_count == other.m_count) &&
			(m_store == other.m_store) && (m_id == other.m_id) )
			return *this;

		decorate_ptr<Y,I,S,D> temp(other);
		temp.swap(*this);
		return *this;
	}

	//接受一个原始指针作为赋值对象
	decorate_ptr& operator=(T* other)
	{
		if (pInstance == other)
			return *this;

		sub_count(m_store, m_count);
		
		pInstance = other;
		m_store = NULL;
		m_count = NULL;

		if (pInstance)
			add_count(m_store, m_count);
		return *this;
	}

	//成员获取
	const T* get() const			{ return pInstance; }
	const T& ref() const			{return *pInstance;}
	
	T& ref()						{return *pInstance;}
	T* get()						{ return pInstance; }
	
	Identify& get_id()				{return m_id;}
	const Identify& get_id() const	{return m_id;}

	StoreType& get_store()				{return *m_store;}
	const StoreType& get_store() const	{return *m_store;}
	
	//返回当前引用的人数
	inline const long use_count() const
	{
		if (m_count)
			return *m_count;

		return 0;
	}

	//相等性不等性
	const bool operator==(const decorate_ptr& other) const
	{ return pInstance == other.get(); }
	const bool operator!=(const decorate_ptr& other) const
	{ return pInstance != other.get();}

	template<class Y, class I, class S, template<class,class,class>class D>
	const bool operator==(const decorate_ptr<Y,I,S,D>& other) const
	{ return pInstance == other.get(); }

	template<class Y, class I, class S, template<class,class,class>class D>
	const bool operator!=(const decorate_ptr<Y,I,S,D>& other) const
	{ return pInstance != other.get();}

		
	//和原始指针的相等性和不等性
	const friend inline bool operator==(const decorate_ptr& rhs, const T* lhs)
	{ return rhs.get() == lhs;	}

	const friend inline bool operator==(const T* lhs, const decorate_ptr& rhs)
	{ return lhs == rhs.get();}

	const friend inline bool operator!=(const decorate_ptr& rhs, const T* lhs)
	{ return rhs.get() != lhs;	}

	const friend inline bool operator!=(const T* lhs, const decorate_ptr& rhs)
	{ return lhs != rhs.get();}	

	//装饰指针与任意原始指针的相等性
	template<class Y>
	const friend inline bool operator==(const decorate_ptr& rhs, const Y* lhs)
	{ return rhs.get() == lhs;	}

	template<class Y>
	const friend inline bool operator==(const Y* lhs, const decorate_ptr& rhs)
	{ return lhs == rhs.get();}
	
	template<class Y>
	const friend inline bool operator!=(const decorate_ptr& rhs, const Y* lhs)
	{ return rhs.get() != lhs;	}

	template<class Y>
	const friend inline bool operator!=(const Y* lhs, const decorate_ptr& rhs)
	{ return lhs != rhs.get();}	

	//if(!sp)  验证
	const inline bool operator!() const 
	{return (pInstance == NULL);}

	//if(sp) 验证
	operator pointer_conversion_type() const
	{ return !(*this) ? 0 : &Tester::dummy; }
	
	//提领
#ifndef NO_DECORATE_PTR
	Decorator<T, Identify, StoreType> operator->()
	{ return Decorator<T, Identify, StoreType>(pInstance, m_id, get_store()); }

	const Decorator<T, Identify, StoreType> operator->() const
	{ return Decorator<T, Identify, StoreType>(pInstance, m_id, get_store()); }

	//输出装饰操作
	void out_decorate()
	{ Decorator<T, Identify, StoreType>(pInstance, m_id, get_store()).out_put(); }
#else
	T* operator->()
	{ return pInstance; }

	const T* operator->() const
	{ return pInstance; }

	void out_decorate()
	{}
#endif

private:		
	//增加引用计数
	inline void add_count(StoreType*& pStore, long*& pCount)
	{
		if (pStore && pCount)
		{
			++(*pCount); 
		}
		else
		{
			if (pStore) delete pStore;
			if (pCount) delete pCount;

			pStore = new StoreType();
			pCount = new long(1); //初始化为1个引用
		}
	}

	//减少引用计数
	inline void sub_count(StoreType*& pStore, long*& pCount)
	{
		if (pStore && pCount && (--(*pCount) == 0) )
		{
			SAFE_DELETE(pStore);
			SAFE_DELETE(pCount);
			SAFE_DELETE(pInstance);
		}	
	}

	template<class Y, class I, class S, template<class,class,class>class D>
	void swap(decorate_ptr<Y,I,S,D>& p)
	{
		using std::swap;
		swap(p.pInstance, pInstance);
		swap(p.m_count, m_count);
		swap(p.m_store, m_store);
		swap(p.m_id, m_id);
	}

	//禁止 *p 操作
	decorate_ptr& operator*();
	const decorate_ptr& operator*() const;

	T* pInstance;						//指向被包装类的指针
	long* m_count;						//内部引用计数
	StoreType* m_store;					//内部存储信息
	Identify m_id;						//对象ID标识
	
};
END_NAMESPACE
#endif


