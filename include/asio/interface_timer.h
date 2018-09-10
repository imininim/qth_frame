/*******************************
function:	定时器接口类
date:		2016/12/30
version:	v1.0
tips:		提供对外部的定时器服务
********************************/
#ifndef		INTERFACE_TIMER_H_
#define		INTERFACE_TIMER_H_
#include "utility/functional.h"
BEGIN_NAMESPACE
	class IProcessor;

	class ITimer
	{
		public:
			typedef		QTH_NAME_SPACE::function<void(void)>			FCallBack;		//定时器回调
			virtual ~ITimer(){}

			virtual bool SetTimer(std::size_t nIDEvent, std::size_t nElaspse,
				const FCallBack& callback,int nRunTimes)						= 0;

			virtual void KillTimer(std::size_t nIDEvent)						= 0;

			virtual void SetProcessor(IProcessor* pProcessor)					= 0;

			virtual IProcessor* GetProcessor(void)								= 0;
	};


END_NAMESPACE
#endif
