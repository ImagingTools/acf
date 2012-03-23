#ifndef iqt_CGeneralTimeStamp_included
#define iqt_CGeneralTimeStamp_included


#include <QtCore/QObject>
#if QT_VERSION >= 0x040700
	#include <QtCore/QElapsedTimer>
#else
	#include <QtCore/QTime>
#endif

#include "istd/ITimeStamp.h"


namespace istd
{


/**
	An general timeStamp implementation.
*/
class CGeneralTimeStamp: virtual public ITimeStamp
{
public:
	CGeneralTimeStamp();

	double GetTimeTo(const CGeneralTimeStamp& timeStamp) const;

	// reimplemented (istd::ITimeStamp)
	virtual void Start(double elapsedTime = 0);
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimeStamp& timeStamp) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
#if QT_VERSION >= 0x040700
	QElapsedTimer m_timer;
#else
	QTime m_timer;
#endif

	double m_timeShift;
};


} // namespace istd


#endif // !iqt_CGeneralTimeStamp_included


