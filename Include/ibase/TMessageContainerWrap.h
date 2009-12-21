#ifndef ibase_TMessageContainerWrap_included
#define ibase_TMessageContainerWrap_included


// STL includes
#include <list>

#include "istd/TSmartPtr.h"
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/IMessage.h"
#include "ibase/IMessageContainer.h"
#include "ibase/THierarchicalBase.h"


namespace ibase
{		


/**
	Basic implementation of a message container.
*/
template <class Base>
class TMessageContainerWrap: public ibase::THierarchicalBase<Base>
{
public:
	typedef ibase::THierarchicalBase<Base> BaseClass;

	TMessageContainerWrap();
	virtual ~TMessageContainerWrap();

	virtual void AddChildContainer(ibase::IHierarchicalMessageContainer* childContainerPtr);

	// pseudo-reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// pseudo-reimplemented (ibase::IMessageContainer)
	virtual void SetMaxMessageCount(int maxMessageCount = -1);
	virtual void SetMaxLiveTime(int maxLiveTime = -1);
	virtual int GetWorstCategory() const;
	virtual ibase::IMessageContainer::Messages GetMessages() const;
	virtual void ClearMessages();

	// pseudo-reimplemented (ibase::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const IMessage* messagePtr = NULL) const;
	virtual void AddMessage(const ibase::IMessageContainer::MessagePtr& messagePtr);

	// pseudo-reimplemented (IHierarchicalMessageContainer)
	virtual int GetChildsCount() const;
	virtual ibase::IHierarchicalMessageContainer* GetChild(int index) const;

protected:
	typedef std::list<ibase::IMessageContainer::MessagePtr> MessageList;
	MessageList m_messages;

	int m_worstCategory;

private:
	typedef std::vector<ibase::IHierarchicalMessageContainer*> Childs;
	Childs m_childContainers;

	int m_maxMessageCount;
	int m_maxLiveTime;
};


// public methods

template <class Base>
TMessageContainerWrap<Base>::TMessageContainerWrap()
:	m_worstCategory(-1),
	m_maxMessageCount(-1),
	m_maxLiveTime(-1)
{
}


template <class Base>
TMessageContainerWrap<Base>::~TMessageContainerWrap()
{
	ClearMessages();
}


template <class Base>
void TMessageContainerWrap<Base>::AddChildContainer(ibase::IHierarchicalMessageContainer* childContainerPtr)
{
	I_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
}


// pseudo-reimplemented (iser::ISerializable)

template <class Base>
bool TMessageContainerWrap<Base>::Serialize(iser::IArchive& archive)
{
	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = true;

	int messageCount = int(m_messages.size());

	static iser::CArchiveTag messagesTag("Messages", "List of messages");
	static iser::CArchiveTag messageTag("Message", "Message");

	retVal = retVal && archive.BeginMultiTag(messagesTag, messageTag, messageCount);

	if (!retVal){
		return false;
	}

	for (		MessageList::const_iterator iter = m_messages.begin();
				iter != m_messages.end();
				++iter){
		const MessagePtr& messagePtr = *iter;

		retVal = retVal && archive.BeginTag(messageTag);
		retVal = retVal && const_cast<ibase::IMessage*>(messagePtr.GetPtr())->Serialize(archive);
		retVal = retVal && archive.EndTag(messageTag);
	}

	retVal = retVal && archive.EndTag(messagesTag);

	return retVal;
}


// pseudo-reimplemented (ibase::IMessageContainer)

template <class Base>
void TMessageContainerWrap<Base>::SetMaxMessageCount(int maxMessageCount)
{
	I_ASSERT(m_maxMessageCount != 0);

	m_maxMessageCount = maxMessageCount;
}


template <class Base>
void TMessageContainerWrap<Base>::SetMaxLiveTime(int maxLiveTime)
{
	m_maxLiveTime = maxLiveTime;
}


template <class Base>
int TMessageContainerWrap<Base>::GetWorstCategory() const
{
	int worstCategory = m_worstCategory;
	int childCount = GetChildsCount();

	if (worstCategory < 0){
		worstCategory = 0;

		for (		MessageList::const_iterator iter = m_messages.begin();
					iter != m_messages.end();
					++iter){
			const MessagePtr& messagePtr = *iter;

			int category = messagePtr->GetCategory();
			if (category > worstCategory){
				worstCategory = category;
			}
		}
	}

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		ibase::IMessageContainer* childPtr = dynamic_cast<ibase::IMessageContainer*>(GetChild(childIndex));
		if (childPtr != NULL){
			int category = childPtr->GetWorstCategory();
			if (category > worstCategory){
				worstCategory = category;
			}
		}
	}

