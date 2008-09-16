#ifndef isys_ITimer_included
#define isys_ITimer_included


#include "isys/isys.h"


#include "istd/IPolymorphic.h"


namespace isys
{


/**
	\ingroup isys

	Common interface for timer implementations.
*/
class ITimer: virtual public istd::IPolymorphic
{
public:
	/**
		Start the timer.
	*/
	virtual void Start() = 0;

	/**
		Return the time in seconds from start until this call.
	*/
	virtual double GetElapsed() const = 0;

	/**
		Get time difference to second timer.
	*/
	virtual double GetTimeTo(const ITimer& timer) const = 0;

	/**
		Wait for specific timer elapsed value is reached.
	*/
	virtual void WaitTo(double time) const = 0;

	/**
		Return the timer resolution in seconds.
	*/
	virtual double GetTimerResolution() const = 0;
};


} // namespace isys


#endif // !isys_ITimer_included

