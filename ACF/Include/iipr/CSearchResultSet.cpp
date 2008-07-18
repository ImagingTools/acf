#include "iipr/CSearchResultSet.h"


namespace iipr
{


void CSearchResultSet::SetTime(double time)
{
	m_time = time;
}


// reimplemented (iipr::ISearchResultSets)

double CSearchResultSet::GetTime() const
{
	return m_time;
}


} // namespace iipr

