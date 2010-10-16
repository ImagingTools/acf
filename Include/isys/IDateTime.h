#ifndef istd_IDateTime_included
#define istd_IDateTime_included


// STL includes
#include <string>


// ACF includes
#include "iser/ISerializable.h"


namespace isys
{


/**
	Interface for date and time representation.

	\ingroup System
*/
class IDateTime: virtual public iser::ISerializable
{
public:
	enum TimeComponent
	{
		TC_YEAR,
		TC_MONTH,
		TC_DAY,
		TC_HOUR,
		TC_MINUTE,
		TC_SECOND,
		TC_MICROSECOND,
		TC_LAST = TC_MICROSECOND
	};

	/**
		Get single component.
	*/
	virtual int GetComponent(int component) const = 0;
	/**
		Set single component.
	*/
	virtual void SetComponent(int component, int value) = 0;

	/**
		Set to current time.
		\return	true, if current time is known and can be set.
	*/
	virtual bool SetCurrentTime() = 0;

	/**
		Returns the datetime as the number of seconds passed since 1970-01-01T00:00:00,
		(Coordinated Universal Time).
	*/
	virtual double ToCTime() const = 0;

	/**
		Set the datetime using the number of seconds passed since 1970-01-01T00:00:00,
		(Coordinated Universal Time).
	*/
	virtual void FromCTime(double ctime) = 0;

	/**
		Returns the datetime object as a string.
	*/
	virtual std::string ToString(int fromComponent = TC_YEAR, int toComponent = TC_MICROSECOND) const = 0;

	/**
		Creates a datetime object from a string.
	*/
	virtual bool FromString(const std::string& dateTime, int fromComponent = TC_YEAR, int toComponent = TC_MICROSECOND) = 0;
};


} // namespace isys


#endif // !istd_IDateTime_included


