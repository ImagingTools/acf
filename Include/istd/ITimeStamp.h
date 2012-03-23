#ifndef istd_ITimeStamp_included
#define istd_ITimeStamp_included


// ACF includes
#include "istd/IChangeable.h"

#include "istd/istd.h"


namespace istd
{


/**
	Common interface for timer implementations.

	\ingroup System
*/
class ITimeStamp: virtual public istd::IChangeable
{
public:
	/**
		Start the timer.
		\param	elapsed	optional current elapsed time.
	*/
	virtual void Start(double elapsedTime = 0) = 0;

	/**
		Return the time in seconds from start until this call.
	*/
	virtual double GetElapsed() const = 0;

	/**
		Get time difference to second timer.
	*/
	virtual double GetTimeTo(const ITimeStamp& timer) const = 0;

	/**
		Wait for specific timer elapsed value is reached.
	*/
	virtual void WaitTo(double time) const = 0;

	/**
		Return the timer resolution in seconds.
	*/
	virtual double GetTimerResolution() const = 0;
};


} // namespace istd


#endif // !istd_ITimeStamp_included

