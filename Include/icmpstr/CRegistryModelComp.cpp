#include "icmpstr/CRegistryModelComp.h"


#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "icomp/CInterfaceManipBase.h"


// public methods


namespace icmpstr
{


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


bool CRegistryModelComp::SerializeRegistry(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive);
}


// reimplemented (icmpstr::IRegistryEditController)

i2d::CVector2d CRegistryModelComp::GetComponentPosition(const std::string& componentName) const
{
	ElementsPositionMap::const_iterator elementIter = m_elementsPositionMap.find(componentName);
	if (elementIter != m_elementsPositionMap.end()){
		return elementIter->second;
	}
	else{
		return i2d::CVector2d(0, 0);
	}
}


void CRegistryModelComp::SetComponentPosition(const std::string& componentName, const i2d::CVector2d& position)
{
	if (position != GetComponentPosition(componentName)){
		istd::CChangeNotifier changePtr(this, i2d::IObject2d::CF_OBJECT_POSITION);

		m_elementsPositionMap[componentName]  = position;
	}
}


// reimplemented (icomp::IRegistry)

CRegistryModelComp::ElementInfo* CRegistryModelComp::InsertElementInfo(
			const std::string& elementId,
			const icomp::CComponentAddress& address,
			bool ensureElementCreated)
{
	ElementInfo* retVal = BaseClass2::InsertElementInfo(elementId, address, ensureElementCreated);

	if (retVal == NULL){
		SendErrorMessage(
					MI_CANNOT_CREATE_ELEMENT,
					iqt::GetCString(QObject::tr("Cannot create %1 (%2: %3)").
								arg(elementId.c_str()).
								arg(address.GetPackageId().c_str()).
								arg(address.GetComponentId().c_str())));
	}

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CRegistryModelComp::Serialize(iser::IArchive& archive)
{
	return BaseClass2::Serialize(archive) && SerializeComponentsLayout(archive);
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


// reimplemented (icomp::CRegistry)

icomp::IRegistryElement* CRegistryModelComp::CreateRegistryElement(
			const std::string& elementId,
			const icomp::CComponentAddress& address) const
{
	Element* registryElementPtr = new Element;
	if (registryElementPtr != NULL){
		registryElementPtr->Initialize(this, address);
		registryElementPtr->SetName(elementId);
		registryElementPtr->SetSlavePtr(const_cast<CRegistryModelComp*>(this));

		return registryElementPtr;
	}

	return NULL;
}


} // namespace icmpstr


