#include "collect.h"

void CCollect::doCollect(time_t start)
{
	if (!m_data.empty() && m_data.front() != m_data.back())
	{
		STimeData& start = m_data.front();
		STimeData& end = m_data.back();
		time_t subDealTime = end.time_deal - start.time_deal;
		if (subDealTime >= 3600) //每小时打印一次
		{
			dump();
		}

	}

	m_data.push_back(STimeData(start));
}

void CCollect::dump()
{
	if (m_data.empty())
		return;

	STimeData& start = m_data.front();
	STimeData& end = m_data.back();
	//当前这段时间的处理
	time_t subDealTime = end.time_deal - start.time_deal;
	size_t num = m_data.size();
	size_t avg = num/(size_t)subDealTime;

	LOG_INFO("服务器信息: 收到的包总量:%u 时间间隔:%lu 平均收到的数量%u包/秒", num, subDealTime, avg);

	//打印完成后，清空数据
	reset();
}

void CCollect::reset()
{
	m_data.clear();
}

CCollect::CCollect()
{
	reset();
}

CCollect::~CCollect()
{
	
}
