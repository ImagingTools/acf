#include "ibase/CStreamLogCompBase.h"


// ACF includes
#include "isys/IDateTime.h"

#include "ibase/IMessage.h"


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
			const IMessage* /*messagePtr*/) const
{
	return true;
}


void CStreamLogCompBase::AddMessage(const MessagePtr& messagePtr)
{
	if (messagePtr.IsValid()){
		if (messagePtr->GetCategory() >= *m_minPriorityAttrPtr){
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

void CStreamLogCompBase::WriteMessageToStream(const ibase::IMessage& message)
{
	QString messageText = GenerateMessageText(message);
	
	WriteText(messageText);

	NewLine();
}


QString CStreamLogCompBase::GenerateMessageText(const ibase::IMessage& message) const
{
	QString messageText = message.GetText();

	if (m_useTimeStampAttrPtr.IsValid() && *m_useTimeStampAttrPtr){
		messageText =
					QString("[") +
					message.GetTimeStamp().ToString(isys::IDateTime::TC_YEAR, isys::IDateTime::TC_SECOND) +
					QString("] ") +
					messageText;
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


