#ifndef iwin_CTimer_included
#define iwin_CTimer_included


#include "isys/ITimer.h"

#include "iwin/iwin.h"


namespace iwin
{


/**
	\ingroup iwin

	An timer implementation for the Windows plattform
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	bool IsVaild() const;

	double GetTimeTo(const CTimer& timer) const;

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	long long m_startCounter;

	// static attributes
	static long long s_timerFrequence;
	static bool s_isTimerFrequenceValid;
};


} // namespace iwin


#endif // !iwin_CTimer_included
