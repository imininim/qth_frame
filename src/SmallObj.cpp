#include "stdafx.h"
#include "utility/SmallObj.h"
#include <assert.h>
BEGIN_NAMESPACE

unsigned char FixedAllocator::MinObjectsPerChunk_ = 8;				
unsigned char FixedAllocator::MaxObjectsPerChunk_ = 255;		

bool Chunk::Init( std::size_t blockSize, unsigned char blocks )
{
	if (pData_)
		return false;

	pData_ = new unsigned char[blockSize * blocks];
	firstAvailableBlock_ = 0;
	blocksAvailable_ = blocks;

	//每一个可用区块的第一个字节存储下一个可用区块的索引号
	unsigned char i = 0;
	unsigned char* p = pData_;
	for (; i != blocks; p += blockSize)
	{
		*p = ++i;
	}

	return true;
}

void* Chunk::Allocate( std::size_t blockSize )
{
	if (! blocksAvailable_ || pData_ == NULL)		return 0;
	
	unsigned char* pResult = pData_ + (firstAvailableBlock_ * blockSize);
	firstAvailableBlock_ = *pResult;		//下一个可用数据块
	--blocksAvailable_;						//可用区块 -1
	return pResult;
}

void Chunk::Deallocate( void* p, std::size_t blockSize )
{
	assert( (pData_) && (p >= pData_));
	unsigned char* toRelease = static_cast<unsigned char*>(p);
	assert( (toRelease - pData_) % blockSize == 0 );           //对齐检测
	
	*toRelease = firstAvailableBlock_;
	firstAvailableBlock_ =  static_cast<unsigned char>((toRelease - pData_)/blockSize);
	
	++blocksAvailable_;
}

void Chunk::Release()
{
	if (pData_)
	{
		delete[] pData_;
		pData_ = NULL;
	}
}

Chunk::Chunk()
	:pData_(NULL)
	, firstAvailableBlock_(0)
	, blocksAvailable_(0)
{
}

Chunk::~Chunk()
{

}


FixedAllocator::FixedAllocator(const BlockSize& blockSize/* = BlockSize(0)*/, const PageSize& pageSize/* = PageSize(0)*/)
	:blockSize_(0)
	,numBlocks_(0)
	,chunks_(0)
	,allocChunk_(NULL)
	,deallocChunk_(NULL)
	,emptyChunk_(NULL)
{
	if (blockSize.Get() != 0 && pageSize.Get() != 0)
		Initialize(blockSize, pageSize);
}

FixedAllocator::~FixedAllocator()
{
	for (ChunkIter it = chunks_.begin(); it != chunks_.end(); ++it)
	{
		it->Release();
	}
}

void FixedAllocator::Initialize( const BlockSize& blockSize, const PageSize& pageSize )
{
	assert(blockSize.Get() > 0);
	assert(pageSize.Get() > 0);
	blockSize_ = blockSize.Get();
	std::size_t numBlocks = pageSize.Get()  / blockSize.Get();

	if (numBlocks > MaxObjectsPerChunk_)
	{
		numBlocks = MaxObjectsPerChunk_;
	}
	else if (numBlocks < MinObjectsPerChunk_)
	{
		numBlocks = MinObjectsPerChunk_;
	}

	numBlocks_ = numBlocks;
}

void* FixedAllocator::Allocate( void )
{
	assert( (NULL == emptyChunk_) || (emptyChunk_->blocksAvailable_ == numBlocks_) );
	assert( (numBlocks_ >= MinObjectsPerChunk_) && (numBlocks_ <= MaxObjectsPerChunk_));
	assert(blockSize_ > 0);

	if ((NULL == allocChunk_) || (allocChunk_->blocksAvailable_ == 0) ) //allocChunk 没有可用块
	{
		if (NULL != emptyChunk_)
		{
			//存在可用的空块
			allocChunk_ = emptyChunk_;
			emptyChunk_ = NULL;
		}
		else
		{
			//创建一个新块
			for (ChunkIter it = chunks_.begin(); ;++it)
			{
				if (chunks_.end() == it)
				{
					if (! MakeNewChunk())
						return NULL;

					break;
				}

				if (it->blocksAvailable_ != 0)
				{
					allocChunk_ = &*it;
					break;
				}
			}//end for
		}//end if


	}
	else if (allocChunk_ == emptyChunk_)
	{
		emptyChunk_ = NULL;
	}

	assert(allocChunk_ != NULL);
	assert(allocChunk_->blocksAvailable_ != 0);

	return allocChunk_->Allocate(blockSize_);
}


bool FixedAllocator::MakeNewChunk( void )
{
	bool allocated = false;

	try
	{
		std::size_t size = chunks_.size();

		if (chunks_.capacity() == size)
		{
			if (0 == size) size = 4;
			chunks_.reserve(size * 2);
		}

		Chunk newChunk;
		allocated = newChunk.Init(blockSize_, numBlocks_);
		if (allocated)
			chunks_.push_back(newChunk);

	}
	catch(...)
	{
		allocated = false;
	}

	if (!allocated)
		return false;

	allocChunk_ = &chunks_.back();
	deallocChunk_ = &chunks_.front();
	return true;
}

bool FixedAllocator::Deallocate( void* p, Chunk* hint )
{
	assert(!chunks_.empty());
	assert(&chunks_.front() <= deallocChunk_);
	assert(&chunks_.back() >= deallocChunk_);
	assert( &chunks_.front() <= allocChunk_ );
	assert( &chunks_.back() >= allocChunk_ );

	Chunk* foundChunk = (NULL == hint) ? VicinityFind(p) : hint;
	if (NULL == foundChunk)
		return false;

	deallocChunk_= foundChunk;
	DoDeallocate(p);
	return true;
}

