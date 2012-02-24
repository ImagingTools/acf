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

	delete attributePtr;

	return false;
}


bool CSimComponentContextBase::SetRef(const std::string& referenceId, IComponent* componentPtr, const std::string& subelementId)
{
	I_ASSERT(IsAttributeTypeCorrect<CReferenceAttribute>(referenceId));
	I_ASSERT(componentPtr != NULL);

	std::string completeId = JoinId(referenceId, subelementId);
	if (SetAttr(referenceId, new CReferenceAttribute(completeId))){
		m_componentsMap[referenceId] = componentPtr;

		return true;
	}

	return false;
}


bool CSimComponentContextBase::InsertMultiRef(const std::string& referenceId, IComponent* componentPtr, const std::string& subelementId)
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
		QString indexString = QString("%1").arg(multiAttrPtr->GetValuesCount());
		std::string attributeId = referenceId + '#' + indexString.toStdString();

		std::string completeId = JoinId(attributeId, subelementId);
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


bool CSimComponentContextBase::SetBoolAttr(const std::string& attributeId, bool value)
{
	return SetSingleAttr<bool>(attributeId, value);
}


bool CSimComponentContextBase::SetIntAttr(const std::string& attributeId, int value)
{
	return SetSingleAttr<int>(attributeId, value);
}


bool CSimComponentContextBase::SetDoubleAttr(const std::string& attributeId, double value)
{
	return SetSingleAttr<double>(attributeId, value);
}


bool CSimComponentContextBase::SetStringAttr(const std::string& attributeId, const QString& value)
{
	return SetSingleAttr<QString>(attributeId, value);
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
	// try to find registered attribute
	const IRegistryElement::AttributeInfo* infoPtr = m_registryElement.GetAttributeInfo(attributeId);
	if (infoPtr != NULL){
		if (definitionLevelPtr != NULL){
			*definitionLevelPtr = 0;
		}
		return infoPtr->attributePtr.GetPtr();
	}

	// resolve using default attribute value
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


} // namespace icomp


