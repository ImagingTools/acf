#include "iwin/CTimer.h"


#include <windows.h>


namespace iwin
{


CTimer::CTimer()
{
	m_startCounter = 0;

	if (s_isTimerFrequenceValid){
		Start();
	}
}


bool CTimer::IsVaild() const
{
	return s_isTimerFrequenceValid;
}


double CTimer::GetTimeTo(const CTimer& timer) const
{
	return (m_startCounter - timer.m_startCounter) / double(s_timerFrequence);
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

	return (endCounter - m_startCounter) / double(s_timerFrequence);
}


double CTimer::GetTimeTo(const ITimer& timer) const
{
	const CTimer* natTimerPtr = dynamic_cast<const CTimer*>(&timer);
	if (natTimerPtr != NULL){
		return GetTimeTo(*natTimerPtr);
	}
	else{
		return GetElapsed() - timer.GetElapsed();
	}
}


void CTimer::WaitTo(double time) const
{
	int restMs;
	while ((restMs = int((time - GetElapsed()) * 1000)) > 0){
		::SleepEx(I_DWORD(restMs), FALSE);
	}
}


double CTimer::GetTimerResolution() const
{
	return 1.0 / double(s_timerFrequence);
}



// static attributes

long long CTimer::s_timerFrequence;
bool CTimer::s_isTimerFrequenceValid = (::QueryPerformanceFrequency((LARGE_INTEGER*)&CTimer::s_timerFrequence) != 0);


} // namespace iwin

