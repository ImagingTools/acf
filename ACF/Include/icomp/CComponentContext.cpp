#include "icomp/CComponentContext.h"


#include "icomp/IRegistryElement.h"
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


CComponentContext::CComponentContext(const IRegistryElement* elementPtr, const IComponentContext* parentPtr)
:	m_registryElement(*elementPtr), m_parentPtr(parentPtr)
{
}


// reimplemented (icomp::IComponentContext)

const IRegistryElement& CComponentContext::GetRegistryElement() const
{
	return m_registryElement;
}


const IComponentContext* CComponentContext::GetParentContext() const
{
	return m_parentPtr;
}


const iser::ISerializable* CComponentContext::GetAttribute(const ::std::string& attributeId, const IComponentContext** realContextPtr) const
{
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);

	if (infoPtr  != NULL){
		const ::std::string& exportId = infoPtr->exportId;
		if (!exportId.empty() && (m_parentPtr != NULL)){
			const iser::ISerializable* parentAttributePtr = m_parentPtr->GetAttribute(exportId, realContextPtr);

			if (parentAttributePtr != NULL){
				return parentAttributePtr;
			}
		}

		if (infoPtr->attributePtr.IsValid()){
			if (realContextPtr != NULL){
				*realContextPtr = this;
			}

			return infoPtr->attributePtr.GetPtr();
		}
	}

	const IComponentStaticInfo& componentInfo = m_registryElement.GetComponentStaticInfo();
	const IComponentStaticInfo::AttributeInfos& attributeInfos = componentInfo.GetAttributeInfos();
	const IComponentStaticInfo::AttributeInfos::ValueType* attributePtr2 = attributeInfos.FindElement(attributeId);
	if (attributePtr2 != NULL){
		I_ASSERT(*attributePtr2 != NULL);

		if ((*attributePtr2)->IsObligatory()){
			const iser::ISerializable* defaultAttributePtr = (*attributePtr2)->GetAttributeDefaultValue();
			if (defaultAttributePtr != NULL){
				if (realContextPtr != NULL){
					*realContextPtr = this;
				}

				return defaultAttributePtr;
			}
		}
	}

	return NULL;
}


IComponent* CComponentContext::GetSubcomponent(const ::std::string& /*componentId*/) const
{
	return NULL;	// normal component has no subcomponents
}


}//namespace icomp


