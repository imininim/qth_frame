#include "collect.h"
#include "utility/utility.h"
#include "utility/logger.h"

void CCollect::doCollect(time_t start)
{
	if (!m_data.empty() && m_data.front() != m_data.back())
	{
		STimeData& start = m_data.front();
		STimeData& end = m_data.back();
		time_t subDealTime = end.time_deal - start.time_deal;
		if (subDealTime >= 20) //每隔多少秒答应一次
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
	LOG_INFO("服务器信息: 收到的包数量:%d 时间间隔:%lu", num, subDealTime);

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
