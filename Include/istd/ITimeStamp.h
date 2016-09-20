#ifndef istd_ITimeStamp_included
#define istd_ITimeStamp_included


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include <istd/IChangeable.h>


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
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		CF_START_SET = 0xa623a
	};

	/**
		Start the timer.
		\param	elapsed	optional current elapsed time.
	*/
	virtual void Start(double elapsedTime = 0) = 0;

	/**
		Get point in time of latest start.
	*/
	virtual QDateTime GetStartTime() const = 0;

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

