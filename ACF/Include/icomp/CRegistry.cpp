#include "icomp/CRegistry.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/CRegistryElement.h"


namespace icomp
{


CRegistry::CRegistry(const IComponentStaticInfo* factoryPtr)
:	m_componentsFactory(*factoryPtr)
{
	I_ASSERT(factoryPtr != NULL);
}


// reimplemented (icomp::IRegistry)

IRegistry::Ids CRegistry::GetElementIds() const
{
	Ids retVal;
	for (		ElementsMap::const_iterator iter = m_elementInfos.begin();
				iter != m_elementInfos.begin();
				++iter){
		retVal.insert(iter->first);
	}

	return retVal;
}


IRegistry::ElementInfo* CRegistry::InsertElementInfo(
			const ::std::string& elementId,
			int elementType,
			const ::std::string& packageId,
			const ::std::string& factoryId,
			bool ensureElementCreated)
{
	ElementsMap::const_iterator iter = m_elementInfos.find(elementId);
	if (iter != m_elementInfos.end()){
		return NULL;
	}

	IRegistryElement* registryPtr = NULL;

	switch (elementType){
	case IRegistry::ET_COMPONENT:
		{
			const IComponentStaticInfo::SubcomponentInfos& subInfos = m_componentsFactory.GetSubcomponentInfos();
			const IComponentStaticInfo::SubcomponentInfos::ValueType* packageInfoPtr2 = subInfos.FindElement(packageId);
			if (packageInfoPtr2 != NULL){
				I_ASSERT(*packageInfoPtr2 != NULL);

				const IComponentStaticInfo::SubcomponentInfos& packageSubInfos = (*packageInfoPtr2)->GetSubcomponentInfos();
				const IComponentStaticInfo::SubcomponentInfos::ValueType* componentInfoPtr2 = packageSubInfos.FindElement(factoryId);
				if (componentInfoPtr2 != NULL){
					I_ASSERT(*componentInfoPtr2 != NULL);

					registryPtr = new CRegistryElement(*componentInfoPtr2);
				}
			}
		}
		break;

	default:
		I_CRITICAL();		// TODO: implement creation of registry element for compositions.
	}

	if (ensureElementCreated && (registryPtr == NULL)){
		return NULL;
	}

	ElementInfo& newElement = m_elementInfos[elementId];
	newElement.elementType = elementType;
	newElement.packageId = packageId;
	newElement.factoryId = factoryId;
	newElement.elementPtr.SetPtr(registryPtr);

	return &newElement;
}


const IRegistry::ElementInfo* CRegistry::GetElementInfo(const ::std::string& elementId) const
{
	ElementsMap::const_iterator iter = m_elementInfos.find(elementId);
	if (iter != m_elementInfos.end()){
		return &(iter->second);
	}

	return NULL;
}


bool CRegistry::RemoveElementInfo(const ::std::string& elementId)
{
	return m_elementInfos.erase(elementId) > 0;
}


const IRegistry::InterfaceInfos& CRegistry::GetExportedInterfaceInfos() const
{
	return m_interfaceInfos;
}


const IRegistry::SubcomponentMap& CRegistry::GetExportedSubcomponentMap() const
{
	return m_subcomponentMap;
}


// reimplemented (iser::ISerializable)

bool CRegistry::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag elementsListTag("ElementsList", "List of registry elements");
	static iser::CArchiveTag elementDescriptionTag("ElementDescription", "Description of single element");
	static iser::CArchiveTag elementIdTag("ID", "ID of element in registry");
	static iser::CArchiveTag elementTypeTag("Type", "Type of element");
	static iser::CArchiveTag packageIdTag("PackageId", "ID of package");
	static iser::CArchiveTag factoryIdTag("FactoryId", "ID of factory");
	static iser::CArchiveTag elementTag("Element", "Data of single element", true);
	static iser::CArchiveTag isValidTag("IsValid", "It is true if this element is valid");

	bool isStoring = archive.IsStoring();

	bool retVal = true;

	int elementsCount = int(m_elementInfos.size());

	retVal = retVal && archive.BeginMultiTag(elementsListTag, elementDescriptionTag, elementsCount);

	if (isStoring){
		for (		ElementsMap::iterator iter = m_elementInfos.begin();
					iter != m_elementInfos.end();
					++iter){
			ElementInfo& element = iter->second;

			retVal = retVal && archive.BeginTag(elementDescriptionTag);

			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(const_cast<::std::string&>(iter->first));
			retVal = retVal && archive.EndTag(elementIdTag);

			retVal = retVal && archive.BeginTag(elementTypeTag);
			retVal = retVal && archive.Process(element.elementType);
			retVal = retVal && archive.EndTag(elementTypeTag);

			retVal = retVal && archive.BeginTag(packageIdTag);
			retVal = retVal && archive.Process(element.packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			retVal = retVal && archive.BeginTag(factoryIdTag);
			retVal = retVal && archive.Process(element.factoryId);
			retVal = retVal && archive.EndTag(factoryIdTag);

			retVal = retVal && archive.BeginTag(elementTag);

			bool isValid = element.elementPtr.IsValid();
			retVal = retVal && archive.BeginTag(isValidTag);
			retVal = retVal && archive.Process(isValid);
			retVal = retVal && archive.EndTag(isValidTag);

			if (isValid){
				retVal = retVal && element.elementPtr->Serialize(archive);
			}

			retVal = retVal && archive.EndTag(elementTag);

			retVal = retVal && archive.EndTag(elementDescriptionTag);
		}
	}
	else{
		for (int i = 0; i < elementsCount; ++i){
			retVal = retVal && archive.BeginTag(elementDescriptionTag);

			ElementInfo newInfo;

			::std::string elementId;
			retVal = retVal && archive.BeginTag(elementIdTag);
			retVal = retVal && archive.Process(elementId);
			retVal = retVal && archive.EndTag(elementIdTag);

			int elementType = ET_NONE;
			retVal = retVal && archive.BeginTag(elementTypeTag);
			retVal = retVal && archive.Process(elementType);
			retVal = retVal && archive.EndTag(elementTypeTag);

			::std::string packageId;
			retVal = retVal && archive.BeginTag(packageIdTag);
			retVal = retVal && archive.Process(packageId);
			retVal = retVal && archive.EndTag(packageIdTag);

			::std::string factoryId;
			retVal = retVal && archive.BeginTag(factoryIdTag);
			retVal = retVal && archive.Process(factoryId);
			retVal = retVal && archive.EndTag(factoryIdTag);

			retVal = retVal && archive.BeginTag(elementTag);

			if (elementType != ET_NONE){
				ElementInfo* newInfoPtr = InsertElementInfo(elementId, elementType, packageId, factoryId, false);
				if (newInfoPtr == NULL){
					return false;
				}

				bool isValid = false;
				retVal = retVal && archive.BeginTag(isValidTag);
				retVal = retVal && archive.Process(isValid);
				retVal = retVal && archive.EndTag(isValidTag);

				if (isValid && newInfoPtr->elementPtr.IsValid()){
					retVal = retVal && newInfoPtr->elementPtr->Serialize(archive);
				}
			}

			retVal = retVal && archive.EndTag(elementTag);

			retVal = retVal && archive.EndTag(elementDescriptionTag);
		}
	}

	retVal = retVal && archive.EndTag(elementsListTag);

	return retVal;
}


}//namespace icomp


