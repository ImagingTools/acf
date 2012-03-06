#ifndef istd_ITrace_included
#define istd_ITrace_included


// STL includes
#include <string>

// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Provide system logging.
*/
class ITrace: virtual public IPolymorphic
{
public:
	/**
	Send message to log system.
	\param	level		priority of this message.
	\param	groupId		ID of trace group, it is used to allow enable/disable of traceing of specified groups.
	\param	message		log message text.
	\param	fileName	optional file name. If it is empty string, no file is specified.
	\param	line		optional line number in file. If it is negative value, no line is specified.
	*/
	virtual void LogMessage(
				TraceLevel level,
				const std::string& groupId,
				const QString& message,
				const std::string& fileName = std::string(),
				int line = -1) const = 0;
	/**
	Check if traceing of this element is enabled.
	This must be checked before LogMessage will be called.
	*/
	virtual bool IsTraceEnabled(TraceLevel level, const std::string& groupId) const = 0;
	/**
	Register ID of group.
	It is used to create list of possible groups.
	*/
	virtual void RegisterGroupId(const std::string& groupId) = 0;
};


} // namespace istd


#endif // !istd_ITrace_included


