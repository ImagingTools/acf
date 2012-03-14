#ifndef istd_ILogger_included
#define istd_ILogger_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/IInformation.h"


namespace istd
{


/**
	Common interface for classes sending some log info.
	Default implementation is provided for convinience and it does nothing.
*/
class ILogger: virtual public IPolymorphic
{
public:
	/**
		Check if any log message is consumed.
		Default implementation retuns false.
	*/
	virtual bool IsLogConsumed(
				const IInformation::InformationCategory* categoryPtr = NULL,
				const int* flagsPtr = NULL) const;

	/**
		Send any message to log.
		Default implementation do nothing.
		\param	category		message category.
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message.
		\return	true, if it was possible to send this message and it is 'consumed'.
	*/
	virtual bool SendLogMessage(
				IInformation::InformationCategory category,
				int id,
				const QString& message,
				const QString& messageSource,
				int flags = 0) const;

protected:
	/**
		Decorate message parts before outputting.
	*/
	virtual void DecorateMessage(
				IInformation::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;
};


// inline methods

inline bool ILogger::IsLogConsumed(
			const IInformation::InformationCategory* /*categoryPtr*/,
			const int* /*flagsPtr*/) const
{
	return false;
}


inline bool ILogger::SendLogMessage(
			IInformation::InformationCategory /*category*/,
			int /*id*/,
			const QString& /*message*/,
			const QString& /*messageSource*/,
			int /*flags*/) const
{
	return false;
}


// protected methods

inline void ILogger::DecorateMessage(
			IInformation::InformationCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& /*message*/,
			QString& /*messageSource*/) const
{
}


} // namespace istd


#endif // !istd_ILogger_included


