#ifndef ibase_TMessageManagerBase_included
#define ibase_TMessageManagerBase_included


#include "ibase/IMessage.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/TPointerVector.h"


namespace ibase
{		

template <class BaseClass>
class TMessageManagerBase:	public BaseClass
{
public:
	TMessageManagerBase();
	virtual ~TMessageManagerBase();

	// pseudo-reimplemented (ibase::IMessageManager)
	virtual int GetWorstCategory() const;
	virtual int GetMessageCount(int messageCategory = -1) const;
	virtual const ibase::IMessage* GetMessageFrom(int index, int messageCategory = -1) const;
	virtual void AddMessage(ibase::IMessage* message);
	virtual void ClearMessages();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	static int SubstractMask(int category);

protected:
	int m_maxCategory;

	istd::TPointerVector<ibase::IMessage> m_messages;
};


template <class BaseClass>
TMessageManagerBase<BaseClass>::TMessageManagerBase()
{
	m_maxCategory = 0;
}


template <class BaseClass>
TMessageManagerBase<BaseClass>::~TMessageManagerBase()
{
	ClearMessages();
}


// pseudo-reimplemented (ibase::IMessageManager)

template <class BaseClass>
int TMessageManagerBase<BaseClass>::GetWorstCategory() const
{
	return m_maxCategory;
}


template <class BaseClass>
int TMessageManagerBase<BaseClass>::GetMessageCount(int messageCategory) const
{
	int count = 0;
	for (int index = 0; index < m_messages.GetCount(); index++){
		const IMessage* messagePtr = m_messages.GetAt(index);
		I_ASSERT(messagePtr != NULL);

		if (messageCategory == messagePtr->GetCategory()){
			++count;
		}
	}

	return count;
}


template <class BaseClass>
const ibase::IMessage* TMessageManagerBase<BaseClass>::GetMessageFrom(int index, int messageCategory) const
{
	int count = 0;
	for (int messageIndex = 0; messageIndex < m_messages.GetCount(); messageIndex++){
		const IMessage* messagePtr = m_messages.GetAt(messageIndex);
		I_ASSERT(messagePtr != NULL);

		if (messageCategory == messagePtr->GetCategory()){
			++count;
		}

		if (count == index){
			return messagePtr;
		}
	}

	return NULL;
}


template <class BaseClass>
void TMessageManagerBase<BaseClass>::AddMessage(ibase::IMessage* messagePtr)
{
	I_ASSERT(messagePtr != NULL);

	m_messages.PushBack(messagePtr);

	if (messagePtr->GetCategory() > m_maxCategory){
		m_maxCategory = SubstractMask(messagePtr->GetCategory());
	}
}


template <class BaseClass>
void TMessageManagerBase<BaseClass>::ClearMessages()
{
	m_messages.Reset();

	m_maxCategory = 0;
}


// reimplemented (iser::ISerializable)

template <class BaseClass>
bool TMessageManagerBase<BaseClass>::Serialize(iser::IArchive& archive)
{
	if (!archive.IsStoring()){
		return false;
	}

	bool retVal = true;

	int messageCount = GetMessageCount();

	static iser::CArchiveTag messagesTag("Messages", "List of messages");
	static iser::CArchiveTag messageTag("Message", "Message");

	retVal = retVal && archive.BeginMultiTag(messagesTag, messageTag, messageCount);

	if (!retVal){
		return false;
	}

    for (int i = 0; i < messageCount; ++i){
		retVal = retVal && archive.BeginTag(messageTag);
		retVal = retVal && m_messages.GetAt(i)->Serialize(archive);
		retVal = retVal && archive.EndTag(messageTag);
	}

	retVal = retVal && archive.EndTag(messagesTag);

	return retVal;
}


// protected static methods

template <class BaseClass>
int TMessageManagerBase<BaseClass>::SubstractMask(int category)
{
	category = category & ~ibase::IMessage::DebugMask;
	category = category & ~ibase::IMessage::SystemMask;
	category = category & ~ibase::IMessage::UserMask;

	return category;
}


} // namespace ibase


#endif // !ibase_TMessageManagerBase_included

