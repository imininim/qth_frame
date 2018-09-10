#include "stdafx.h"
#include "utility/single.h"

BEGIN_NAMESPACE

QTH_UTILITY_EXPORT TrackerArray* pTrackerArray = NULL;


QTH_UTILITY_EXPORT void atExitFn()
{
	assert( pTrackerArray!=0 && !pTrackerArray->empty() );
	LifetimeTracker* pTop = pTrackerArray->back();
	pTrackerArray->pop_back();
	delete pTop;

	if (pTrackerArray->empty())
	{
		delete pTrackerArray;
		pTrackerArray = 0;
	}
}


LifetimeTracker::LifetimeTracker( unsigned int x ):longevity_(x)
{

}

LifetimeTracker::~LifetimeTracker() {}


const char* SingletonException::what() const throw()
{
	return "[错误]高寿命单例的析构函数中不应该调用比他寿命低的单例";
}

END_NAMESPACE