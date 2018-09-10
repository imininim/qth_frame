/*******************
function:  单线程读取， 多线程写入的安全队列
date:		2016/12/14
tips:
	依赖于boost库 以提供跨平台的线程安全支持, 使用 boost::mutex, boost::atomic
********************/
#ifndef RW_QUEUE_H_
#define RW_QUEUE_H_
#include <assert.h>
#include"boost/thread/mutex.hpp"
#include "boost/atomic.hpp"

template<class T>
class RWQueue
{
	struct SNode;
public:
	RWQueue(void): m_pHead(new SNode), m_pTail(new SNode)
	{
		m_pHead->pNext = m_pTail;
		m_pTail->pNext = NULL;
		m_nSize = 0;
	}

	~RWQueue()
	{
		clear();
		SAFE_DELETE(m_pHead);
		SAFE_DELETE(m_pTail);
	}

	bool empty()
	{
		return m_pHead->pNext == m_pTail;
	}

	size_t size()
	{
		return m_nSize;
	}

	T& front(void)
	{
		return m_pHead->pNext->value;
	}

	const T& front(void) const 
	{
		return m_pHead->pNext->value;
	}

	void push(const T& value)
	{
		SNode *pNode = new SNode;
		pNode->pNext = NULL;
		m_mutex.lock();
		m_pTail->pNext = pNode;
		m_pTail->value = value;
		m_pTail = pNode;
		++m_nSize;
		m_mutex.unlock();
	}

	void pop(void)
	{
		assert(m_pHead->pNext != m_pTail);
		--m_nSize;
		SNode *pNode = m_pHead->pNext;
		m_pHead->pNext = pNode->pNext;
		delete pNode;
	}

	void clear()
	{
		while (!empty())
		{
			pop();
		}
	}
private:
	struct SNode
	{
		SNode *pNext;
		T value;
	};

	SNode* m_pHead;
	SNode* m_pTail;

	boost::atomic_size_t m_nSize;
	boost::mutex		m_mutex;
};




#endif
