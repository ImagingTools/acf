#include "iwin/CTimer.h"


#include <windows.h>


namespace iwin
{


CTimer::CTimer()
{
	m_startCounter = 0;

	m_isValid = (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_timerFrequence) != 0);

	if (m_isValid){
		Start();
	}
}


bool CTimer::IsVaild() const
{
	return m_isValid;
}


// reimplemented (isys::ITimer)

void CTimer::Start()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_startCounter);
}


double CTimer::GetElapsed() const
{
	I_ASSERT(sizeof(long long) == sizeof(LARGE_INTEGER));

	long long endCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&endCounter);

	return (endCounter - m_startCounter) / double(m_timerFrequence);
}


double CTimer::GetTimerResolution() const
{
	return 1.0 / double(m_timerFrequence);
}



} // namespace iwin

