#ifndef ibase_IMessageContainer_included
#define ibase_IMessageContainer_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/IInformationProvider.h"
#include "istd/TIHierarchical.h"

#include "iser/ISerializable.h"

#include "ibase/IMessageConsumer.h"


namespace ibase
{


/**
	Common interface for an message container.
*/
class IMessageContainer: virtual public iser::ISerializable
{
public:
	typedef QVector<ibase::IMessageConsumer::MessagePtr> Messages;

	enum ChangeFlags
	{
		CF_MESSAGE_ADDED = 1 << 6,
		CF_MESSAGE_REMOVED = 1 << 7,
		CF_RESET = 1 << 8
	};

	/**
		Get worst message category.
		\sa IInformationProvider::InformationCategory
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

