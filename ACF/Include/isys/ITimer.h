#ifndef isys_ITimer_h_included
#define isys_ITimer_h_included


#include "istd/IPolymorphic.h"


namespace isys
{


/**
Common interface for timer implementations.
*/
class ITimer: virtual public istd::IPolymorphic
{
public:
	/**
	* Starts the timer.
	*/
	virtual void Start() = 0;

	/**
	* Returns the time in seconds from start until this call.
	*/
	virtual double GetElapsed() const = 0;

	/**
	* Returns the timer resolution in seconds.
	*/
	virtual double GetTimerResolution() const = 0;
};


} // namespace isys


#endif // !isys_ITimer_h_included

