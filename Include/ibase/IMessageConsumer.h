#ifndef ibase_IMessageConsumer_included
#define ibase_IMessageConsumer_included


#include "istd/IPolymorphic.h"

#include "ibase/ibase.h"


namespace ibase
{


class IMessage;


/**
	Common interface for a message container consuming message objects.

	\ingroup Main
*/
class IMessageConsumer: virtual public istd::IPolymorphic
{
public:
	/**
		Adds a message item to the container. 
		Please note, that the container takes controll over the message item, 
		so you cannot destroy this object manually.
	*/
	virtual void AddMessage(IMessage* messagePtr) = 0;
};


} // namespace ibase


#endif // !ibase_IMessageConsumer_included


