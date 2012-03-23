#include "istd/CGeneralTimeStamp.h"


// Qt includes
#include <QtCore/QThread>

// ACF includes
#include "istd/TChangeNotifier.h"


class ThreadBrute: public QThread
{
public:
	using QThread::usleep;
};


namespace istd
{


CGeneralTimeStamp::CGeneralTimeStamp()
{
	m_timeShift = 0;
	m_timer.start();
}


double CGeneralTimeStamp::GetTimeTo(const CGeneralTimeStamp& timeStamp) const
{
	return m_timer.msecsTo(timeStamp.m_timer) * 0.001 + timeStamp.m_timeShift - m_timeShift;
}


// reimplemented (istd::ITimeStamp)

void CGeneralTimeStamp::Start(double elapsedTime)
{
	istd::CChangeNotifier notifier(this);

	m_timeShift = elapsedTime;
	m_timer.start();
}


double CGeneralTimeStamp::GetElapsed() const
{
#if QT_VERSION >= 0x040800
	return m_timer.nsecsElapsed() * 0.000000001 + m_timeShift;
#else
	return m_timer.elapsed() * 0.001 + m_timeShift;
#endif
}


double CGeneralTimeStamp::GetTimeTo(const ITimeStamp& timeStamp) const
{
	const CGeneralTimeStamp* natTimeStampPtr = dynamic_cast<const CGeneralTimeStamp*>(&timeStamp);
	if (natTimeStampPtr != NULL){
		return GetTimeTo(*natTimeStampPtr);
	}
	else{
		return GetElapsed() - timeStamp.GetElapsed();
	}
}


void CGeneralTimeStamp::WaitTo(double time) const
{
	double restMicroseconds;
	while ((restMicroseconds = (time - GetElapsed()) * 1000000) >= 1){
		ThreadBrute::usleep(I_DWORD(restMicroseconds));
	}
}


double CGeneralTimeStamp::GetTimerResolution() const
{
#if QT_VERSION >= 0x040700
	return 0.000001;
#else
	return 0.001;
#endif
}


} // namespace istd


