#ifndef iqt_CTimer_included
#define iqt_CTimer_included


#include <QTime>

#include "isys/ITimer.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	\ingroup iqt

	An timer implementation for Qt library.
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	double GetTimeTo(const CTimer& timer) const;

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	QTime m_time;
};


} // namespace iqt


#endif // !iqt_CTimer_included
