#ifndef ibase_IMessageConsumer_included
#define ibase_IMessageConsumer_included


#include "istd/IPolymorphic.h"
#include "istd/TSmartPtr.h"

#include "ibase/IMessage.h"


namespace ibase
{


/**
	Common interface for a message container consuming message objects.

	\ingroup Main
*/
class IMessageConsumer: virtual public istd::IPolymorphic
{
public:
	typedef istd::TSmartPtr<const IMessage> MessagePtr;

	/**
		Check if some messge is supported.
		\param	messageCategory	category of message or -1 if undefined,
								\sa istd::ILogger::MessageCategory.
		\param	messageId		ID of message as defined in ibase::IMessage or -1 if undefined.
		\param	messagePtr		optional message object, or NULL if undefined.
								Please note, this object will not be stored by receiver.
	*/
	virtual bool IsMessageSupported(
				int messageCategory = -1,
				int messageId = -1,
				const IMessage* messagePtr = NULL) const = 0;
	/**
		Adds a message item to the container. 
	*/
	virtual void AddMessage(const MessagePtr& messagePtr) = 0;
};


} // namespace ibase


#endif // !ibase_IMessageConsumer_included


