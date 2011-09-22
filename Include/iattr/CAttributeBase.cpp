#include "iattr/CAttributeBase.h"


#include "iattr/IAttributesManager.h"


namespace iattr
{


// public methods

CAttributeBase::CAttributeBase()
	:m_attributeOwnerPtr(NULL),
	m_changeFlags(0)
{
}


CAttributeBase::CAttributeBase(
				iattr::IAttributesManager* attributeOwnerPtr,
				const std::string& attributeId,
				const std::string& attributeDescription,
				int attributeFlags,
				int changeFlags)
	:m_attributeOwnerPtr(attributeOwnerPtr),
	m_changeFlags(changeFlags)
{
	if (attributeOwnerPtr != NULL){
		attributeOwnerPtr->InsertAttribute(this, attributeId, attributeDescription, attributeFlags, false);
	}
}


// reimplemented (iattr::IAttribute)

const iser::IObject* CAttributeBase::GetDefaultAttributeValue() const
{
	return NULL;
}


} // namespace iattr


