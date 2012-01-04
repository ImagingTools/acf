#include "iqt/CTimer.h"


// Qt includes
#include <QThread>

// ACF includes
#include "istd/TChangeNotifier.h"


class ThreadBrute: public QThread
{
public:
	using QThread::usleep;
};


namespace iqt
{


CTimer::CTimer()
{
	m_timeShift = 0;
	m_time.start();
}


double CTimer::GetTimeTo(const CTimer& timer) const
{
	return m_time.msecsTo(timer.m_time) * 0.001 + timer.m_timeShift - m_timeShift;
}


unsigned long long CTimer::GetNativeRepresentation() const
{
	return 0;
}


void CTimer::SetNativeRepresentation(unsigned long long/* value*/)
{
	I_CRITICAL();	// not implemented yet!
}


// reimplemented (isys::ITimer)

void CTimer::Start(double elapsedTime)
{
	istd::CChangeNotifier notifier(this);

	m_timeShift = elapsedTime;
	m_time.start();
}


double CTimer::GetElapsed() const
{
	return m_time.elapsed() * 0.001 + m_timeShift;
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
	double restMicroseconds;
	while ((restMicroseconds = (time - GetElapsed()) * 1000000) >= 1){
		ThreadBrute::usleep(I_DWORD(restMicroseconds));
	}
}


double CTimer::GetTimerResolution() const
{
	return 0.001;
}


} // namespace iqt


