#ifndef ibase_IMessageContainer_included
#define ibase_IMessageContainer_included


#include "istd/TIHierarchical.h"

#include "iser/ISerializable.h"

#include "ibase/IMessage.h"
#include "ibase/IMessageConsumer.h"


namespace ibase
{


/**
	Common interface for an message container.
*/
class IMessageContainer: virtual public IMessageConsumer, virtual public iser::ISerializable
{
public:
	typedef std::vector< istd::TSmartPtr<const IMessage> > Messages;

	enum ChangeFlags
	{
		MessageAdded = 0x0001,
		MessageRemoved = 0x0010,
		Reset = 0x0020
	};

	/**
		Set maximal number of messages in the container. 
		If a new message will be added to the full container, the oldest message will be removed.
		\param maxMessageCount maximal number of messages in the container. If the value equals -1,
		no limit is set for.
	*/
	virtual void SetMaxMessageCount(int maxMessageCount = -1) = 0;

	/**
		Set maximum time difference between the oldest and newest message. If calculated value is grated then this threshold,
		the oldest message will be removed if a new message will be added.
		\param maxLiveTime maximal time difference in seconds
	*/
	virtual void SetMaxLiveTime(int maxLiveTime = -1) = 0;

	/**
		Get worst message category.
		\sa IMessage::Category
	*/
	virtual int GetWorstCategory() const = 0;

	/**
		This function returns messages in the container. 
	*/
	virtual Messages GetMessages() const = 0;

	/**
		Remove all message items from container.
	*/
	virtual void ClearMessages() = 0;
};


typedef istd::TIHierarchical<ibase::IMessageContainer> IHierarchicalMessageContainer;


} // namespace ibase


#endif // !ibase_IMessageContainer_included

