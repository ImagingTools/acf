#ifndef ibase_TMessageContainerWrap_included
#define ibase_TMessageContainerWrap_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/IInformationProvider.h"
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/IMessageContainer.h"
#include "ibase/THierarchicalBase.h"


namespace ibase
{		


/**
	Basic implementation of a message container.
*/
template <class Base>
class TMessageContainerWrap: public THierarchicalBase<Base>
{
public:
	typedef THierarchicalBase<Base> BaseClass;

	TMessageContainerWrap();
	virtual ~TMessageContainerWrap();

	virtual void AddChildContainer(IMessageContainer* childContainerPtr);

	// pseudo-reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// pseudo-reimplemented (ibase::IMessageContainer)
	virtual void SetMaxMessageCount(int maxMessageCount = -1);
	virtual void SetMaxLiveTime(int maxLiveTime = -1);
	virtual int GetWorstCategory() const;
	virtual IMessageContainer::Messages GetMessages() const;
	virtual void ClearMessages();

	// pseudo-reimplemented (ibase::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const IMessageConsumer::MessagePtr& messagePtr);

	// pseudo-reimplemented (ibase::IHierarchicalMessageContainer)
	virtual int GetChildsCount() const;
	virtual IMessageContainer* GetChild(int index) const;

protected:
	typedef QList<IMessageContainer::MessagePtr> MessageList;
	MessageList m_messages;

	int m_worstCategory;

private:
	typedef QVector<IMessageContainer*> Childs;
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
void TMessageContainerWrap<Base>::AddChildContainer(IMessageContainer* childContainerPtr)
{
	I_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
}


// pseudo-reimplemented (iser::ISerializable)

template <class Base>
bool TMessageContainerWrap<Base>::Serialize(iser::IArchive& archive)
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
			const IMessageContainer::MessagePtr& messagePtr = *iter;

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


template <class Base>
IMessageContainer::Messages TMessageContainerWrap<Base>::GetMessages() const
{
	IMessageContainer::Messages messages;

	for (		MessageList::const_iterator iter = m_messages.begin();
				iter != m_messages.end();
				++iter){
		const IMessageContainer::MessagePtr& messagePtr = *iter;

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


template <class Base>
bool TMessageContainerWrap<Base>::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


template <class Base>
void TMessageContainerWrap<Base>::AddMessage(const IMessageConsumer::MessagePtr& messagePtr)
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
			const IMessageContainer::MessagePtr& messageToRemovePtr = m_messages.back();

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


template <class Base>
void TMessageContainerWrap<Base>::ClearMessages()
{
	istd::TChangeNotifier<IMessageContainer> changePtr(this, IMessageContainer::CF_RESET);
	
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
IMessageContainer* TMessageContainerWrap<Base>::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_childContainers.size()));

	return m_childContainers.at(index);
}


typedef TMessageContainerWrap<IHierarchicalMessageContainer> CMessageContainer;


} // namespace ibase


#endif // !ibase_TMessageContainerWrap_included


