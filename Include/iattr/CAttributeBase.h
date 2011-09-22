#ifndef iattr_CAttributeBase_included
#define iattr_CAttributeBase_included


#include "iser/TCopySerializedWrap.h"

#include "iattr/IAttribute.h"


namespace iattr
{


class IAttributesManager;


/**
	Basic implementation of an attribute.
*/
class CAttributeBase: public iser::TCopySerializedWrap<iattr::IAttribute>
{
public:
	CAttributeBase();
	CAttributeBase(
				iattr::IAttributesManager* attributeOwnerPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags = 0);

	// reimplemented (iattr::IAttribute)
	virtual const iser::IObject* GetDefaultAttributeValue() const;

protected:
	iattr::IAttributesManager* m_attributeOwnerPtr;
	int m_changeFlags;
};


} // namespace iattr


#endif // !iattr_CAttributeBase_included


