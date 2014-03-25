#ifndef ilog_TLoggerWrap_included
#define ilog_TLoggerWrap_included


// ACF includes
#include "istd/ILogger.h"
#include "ilog/IMessageConsumer.h"
#include "ilog/ILoggable.h"
#include "ilog/CMessage.h"


namespace ilog
{


/**
	Class wrapper implementing interface istd::ILogger sending log messages over ilog::IMessageConsumer.
	Access to interface ilog::IMessageConsumer must be registered by user.
	To register it use interface ilog::ILoggable implemented by this wrapper.

	\ingroup Logging
*/
template <class Base>
class TLoggerWrap: public Base, virtual public ILoggable, virtual public istd::ILogger
{
public:
	TLoggerWrap();

	// reimplemented (ilog::ILoggable)
	virtual void SetLogPtr(ilog::IMessageConsumer* logPtr);
	virtual ilog::IMessageConsumer* GetLogPtr() const;
	virtual void SetTracingEnabled(bool trace);
	virtual bool IsTracingEnabled() const;

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
	ilog::IMessageConsumer* m_logPtr;
	bool m_isTracingEnabled;
	mutable QSet<int> m_onceMessageIds;
};


// public methods

template <class Base>
TLoggerWrap<Base>::TLoggerWrap()
:	m_logPtr(NULL),
	m_isTracingEnabled(false)
{
}


// pseudo-reimplemented (ilog::ILoggable)

template <class Base>
inline void TLoggerWrap<Base>::SetLogPtr(ilog::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


template <class Base>
inline ilog::IMessageConsumer* TLoggerWrap<Base>::GetLogPtr() const
{
	return m_logPtr;
}


// Trace protocol

template <class Base>
void TLoggerWrap<Base>::SetTracingEnabled(const bool trace)
{
	m_isTracingEnabled = trace;
}


template <class Base>
bool TLoggerWrap<Base>::IsTracingEnabled() const
{
	return m_isTracingEnabled;
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


template <class Base>
bool TLoggerWrap<Base>::SendInfoMessageOnce(
			int id,
			const QString& message,
			const QString& messageSource,
			int flags) const
{
	if (!m_onceMessageIds.contains(id)){
		m_onceMessageIds.insert(id);

		return SendInfoMessage(id, message, messageSource, flags);
	}

	return false;
}


template <class Base>
bool TLoggerWrap<Base>::SendWarningMessageOnce(
			int id,
			const QString& message,
			const QString& messageSource,
			int flags) const
{
	if (!m_onceMessageIds.contains(id)){
		m_onceMessageIds.insert(id);

		return SendWarningMessage(id, message, messageSource, flags);
	}

	return false;
}



template <class Base>
bool TLoggerWrap<Base>::SendErrorMessageOnce(
			int id,
			const QString& message,
			const QString& messageSource,
			int flags) const
{
	if (!m_onceMessageIds.contains(id)){
		m_onceMessageIds.insert(id);

		return SendErrorMessage(id, message, messageSource, flags);
	}

	return false;
}



template <class Base>
bool TLoggerWrap<Base>::SendCriticalMessageOnce(
			int id,
			const QString& message,
			const QString& messageSource,
			int flags) const
{
	if (!m_onceMessageIds.contains(id)){
		m_onceMessageIds.insert(id);

		return SendCriticalMessage(id, message, messageSource, flags);
	}

	return false;
}


// reimplemented (istd::ILogger)

template <class Base>
bool TLoggerWrap<Base>::IsLogConsumed(
			const istd::IInformationProvider::InformationCategory* categoryPtr,
			const int* /*flagsPtr*/) const
{
	return (m_logPtr != NULL) && ((categoryPtr == NULL) || m_logPtr->IsMessageSupported(*categoryPtr));
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

		m_logPtr->AddMessage(istd::TSmartPtr<const istd::IInformationProvider>(new ilog::CMessage(category, id, correctedMessage, correctedMessageSource, flags)));
	
		return true;
	}

	return false;
}


} // namespace ilog


#endif // !ilog_TLoggerWrap_included


