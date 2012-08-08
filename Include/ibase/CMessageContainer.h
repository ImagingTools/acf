#ifndef ibase_CMessageContainer_included
#define ibase_CMessageContainer_included


// Qt includes
#include <QtCore/QList>

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
	virtual ~CMessageContainer();

	virtual void AddChildContainer(IHierarchicalMessageContainer* childContainerPtr);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (ibase::IMessageContainer)
	virtual void SetMaxMessageCount(int maxMessageCount = -1);
	virtual void SetMaxLiveTime(int maxLiveTime = -1);
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

private:
	typedef QList<IMessageConsumer::MessagePtr> MessageList;
	MessageList m_messages;

	typedef QVector<IHierarchicalMessageContainer*> Childs;
	Childs m_childContainers;

	int m_maxMessageCount;
	int m_maxLiveTime;

	int m_worstCategory;
};


} // namespace ibase


#endif // !ibase_CMessageContainer_included


