#ifndef istd_ILogger_included
#define istd_ILogger_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"


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
		Category of message.
	*/
	enum MessageCategory
	{
		MC_INFO,
		MC_WARNING,
		MC_ERROR,
		MC_CRITICAL
	};

	/**
		Additionaly message flags.
	*/
	enum MessageFlags
	{
		MF_DEBUG = 0x1,
		MF_SYSTEM = 0x2,
		MF_USER = 0x4
	};

protected:
	/**
		Check if any log message is consumed.
		Default implementation retuns false.
	*/
	virtual bool IsLogConsumed(
				const MessageCategory* categoryPtr = NULL,
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
				MessageCategory category,
				int id,
				const QString& message,
				const QString& messageSource,
				int flags = 0) const;

	/**
		Decorate message parts before outputting.
	*/
	virtual void DecorateMessage(
				MessageCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;
};


// inline methods

inline bool ILogger::IsLogConsumed(
			const MessageCategory* /*categoryPtr*/,
			const int* /*flagsPtr*/) const
{
	return false;
}


inline bool ILogger::SendLogMessage(MessageCategory /*category*/, int /*id*/, const QString& /*message*/, const QString& /*messageSource*/, int /*flags*/) const
{
	return false;
}


inline void ILogger::DecorateMessage(
			MessageCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& /*message*/,
			QString& /*messageSource*/) const
{
}


} // namespace istd


#endif // !istd_ILogger_included


