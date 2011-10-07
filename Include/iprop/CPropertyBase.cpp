#include "iprop/CPropertyBase.h"


#include "iprop/IPropertiesManager.h"


namespace iprop
{


// public methods

CPropertyBase::CPropertyBase()
	:m_attributeOwnerPtr(NULL),
	m_changeFlags(0)
{
}


CPropertyBase::CPropertyBase(
				iprop::IPropertiesManager* attributeOwnerPtr,
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


// reimplemented (iprop::IProperty)

const iser::IObject* CPropertyBase::GetDefaultAttributeValue() const
{
	return NULL;
}


} // namespace iprop


