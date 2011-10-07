#ifndef iprop_IProperty_included
#define iprop_IProperty_included


#include "iser/IObject.h"

#include "iprop/iprop.h"


namespace iprop
{


/**
	Common interface for an attribute.
*/
class IProperty: virtual public iser::IObject
{
public:
	enum AttributeFlags
	{
		/**
			Attribute is persistent.
		*/
		AF_PERSISTENT = 0x1
	};

	/**
		Get default value for the attribute.
	*/
	virtual const iser::IObject* GetDefaultAttributeValue() const = 0;
};


} // namespace iprop


#endif // !iprop_IProperty_included