	return worstCategory;
}


template <class Base>
ibase::IMessageContainer::Messages TMessageContainerWrap<Base>::GetMessages() const
{
	ibase::IMessageContainer::Messages messages;

	for (		MessageList::const_iterator iter = m_messages.begin();
				iter != m_messages.end();
				++iter){
		const MessagePtr& messagePtr = *iter;

		messages.push_back(messagePtr);
	}

	int childsCount = GetChildsCount();
	for (int childIndex = 0; childIndex < childsCount; childIndex++){
		ibase::IHierarchicalMessageContainer* childPtr = GetChild(childIndex);
		I_ASSERT(childPtr != NULL);

		ibase::IMessageContainer::Messages childMessages = childPtr->GetMessages();

		messages.insert(messages.end(), childMessages.begin(), childMessages.end());
	}

	return messages;
}


template <class Base>
bool TMessageContainerWrap<Base>::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const IMessage* /*messagePtr*/) const
{
	return true;
}


template <class Base>
void TMessageContainerWrap<Base>::AddMessage(const ibase::IMessageContainer::MessagePtr& messagePtr)
{
	I_ASSERT(messagePtr.IsValid());

	if (m_maxMessageCount == 0){
		return;
	}

	{
		istd::TChangeNotifier<ibase::IMessageContainer> changePtr(
					this,
					ibase::IMessageContainer::MessageAdded,
					const_cast<IMessage*>(messagePtr.GetPtr()));

		m_messages.push_back(messagePtr);

		int messageCategory = messagePtr->GetCategory();
		if ((m_worstCategory >= 0) && (messageCategory > m_worstCategory)){
			m_worstCategory = messageCategory;
		}
	}

	if (m_maxMessageCount >= 0){
		while (int(m_messages.size()) > m_maxMessageCount){
			I_ASSERT(!m_messages.empty());
			const MessagePtr& messageToRemovePtr = m_messages.back();

			istd::TChangeNotifier<ibase::IMessageContainer> changePtr(
						this,
						ibase::IMessageContainer::MessageRemoved,
						const_cast<IMessage*>(messageToRemovePtr.GetPtr()));

			int removeCategory = messageToRemovePtr->GetCategory();
			if (removeCategory >= m_worstCategory){
				m_worstCategory = -1;
			}

			m_messages.pop_back();
		}
	}

}


template <class Base>
void TMessageContainerWrap<Base>::ClearMessages()
{
	istd::TChangeNotifier<ibase::IMessageContainer> changePtr(this, ibase::IMessageContainer::Reset);
	
	m_messages.clear();

	m_worstCategory = 0;
}


// pseudo-reimplemented (istd::IHierarchical)

template <class Base>
int TMessageContainerWrap<Base>::GetChildsCount() const
{
	return int(m_childContainers.size());
}


template <class Base>
ibase::IHierarchicalMessageContainer* TMessageContainerWrap<Base>::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_childContainers.size()));

	return m_childContainers.at(index);
}


typedef ibase::TMessageContainerWrap<ibase::IHierarchicalMessageContainer> CMessageContainer;


} // namespace ibase


#endif // !ibase_TMessageContainerWrap_included


