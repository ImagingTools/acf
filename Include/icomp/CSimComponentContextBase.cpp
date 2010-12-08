#include "icomp/CSimComponentContextBase.h"


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IAttributeStaticInfo.h"
#include "icomp/CReferenceAttribute.h"
#include "icomp/CFactoryAttribute.h"
#include "icomp/CMultiReferenceAttribute.h"


namespace icomp
{


CSimComponentContextBase::CSimComponentContextBase(const IComponentStaticInfo* infoPtr)
:	m_metaInfo(*infoPtr)
{
	I_ASSERT(infoPtr != NULL);
}


bool CSimComponentContextBase::SetAttr(const std::string& attributeId, const iser::IObject* attributePtr)
{
	I_ASSERT(attributePtr != NULL);

	IRegistryElement::AttributeInfo* infoPtr = m_registryElement.InsertAttributeInfo(attributeId, attributePtr->GetFactoryId());
	if (infoPtr != NULL){
		infoPtr->attributePtr.SetPtr(const_cast<iser::IObject*>(attributePtr));

		return true;
	}

	return false;
}


bool CSimComponentContextBase::SetRef(const std::string& referenceId, IComponent* componentPtr)
{
	I_ASSERT(IsAttributeTypeCorrect<CReferenceAttribute>(referenceId));
	I_ASSERT(componentPtr != NULL);

	if (SetAttr(referenceId, new CReferenceAttribute(referenceId))){
		m_componentsMap[referenceId] = componentPtr;

		return true;
	}

	return false;
}


bool CSimComponentContextBase::InsertMultiRef(const std::string& referenceId, IComponent* componentPtr)
{
	I_ASSERT(IsAttributeTypeCorrect<CMultiReferenceAttribute>(referenceId));

	CMultiReferenceAttribute* multiAttrPtr = NULL;

	const IRegistryElement::AttributeInfo* existingInfoPtr = m_registryElement.GetAttributeInfo(referenceId);
	if (existingInfoPtr != NULL){
		multiAttrPtr = dynamic_cast<CMultiReferenceAttribute*>(existingInfoPtr->attributePtr.GetPtr());
	}
	else{
		IRegistryElement::AttributeInfo* newInfoPtr = m_registryElement.InsertAttributeInfo(referenceId, istd::CClassInfo::GetName<CMultiReferenceAttribute>());
		if (newInfoPtr != NULL){
			IRegistryElement::AttributePtr& attributePtr = newInfoPtr->attributePtr;
			if (!attributePtr.IsValid()){
				attributePtr.SetPtr(new CMultiReferenceAttribute);
			}

			multiAttrPtr = dynamic_cast<CMultiReferenceAttribute*>(attributePtr.GetPtr());
		}
	}

	if (multiAttrPtr != NULL){
		istd::CString indexString = istd::CString::FromNumber(multiAttrPtr->GetValuesCount());
		std::string attributeId = referenceId + '#' + indexString.ToString();

		multiAttrPtr->InsertValue(attributeId);

		m_componentsMap[attributeId] = componentPtr;

		return true;
	}

	return false;
}


bool CSimComponentContextBase::SetFactory(const std::string& factoryId, const ComponentsFactory* factoryPtr)
{
	I_ASSERT(IsAttributeTypeCorrect<CFactoryAttribute>(factoryId));
	I_ASSERT(factoryPtr != NULL);

	if (SetAttr(factoryId, new CFactoryAttribute(factoryId))){
		m_factoriesMap[factoryId] = factoryPtr;

		return true;
	}

	return false;
}


void CSimComponentContextBase::SetBoolAttr(const std::string& attributeId, bool value)
{
	SetSingleAttr<bool>(attributeId, value);
}


void CSimComponentContextBase::SetIntAttr(const std::string& attributeId, int value)
{
	SetSingleAttr<int>(attributeId, value);
}


void CSimComponentContextBase::SetDoubleAttr(const std::string& attributeId, double value)
{
	SetSingleAttr<double>(attributeId, value);
}


void CSimComponentContextBase::SetStringAttr(const std::string& attributeId, const istd::CString& value)
{
	SetSingleAttr<istd::CString>(attributeId, value);
}


// reimplemeted (icomp::IComponentContext)

const IRegistryElement& CSimComponentContextBase::GetRegistryElement() const
{
	return m_registryElement;
}


const IComponentStaticInfo& CSimComponentContextBase::GetStaticInfo() const
{
	return m_metaInfo;
}


const IComponentContext* CSimComponentContextBase::GetParentContext() const
{
	return this;
}


const iser::IObject* CSimComponentContextBase::GetAttribute(const std::string& attributeId, int* definitionLevelPtr) const
{
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);
	if (infoPtr != NULL){
		if (definitionLevelPtr != NULL){
			*definitionLevelPtr = 0;
		}
		return infoPtr->attributePtr.GetPtr();
	}

	const IAttributeStaticInfo* attributeInfoPtr = m_metaInfo.GetAttributeInfo(attributeId);
	if (attributeInfoPtr != NULL){
		if ((attributeInfoPtr->GetAttributeFlags() & IAttributeStaticInfo::AF_OBLIGATORY) != 0){
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


}//namespace icomp


