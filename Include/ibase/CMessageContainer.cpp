#include "ibase/CMessageContainer.h"


// ACF includes
#include "istd/CEventBasedNotifier.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{


CMessageContainer::CMessageContainer()
:	m_slaveConsumerPtr(NULL),
	m_maxMessagesCount(-1),
	m_maxLiveTime(-1),
	m_worstCategory(-1),
	m_lock(QMutex::Recursive)
{
}


void CMessageContainer::AddChildContainer(IHierarchicalMessageContainer* childContainerPtr)
{
	QMutexLocker lock(&m_lock);

	I_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
}


void CMessageContainer::SetSlaveConsumer(ibase::IMessageConsumer* consumerPtr)
{
	QMutexLocker lock(&m_lock);

	m_slaveConsumerPtr = consumerPtr;
}


void CMessageContainer::SetMaxMessageCount(int maxMessageCount)
{
	I_ASSERT(m_maxMessagesCount != 0);

	QMutexLocker lock(&m_lock);

	m_maxMessagesCount = maxMessageCount;
}


void CMessageContainer::SetMaxLiveTime(int maxLiveTime)
{
	QMutexLocker lock(&m_lock);

	m_maxLiveTime = maxLiveTime;
}


// reimplemented (iser::ISerializable)

bool CMessageContainer::Serialize(iser::IArchive& archive)
{
	QMutexLocker lock(&m_lock);

	static iser::CArchiveTag messagesTag("Messages", "List of messages");
	static iser::CArchiveTag messageTag("Message", "Message");

	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = true;

	int messageCount = int(m_messages.size());

	retVal = retVal && archive.BeginMultiTag(messagesTag, messageTag, messageCount);

	if (!retVal){
		return false;
	}

	for (		MessageList::ConstIterator iter = m_messages.constBegin();
				iter != m_messages.constEnd();
				++iter){
		const IMessageConsumer::MessagePtr& messagePtr = *iter;

		iser::ISerializable* serMessagePtr = const_cast<iser::ISerializable*>(dynamic_cast<const iser::ISerializable*>(messagePtr.GetPtr()));

		retVal = retVal && archive.BeginTag(messageTag);
		retVal = retVal && serMessagePtr->Serialize(archive);
		retVal = retVal && archive.EndTag(messageTag);
	}

	retVal = retVal && archive.EndTag(messagesTag);

	return retVal;
}


// reimplemented (ibase::IMessageContainer)

int CMessageContainer::GetWorstCategory() const
{
	QMutexLocker lock(&m_lock);

	int worstCategory = m_worstCategory;
	int childCount = GetChildsCount();

	if (worstCategory < 0){
		worstCategory = 0;

	for (		MessageList::ConstIterator iter = m_messages.constBegin();
				iter != m_messages.constEnd();
				++iter){
			const IMessageConsumer::MessagePtr& messagePtr = *iter;

			int category = messagePtr->GetInformationCategory();
			if (category > worstCategory){
				worstCategory = category;
			}
		}
	}

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		IMessageContainer* childPtr = dynamic_cast<IMessageContainer*>(GetChild(childIndex));
		if (childPtr != NULL){
			int category = childPtr->GetWorstCategory();
			if (category > worstCategory){
				worstCategory = category;
			}
		}
	}

	return worstCategory;
}


IMessageContainer::Messages CMessageContainer::GetMessages() const
{
	QMutexLocker lock(&m_lock);

	IMessageContainer::Messages messages;

	for (		MessageList::ConstIterator iter = m_messages.constBegin();
				iter != m_messages.constEnd();
				++iter){
		
		istd::TDelPtr<istd::IInformationProvider> messagePtr;

		istd::IChangeable *cloned = (*iter)->CloneMe();
		cloned->CopyFrom(*(*iter));
		messagePtr.SetCastedOrRemove<istd::IChangeable>(cloned);		
		if (messagePtr.IsValid()){
			IMessageConsumer::MessagePtr messageCopyPtr(messagePtr.PopPtr());
	
			messages.push_back(messageCopyPtr);
		}
	}

	int childsCount = GetChildsCount();
	for (int childIndex = 0; childIndex < childsCount; childIndex++){
		IMessageContainer* childPtr = GetChild(childIndex);
		I_ASSERT(childPtr != NULL);

		IMessageContainer::Messages childMessages = childPtr->GetMessages();

		messages += childMessages;
	}

	return messages;
}


bool CMessageContainer::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


void CMessageContainer::AddMessage(const IMessageConsumer::MessagePtr& messagePtr)
{
	QMutexLocker lock(&m_lock);

	I_ASSERT(messagePtr.IsValid());

	if (m_maxMessagesCount == 0){
		return;
	}

	{
		istd::CEventBasedNotifier changePtr(
					this,
					CF_MODEL | CF_MESSAGE_ADDED,
					const_cast<istd::IInformationProvider*>(messagePtr.GetPtr()));

		m_messages.push_front(messagePtr);

		int messageCategory = messagePtr->GetInformationCategory();
		if ((m_worstCategory >= 0) && (messageCategory > m_worstCategory)){
			m_worstCategory = messageCategory;
		}
	}

	if (m_maxMessagesCount >= 0){
		while (int(m_messages.size()) > m_maxMessagesCount){
			I_ASSERT(!m_messages.isEmpty());
			const IMessageConsumer::MessagePtr& messageToRemovePtr = m_messages.back();

			istd::CEventBasedNotifier changePtr(
						this,
						CF_MODEL | CF_MESSAGE_REMOVED,
						const_cast<istd::IInformationProvider*>(messageToRemovePtr.GetPtr()));

			int removeCategory = messageToRemovePtr->GetInformationCategory();
			if (removeCategory >= m_worstCategory){
				m_worstCategory = -1;
			}

			m_messages.pop_back();
		}
	}

	if (m_slaveConsumerPtr != NULL){
		m_slaveConsumerPtr->AddMessage(messagePtr);
	}
}


void CMessageContainer::ClearMessages()
{
	QMutexLocker lock(&m_lock);
	if (!m_messages.isEmpty()){
		istd::CEventBasedNotifier changePtr(this, CF_MODEL | CF_RESET);
		
		m_messages.clear();

		m_worstCategory = 0;
	}
}


// reimplemented (istd::IHierarchical)

int CMessageContainer::GetChildsCount() const
{
	return int(m_childContainers.size());
}


IHierarchicalMessageContainer* CMessageContainer::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_childContainers.size()));

	return m_childContainers.at(index);
}


// reimplemented (istd::IChangeable)

bool CMessageContainer::CopyFrom(const istd::IChangeable& object)
{
	QMutexLocker lock(&m_lock);

	m_messages.clear();
	istd::CEventBasedNotifier changePtr(this, CF_MODEL | CF_RESET | CF_MESSAGE_ADDED);

	const CMessageContainer* sourcePtr = dynamic_cast<const CMessageContainer*>(&object);
	if (sourcePtr != NULL){
		int sourceMessageCount = sourcePtr->m_messages.count();
		for (int messageIndex = 0; messageIndex < sourceMessageCount; messageIndex++){
			const MessagePtr& sourceMessage = sourcePtr->m_messages[messageIndex];
			I_ASSERT(sourceMessage.IsValid());

			istd::TDelPtr<istd::IInformationProvider> newMessagePtr;
			newMessagePtr.SetCastedOrRemove<istd::IChangeable>(sourceMessage->CloneMe());

			MessagePtr messagePtr(newMessagePtr.PopPtr());
			if (messagePtr.IsValid()){
				m_messages.push_back(messagePtr);
			}
		}

		return true;
	}

	return false;
}


} // namespace ibase


