#include "icomp/CComponentContext.h"


#include "icomp/IRegistryElement.h"
#include "icomp/IComponentStaticInfo.h"
#include "icomp/IAttributeStaticInfo.h"


namespace icomp
{


CComponentContext::CComponentContext(
			const IRegistryElement* elementPtr,
			const IComponentStaticInfo* staticInfoPtr,
			const IComponentContext* parentPtr,
			const QByteArray& contextId)
:	m_registryElement(*elementPtr),
	m_staticInfo(*staticInfoPtr),
	m_parentPtr(parentPtr),
	m_contextId(contextId)
{
	I_ASSERT(elementPtr != NULL);
	I_ASSERT(staticInfoPtr != NULL);
}


// reimplemented (icomp::IComponentContext)

const IRegistryElement& CComponentContext::GetRegistryElement() const
{
	return m_registryElement;
}


const IComponentStaticInfo& CComponentContext::GetStaticInfo() const
{
	return m_staticInfo;
}


const IComponentContext* CComponentContext::GetParentContext() const
{
	return m_parentPtr;
}


const iser::IObject* CComponentContext::GetAttribute(const QByteArray& attributeId, int* definitionLevelPtr) const
{
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);

	if (infoPtr  != NULL){
		const QByteArray& exportId = infoPtr->exportId;
		if (!exportId.isEmpty() && (m_parentPtr != NULL)){
			const iser::IObject* parentAttributePtr = NULL;
			if (definitionLevelPtr != NULL){
				int parentLevel = -1;

				parentAttributePtr = m_parentPtr->GetAttribute(exportId, &parentLevel);
				I_ASSERT((parentAttributePtr == NULL) || (parentLevel >= 0));	// if attribute is retured parent level must be set

				*definitionLevelPtr = parentLevel + 1;
			}
			else{
				parentAttributePtr = m_parentPtr->GetAttribute(exportId);
			}

			if (parentAttributePtr != NULL){
				return parentAttributePtr;
			}
		}

		if (infoPtr->attributePtr.IsValid()){
			if (definitionLevelPtr != NULL){
				*definitionLevelPtr = 0;
			}

			return infoPtr->attributePtr.GetPtr();
		}
	}

	const IAttributeStaticInfo* attributeInfoPtr = m_staticInfo.GetAttributeInfo(attributeId);
	if (attributeInfoPtr != NULL){
		int attributeFlags = attributeInfoPtr->GetAttributeFlags();
		static const int UseDefaultFlags = icomp::IAttributeStaticInfo::AF_OBLIGATORY | icomp::IAttributeStaticInfo::AF_VALUE;
		if ((attributeFlags & UseDefaultFlags) == UseDefaultFlags){
			const iser::IObject* defaultAttributePtr = attributeInfoPtr->GetAttributeDefaultValue();
			if (defaultAttributePtr != NULL){
				if (definitionLevelPtr != NULL){
					*definitionLevelPtr = 0;
				}

				return defaultAttributePtr;
			}
		}
	}

	return NULL;
}


} // namespace icomp


