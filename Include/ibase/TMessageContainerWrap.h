#ifndef ibase_TMessageContainerWrap_included
#define ibase_TMessageContainerWrap_included


#include "istd/TPointerVector.h"
#include "istd/TChangeNotifier.h"
#include "ibase/THierarchicalBase.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "ibase/IMessage.h"
#include "ibase/IMessageContainer.h"


namespace ibase
{		

template <class Base>
class TMessageContainerWrap: public ibase::THierarchicalBase<Base, ibase::IHierarchicalMessageContainer>
{
public:
	typedef Base BaseClass;

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
	virtual void AddMessage(ibase::IMessage* message);

	// pseudo-reimplemented (IHierarchicalMessageContainer)
	virtual int GetChildsCount() const;
	virtual ibase::IHierarchicalMessageContainer* GetChild(int index) const;

protected:
	static int SubstractMask(int category);

protected:
	typedef istd::TPointerVector<ibase::IMessage> MessageList;
	MessageList m_messages;

	int m_maxCategory;

private:
	typedef std::vector<ibase::IHierarchicalMessageContainer*> Childs;
	Childs m_childContainers;

	int m_maxMessageCount;
	int m_maxLiveTime;
};


// public methods

template <class Base>
TMessageContainerWrap<Base>::TMessageContainerWrap()
	:m_maxCategory(0),
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

	Messages messages = GetMessages();
	int messageCount = messages.size();

	static iser::CArchiveTag messagesTag("Messages", "List of messages");
	static iser::CArchiveTag messageTag("Message", "Message");

	retVal = retVal && archive.BeginMultiTag(messagesTag, messageTag, messageCount);

	if (!retVal){
		return false;
	}

    for (int i = 0; i < messageCount; ++i){
		retVal = retVal && archive.BeginTag(messageTag);
		retVal = retVal && (const_cast<ibase::IMessage*>(messages.at(i)))->Serialize(archive);
		retVal = retVal && archive.EndTag(messageTag);
	}

	retVal = retVal && archive.EndTag(messagesTag);

	return retVal;
}


// pseudo-reimplemented (ibase::IMessageContainer)

template <class Base>
void TMessageContainerWrap<Base>::SetMaxMessageCount(int maxMessageCount)
{
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
	int worstCategory = m_maxCategory;
	int childCount = GetChildsCount();

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		ibase::IMessageContainer* childPtr = dynamic_cast<ibase::IMessageContainer*>(GetChild(childIndex));
		if (childPtr != NULL){
			worstCategory = istd::Max(worstCategory, childPtr->GetWorstCategory());
		}
	}

	return worstCategory;
}


template <class Base>
ibase::IMessageContainer::Messages TMessageContainerWrap<Base>::GetMessages() const
{
	Messages messages;

	for (int messageIndex = 0; messageIndex < m_messages.GetCount(); messageIndex++){
		const IMessage* messagePtr = m_messages.GetAt(messageIndex);

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
void TMessageContainerWrap<Base>::AddMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	if (m_maxMessageCount >= 0){
		if (m_messages.GetCount() + 1 > m_maxMessageCount){
			ibase::IMessage* removeMessagePtr = m_messages.GetAt(0);
			istd::TChangeNotifier<ibase::IMessageContainer> changePtr(this, MessageRemoved, removeMessagePtr);

			m_messages.PopAt(0);
		}
	}

	istd::TChangeNotifier<ibase::IMessageContainer> changePtr(this, MessageAdded, messagePtr);

	m_messages.PushBack(messagePtr);

	int messageCategory = SubstractMask(messagePtr->GetCategory());
	if (messageCategory > m_maxCategory){
		m_maxCategory = messageCategory;
	}
}


template <class Base>
void TMessageContainerWrap<Base>::ClearMessages()
{
	istd::TChangeNotifier<ibase::IMessageContainer> changePtr(this, Reset);
	
	m_messages.Reset();

	m_maxCategory = 0;
}


// pseudo-reimplemented (istd::IHierarchical)

template <class Base>
int TMessageContainerWrap<Base>::GetChildsCount() const
{
	return m_childContainers.size();
}


template <class Base>
ibase::IHierarchicalMessageContainer* TMessageContainerWrap<Base>::GetChild(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_childContainers.size()));

	return m_childContainers.at(index);
}


// protected static methods

template <class Base>
int TMessageContainerWrap<Base>::SubstractMask(int category)
{
	category = category & ~ibase::IMessage::DebugMask;
	category = category & ~ibase::IMessage::SystemMask;
	category = category & ~ibase::IMessage::UserMask;

	return category;
}



typedef ibase::TMessageContainerWrap<ibase::IMessageContainer> CMessageContainer;


} // namespace ibase


#endif // !ibase_TMessageContainerWrap_included


