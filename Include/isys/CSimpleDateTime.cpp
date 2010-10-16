#include "isys/CSimpleDateTime.h"


// STD includes
#include <time.h>


namespace isys
{


CSimpleDateTime::CSimpleDateTime()
{
	m_components.Clear();
}


CSimpleDateTime::CSimpleDateTime(const IDateTime& dateTime)
{
	for (int i = 0; i <= TC_LAST; ++i){
		m_components[i] = dateTime.GetComponent(i);
	}

}


CSimpleDateTime& CSimpleDateTime::operator=(const IDateTime& dateTime)
{
	for (int i = 0; i <= TC_LAST; ++i){
		m_components[i] = dateTime.GetComponent(i);
	}

	return *this;
}


bool CSimpleDateTime::operator==(const IDateTime& dateTime) const
{
	for (int i = 0; i <= TC_LAST; ++i){
		if (m_components[i] != dateTime.GetComponent(i)){
			return false;
		}
	}

	return true;
}


bool CSimpleDateTime::operator!=(const IDateTime& dateTime) const
{
	return !operator==(dateTime);
}


isys::CSimpleDateTime CSimpleDateTime::GetCurrent()
{
	isys::CSimpleDateTime currentDateTime;

	currentDateTime.SetCurrentTime();

	return currentDateTime;
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
#ifdef WIN32
	_tzset();
#elif defined(__linux__)
	tzset();
#endif

#ifdef _MSC_VER
	__time64_t t;
	_time64(&t);

	std::tm currentTime;
	_localtime64_s(&currentTime, &t);
#else
	time_t t;
	time(&t);
	std::tm currentTime = *localtime(&t);
#endif

	m_components[TC_YEAR] = currentTime.tm_year + 1900;
	m_components[TC_MONTH] = currentTime.tm_mon + 1;
	m_components[TC_DAY] = currentTime.tm_mday;
	m_components[TC_HOUR] = currentTime.tm_hour;
	m_components[TC_MINUTE] = currentTime.tm_min;
	m_components[TC_SECOND] = currentTime.tm_sec;
	m_components[TC_MICROSECOND] = 0;

	return false;
}


double CSimpleDateTime::ToCTime() const
{
	std::tm tmTime = {0};

	tmTime.tm_year = m_components[TC_YEAR] - 1900;
	tmTime.tm_mon = m_components[TC_MONTH] - 1;
	tmTime.tm_mday = m_components[TC_DAY];
	tmTime.tm_hour = m_components[TC_HOUR];
	tmTime.tm_min = m_components[TC_MINUTE];
	tmTime.tm_sec = m_components[TC_SECOND];

#ifdef _MSC_VER
	return double(_mktime64(&tmTime));
#else
	return double(mktime(&tmTime));
#endif
}


void CSimpleDateTime::FromCTime(double ctime)
{
	time_t time = (time_t)ctime;

	std::tm* timePtr = localtime(&time);;
	if (timePtr != NULL){
		m_components[TC_YEAR] = timePtr->tm_year + 1900;
		m_components[TC_MONTH] = timePtr->tm_mon + 1;
		m_components[TC_DAY] = timePtr->tm_mday;
		m_components[TC_HOUR] = timePtr->tm_hour;
		m_components[TC_MINUTE] = timePtr->tm_min;
		m_components[TC_SECOND] = timePtr->tm_sec;
	}
}


} // namespace isys


