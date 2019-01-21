/********************************
function:	内存池
date:		2016/8/25
tips:	
你可以使用如下类：
		class FixedAllocator		分配大小固定的类对象
		class SmallObjAllocator		分配任意大小的类对象
		class SmallObject			提供任意大小的对象分配， 重载 operator new/delete， 继承使用
*********************************/
#ifndef SMALLOBJ_H_
#define SMALLOBJ_H_
#include "utility/single.h"

BEGIN_NAMESPACE

	
	/*每页的大小*/
	struct PageSize
	{
		PageSize(std::size_t pagesize):pagesize_(pagesize){}
		inline const std::size_t Get(void) const	{ return pagesize_; }
		inline void Set(std::size_t pagesize)		{ pagesize_ = pagesize;}
	private:
		std::size_t pagesize_;
	};

	/*每块内存大小*/
	struct BlockSize
	{
		BlockSize(std::size_t blocksize):blocksize_(blocksize){}
		
		inline const std::size_t Get(void) const	{ return blocksize_; }
		inline void Set(std::size_t blocksize)		{ blocksize_ = blocksize;}
		
	private:
		std::size_t blocksize_;
	};

	/*最大对象大小*/
	struct MaxObjSize
	{
		MaxObjSize(std::size_t maxobjsize):maxobjsize_(maxobjsize){}
		inline const std::size_t Get(void) const	{ return maxobjsize_; }
		inline void Set(std::size_t maxobjsize)		{ maxobjsize_ = maxobjsize;}

	private:
		std::size_t maxobjsize_;
	};

	//短式声明
	class FixedAllocator;

	struct Chunk
	{
		friend class FixedAllocator;
	private:
		bool Init(std::size_t blockSize, unsigned char blocks);
		void Release();
		void* Allocate(std::size_t blockSize);
		void Deallocate(void* p, std::size_t blockSize);

		

		inline bool HasBlock( void * p, std::size_t chunkLength ) const
		{
			unsigned char * pc = static_cast< unsigned char * >( p );
			return (pData_ <= pc) && (pData_ + chunkLength > pc);
		}

		unsigned char* pData_;
		unsigned char firstAvailableBlock_;				//第一块可分配的数据索引
		unsigned char blocksAvailable_;					//总的可分配数据块数

	public:
		Chunk();
		~Chunk();
	};
	
	/*
		function: 大小固定的内存块分配
		params:
			blockSize	每一块内存的大小
			pageSize	每一页内存的大小， 通常为块内存大小的倍数

		例:
			FixedAllocator fa(sizeof(int), sizeof(int)*256);
			
			分配
			int* pI = (int*)fa.Allocate();

			//释放
			fa.Deallocate(pI);
	*/
	class QTH_UTILITY_EXPORT FixedAllocator
	{
	public:
		FixedAllocator(const BlockSize& blockSize = BlockSize(0), const PageSize& pageSize = PageSize(0));
		~FixedAllocator();

		void Initialize(const BlockSize& blockSize, const PageSize& pageSize);
		
		void* Allocate(void);
		bool Deallocate(void* p, Chunk* hint = NULL);
		
		inline const std::size_t GetBlockSize(void) const		{ return blockSize_;}
		const Chunk* HasBlock(void* p) const;
		
		inline Chunk* HasBlock(void* p)
		{
			return (Chunk*)((const FixedAllocator*)this)->HasBlock(p);
		}

	private:
		FixedAllocator(const FixedAllocator&);
		void operator=(const FixedAllocator&);

		void DoDeallocate( void * p );
		bool MakeNewChunk( void );
		Chunk * VicinityFind( void * p ) const; //线性查找
	private:
		static unsigned char MinObjectsPerChunk_;			//每各chunk_ 最小多少个对象
		static unsigned char MaxObjectsPerChunk_;			//每个chunk_ 最大多少个对象

		std::size_t			blockSize_;						
		unsigned char		numBlocks_;						
		
		typedef		std::vector<Chunk>			Chunks;
		typedef		Chunks::iterator			ChunkIter;
		typedef		Chunks::const_iterator		ChunkCIter;

		Chunks chunks_;

		Chunk* allocChunk_;				
		Chunk* deallocChunk_;			
		Chunk* emptyChunk_;
	};
	
	/*
		function: 小型对象内存块分配
		params:
			pageSize	每一页内存的大小
			maxObjSize	能够享受到内存池特性的最大对象大小

		例:
			SmallObjAllocator soa(PageSize(4096*2), MaxObjSize(64));
			
			分配
			int* pI = (int*)soa.Allocate( sizeof(int) );

			double* pD = (double*)soa.Allocate( sizeof(double) );

			//释放
			soa.Deallocate(pI);
			soa.Deallocate(pD);
	*/
	class QTH_UTILITY_EXPORT SmallObjAllocator
	{
	public:
		SmallObjAllocator(const PageSize& pageSize = PageSize(4096*2), const MaxObjSize& maxObjSize = MaxObjSize(64));
		~SmallObjAllocator(void);

	public:
		void* Allocate(std::size_t numBytes, bool doThrow = true);
		void Deallocate(void* p, std::size_t numBytes);
		void Deallocate(void* p);

		inline std::size_t GetMaxObjectSize() const			{ return maxSmallObjectSize_; }
		inline std::size_t GetPageSize() const				{ return pageSize_; }
	private:

		/*
			申请的对象大小超出最大对象大小时，默认转发给 operator new
		*/
		inline void* DefaultAllocator(std::size_t numBytes)
		{
			return ::operator new(numBytes);
		}

		inline void DefaultDeallocator(void * p)
		{
			::operator delete(p);
		}

		SmallObjAllocator(const SmallObjAllocator&);
		void operator=(const SmallObjAllocator&);

		FixedAllocator* pool_;
		std::size_t maxSmallObjectSize_;					//最大对象的大小， 当申请的对象大小超过此值， 则委派给::operator new 
		std::size_t pageSize_;											
	};


	/*
		function: 小型对象池, 小于64Byte 的对象继承此类自动享用内存池分配.  
		
		例:
			class A : public SmallObject<>
			{};

			//申请
			A* pA = new A;

			//释放
			delete pA;
	*/
	template
		<
			template<class,class>class ThreadingModel = ::Loki::ClassLevelLockable		//默认使用多线程
		>
	class SmallObject
	{
	public:
		virtual ~SmallObject(){}

		typedef Singleton<SmallObjAllocator, 0, QTH_NAME_SPACE::CreateUsingNew<SmallObjAllocator>, ThreadingModel> AllocSingeleton;

	protected:
		SmallObject(){}
		SmallObject(const SmallObject&){}
		SmallObject& operator=(const SmallObject&) { return *this;}


	public:
		//正常版 new/delete
		static void* operator new(std::size_t size) throw(std::bad_alloc)
		{
			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			return AllocSingeleton::Instance().Allocate(size, true);
			
		}

		static void operator delete(void* p, std::size_t size) throw()
		{
			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			AllocSingeleton::Instance().Deallocate(p, size);
		}

		//不抛异常版 new/delete 
		static void* operator new(std::size_t size, const std::nothrow_t&) throw()
		{
			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			return AllocSingeleton::Instance().Allocate(size, false);
		}

		static void operator delete(void* p, const std::nothrow_t&) throw()
		{
			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			AllocSingeleton::Instance().Deallocate(p);
		}

		
#if !defined(_WIN32) && !defined(_WIN64)
		//正常版 new[]/delete[]
		inline static void* operator new[](std::size_t size) throw(std::bad_alloc)
		{
			return operator new(size);
		}

		inline static void operator delete[](void* p, std::size_t size) throw()
		{
			operator delete(p, size);
		}

		//不抛异常版 new/delete 
		inline static void* operator new[](std::size_t size, const std::nothrow_t&) throw()
		{
			return operator new(size, std::nothrow);
		}

		inline static void operator delete[](void* p, const std::nothrow_t&) throw()
		{
			operator delete(p, std::nothrow);
		}

#endif
	
	};


	
	template<class T, std::size_t blockSize = sizeof(T) > 
	class CreateFixedObjectNew
	{
	public:
		static T* Create()				{ return new T(blockSize, blockSize * 256);}
		static void Destroy(T* p)		{ delete p;}
	};


		/*
		function: 任意对象池
		例:
			class A : public FixedObject<A>
			{};

			//错误情况,注意，使用这种对象池后， 类A就不允许被继承, 如下:
			class B : public A		// error
			{};

			//申请
			A* pA = new A;

			//释放
			delete pA;
	*/
	template
	<
		class T,																	//需要使用对象池的类型
		template<class,class>class ThreadingModel = ::Loki::ClassLevelLockable		//默认使用多线程
	>
	class FixedObject
	{
	public:
		typedef Singleton<FixedAllocator, 0, CreateFixedObjectNew<FixedAllocator>, ThreadingModel> AllocSingeleton;

		virtual ~FixedObject(){}
	protected:
		FixedObject(){}
		FixedObject(const FixedObject&){}
		FixedObject& operator=(const FixedObject&) { return *this;}

	public:
		//正常版 new/delete
		static void* operator new(std::size_t size) throw(std::bad_alloc)
		{
			//这里应该不允许继承的情况
			assert(sizeof(T) == size);

			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			return AllocSingeleton::Instance().Allocate();

		}

		static void operator delete(void* p, std::size_t size) throw()
		{
			//这里应该不允许继承的情况
			assert(sizeof(T) == size);

			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			AllocSingeleton::Instance().Deallocate(p);
		}

		//不抛异常版 new/delete 
		static void* operator new(std::size_t size, const std::nothrow_t&) throw()
		{
			//这里应该不允许继承的情况
			assert(sizeof(T) == size);

			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			return AllocSingeleton::Instance().Allocate();
		}

		static void operator delete(void* p, const std::nothrow_t&) throw()
		{
			//这里应该不允许继承的情况
			assert(sizeof(T) == size);

			typename ThreadingModel<AllocSingeleton, LOKI_DEFAULT_MUTEX>::Lock guard;
			AllocSingeleton::Instance().Deallocate(p);
		}

	};

END_NAMESPACE
#endif

