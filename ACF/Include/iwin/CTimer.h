#ifndef iwin_CTimer_included
#define iwin_CTimer_included


#include "iwin/iwin.h"

#include "isys/ITimer.h"


namespace iwin
{


/**
	\ingroup iwin

	An timer implementation for the Windows plattform
*/
class CTimer: public isys::ITimer
{
public:
	CTimer();

	bool IsVaild() const;

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimerResolution() const;

protected:
	long long m_startCounter;
	long long m_timerFrequence;
	bool m_isValid;
};


} // namespace iwin


#endif // !iwin_CTimer_included
