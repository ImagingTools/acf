#include "ibase/CStreamLogCompBase.h"


#include "ibase/IMessage.h"


namespace ibase
{


// public methods

CStreamLogCompBase::CStreamLogCompBase()
:	m_isLastDotShown(false)
{
}


// reimplemented (ibase::IMessageConsumer)

void CStreamLogCompBase::AddMessage(IMessage* messagePtr)
{
	if (messagePtr != NULL){
		if (messagePtr->GetCategory() >= *m_minPriorityAttrPtr){
			WriteMessageToStream(*messagePtr);
		}

		ProcessIngoredMessage(*messagePtr);
	}
}


// reimplemented (icomp::IComponent)

void CStreamLogCompBase::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_isLastDotShown){
		NewLine();

		m_isLastDotShown = false;
	}
}


// protected methods

void CStreamLogCompBase::ProcessIngoredMessage(const ibase::IMessage& message)
{
	if (message.GetCategory() >= *m_minPriorityAttrPtr){
		if (m_isLastDotShown){
			NewLine();

			m_isLastDotShown = false;
		}
	}
	else if (*m_isDotEnabledAttrPtr){
		WriteLine(".");

		m_isLastDotShown = true;
	}
}


void CStreamLogCompBase::WriteMessageToStream(const ibase::IMessage& message)
{
	WriteLine(message.GetText());
	NewLine();
}


} // namespace ibase