Chunk * FixedAllocator::VicinityFind( void * p ) const
{
	if (chunks_.empty()) return NULL;
	 assert(deallocChunk_);

	 const std::size_t chunkLength = numBlocks_ * blockSize_;	//一个chunk 的块大小
	 
	 //线性查找以 deallocChunk 位置为中心点，向两边分头查找
	 Chunk * lo = deallocChunk_;								
	 Chunk * hi = deallocChunk_ + 1;

	 //两头查找的边界线
	 const Chunk * loBound = &chunks_.front();
	 const Chunk * hiBound = &chunks_.back() + 1;

	 //这个是特殊的分支处理,  在 deallocChunk 是最后一个chunk 的情况下， hi 到达 chunks.end()
	 if (hi == hiBound) hi = NULL;

	 for (;;)
	 {
		 if (lo)
		 {
				if (lo->HasBlock(p, chunkLength))	return lo;

				if (lo == loBound) //查找到边界，停止这一边的查找
				{
					lo = NULL;	
					if (NULL == hi) break;
				}
				else
				{
					//继续往前查找
					--lo;
				}
		 } // end if(lo)

		 if (hi)
		 {
				if (hi->HasBlock(p, chunkLength)) return hi;
				if (++hi == hiBound)
				{
					hi = NULL;
					if (NULL == lo) break;
				}
		 }//end if(hi)

	 }//end for(;;)

	 return NULL;
}

void FixedAllocator::DoDeallocate( void * p )
{
	deallocChunk_->Deallocate(p,blockSize_);

	if (deallocChunk_->blocksAvailable_ == numBlocks_)
	{
		//deallocChunk_ 为空（即该内存区块没有内存被使用）， 但是我们需要在至少有2个空区块时才释放一个区块
		if (NULL != emptyChunk_)
		{
			/*
				现在手上有两个空块，  emptyChunk_ 和 deallocChunk
				将这两个空块中的一个和 chunks_  的尾节点交换， 并且令  emptyChunk_, deallocChunk 指向不打算释放的那一个空块
				最后释放掉尾节点
			*/
			
			Chunk* lastChunk = &chunks_.back();
			if (lastChunk == deallocChunk_)
				deallocChunk_ = emptyChunk_;
			else if (lastChunk != emptyChunk_)
				std::swap(*emptyChunk_, *lastChunk);

			lastChunk->Release();
			chunks_.pop_back();
				
			if ( ( allocChunk_ == lastChunk ) || (allocChunk_->blocksAvailable_ == 0) ) 
				allocChunk_ = deallocChunk_;
		}//end if(NULL != emptyChunk_)

		emptyChunk_ = deallocChunk_;
	}
}

const Chunk* FixedAllocator::HasBlock( void* p ) const
{
	const std::size_t chunkLength = numBlocks_ * blockSize_;
	for (ChunkCIter it = chunks_.begin(); it != chunks_.end(); ++it)
	{
		const Chunk& chunk = *it;
		if (chunk.HasBlock(p, chunkLength))
			return &chunk;
	}
	return NULL;
}


SmallObjAllocator::SmallObjAllocator(const PageSize& pageSize, const MaxObjSize& maxObjSize)
	:pool_(NULL),
	maxSmallObjectSize_(maxObjSize.Get())
	,pageSize_(pageSize.Get())
{	
	pool_ = new FixedAllocator[maxSmallObjectSize_];
	for (size_t i = 0; i < maxSmallObjectSize_; ++i)
		pool_[i].Initialize(i+1, pageSize_);
}

SmallObjAllocator::~SmallObjAllocator( void )
{
	delete[] pool_;
}

void* SmallObjAllocator::Allocate(std::size_t numBytes, bool doThrow)
{
	if (numBytes > GetMaxObjectSize())
		return DefaultAllocator(numBytes);

	if (numBytes == 0)	numBytes = 1;
	FixedAllocator& allocator_ = pool_[numBytes - 1];
	assert(allocator_.GetBlockSize() == numBytes);

	void* place = allocator_.Allocate();
	if ((place == NULL) && doThrow)
	{
		throw std::bad_alloc();
	}

	return place;
}

void SmallObjAllocator::Deallocate( void* p, std::size_t numBytes )
{
	if (NULL == p) return;
	assert(NULL != pool_);

	if (numBytes > GetMaxObjectSize())
	{
		DefaultDeallocator(p);
		return;
	}
	
	if (0 == numBytes) numBytes = 1;
	FixedAllocator& allocator_ = pool_[numBytes - 1];
	bool found = allocator_.Deallocate(p, NULL);
	assert(found);
	(void)found;
}

void SmallObjAllocator::Deallocate( void* p)
{
	if (NULL == p) return;
	assert(NULL != pool_);

	FixedAllocator* pAllocator = NULL;
	Chunk* chunk = NULL;

	for (std::size_t i = 0; i < GetMaxObjectSize(); ++i)
	{
		chunk = pool_[i].HasBlock(p);
		if (NULL != chunk)
		{
			pAllocator = &pool_[i];
			break;
		}
	}

	if (NULL == pAllocator)
	{
		DefaultDeallocator(p);
		return;
	}

	assert(NULL != chunk);
	bool found = pAllocator->Deallocate(p, chunk);
	assert(found);
	(void)found;
}

END_NAMESPACE