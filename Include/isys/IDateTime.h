#ifndef istd_IDateTime_included
#define istd_IDateTime_included


// STL includes
#include <string>


// Qt includes
#include <QString>


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
	/**
		Separator token for the Date/Time string conversion given in the form:
		YEAR TimeSeparator[0] MONTH TimeSeparator[0] DAY TimeSeparator[1] HOUR TimeSeparator[2] MINUTE TimeSeparator[2] SECOND TimeSeparator[3] MILLISECOND
	*/
	typedef char TimeSeparator[4];

	/**
		Separator tokens for ISO string representation of a date/time object.
		TS_ISO = {'-',' ',':', '.'}, for example: 2010-01-01 12:30:05.45
	*/
	static const TimeSeparator TS_ISO;

	/**
		Separator tokens for string representation of a date/time object, which is valid as a name for a file system item.
		TS_FILENAME = {'-',' ','.','.'}
	*/
	static const TimeSeparator TS_FILENAME;

	/**
		Date/Time basic components.
	*/
	enum TimeComponent
	{
		TC_YEAR,
		TC_MONTH,
		TC_DAY,
		TC_HOUR,
		TC_MINUTE,
		TC_SECOND,
		TC_MILLISECOND,
		TC_LAST = TC_MILLISECOND
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
	virtual QString ToString(int fromComponent = TC_YEAR, int toComponent = TC_MILLISECOND, const TimeSeparator& separator = TS_ISO) const = 0;

	/**
		Creates a datetime object from a string.
	*/
	virtual bool FromString(const std::string& dateTime, int fromComponent = TC_YEAR, int toComponent = TC_MILLISECOND) = 0;
};


} // namespace isys


#endif // !istd_IDateTime_included


