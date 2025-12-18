#ifndef ilog_IMessageContainer_included
#define ilog_IMessageContainer_included


// STL includes
#include <list>

// ACF includes
#include <istd/IInformationProvider.h>
#include <istd/TIHierarchical.h>
#include <iser/ISerializable.h>
#include <ilog/IMessageConsumer.h>


namespace ilog
{


/**
	Common interface for a message container.

	\ingroup Logging
*/
class IMessageContainer: virtual public iser::ISerializable
{
public:
	typedef std::list<IMessageConsumer::MessagePtr> Messages;

	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		CF_MESSAGE_ADDED = 0x1d32820,
		CF_MESSAGE_REMOVED,
		CF_RESET
	};

	/**
		Get worst message category.
		\sa IInformationProvider::InformationCategory
	*/
	virtual istd::IInformationProvider::InformationCategory GetWorstCategory() const = 0;

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

