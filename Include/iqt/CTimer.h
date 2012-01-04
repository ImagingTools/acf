#ifndef iqt_CTimer_included
#define iqt_CTimer_included


#include <QElapsedTimer>

#include "isys/ITimer.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	An timer implementation for Qt library.
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	double GetTimeTo(const CTimer& timer) const;

	/**
		Dummie implementation, provided only for compatibility with iwin::CTimer.
		Please don't use!
	*/
	unsigned long long GetNativeRepresentation() const;
	/**
		Dummie implementation, provided only for compatibility with iwin::CTimer.
		Please don't use!
	*/
	void SetNativeRepresentation(unsigned long long value);

	// reimplemented (isys::ITimer)
	virtual void Start(double elapsedTime = 0);
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	QElapsedTimer m_time;
	double m_timeShift;
};


} // namespace iqt


#endif // !iqt_CTimer_included
