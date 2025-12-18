#include <ilog/CMessageContainer.h>


// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/TDelPtr.h>
#include <iser/IObject.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <ilog/CMessage.h>
#include <ilog/CExtMessage.h>


namespace ilog
{


static const istd::IChangeable::ChangeSet s_resetChange(CMessageContainer::CF_ALL_DATA, CMessageContainer::CF_RESET);


// public methods

CMessageContainer::CMessageContainer()
:	m_slaveConsumerPtr(NULL),
	m_maxMessagesCount(-1),
	m_worstCategory(-1)
{
}


CMessageContainer::CMessageContainer(const CMessageContainer& container)
:	m_slaveConsumerPtr(NULL),
	m_maxMessagesCount(container.m_maxMessagesCount),
	m_worstCategory(container.m_worstCategory)
{
	for (const auto& message : container.m_messages){
		ilog::IMessageConsumer::MessagePtr newMessagePtr;
		newMessagePtr.MoveCastedPtr(message->CloneMe());
		m_messages.push_back(newMessagePtr);
	}
}


CMessageContainer& CMessageContainer::operator=(const CMessageContainer& container)
{
	m_maxMessagesCount = container.m_maxMessagesCount;
	m_worstCategory = container.m_worstCategory;

	for (const auto& message : container.m_messages){
		ilog::IMessageConsumer::MessagePtr newMessagePtr;
		newMessagePtr.MoveCastedPtr(message->CloneMe());
		m_messages.push_back(newMessagePtr);
	}

	return *this;
}


int CMessageContainer::GetMessagesCount() const
{
	return (int)m_messages.size();
}


void CMessageContainer::AddChildContainer(IHierarchicalMessageContainer* childContainerPtr)
{
	Q_ASSERT(childContainerPtr != NULL);

	m_childContainers.push_back(childContainerPtr);
}


void CMessageContainer::SetSlaveConsumer(ilog::IMessageConsumer* consumerPtr)
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
	static iser::CArchiveTag messagesTag("Messages", "List of messages", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag messageTag("Message", "Message", iser::CArchiveTag::TT_GROUP, &messagesTag);
	static iser::CArchiveTag messageTypeIdTag("TypeId", "ID of the message type", iser::CArchiveTag::TT_LEAF, &messageTag);

	istd::IFactoryInfo::KeyList knownMessageTypes = GetMessageFactory().GetFactoryKeys();

	bool retVal = true;

	int messageCount = int(m_messages.size());

	if (archive.IsStoring()){
		int serializableMessageCount = 0;
		for (		Messages::const_iterator iter = m_messages.cbegin();
					iter != m_messages.cend();
					++iter){
			const IMessageConsumer::MessagePtr& messagePtr = *iter;

			iser::IObject* messageObjectPtr = const_cast<iser::IObject*>(dynamic_cast<const iser::IObject*>(messagePtr.GetPtr()));
			if (messageObjectPtr != NULL){
				if (knownMessageTypes.contains(messageObjectPtr->GetFactoryId())){
					++serializableMessageCount;
				}
			}
		}

		messageCount = serializableMessageCount;
	}

	retVal = retVal && archive.BeginMultiTag(messagesTag, messageTag, messageCount);
	if (!retVal){
		return false;
	}

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	if (archive.IsStoring()){
		for (		Messages::const_iterator iter = m_messages.cbegin();
					iter != m_messages.cend();
					++iter){
			const IMessageConsumer::MessagePtr& messagePtr = *iter;

			iser::IObject* messageObjectPtr = const_cast<iser::IObject*>(dynamic_cast<const iser::IObject*>(messagePtr.GetPtr()));
			if (messageObjectPtr != NULL){
				QByteArray messageTypeId = messageObjectPtr->GetFactoryId();
				if (knownMessageTypes.contains(messageTypeId)){
					retVal = retVal && archive.BeginTag(messageTag);

					retVal = retVal && archive.BeginTag(messageTypeIdTag);
					retVal = retVal && archive.Process(messageTypeId);
					retVal = retVal && archive.EndTag(messageTypeIdTag);

					retVal = retVal && messageObjectPtr->Serialize(archive);

					retVal = retVal && archive.EndTag(messageTag);
				}
			}
		}
	}
	else{
		m_messages.clear();
		m_worstCategory = -1;

		for (int messageIndex = 0; messageIndex < messageCount; ++messageIndex){
			retVal = retVal && archive.BeginTag(messageTag);

			QByteArray messageTypeId;
			retVal = retVal && archive.BeginTag(messageTypeIdTag);
			retVal = retVal && archive.Process(messageTypeId);
			retVal = retVal && archive.EndTag(messageTypeIdTag);

			if (retVal){
				if (knownMessageTypes.contains(messageTypeId)){
					istd::TDelPtr<iser::IObject> objectPtr(GetMessageFactory().CreateInstance(messageTypeId));

					if (objectPtr.IsValid()){
						retVal = retVal && objectPtr->Serialize(archive);

						if (retVal){
							istd::IInformationProvider* infoPtr = dynamic_cast<istd::IInformationProvider*>(objectPtr.GetPtr());
							if (infoPtr != NULL){
								IMessageConsumer::MessagePtr messageObjectPtr(dynamic_cast<istd::IInformationProvider*>(objectPtr.PopPtr()));

								m_messages.push_back(messageObjectPtr);
							}
						}
					}
				}
				else{
					qDebug("Message type: '%s' not supported", messageTypeId.constData());
				}
			}

			retVal = retVal && archive.EndTag(messageTag);
		}
	}

	retVal = retVal && archive.EndTag(messagesTag);

	return retVal;
}


// reimplemented (ilog::IMessageContainer)

istd::IInformationProvider::InformationCategory CMessageContainer::GetWorstCategory() const
{
	int childCount = GetChildsCount();

	if (m_worstCategory < 0){
		m_worstCategory = istd::IInformationProvider::InformationCategory::IC_NONE;

		for (		Messages::const_iterator iter = m_messages.cbegin();
					iter != m_messages.cend();
					++iter){
			const IMessageConsumer::MessagePtr& messagePtr = *iter;

			istd::IInformationProvider::InformationCategory category = messagePtr->GetInformationCategory();
			if (category > m_worstCategory){
				m_worstCategory = category;
			}
		}
	}

	istd::IInformationProvider::InformationCategory worstCategory = (istd::IInformationProvider::InformationCategory)m_worstCategory;
	for (int childIndex = 0; childIndex < childCount; childIndex++){
		IMessageContainer* childPtr = dynamic_cast<IMessageContainer*>(GetChild(childIndex));
		if (childPtr != NULL){
			istd::IInformationProvider::InformationCategory category = childPtr->GetWorstCategory();
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

	for (		Messages::const_iterator iter = m_messages.cbegin();
				iter != m_messages.cend();
				++iter){	
		messages.push_back(*iter);
	}

	int childsCount = GetChildsCount();
	for (int childIndex = 0; childIndex < childsCount; childIndex++){
		IMessageContainer* childPtr = GetChild(childIndex);
		Q_ASSERT(childPtr != NULL);

		IMessageContainer::Messages childMessages = childPtr->GetMessages();

		messages.insert(messages.end(), childMessages.begin(), childMessages.end());
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

	istd::CChangeNotifier changeNotifier(this);

	m_messages.push_front(messagePtr);
	if (m_worstCategory >= 0) {
		istd::IInformationProvider::InformationCategory messageCategory = messagePtr->GetInformationCategory();
		if (messageCategory > m_worstCategory){
			m_worstCategory = messageCategory;
		}
	}

	if (m_maxMessagesCount >= 0){
		while (int(m_messages.size()) > m_maxMessagesCount){
			Q_ASSERT(!m_messages.empty());
			const IMessageConsumer::MessagePtr& messageToRemovePtr = m_messages.back();
			
			int removeCategory = messageToRemovePtr->GetInformationCategory();
			if (removeCategory >= m_worstCategory){
				//invalidate m_worstCategory
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
	if (!m_messages.empty()){
		istd::CChangeNotifier notifier(this, &s_resetChange);
		Q_UNUSED(notifier);
	
		m_messages.clear();

		m_worstCategory = istd::IInformationProvider::InformationCategory::IC_NONE;
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

int CMessageContainer::GetSupportedOperations() const
{
	return SO_COPY | SO_COMPARE | SO_CLONE;
}


bool CMessageContainer::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	istd::CChangeNotifier notifier(this, &s_resetChange);
	Q_UNUSED(notifier);

	m_messages.clear();

	switch (mode){
		case CM_STRICT:
		case CM_WITHOUT_REFS:
		{
			const CMessageContainer* sourcePtr = dynamic_cast<const CMessageContainer*>(&object);
			if (sourcePtr != NULL){
				for (Messages::const_iterator iter = sourcePtr->m_messages.cbegin();
					iter != sourcePtr->m_messages.cend();
					++iter) {
					const MessagePtr sourceMessage = *iter;
					Q_ASSERT(sourceMessage.IsValid());

					ilog::IMessageConsumer::MessagePtr newMessagePtr;
					newMessagePtr.MoveCastedPtr(sourceMessage->CloneMe(mode));

					if (newMessagePtr.IsValid()){
						m_messages.push_back(newMessagePtr);
					}
				}

				return true;
			}
		}
		break;

		case CM_WITH_REFS:
		case CM_CONVERT:
		{
			const IMessageContainer* sourcePtr = dynamic_cast<const IMessageContainer*>(&object);
			if (sourcePtr != NULL){
				Messages messages = sourcePtr->GetMessages();
				for (Messages::const_iterator iter = messages.cbegin();
					iter != messages.cend();
					++iter) {
					const MessagePtr sourceMessage = *iter;
					Q_ASSERT(sourceMessage.IsValid());

					istd::TUniqueInterfacePtr<istd::IInformationProvider> newMessagePtr;
					newMessagePtr.MoveCastedPtr(sourceMessage->CloneMe(mode));

					MessagePtr messagePtr(newMessagePtr.PopInterfacePtr());
					if (messagePtr.IsValid()){
						m_messages.push_back(messagePtr);
					}
				}

				return true;
			}
		}
		break;
	}

	return false;
}


bool CMessageContainer::IsEqual(const istd::IChangeable& object) const
{
	const CMessageContainer* otherObjectPtr = dynamic_cast<const CMessageContainer*>(&object);
	if (otherObjectPtr != NULL){
		Messages otherMessages = otherObjectPtr->GetMessages();
		if (m_messages.size() != otherMessages.size()){
			return false;
		}

		Messages::const_iterator it = m_messages.begin();
		Messages::const_iterator other_it = otherMessages.begin();

		for (size_t i = 0; i < m_messages.size(); ++i){
			IMessageConsumer::MessagePtr messagePtr = *it;
			IMessageConsumer::MessagePtr otherMessagePtr = *other_it;

			if (messagePtr->GetSupportedOperations() & SO_COMPARE){
				if (messagePtr->IsEqual(*otherMessagePtr)){
					return false;
				}
			}
			else{
				int id = messagePtr->GetInformationId();
				int otherId = otherMessagePtr->GetInformationId();
				if (id != otherId){
					return false;
				}

				istd::IInformationProvider::InformationCategory category = messagePtr->GetInformationCategory();
				istd::IInformationProvider::InformationCategory otherCategory = otherMessagePtr->GetInformationCategory();
				if (category != otherCategory){
					return false;
				}

				QString description = messagePtr->GetInformationDescription();
				QString otherDescription = otherMessagePtr->GetInformationDescription();
				if (description != otherDescription){
					return false;
				}

				int flags = messagePtr->GetInformationFlags();
				int otherFlags = otherMessagePtr->GetInformationFlags();
				if (flags != otherFlags){
					return false;
				}

				QString source = messagePtr->GetInformationSource();
				QString otherSource = otherMessagePtr->GetInformationSource();
				if (source != otherSource){
					return false;
				}

				QDateTime timeStamp = messagePtr->GetInformationTimeStamp();
				QDateTime otherTimeStamp = otherMessagePtr->GetInformationTimeStamp();
				if (timeStamp != otherTimeStamp){
					return false;
				}
			}
		
			it++;
			other_it++;
		}

		return true;
	}

	return false;
}


istd::IChangeableUniquePtr CMessageContainer::CloneMe(CompatibilityMode mode) const
{
	istd::IChangeableUniquePtr clonePtr(new CMessageContainer);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr;
	}

	return nullptr;
}


bool CMessageContainer::ResetData(CompatibilityMode /*mode*/)
{
	ClearMessages();

	return true;
}


// private static methods

CMessageContainer::MessageFactory& CMessageContainer::GetMessageFactory()
{
	static MessageFactory messageFactory;

	return messageFactory;
}


I_REGISTER_MESSAGE_TYPE(CMessage, CMessage::GetMessageTypeId());
I_REGISTER_MESSAGE_TYPE(CExtMessage, CExtMessage::GetTypeName());


} // namespace ilog


