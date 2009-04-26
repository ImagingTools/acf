#ifndef iwin_CTimer_included
#define iwin_CTimer_included


#include "isys/ITimer.h"

#include "iwin/iwin.h"


namespace iwin
{


/**
	An timer implementation for the Windows plattform

	\ingroup Windows
*/
class CTimer: virtual public isys::ITimer
{
public:
	CTimer();

	bool IsVaild() const;

	double GetTimeTo(const CTimer& timer) const;

	unsigned long long GetNativeRepresentation() const;
	void SetNativeRepresentation(unsigned long long value);

	// reimplemented (isys::ITimer)
	virtual void Start();
	virtual double GetElapsed() const;
	virtual double GetTimeTo(const ITimer& timer) const;
	virtual void WaitTo(double time) const;
	virtual double GetTimerResolution() const;

private:
	long long m_startCounter;

	// static attributes
	static unsigned long long s_timerFrequence;
	static bool s_isTimerFrequenceValid;
};


// inline methods

inline unsigned long long CTimer::GetNativeRepresentation() const
{
	return m_startCounter;
}


inline void CTimer::SetNativeRepresentation(unsigned long long value)
{
	m_startCounter = value;
}


} // namespace iwin


#endif // !iwin_CTimer_included
