#ifndef iprop_CPropertyBase_included
#define iprop_CPropertyBase_included


#include "iser/TCopySerializedWrap.h"

#include "iprop/IProperty.h"


namespace iprop
{


class IPropertiesManager;


/**
	Basic implementation of an attribute.
*/
class CPropertyBase: public iser::TCopySerializedWrap<iprop::IProperty>
{
public:
	CPropertyBase();
	CPropertyBase(
				iprop::IPropertiesManager* attributeOwnerPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags = 0);

	// reimplemented (iprop::IProperty)
	virtual const iser::IObject* GetDefaultAttributeValue() const;

protected:
	iprop::IPropertiesManager* m_attributeOwnerPtr;
	int m_changeFlags;
};


} // namespace iprop


#endif // !iprop_CPropertyBase_included


