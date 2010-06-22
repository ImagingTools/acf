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
					iqt::GetCString(QObject::tr("Cannot create %1 (%2: %3)").
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
	return BaseClass2::Serialize(archive) && SerializeComponentsLayout(archive);
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


