#ifndef iwin_CWinTimer_h_included
#define iwin_CWinTimer_h_included


#include "isys/ITimer.h"


namespace iwin
{


class CWinTimer: public isys::ITimer
{
public:
	CWinTimer();

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


#endif // !iwin_CWinTimer_h_included
