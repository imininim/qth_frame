/************************************************************************/
/*
	function:	���ü�������
	date:		2016/10/11
	author��	�����
	tips:		
	                                                                     */
/************************************************************************/

#ifndef SP_COUNTED_H_
#define SP_COUNTED_H_
BEGIN_NAMESPACE


template<template<class,class>class ThreadingModel, std::size_t, std::size_t>
class SmallObject;

template<class T>
class lock_guard
{
public:
	lock_guard(T& t): m_lock(t){ m_lock.Lock(); }
	~lock_guard(){ m_lock.Unlock(); }
private:
	T& m_lock;
};

/*
	���ü�������
*/
class sp_count: public QTH_NAME_SPACE::SmallObject<>
{
public:
	sp_count(void):use_count(1), weak_count(0){}
	sp_count(int):use_count(0), weak_count(1){}

	void add_shared_ref(void)
	{
		lock_guard<Loki::Mutex> lock(m_mutex);
		++use_count;
	}
	void sub_shared_ref(void)
	{
		m_mutex.Lock();
		if ( (--use_count == 0) && (weak_count == 0) )
		{
			m_mutex.Unlock();
			destroy();
			return ;
		}
		m_mutex.Unlock();
	}

	void add_weak_ref(void)
	{
		lock_guard<Loki::Mutex> lock(m_mutex);
		++weak_count;
	}

	void sub_weak_ref(void)
	{
		m_mutex.Lock();
		if ((--weak_count == 0) && (use_count == 0))
		{
			m_mutex.Unlock();
			destroy();
			return;
		}
		m_mutex.Unlock();
	}
	
	long usecount(void)				
	{
		lock_guard<Loki::Mutex> lock(m_mutex);
		return use_count;
	}
	long weakcount(void)				
	{
		lock_guard<Loki::Mutex> lock(m_mutex);
		return weak_count;
	}

protected:
	//˽�л�������Ϊ��ʹ���������ڶ��ϣ� ����Ϊ��ʹ�ö���صĹ��ܣ� �����������ʼ���Ϊprotected
	~sp_count(void){}
private:
	sp_count(long uc, long wc): use_count(uc), weak_count(wc){}

	//weak_count == 0 ʱ���ô˺������ͷ����ü���������
	inline void destroy()				
	{
		delete this;
	}

	volatile long use_count;				//shared_ptr ����
	volatile long weak_count;				//weak_ptr ����
	Loki::Mutex m_mutex;
};


END_NAMESPACE

#endif
