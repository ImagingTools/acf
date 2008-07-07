#include "CRegistryModelComp.h"

#include "istd/TChangeNotifier.h"


#include "icomp/CCompositeComponent.h"


// public methods


bool CRegistryModelComp::SerializeComponentsLayout(iser::IArchive& archive)
{
	static iser::CArchiveTag positionMapTag("PositionMap", "Map of component name to its positions");
	static iser::CArchiveTag elementTag("Element", "Map element");

	bool retVal = true;

	int positionsCount = m_elementsPositionMap.size();

	retVal = retVal && archive.BeginMultiTag(positionMapTag, elementTag, positionsCount);

	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		for (int i = 0; i < positionsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			std::string componentRole;
			i2d::CVector2d position;

			retVal = retVal && SerializeComponentPosition(archive, componentRole, position);
				
			retVal = retVal && archive.EndTag(elementTag);

			if (!retVal){
				return false;
			}

			m_elementsPositionMap[componentRole] = position;
		}
	}
	else{
		for (		ElementsPositionMap::const_iterator index = m_elementsPositionMap.begin(); 
					index != m_elementsPositionMap.end();
					index++){
			
			retVal = retVal && archive.BeginTag(elementTag);
			
			std::string componentRole = index->first;
			i2d::CVector2d position = index->second;

			retVal = retVal && SerializeComponentPosition(archive, componentRole, position);

			retVal = retVal && archive.EndTag(elementTag);
		}
	
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	return retVal;
}


int CRegistryModelComp::CheckAttributeConsistency(const icomp::IRegistryElement& element, const std::string& attributeId)
{
	const icomp::IComponentStaticInfo& elementStaticInfo = element.GetComponentStaticInfo();
	const icomp::IComponentStaticInfo::AttributeInfos staticAttributes = elementStaticInfo.GetAttributeInfos();
	const icomp::IComponentStaticInfo::AttributeInfos::ValueType* attrStaticInfoPtrPtr = staticAttributes.FindElement(attributeId);
	if (attrStaticInfoPtrPtr == NULL){
		return CS_UNKNOWN;
	}
	I_ASSERT(*attrStaticInfoPtrPtr != NULL);

	const icomp::IAttributeStaticInfo& attrStaticInfo = **attrStaticInfoPtrPtr;
	const type_info& attrType = attrStaticInfo.GetAttributeType();

	const icomp::IRegistryElement::AttributeInfo* attributeInfoPtr = element.GetAttributeInfo(attributeId);

	if (		(attrType == typeid(icomp::CReferenceAttribute)) ||
				(attrType == typeid(icomp::CMultiReferenceAttribute)) ||
				(attrType == typeid(icomp::CFactoryAttribute)) ||
				(attrType == typeid(icomp::CMultiFactoryAttribute))){
		if ((attributeInfoPtr == NULL) || !attributeInfoPtr->attributePtr.IsValid()){
			if (attrStaticInfo.IsObligatory() && attributeInfoPtr->exportId.empty()){
				return CS_INVALID;
			}
			else if (attributeInfoPtr->exportId.empty()){
				return CS_OPTIONAL;
			}
			else{
				return CS_OK;
			}
		}

		const icomp::TSingleAttribute<std::string>* idPtr = dynamic_cast<const icomp::TSingleAttribute<std::string>*>(attributeInfoPtr->attributePtr.GetPtr());
		if (idPtr != NULL){		
			const icomp::CReferenceAttribute* referencePtr = dynamic_cast<const icomp::CReferenceAttribute*>(attributeInfoPtr->attributePtr.GetPtr());
			if ((referencePtr != NULL) && (attrType != typeid(icomp::CReferenceAttribute))){
				return CS_INVALID;
			}

			std::string componentId;
			std::string restId;
			icomp::CCompositeComponent::SplitComponentId(idPtr->GetValue().c_str(), componentId, restId);

			if (GetElementInfo(componentId) == NULL){
				return CS_INVALID;
			}
		}
	}

	return CS_OK;
}


// reimplemented (icomp::IRegistryGeometryProvider)

i2d::CVector2d CRegistryModelComp::GetComponentPosition(const std::string& componentRole) const
{
	ElementsPositionMap::const_iterator elementIter = m_elementsPositionMap.find(componentRole);
	if (elementIter != m_elementsPositionMap.end()){
		return elementIter->second;
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CRegistryModelComp::SetComponentPosition(const std::string& componentRole, const i2d::CVector2d& position)
{
	if (position != GetComponentPosition(componentRole)){
		istd::CChangeNotifier changePtr(this, CF_POSITION);

		m_elementsPositionMap[componentRole]  = position;
	}
}


// reimplemented (icomp::IRegistryNotesProvider)

istd::CString CRegistryModelComp::GetComponentNote(const std::string& componentRole) const
{
	static istd::CString emptyNote;

	ElementsNoteMap::const_iterator noteIter = m_elementsNoteMap.find(componentRole);
	if (noteIter != m_elementsNoteMap.end()){
		return noteIter->second;
	}

	return emptyNote;
}


void CRegistryModelComp::SetComponentNote(const std::string& componentRole, const istd::CString& componentNote)
{	
	if (componentNote != GetComponentNote(componentRole)){
		istd::CChangeNotifier changePtr(this, CF_NOTE);

		m_elementsNoteMap[componentRole]  = componentNote;
	}
}


void CRegistryModelComp::RemoveComponentNote(const std::string& componentRole)
{
	ElementsNoteMap::iterator noteIter = m_elementsNoteMap.find(componentRole);
	if (noteIter != m_elementsNoteMap.end()){
		istd::CChangeNotifier changePtr(this, CF_NOTE);

		m_elementsNoteMap.erase(noteIter);
	}
}



// reimplemented (icomp::IComponent)

void CRegistryModelComp::OnComponentCreated()
{
	if (m_staticInfoCompPtr.IsValid()){
		SetComponentStaticInfo(m_staticInfoCompPtr.GetPtr());
	}
}


// protected methods

bool CRegistryModelComp::SerializeComponentPosition(iser::IArchive& archive, std::string& componentRole, i2d::CVector2d& position)
{
	static iser::CArchiveTag nameTag("ComponentName", "Name of component");
	static iser::CArchiveTag positionXTag("X", "X position of component");
	static iser::CArchiveTag positionYTag("Y", "Y position of component");
	
	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(componentRole);
	retVal = retVal && archive.EndTag(nameTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[0]);
	retVal = retVal && archive.EndTag(positionXTag);

	retVal = retVal && archive.BeginTag(positionXTag);
	retVal = retVal && archive.Process(position[1]);
	retVal = retVal && archive.EndTag(positionXTag);

	return retVal;
}


bool CRegistryModelComp::SerializeNote(iser::IArchive& archive, std::string& componentRole, istd::CString& note)
{
	static iser::CArchiveTag noticeTag("ComponentNote", "Note for this component");
	
	bool retVal = archive.BeginTag(noticeTag);
	retVal = retVal && archive.Process(note);
	retVal = retVal && archive.EndTag(noticeTag);

	return retVal;
}
