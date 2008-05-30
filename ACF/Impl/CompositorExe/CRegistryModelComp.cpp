#include "CRegistryModelComp.h"

#include "istd/TChangeNotifier.h"


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
			imath::CVector2d position;

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
			imath::CVector2d position = index->second;

			retVal = retVal && SerializeComponentPosition(archive, componentRole, position);

			retVal = retVal && archive.EndTag(elementTag);
		}
	
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	return retVal;
}


// reimplemented (icomp::IRegistryGeometryProvider)

imath::CVector2d CRegistryModelComp::GetComponentPosition(const std::string& componentRole) const
{
	ElementsPositionMap::const_iterator elementIter = m_elementsPositionMap.find(componentRole);
	if (elementIter != m_elementsPositionMap.end()){
		return elementIter->second;
	}
	else{
		return imath::CVector2d(0, 0);
	}
}


void CRegistryModelComp::SetComponentPosition(const std::string& componentRole, const imath::CVector2d& position)
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

bool CRegistryModelComp::SerializeComponentPosition(iser::IArchive& archive, std::string& componentRole, imath::CVector2d& position)
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
