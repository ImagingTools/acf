#include <ilog/CLoggerBase.h>


// ACF includes
#include <ilog/IMessageConsumer.h>
#include <ilog/CMessage.h>


namespace ilog
{


CLoggerBase::CLoggerBase()
:	m_logPtr(NULL),
	m_isTracingEnabled(false)
{
}


// reimplemented (ilog::ILoggable)

inline void CLoggerBase::SetLogPtr(ilog::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


inline ilog::IMessageConsumer* CLoggerBase::GetLogPtr() const
{
	return m_logPtr;
}


// Trace protocol

void CLoggerBase::SetTracingEnabled(const bool trace)
{
	m_isTracingEnabled = trace;
}


bool CLoggerBase::IsTracingEnabled() const
{
	return m_isTracingEnabled;
}


// protected methods

bool CLoggerBase::SendInfoMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_INFO, id, message, messageSource, flags);
}


bool CLoggerBase::SendWarningMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_WARNING, id, message, messageSource, flags);
}


bool CLoggerBase::SendErrorMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_ERROR, id, message, messageSource, flags);
}


bool CLoggerBase::SendCriticalMessage(int id, const QString& message, const QString& messageSource, int flags) const
{
	return SendLogMessage(istd::IInformationProvider::IC_CRITICAL, id, message, messageSource, flags);
}


bool CLoggerBase::SendInfoMessageOnce(
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


bool CLoggerBase::SendWarningMessageOnce(
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


bool CLoggerBase::SendErrorMessageOnce(
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


bool CLoggerBase::SendCriticalMessageOnce(
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


void CLoggerBase::DecorateMessage(
			istd::IInformationProvider::InformationCategory /*category*/,
			int /*id*/,
			int /*flags*/,
			QString& /*message*/,
			QString& /*messageSource*/) const
{
}


// reimplemented (istd::ILogger)

bool CLoggerBase::IsLogConsumed(
			const istd::IInformationProvider::InformationCategory* categoryPtr,
			const int* /*flagsPtr*/) const
{
	return (m_logPtr != NULL) && ((categoryPtr == NULL) || m_logPtr->IsMessageSupported(*categoryPtr));
}


bool CLoggerBase::SendLogMessage(
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


bool CLoggerBase::AllowMessageOnceAgain(int id)
{
	return m_onceMessageIds.remove(id);
}


} // namespace ilog


