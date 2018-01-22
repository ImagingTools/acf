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


// reimplemented (ilog::IMessageContainer)

void CLogComp::ClearMessages()
{
	BaseClass2::ClearMessages();

	int childCountainersCount = GetChildsCount();
	for (int i = 0; i < childCountainersCount; ++i){
		IHierarchicalMessageContainer* childContainerPtr = GetChild(i);
		if (childContainerPtr != NULL){
			childContainerPtr->ClearMessages();
		}
	}
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

	if (m_slaveMessageConsumerCompPtr.IsValid()){
		BaseClass2::SetSlaveConsumer(m_slaveMessageConsumerCompPtr.GetPtr());
	}

	ilog::IHierarchicalMessageContainer* slaveContainerPtr = dynamic_cast<ilog::IHierarchicalMessageContainer*>(m_slaveMessageConsumerCompPtr.GetPtr());
	if (slaveContainerPtr != NULL){
		BaseClass2::AddChildContainer(slaveContainerPtr);
	}

}


} // namespace ilog


