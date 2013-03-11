#ifndef ibase_CMessageContainer_included
#define ibase_CMessageContainer_included


// Qt includes
#include <QtCore/QList>
#include <QtCore/QMutex>

// ACF includes
#include "istd/IInformationProvider.h"

#include "ibase/IMessageContainer.h"
#include "ibase/IMessageConsumer.h"
#include "ibase/THierarchicalBase.h"


namespace ibase
{		


/**
	Basic implementation of a message container.
*/
class CMessageContainer:
			public ibase::THierarchicalBase<IHierarchicalMessageContainer>,
			virtual public IMessageConsumer
{
public:
	CMessageContainer();
	CMessageContainer(const CMessageContainer& container);

	virtual void AddChildContainer(IHierarchicalMessageContainer* childContainerPtr);

	void SetSlaveConsumer(ibase::IMessageConsumer* consumerPtr);

	/**
		Set maximal number of messages in the container. 
		If the container is full and a new message is added, the oldest message will be removed.
		\param maxMessageCount maximal number of messages in the container. If the value equals -1,
		no limit is set for.
	*/
	void SetMaxMessageCount(int maxMessageCount = -1);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (ibase::IMessageContainer)
	virtual int GetWorstCategory() const;
	virtual Messages GetMessages() const;
	virtual void ClearMessages();

	// reimplemented (ibase::IMessageConsumer)
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const istd::IInformationProvider* messagePtr = NULL) const;
	virtual void AddMessage(const IMessageConsumer::MessagePtr& messagePtr);

	// reimplemented (ibase::IHierarchicalMessageContainer)
	virtual int GetChildsCount() const;
	virtual IHierarchicalMessageContainer* GetChild(int index) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

private:
	typedef QList<IMessageConsumer::MessagePtr> MessageList;
	MessageList m_messages;

	typedef QVector<IHierarchicalMessageContainer*> Childs;
	Childs m_childContainers;

	ibase::IMessageConsumer* m_slaveConsumerPtr;

	int m_maxMessagesCount;

	int m_worstCategory;

	mutable QMutex m_lock;
};


} // namespace ibase


#endif // !ibase_CMessageContainer_included


