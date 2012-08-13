#include "ibase/CMessageContainer.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace ibase
{		


CMessageContainer::CMessageContainer()
    :m_maxMessageCount(-1),
	m_maxLiveTime(-1),
    m_worstCategory(-1)
{
}


CMessageContainer::~CMessageContainer()
{
	ClearMessages();
}


void CMessageContainer::AddChildContainer(IHierarchicalMessageContainer* childContainerPtr)
{
	I_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
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

	for (		MessageList::const_iterator iter = m_messages.begin();
				iter != m_messages.end();
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

void CMessageContainer::SetMaxMessageCount(int maxMessageCount)
{
	I_ASSERT(m_maxMessageCount != 0);

	m_maxMessageCount = maxMessageCount;
}


void CMessageContainer::SetMaxLiveTime(int maxLiveTime)
{
	m_maxLiveTime = maxLiveTime;
}


int CMessageContainer::GetWorstCategory() const
{
	int worstCategory = m_worstCategory;
	int childCount = GetChildsCount();

	if (worstCategory < 0){
		worstCategory = 0;

		for (		MessageList::const_iterator iter = m_messages.begin();
					iter != m_messages.end();
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

	for (		MessageList::const_iterator iter = m_messages.begin();
				iter != m_messages.end();
				++iter){
		const IMessageConsumer::MessagePtr& messagePtr = *iter;

		messages.push_back(messagePtr);
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
	I_ASSERT(messagePtr.IsValid());

	if (m_maxMessageCount == 0){
		return;
	}

	{
		istd::TChangeNotifier<IMessageContainer> changePtr(
					this,
					IMessageContainer::CF_MESSAGE_ADDED,
					const_cast<istd::IInformationProvider*>(messagePtr.GetPtr()));

		m_messages.push_back(messagePtr);

		int messageCategory = messagePtr->GetInformationCategory();
		if ((m_worstCategory >= 0) && (messageCategory > m_worstCategory)){
			m_worstCategory = messageCategory;
		}
	}

	if (m_maxMessageCount >= 0){
		while (int(m_messages.size()) > m_maxMessageCount){
			I_ASSERT(!m_messages.isEmpty());
			const IMessageConsumer::MessagePtr& messageToRemovePtr = m_messages.back();

			istd::TChangeNotifier<IMessageContainer> changePtr(
						this,
						IMessageContainer::CF_MESSAGE_REMOVED,
						const_cast<istd::IInformationProvider*>(messageToRemovePtr.GetPtr()));

			int removeCategory = messageToRemovePtr->GetInformationCategory();
			if (removeCategory >= m_worstCategory){
				m_worstCategory = -1;
			}

			m_messages.pop_back();
		}
	}

}


void CMessageContainer::ClearMessages()
{
	istd::TChangeNotifier<IMessageContainer> changePtr(this, IMessageContainer::CF_RESET);
	
	m_messages.clear();

	m_worstCategory = 0;
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


} // namespace ibase


