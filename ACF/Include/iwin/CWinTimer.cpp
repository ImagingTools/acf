#include "iwin/CWinTimer.h"


#include <windows.h>


namespace iwin
{


CWinTimer::CWinTimer()
{
	m_startCounter = 0;

	m_isValid = (::QueryPerformanceFrequency((LARGE_INTEGER*)&m_timerFrequence) != 0);

	if (m_isValid){
		Start();
	}
}


bool CWinTimer::IsVaild() const
{
	return m_isValid;
}


// reimplemented (isys::ITimer)

void CWinTimer::Start()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_startCounter);
}


double CWinTimer::GetElapsed() const
{
	long long endCounter;
	::QueryPerformanceCounter((LARGE_INTEGER*)&endCounter);

	return (endCounter - m_startCounter) / (double)(m_timerFrequence);
}


double CWinTimer::GetTimerResolution() const
{
	return 1.0 / (double)(m_timerFrequence);
}



} // namespace iwin

