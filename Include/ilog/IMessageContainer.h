#ifndef ilog_IMessageContainer_included
#define ilog_IMessageContainer_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/IInformationProvider.h"
#include "istd/TIHierarchical.h"
#include "iser/ISerializable.h"
#include "ilog/IMessageConsumer.h"


namespace ilog
{


/**
	Common interface for a message container.

	\ingroup Logging
*/
class IMessageContainer: virtual public iser::ISerializable
{
public:
	typedef QVector<ilog::IMessageConsumer::MessagePtr> Messages;

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


typedef istd::TIHierarchical<ilog::IMessageContainer> IHierarchicalMessageContainer;


} // namespace ilog


#endif // !ilog_IMessageContainer_included

