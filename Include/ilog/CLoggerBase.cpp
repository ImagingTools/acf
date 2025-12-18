#include <ilog/CLoggerBase.h>


// ACF includes
#include <ilog/IMessageConsumer.h>
#include <ilog/CMessage.h>


namespace ilog
{


CLoggerBase::CLoggerBase()
:	m_logPtr(NULL)
{
}


// reimplemented (ilog::ILoggable)

void CLoggerBase::SetLogPtr(ilog::IMessageConsumer* logPtr)
{
	m_logPtr = logPtr;
}


ilog::IMessageConsumer* CLoggerBase::GetLogPtr() const
{
	return m_logPtr;
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

		m_logPtr->AddMessage(ilog::IMessageConsumer::MessagePtr(new ilog::CMessage(category, id, correctedMessage, correctedMessageSource, flags)));
	
		return true;
	}

	return false;
}


bool CLoggerBase::SendUserMessage(const istd::IInformationProvider* messagePtr) const
{
	if (m_logPtr != NULL){
		ilog::IMessageConsumer::MessagePtr smartMessagePtr(const_cast<istd::IInformationProvider*>(messagePtr));

		m_logPtr->AddMessage(smartMessagePtr);
	
		return true;
	}

	return false;
}


bool CLoggerBase::AllowMessageOnceAgain(int id)
{
	return m_onceMessageIds.remove(id);
}


} // namespace ilog


