#include "iwin/CWinTimer.h"


#include <windows.h>


namespace iwin
{


CWinTimer::CWinTimer()
{
	m_startCounter = 0;

	m_valid = ::QueryPerformanceFrequency((LARGE_INTEGER*)&m_timerFrequence) != 0;

	if (m_valid){
		Start();
	}
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


} // namespace iwin

