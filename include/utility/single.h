/********************************
function:	携带寿命管理器的单例组件
date:		2016/09/05
tips:		
		该单例模式与通常单例不同的地方：
		
		1.  不需要手动释放，没有 delInstance 等方法， 只有Instance 获取单例类的方法
		2.  带寿命管理器 
		3. 带构造器和删除器 第三模板参数， 具体看类声明
		4. 支持多线程
*********************************/

#ifndef CSINGLE_H_
#define CSINGLE_H_
#include "utility/Threads.h"
BEGIN_NAMESPACE

	/*单例死亡引用异常*/
	class QTH_UTILITY_EXPORT SingletonException: public std::exception
	{
		virtual const char* what() const throw();
	};


	/*  单例生存时间跟踪基类*/
	class QTH_UTILITY_EXPORT LifetimeTracker
	{
	public:
		LifetimeTracker(unsigned int x);
		virtual ~LifetimeTracker() = 0;
		inline static bool Compare(const LifetimeTracker* p1, const LifetimeTracker* p2)	{ return p2->longevity_ < p1->longevity_; }
	private:
		unsigned int longevity_;
	};	

	QTH_UTILITY_EXPORT void atExitFn();

	/*全局数据，保存所有被追踪的单例类*/
	typedef std::list<LifetimeTracker*> TrackerArray;
	extern QTH_UTILITY_EXPORT TrackerArray* pTrackerArray;
		
	/* 带删除器的生存时间跟踪类*/
	template<typename T, typename Destroyer>
	class ConcreateLifetimeTracker: public LifetimeTracker
	{
	public:
		ConcreateLifetimeTracker(T* pDynObject, unsigned int longevity, Destroyer destroyer)
			:LifetimeTracker(longevity)
			,pTracked_(pDynObject)
			,destroyer_(destroyer)
		{}

		~ConcreateLifetimeTracker()		{ destroyer_(pTracked_); }
	private:
		T* pTracked_;
		Destroyer destroyer_;
	};

	/*生存时间登记*/
	template<typename T, typename Destroyer>
	void SetLongevity(T* pDynObject, unsigned int longevity, Destroyer d)
	{
		if (pTrackerArray == NULL)
			pTrackerArray = new TrackerArray;

		std::auto_ptr<LifetimeTracker> p( new ConcreateLifetimeTracker<T, Destroyer>(pDynObject, longevity, d) );
		TrackerArray::iterator pos = std::upper_bound(pTrackerArray->begin(), pTrackerArray->end(), p.get(), LifetimeTracker::Compare);
		pTrackerArray->insert(pos, p.get());
		p.release();			//这里必须释放对这个指针的掌控，不能让他删除 p 所指向的内存
		std::atexit(atExitFn);	
	}

	/*
		默认的创建、销毁方式， 
	*/
	template<class T> class CreateUsingNew
	{
	public:
		static T* Create()				{ return new T;}
		static void Destroy(T* p)		{ delete p;}
	};


	/*
		单例类
	*/
	template<
						class T,			
						size_t longevity = 0,													//生存周期，数字越大的类越迟被释放掉			
						template<class>class CreationPolicy = CreateUsingNew,					//创建、释放策略
						template<class,class> class ThreadingModel = LOKI_DEFAULT_THREADING_NO_OBJ_LEVEL	//线程模型，默认单线程
	> 
	class Singleton
	{
	public:
		static T& Instance()
		{
			if (!pInstance_)
			{
				typename ThreadingModel<T, LOKI_DEFAULT_MUTEX>::Lock guard;
				if (!pInstance_)
				{
					if (live_)
					{
						throw SingletonException();
					}

					pInstance_ = CreationPolicy<T>::Create();
					if (longevity <= 999)
						SetLongevity(pInstance_, longevity, DestroySingleton);
					else
						SetLongevity(pInstance_, 999, DestroySingleton);
			
					live_ = true;
				}
			}

			return *pInstance_;
		}
	
	protected:
		Singleton(){}
	private:


		static void DestroySingleton(T*)
		{
			CreationPolicy<T>::Destroy(pInstance_);
			pInstance_ = NULL;
		}
		
		DISALLOW_COPY_AND_ASSIGN(Singleton)

		//data
		typedef typename ThreadingModel<T*, LOKI_DEFAULT_MUTEX>::VolatileType InstanceType;	//T*类型， 根据是否处于多线程模型下 展开为 volatile T*   或者  T*
		static InstanceType pInstance_;
		static bool live_;
	};

	template<class T, size_t longevity, template<class> class CreationPolicy, template<class,class> class ThreadingModel>
	typename Singleton<T, longevity,CreationPolicy, ThreadingModel>::InstanceType Singleton<T, longevity,CreationPolicy, ThreadingModel>::pInstance_ = NULL;

	template<class T, size_t longevity, template<class> class CreationPolicy, template<class,class> class ThreadingModel>
	bool Singleton<T, longevity,CreationPolicy, ThreadingModel>::live_ = false;
END_NAMESPACE
#endif



