#ifndef ibase_IMessageManager_included
#define ibase_IMessageManager_included


#include "ibase/IMessage.h"


#include "iser/ISerializable.h"


namespace ibase
{		



/**
*/
class IMessageManager: virtual public iser::ISerializable
{
public:
	/**
		Get worst message category.
		\sa IMessage::Category
	*/
	virtual int GetWorstCategory() const = 0;

	/**
		This function returns the number of messages with the category \c category.
		in the container.
	*/
	virtual int GetMessageCount(int messageCategory = -1) const = 0;

	/**
		This function returns the message with the index \c index and with the category \c category.
		from the container. 
	*/
	virtual const ibase::IMessage* GetMessageFrom(int index, int messageCategory = -1) const = 0;

	/**
		Adds a message item to the container. 
		Please note, that the message manager takes controll over the message item, 
		so you cannot destroy this object manually.
	*/
	virtual void AddMessage(ibase::IMessage* message) = 0;

	/**
		Remove all message items from this manager.
	*/
	virtual void ClearMessages() = 0;
};


} // namespace ibase


#endif // !ibase_IMessageManager_included

