#ifndef iattr_IAttributesProvider_included
#define iattr_IAttributesProvider_included


// ACF includes
#include "iser/IObject.h"


namespace iattr
{


/**
	Common interface for a container of properties.
*/
class IAttributesProvider: virtual public istd::IChangeable
{
public:
	typedef QSet<QByteArray> AttributeIds;

	/**
		Data model change notification flags.
	*/
	enum ChangeFlags
	{
		/**
			The properties were removed completelly.
		*/
		CF_RESET = 0xb812936,
		/**
			An attribute was removed from the set.
		*/
		CF_ATTR_REMOVED,
		/**
			A new attribute was added to the set.
		*/
		CF_ATTR_ADDED
	};

	/**
		Get list of all attribute IDs.
	*/
	virtual AttributeIds GetAttributeIds() const = 0;

	/**
		Get property with the given index.
	*/
	virtual iser::IObject* GetAttribute(const QByteArray& attributeId) const = 0;
};


} // namespace iattr


#endif // !iattr_IAttributesProvider_included


