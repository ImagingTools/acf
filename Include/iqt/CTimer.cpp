#include "iqt/CTimer.h"


#include <QThread>


class ThreadBrute: public QThread
{
public:
	using QThread::usleep;
};


namespace iqt
{


CTimer::CTimer()
{
	m_time.start();
}


double CTimer::GetTimeTo(const CTimer& timer) const
{
	return m_time.msecsTo(timer.m_time) * 0.001;
}


// reimplemented (isys::ITimer)

void CTimer::Start()
{
	m_time.start();
}


double CTimer::GetElapsed() const
{
	return m_time.elapsed() * 0.001;
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
	int restMis;
	while ((restMis = int((time - GetElapsed()) * 1000000)) > 0){
		ThreadBrute::usleep(I_DWORD(restMis));
	}
}


double CTimer::GetTimerResolution() const
{
	return 0.001;
}



} // namespace iqt


