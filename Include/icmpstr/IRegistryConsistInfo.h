#ifndef icmpstr_IRegistryConsistInfo_included
#define icmpstr_IRegistryConsistInfo_included


// STL includes
#include <string>

// Qt includes
#include <QIcon>

#include "istd/IPolymorphic.h"

#include "icomp/IRegistry.h"

#include "ibase/IMessageConsumer.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class IRegistryConsistInfo: virtual public istd::IPolymorphic
{
public:
	enum QueryFlags
	{
		QF_NONE = 0,
		/**
			Supporting of any specified interfaces is enough to interprete component as compatible one.
		*/
		QF_ANY_INTERFACE = 1 << 0,
		/**
			If active, all undefined (without available meta information) elements will be also included.
		*/
		QF_INCLUDE_UNDEFINED = 1 << 1,
	};

	/**
		Get list of compatible element names.
		\param	interfaceNames		list of interfaces must be implemented.
		\param	registry			registry will be looked in.
	*/
	virtual icomp::IRegistry::Ids GetCompatibleElements(
				const icomp::IElementStaticInfo::Ids& interfaceNames,
				const icomp::IRegistry& registry,
				int queryFlags = QF_NONE) const = 0;

	/**
		Check if registry is valid.
		\param	registry			registry object.
		\param	ignoreUndef			if true, udefined elements will be ignored.
		\param	allReasons			if true, all reasons will be explained.
		\param	reasonConsumerPtr	optional consumer of messages explaining reason of calculated result.
	*/
	virtual bool IsRegistryValid(
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const = 0;

	/**
		Check if element is valid.
		\param	attributeName		name of registry element in specified registry.
		\param	registry			registry object.
		\param	ignoreUndef			if true, udefined elements will be ignored.
		\param	allReasons			if true, all reasons will be explained.
		\param	reasonConsumerPtr	optional consumer of messages explaining reason of calculated result.
	*/
	virtual bool IsElementValid(
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const = 0;

	/**
		Check if attribute is valid.
		\param	attributeName		name of attribute in specified registry element.
		\param	attributeName		name of registry element in specified registry.
		\param	registry			registry object.
		\param	ignoreUndef			if true, udefined elements will be ignored.
		\param	allReasons			if true, all reasons will be explained.
		\param	reasonConsumerPtr	optional consumer of messages explaining reason of calculated result.
	*/
	virtual bool IsAttributeValid(
				const std::string& attributeName,
				const std::string& elementName,
				const icomp::IRegistry& registry,
				bool ignoreUndef,
				bool allReasons,
				ibase::IMessageConsumer* reasonConsumerPtr) const = 0;
	/**
		Get icon of component.
	*/
	virtual QIcon GetComponentIcon(const icomp::CComponentAddress& address) const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IRegistryConsistInfo_included


