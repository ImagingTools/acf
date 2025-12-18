#ifndef ilog_CLoggerBase_included
#define ilog_CLoggerBase_included


// ACF includes
#include <istd/ILogger.h>
#include <ilog/ILoggable.h>


namespace ilog
{


class IMessageConsumer;


/**
	Base class implementing interface istd::ILogger sending log messages over ilog::IMessageConsumer.
	Access to interface ilog::IMessageConsumer must be registered by user.
	To register it use interface ilog::ILoggable implemented by this wrapper.

	\ingroup Logging
*/
class CLoggerBase:
			virtual public ILoggable,
			virtual protected istd::ILogger
{
public:
	CLoggerBase();

	// reimplemented (ilog::ILoggable)
	virtual void SetLogPtr(ilog::IMessageConsumer* logPtr) override;
	virtual ilog::IMessageConsumer* GetLogPtr() const override;

protected:
	/**
		Send info message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendInfoMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send warning message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendWarningMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send error message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendErrorMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send critical message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once info message to log.
		Agains to \c SendInfoMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendInfoMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once warning message to log.
		Agains to \c SendWarningMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendWarningMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once error message to log.
		Agains to \c SendErrorMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	source of the message
	*/
	bool SendErrorMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send once critical message to log.
		Agains to \c SendCriticalMessage the message will be sent only first time.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatic processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessageOnce(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	/**
		Send message with user object.
		\param	messagePtr	pointer to user message object.
							This function overtake ownership to this object.
	*/
	bool SendUserMessage(const istd::IInformationProvider* messagePtr) const;

	/**
		Reset message lock. Enable message to be send again.
		Agains to \c SendXXXlMessageOnce for id will be sent once again.
		\param	id				binary id identifying this message type for automatic processing.
	*/
	bool AllowMessageOnceAgain(int id);

	/**
		Decorate message parts before outputting.
		It is designed to be overloaded if you want to change the decoration.
	*/
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	// reimplemented (istd::ILogger)
	virtual bool IsLogConsumed(
				const istd::IInformationProvider::InformationCategory* categoryPtr = NULL,
				const int* flagsPtr = NULL) const override;
	virtual bool SendLogMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& message,
				const QString& messageSource,
				int flags = 0) const override;

protected:
	mutable QSet<int> m_onceMessageIds;

private:
	ilog::IMessageConsumer* m_logPtr;
};


} // namespace ilog


#endif // !ilog_CLoggerBase_included


