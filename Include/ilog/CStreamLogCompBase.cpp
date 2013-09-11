#include "ilog/CStreamLogCompBase.h"


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ilog
{


// public methods

CStreamLogCompBase::CStreamLogCompBase()
:	m_isLastDotShown(false)
{
}


// reimplemented (ilog::IMessageConsumer)

bool CStreamLogCompBase::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CStreamLogCompBase::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid()){
		if (messagePtr->GetInformationCategory() >= *m_minPriorityAttrPtr){
			if (m_isLastDotShown){
				NewLine();

				m_isLastDotShown = false;
			}

			WriteMessageToStream(*messagePtr);
		}
		else if (*m_isDotEnabledAttrPtr){
			WriteText(".");

			m_isLastDotShown = true;
		}

		BaseClass::AddMessage(messagePtr);
	}
}


// protected methods

void CStreamLogCompBase::WriteMessageToStream(const istd::IInformationProvider& message)
{
	QString messageText = GenerateMessageText(message);
	
	WriteText(messageText);

	NewLine();
}


QString CStreamLogCompBase::GenerateMessageText(const istd::IInformationProvider& message) const
{
	QString messageText;

	QString categoryCode;
	if (*m_useCategoryAttrPtr){
		switch (message.GetInformationCategory()){
		case istd::IInformationProvider::IC_WARNING:
			messageText += "warning: ";
			categoryCode = "W";
			break;

		case istd::IInformationProvider::IC_ERROR:
			messageText += "error: ";
			categoryCode = "E";
			break;

		case istd::IInformationProvider::IC_CRITICAL:
			messageText += "fatal error: ";
			categoryCode = "E!";
			break;

		default:
			categoryCode = "I";
			break;
		}
	}

	if (*m_useCodeAttrPtr){
		QString codeText = QString::number(message.GetInformationId());

		messageText += categoryCode + codeText + ": ";
	}

	if (*m_useTimeStampAttrPtr){
		QString timeStampText = message.GetInformationTimeStamp().toString();

		messageText += "[" + timeStampText + "] ";
	}

	messageText += message.GetInformationDescription();

	return messageText;
}


// reimplemented (icomp::CComponentBase)

void CStreamLogCompBase::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_isLastDotShown){
		NewLine();

		m_isLastDotShown = false;
	}
}


} // namespace ilog


