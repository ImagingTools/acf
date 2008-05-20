#include "CRegistryModelComp.h"

#include "istd/TChangeNotifier.h"


// public methods


// reimplemented (IRegistryGeometryProvider)

QPoint CRegistryModelComp::GetComponentPosition(const istd::CString& componentRole) const
{
	QPoint retVal;

	ElementsPositionMap::const_iterator elementIter = m_elementsPositionMap.find(componentRole);
	if (elementIter != m_elementsPositionMap.end()){
		retVal = elementIter->second;
	}

	return retVal;
}


void CRegistryModelComp::SetComponentPosition(const istd::CString& componentRole, const QPoint& point)
{
/*	bool isChangeNeed = false;
	ElementsPositionMap::const_iterator elementIter = m_elementsPositionMap.find(componentRole);
	if (elementIter == m_elementsPositionMap.end()){
		isChangeNeed = true;
	}
	else{
		if(elementIter->second != point){
			isChangeNeed = true;
		}
	}	

	if (isChangeNeed){
		istd::CChangeNotifier changePtr(this, CF_POSITION);
*/
		m_elementsPositionMap[componentRole]  = point;
//	}
}


// reimplemented (icomp::IComponent)

void CRegistryModelComp::OnComponentCreated()
{
	if (m_staticInfoCompPtr.IsValid()){
		SetComponentStaticInfo(m_staticInfoCompPtr.GetPtr());
	}
}


// reimplemented (iser::ISerializable)

bool CRegistryModelComp::Serialize(iser::IArchive& archive)
{
	bool retVal = BaseClass2::Serialize(archive);

	if (retVal){
		static iser::CArchiveTag positionsTag("ElementPositions", "List of component element positions");
		static iser::CArchiveTag positionTag("ElementPosition", "Element Position");

		int positionsCount = m_elementsPositionMap.size();

		if (!archive.BeginMultiTag(positionsTag, positionTag, positionsCount)){
			return retVal;
		}

		if (!archive.IsStoring()){
			for (int i = 0; i < positionsCount; ++i){
				retVal = retVal && archive.BeginTag(positionTag);
				
				istd::CString componentRole;
				int x;
				int y;

				retVal = retVal && SerializeComponentPosition(archive, componentRole, x, y);
					
				retVal = retVal && archive.EndTag(positionTag);

				if (retVal){
					m_elementsPositionMap[componentRole] = QPoint(x, y);
				}
			}
		}
		else{
			for (		ElementsPositionMap::const_iterator index = m_elementsPositionMap.begin(); 
						index != m_elementsPositionMap.end();
						index++){
				
				retVal = retVal && archive.BeginTag(positionTag);
				
				istd::CString componentRole = index->first;
				int x = index->second.x();
				int y = index->second.y();

				retVal = retVal && SerializeComponentPosition(archive, componentRole, x, y);

				retVal = retVal && archive.EndTag(positionTag);
			}
		
		}

		retVal = retVal && archive.EndTag(positionsTag);
	}

	return retVal;
}


// private methods

bool CRegistryModelComp::SerializeComponentPosition(iser::IArchive& archive, istd::CString& componentRole, int& x, int& y)
{
	static iser::CArchiveTag componentRoleTag("ComponentRole", "Component Role");
	static iser::CArchiveTag componentXTag("X", "X");
	static iser::CArchiveTag componentYTag("Y", "Y");
	
	bool retVal = archive.BeginTag(componentRoleTag);
	retVal = retVal && archive.Process(componentRole);
	retVal = retVal && archive.EndTag(componentRoleTag);

	retVal = retVal && archive.BeginTag(componentXTag);
	retVal = retVal && archive.Process(x);
	retVal = retVal && archive.EndTag(componentXTag);

	retVal = retVal && archive.BeginTag(componentYTag);
	retVal = retVal && archive.Process(y);
	retVal = retVal && archive.EndTag(componentYTag);

	return retVal;
}


