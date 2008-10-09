#include "isys/CSimpleDateTime.h"


#include <time.h>


namespace isys
{


CSimpleDateTime& CSimpleDateTime::operator=(const IDateTime& dateTime)
{
	for (int i = 0; i <= TC_LAST; ++i){
		m_components[i] = dateTime.GetComponent(i);
	}

	return *this;
}


// reimplemented (isys::IDateTime)

int CSimpleDateTime::GetComponent(int component) const
{
	I_ASSERT(component >= 0);
	I_ASSERT(component <= TC_LAST);

	return m_components[component];
}


void CSimpleDateTime::SetComponent(int component, int value)
{
	I_ASSERT(component >= 0);
	I_ASSERT(component <= TC_LAST);

	m_components[component] = value;
}


bool CSimpleDateTime::SetCurrentTime()
{
	_tzset();

	__time64_t t;
	_time64(&t);
	std::tm* currentTime = _localtime64(&t);

	m_components[TC_YEAR] = currentTime->tm_year + 1900;
	m_components[TC_MONTH] = currentTime->tm_mon + 1;
	m_components[TC_DAY] = currentTime->tm_mday;
	m_components[TC_HOUR] = currentTime->tm_hour;
	m_components[TC_MINUTE] = currentTime->tm_min;
	m_components[TC_SECOND] = currentTime->tm_sec;
	m_components[TC_MICROSECOND] = 0;

	return false;
}


double CSimpleDateTime::ToCTime() const
{
	std::tm  tmTime;

	tmTime.tm_year = m_components[TC_YEAR] - 1900;
	tmTime.tm_mon = m_components[TC_MONTH] - 1;
	tmTime.tm_mday = m_components[TC_DAY];
	tmTime.tm_hour = m_components[TC_HOUR];
	tmTime.tm_min = m_components[TC_MINUTE];
	tmTime.tm_sec = m_components[TC_SECOND];

	return double(_mktime64(&tmTime));
}


void CSimpleDateTime::FromCTime(double /*ctime*/)
{
}


} // namespace isys


