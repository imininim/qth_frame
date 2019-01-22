//数据信息搜集类
#ifndef _COLLECT_H_
#define _COLLECT_H_
#include <vector>
#include <time.h>
#include "utility/utility.h"
#include "utility/logger.h"

struct STimeData
{
	explicit STimeData(time_t start)
	{
		time_start = start;
		time_deal = time(NULL);
	}

	bool operator==(const STimeData& other)
	{ return time_start == other.time_start && time_deal == other.time_deal; }
	const bool operator==(const STimeData& other) const
	{ return time_start == other.time_start && time_deal == other.time_deal; }
	bool operator!=(const STimeData& other)
	{ return time_start != other.time_start || time_deal != other.time_deal; }
	const bool operator!=(const STimeData& other) const
	{ return time_start != other.time_start || time_deal != other.time_deal; }

	bool operator<(const STimeData& other)
	{return time_start < other.time_start;}

	const bool operator<(const STimeData& other) const
	{return time_start < other.time_start;}


	time_t time_start;		//数据发送的时间
	time_t time_deal;		//数据接收的时间 

};

//搜集类
class CCollect
{
public:
	CCollect();
	~CCollect();

	//进行搜集数据
	void doCollect(time_t start);
	
	//清空搜集的数据信息
	void reset();

	//整理数据并打印信息 
	void dump();
private:
	

	std::vector<STimeData> m_data;	
};


#endif