#include "ilog/CLogComp.h"


namespace ilog
{


// public methods

// reimplemented (ilog::IMessageConsumer)

bool CLogComp::IsMessageSupported(
			int messageCategory,
			int messageId,
			const istd::IInformationProvider* messagePtr) const
{
	return BaseClass2::IsMessageSupported(messageCategory, messageId, messagePtr);
}


void CLogComp::AddMessage(const IMessageConsumer::MessagePtr& messagePtr)
{
	BaseClass2::AddMessage(messagePtr);

	BaseClass::AddMessage(messagePtr);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CLogComp::OnComponentCreated()
{
	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


} // namespace ilog


