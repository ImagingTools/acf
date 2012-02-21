#include "icmpstr/CVisualRegistryComp.h"


// Qt includes
#include <QDir>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "icomp/CInterfaceManipBase.h"


// public methods


namespace icmpstr
{


bool CVisualRegistryComp::SerializeNotes(iser::IArchive& archive)
{
	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();
	I_DWORD frameworkVersion = 0;
	versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion);
	if (frameworkVersion < 2192 && !archive.IsStoring()){
		return true;
	}

	static iser::CArchiveTag noteMapTag("NotesMap", "Map of component name to its note");
	static iser::CArchiveTag elementTag("Element", "Map element");

	int notesCount = m_notesMap.size();

	bool retVal = archive.BeginMultiTag(noteMapTag, elementTag, notesCount);
	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (		NotesMap::const_iterator noteIndex = m_notesMap.begin();
					noteIndex != m_notesMap.end();
					noteIndex++){
			retVal = retVal && archive.BeginTag(elementTag);

			std::string elementId = noteIndex->first;
			istd::CString componentNote = noteIndex->second;

			retVal = retVal && SerializeComponentNote(archive, elementId, componentNote);
				
			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < notesCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			std::string elementId;
			istd::CString componentNote;

			retVal = retVal && SerializeComponentNote(archive, elementId, componentNote);
			if (!retVal){
				return false;
			}

			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				SetComponentNote(elementId, componentNote);
			}

			retVal = retVal && archive.EndTag(elementTag);
		}
	}

	retVal = retVal && archive.EndTag(noteMapTag);

	return retVal;
}


bool CVisualRegistryComp::SerializeComponentsLayout(iser::IArchive& archive)
{
	static iser::CArchiveTag positionMapTag("PositionMap", "Map of component name to its positions");
	static iser::CArchiveTag elementTag("Element", "Map element");

	bool retVal = true;

	Ids ids = GetElementIds();
	int positionsCount = ids.size();

	retVal = retVal && archive.BeginMultiTag(positionMapTag, elementTag, positionsCount);

	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (Ids::const_iterator iter = ids.begin(); iter != ids.end(); ++iter){
			std::string elementId = *iter;

			i2d::CVector2d position(0, 0);
			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					position = elementPtr->GetCenter();
				}
			}

			retVal = retVal && archive.BeginTag(elementTag);

			retVal = retVal && SerializeComponentPosition(archive, elementId, position);
				
			retVal = retVal && archive.EndTag(elementTag);
		}
	}
	else{
		istd::CChangeNotifier notifier(this);

		for (int i = 0; i < positionsCount; ++i){
			retVal = retVal && archive.BeginTag(elementTag);
			
			std::string elementId;
			i2d::CVector2d position;

			retVal = retVal && SerializeComponentPosition(archive, elementId, position);
			if (!retVal){
				return false;
			}

			const ElementInfo* infoPtr = GetElementInfo(elementId);
			if (infoPtr != NULL){
				CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
				if (elementPtr != NULL){
					elementPtr->MoveCenterTo(position);
				}
			}

			retVal = retVal && archive.EndTag(elementTag);
		}
	
	}

	retVal = retVal && archive.EndTag(positionMapTag);

	return retVal;
}


bool CVisualRegistryComp::SerializeRegistry(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive);
}


bool CVisualRegistryComp::SerializeUserData(iser::IArchive& archive)
{
	return SerializeComponentsLayout(archive) && SerializeNotes(archive);
}


// reimplemented (IComponentNoteController)

void CVisualRegistryComp::SetComponentNote(const std::string& componentName, const istd::CString& componentNote)
{
	const ElementInfo* elementInfoPtr = GetElementInfo(componentName);
	if (elementInfoPtr == NULL){
		return;
	}

	istd::CChangeNotifier changePtr(this, CF_NOTE_ADDED);

	m_notesMap[componentName] = componentNote;
}


void CVisualRegistryComp::RemoveComponentNote(const std::string& componentName)
{
	NotesMap::iterator foundIter = m_notesMap.find(componentName);

	if (foundIter != m_notesMap.end()){
		istd::CChangeNotifier changePtr(this, CF_NOTE_REMOVED);

		m_notesMap.erase(foundIter);
	}
}


istd::CString CVisualRegistryComp::GetComponentNote(const std::string& componentName)
{
	NotesMap::iterator foundIter = m_notesMap.find(componentName);
	if (foundIter != m_notesMap.end()){
		return foundIter->second;
	}

	return istd::CString();
}


// reimplemented (icomp::IRegistry)

CVisualRegistryComp::ElementInfo* CVisualRegistryComp::InsertElementInfo(
			const std::string& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* infoPtr = BaseClass2::InsertElementInfo(elementId, address, ensureElementCreated);

	if (infoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(infoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			elementPtr->SetName(elementId);
		}
	}
	else{
		SendErrorMessage(
					MI_CANNOT_CREATE_ELEMENT,
					iqt::GetCString(tr("Cannot create %1 (%2: %3)").
								arg(elementId.c_str()).
								arg(address.GetPackageId().c_str()).
								arg(address.GetComponentId().c_str())));
	}

	return infoPtr;
}


bool CVisualRegistryComp::RenameElement(const std::string& oldElementId, const std::string& newElementId)
{
	i2d::CVector2d oldPosition;
	const ElementInfo* oldElementInfoPtr = GetElementInfo(oldElementId);
	if (oldElementInfoPtr != NULL){
		CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(oldElementInfoPtr->elementPtr.GetPtr());
		if (elementPtr != NULL){
			oldPosition = elementPtr->GetCenter();
		}
	}

	istd::TChangeNotifier<icomp::IRegistry> changePtr(this, CF_MODEL | CF_COMPONENT_RENAMED);

	if (BaseClass2::RenameElement(oldElementId, newElementId)){
		const ElementInfo* newElementInfoPtr = GetElementInfo(newElementId);
		if (newElementInfoPtr != NULL){
			CVisualRegistryElement* elementPtr = dynamic_cast<CVisualRegistryElement*>(newElementInfoPtr->elementPtr.GetPtr());
			if (elementPtr != NULL){
				elementPtr->MoveCenterTo(oldPosition);
			}
		}

		return true;
	}
	else{
		changePtr.Abort();
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CVisualRegistryComp::Serialize(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive) && SerializeUserData(archive);
}


// protected methods

bool CVisualRegistryComp::SerializeComponentPosition(iser::IArchive& archive, std::string& componentRole, i2d::CVector2d& position)
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


bool CVisualRegistryComp::SerializeComponentNote(iser::IArchive& archive, std::string& componentName, istd::CString& componentNote)
{
	static iser::CArchiveTag nameTag("ComponentName", "Name of component");
	static iser::CArchiveTag componentNoteTag("Note", "Component note");
	
	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(componentName);
	retVal = retVal && archive.EndTag(nameTag);

	retVal = retVal && archive.BeginTag(componentNoteTag);
	retVal = retVal && archive.Process(componentNote);
	retVal = retVal && archive.EndTag(componentNoteTag);

	return retVal;
}


// reimplemented (icomp::CRegistry)

icomp::IRegistryElement* CVisualRegistryComp::CreateRegistryElement(
			const std::string& elementId,
			const icomp::CComponentAddress& address) const
{
	Element* registryElementPtr = new Element;
	if (registryElementPtr != NULL){
		registryElementPtr->Initialize(this, address);
		registryElementPtr->SetName(elementId);
		
		registryElementPtr->SetSlavePtr(const_cast<CVisualRegistryComp*>(this));

		return registryElementPtr;
	}

	return NULL;
}


} // namespace icmpstr


