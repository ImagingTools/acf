#include <ilog/CLogComp.h>


// ACF includes
#include <ilog/CMessage.h>


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


void CLogComp::AddMessage(const MessagePtr& messagePtr)
{
	BaseClass::AddMessage(messagePtr);
}


// protected methods

// reimplemented (ilog::CLogCompBase)

void CLogComp::WriteMessageToLog(const MessagePtr& messagePtr)
{
	BaseClass2::AddMessage(messagePtr);
}


// reimplemented (icomp::CComponentBase)

void CLogComp::OnComponentCreated()
{
	if (m_maxMessageCountAttrPtr.IsValid()){
		SetMaxMessageCount(m_maxMessageCountAttrPtr->GetValue());
	}
}


} // namespace ilog


