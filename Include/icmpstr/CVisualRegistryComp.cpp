#include "icmpstr/CVisualRegistryComp.h"


// Qt includes
#include <QDir>


// ACF includes
#include "istd/TChangeNotifier.h"
#include "istd/CClassInfo.h"

#include "icomp/CInterfaceManipBase.h"

#include "icmpstr/CRegistryModelComp.h"


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
					elementPtr->MoveTo(position);
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


void CVisualRegistryComp::SetSelectedElement(CVisualRegistryElement* selectedElementPtr)
{
	std::string elementId;
	if (selectedElementPtr != NULL){
		elementId = selectedElementPtr->GetName();
	}

	if (m_selectedElementId != elementId){
		istd::CChangeNotifier changePtr(this, CF_SELECTION);

		m_selectedElementId = elementId;
	}
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CVisualRegistryComp::GetSelectedRegistry() const
{
	return const_cast<CVisualRegistryComp*>(this);
}


iser::ISerializable* CVisualRegistryComp::GetSelectedElement() const
{
	const ElementInfo* elementInfoPtr = GetElementInfo(m_selectedElementId);
	if (elementInfoPtr != NULL){
		return elementInfoPtr->elementPtr.GetPtr();
	}

	return NULL;
}


const std::string& CVisualRegistryComp::GetSelectedElementName() const
{
	const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(GetSelectedElement());
	if (elementPtr == NULL){
		static std::string empty;

		return empty;
	}

	return elementPtr->GetName();
}


const QIcon* CVisualRegistryComp::GetSelectedElementIcon() const
{
	const icomp::CComponentAddress* addressPtr = GetSelectedElementAddress();
	if (addressPtr == NULL){
		NULL;
	}

	return GetIcon(*addressPtr);
}


const icomp::CComponentAddress* CVisualRegistryComp::GetSelectedElementAddress() const
{
	const CVisualRegistryElement* elementPtr = dynamic_cast<const CVisualRegistryElement*>(GetSelectedElement());
	if (elementPtr == NULL){
		return NULL;
	}

	return &elementPtr->GetAddress();
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


bool CVisualRegistryComp::RemoveElementInfo(const std::string& elementId)
{
	bool resetSelection = (elementId == m_selectedElementId);

	if (BaseClass2::RemoveElementInfo(elementId)){
		if (resetSelection){
			m_selectedElementId.clear();
		}

		return true;
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


const QIcon* CVisualRegistryComp::GetIcon(const icomp::CComponentAddress& address) const
{
	const QIcon* retVal = NULL;

	IconMap::const_iterator iter = m_iconMap.find(address);
	if (iter != m_iconMap.end()){
		retVal = iter->second.GetPtr();
	}
	else{
		IconPtr& newIconPtr = m_iconMap[address];

		if (m_envManagerCompPtr.IsValid()){
			istd::CString packageInfoPath = m_envManagerCompPtr->GetPackageDirPath(address.GetPackageId());
			if (!packageInfoPath.IsEmpty()){
				QDir packageDir(iqt::GetQString(packageInfoPath) + ".info");

				QFileInfo svgFileInfo(packageDir.absoluteFilePath((address.GetComponentId() + ".svg").c_str()));
				if (svgFileInfo.exists()){
					newIconPtr.SetPtr(new QIcon(svgFileInfo.absoluteFilePath()));
				}
				else{
					QFileInfo pngFileInfo(packageDir.absoluteFilePath((address.GetComponentId() + ".big.png").c_str()));
					if (pngFileInfo.exists()){
						newIconPtr.SetPtr(new QIcon(pngFileInfo.absoluteFilePath()));
					}
				}
			}
		}

		retVal = newIconPtr.GetPtr();
	}

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
		
		const QIcon* iconPtr = GetIcon(address);
		if (iconPtr != NULL){
			registryElementPtr->SetIcon(*iconPtr);
		}

		registryElementPtr->SetSlavePtr(const_cast<CVisualRegistryComp*>(this));

		return registryElementPtr;
	}

	return NULL;
}


} // namespace icmpstr


