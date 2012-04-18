#ifndef ibase_TLoggerWrap_included
#define ibase_TLoggerWrap_included


#include "istd/ILogger.h"

#include "ibase/IMessageConsumer.h"
#include "ibase/ILoggable.h"
#include "ibase/CMessage.h"


namespace ibase
{		


/**
	Class wrapper implementing interface istd::ILogger sending log messages over ibase::IMessageConsumer.
	Access to interface ibase::IMessageConsumer must be registered by user.
	To register it use interface ibase::ILoggable implemented by this wrapper.

	\ingroup Helpers
*/
template <class Base>
class TLoggerWrap: public Base, virtual public ILoggable, virtual public istd::ILogger
{
public:
	TLoggerWrap();

	// reimplemented (ibase::ILoggable)
	virtual void SetLogPtr(ibase::IMessageConsumer* logPtr);
	virtual ibase::IMessageConsumer* GetLogPtr() const;

protected:
	/**
		Send info message to log.
		\sa istd::IInformationProvider for message meaning documentation.
		\param	id				binary id identifying this message type for automatical processing.
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
		\param	id				binary id identifying this message type for automatical processing.
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
		\param	id				binary id identifying this message type for automatical processing.
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
		\param	id				binary id identifying this message type for automatical processing.
		\param	message			message text will be send.
		\param	messageSource	optional human readable description of message source.
	*/
	bool SendCriticalMessage(
				int id,
				const QString& message,
				const QString& messageSource = QString(),
				int flags = 0) const;

	// reimplemented (istd::ILogger)
	virtual bool IsLogConsumed(
				const istd::IInformationProvider::InformationCategory* categoryPtr = NULL,
				const int* flagsPtr = NULL) const;
	virtual bool SendLogMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& message,
				const QString& messageSource,
				int flags = 0) const;

private:
	ibase::IMessageConsumer* m_logPtr;
};


// public methods

template <class Base>
TLoggerWrap<Base>::TLoggerWrap()
:	m_logPtr(NULL)
{
}


// pseudo-reimplemented (ibase::ILoggable)

template <class Base>
inline void TLoggerWrap<Base>::SetLogPtr(ibase::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


template <class Base>
inline ibase::IMessageConsumer* TLoggerWrap<Base>::GetLogPtr() const
{
	return m_logPtr;
}


// protected methods

template <class Base>
bool TLoggerWrap<Base>::SendInfoMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_INFO, id, message, messageSource, flags);
}


template <class Base>
bool TLoggerWrap<Base>::SendWarningMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_WARNING, id, message, messageSource, flags);
}


template <class Base>
bool TLoggerWrap<Base>::SendErrorMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_ERROR, id, message, messageSource, flags);
}


template <class Base>
bool TLoggerWrap<Base>::SendCriticalMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_CRITICAL, id, message, messageSource, flags);
}


// reimplemented (istd::ILogger)

template <class Base>
bool TLoggerWrap<Base>::IsLogConsumed(
			const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
			const int* /*flagsPtr*/) const
{
	return (m_logPtr != NULL);
}


template <class Base>
bool TLoggerWrap<Base>::SendLogMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& message,
			const QString& messageSource,
			int flags) const
{
	if (m_logPtr != NULL){
		QString correctedMessage = message;
		QString correctedMessageSource = messageSource;

		DecorateMessage(category, id, flags, correctedMessage, correctedMessageSource);

		m_logPtr->AddMessage(istd::TSmartPtr<const istd::IInformationProvider>(new ibase::CMessage(category, id, correctedMessage, correctedMessageSource, flags)));
	
		return true;
	}

	return false;
}


} // namespace ibase


#endif // !ibase_TLoggerWrap_included


