#include "ibase/CStreamLogCompBase.h"


// Qt includes
#include <QtCore/QDateTime.h>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ibase
{


// public methods

CStreamLogCompBase::CStreamLogCompBase()
:	m_isLastDotShown(false)
{
}


// reimplemented (ibase::IMessageConsumer)

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
	QString messageText = message.GetInformationDescription();

	if (m_useTimeStampAttrPtr.IsValid() && *m_useTimeStampAttrPtr){
		messageText = QObject::tr("[%1] %2").arg(message.GetInformationTimeStamp().toString()).arg(messageText);
	}

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


} // namespace ibase


