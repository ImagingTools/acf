#include "ibase/CMessageContainer.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{


CMessageContainer::CMessageContainer()
:	m_slaveConsumerPtr(NULL),
	m_maxMessagesCount(-1),
	m_worstCategory(-1)
{
}


CMessageContainer::CMessageContainer(const CMessageContainer& container)
:	m_messages(container.m_messages),
	m_slaveConsumerPtr(NULL),
	m_maxMessagesCount(container.m_maxMessagesCount),
	m_worstCategory(container.m_worstCategory)
{
}


void CMessageContainer::AddChildContainer(IHierarchicalMessageContainer* childContainerPtr)
{
	Q_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
}


void CMessageContainer::SetSlaveConsumer(ibase::IMessageConsumer* consumerPtr)
{
	m_slaveConsumerPtr = consumerPtr;
}


void CMessageContainer::SetMaxMessageCount(int maxMessageCount)
{
	Q_ASSERT(m_maxMessagesCount != 0);

	m_maxMessagesCount = maxMessageCount;
}


// reimplemented (iser::ISerializable)

bool CMessageContainer::Serialize(iser::IArchive& archive)
{
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
	IMessageContainer::Messages messages;

	for (		MessageList::ConstIterator iter = m_messages.constBegin();
				iter != m_messages.constEnd();
				++iter){	
		messages.push_back(*iter);
	}

	int childsCount = GetChildsCount();
	for (int childIndex = 0; childIndex < childsCount; childIndex++){
		IMessageContainer* childPtr = GetChild(childIndex);
		Q_ASSERT(childPtr != NULL);

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
	Q_ASSERT(messagePtr.IsValid());

	if (m_maxMessagesCount == 0){
		return;
	}

	{
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_MESSAGE_ADDED, const_cast<istd::IInformationProvider*>(messagePtr.GetPtr()));

		m_messages.push_front(messagePtr);

		int messageCategory = messagePtr->GetInformationCategory();
		if ((m_worstCategory >= 0) && (messageCategory > m_worstCategory)){
			m_worstCategory = messageCategory;
		}
	}

	if (m_maxMessagesCount >= 0){
		while (int(m_messages.size()) > m_maxMessagesCount){
			Q_ASSERT(!m_messages.isEmpty());
			const IMessageConsumer::MessagePtr& messageToRemovePtr = m_messages.back();

			istd::CChangeNotifier changePtr(this, CF_MODEL | CF_MESSAGE_REMOVED, const_cast<istd::IInformationProvider*>(messageToRemovePtr.GetPtr()));

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
	if (!m_messages.isEmpty()){
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_RESET);
		
		m_messages.clear();

		m_worstCategory = 0;
	}

	for (		Childs::ConstIterator childIter = m_childContainers.constBegin();
				childIter != m_childContainers.constEnd();
				childIter++){
		(*childIter)->ClearMessages();
	}
}


// reimplemented (istd::IHierarchical)

int CMessageContainer::GetChildsCount() const
{
	return int(m_childContainers.size());
}


IHierarchicalMessageContainer* CMessageContainer::GetChild(int index) const
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_childContainers.size()));

	return m_childContainers.at(index);
}


// reimplemented (istd::IChangeable)

bool CMessageContainer::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	m_messages.clear();

	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_RESET | CF_MESSAGE_ADDED);

	const CMessageContainer* sourcePtr = dynamic_cast<const CMessageContainer*>(&object);
	if (sourcePtr != NULL){
		int sourceMessageCount = sourcePtr->m_messages.count();
		for (int messageIndex = 0; messageIndex < sourceMessageCount; messageIndex++){
			const MessagePtr& sourceMessage = sourcePtr->m_messages[messageIndex];
			Q_ASSERT(sourceMessage.IsValid());

			istd::TDelPtr<istd::IInformationProvider> newMessagePtr;
			newMessagePtr.SetCastedOrRemove<istd::IChangeable>(sourceMessage->CloneMe(mode));

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


