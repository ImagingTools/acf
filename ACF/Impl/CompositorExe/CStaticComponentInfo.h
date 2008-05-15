#ifndef CStaticComponentInfo_included
#define CStaticComponentInfo_included


#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistry.h"


class CStaticComponentInfo
{
public:
	CStaticComponentInfo(	const icomp::IComponentStaticInfo* componentStaticInfoPtr, 
		const std::string& packageId, 
		const std::string& componentId,
		int elementType = icomp::IRegistry::ET_COMPONENT)
		:m_componentStaticInfoPtr(componentStaticInfoPtr),
		m_packageId(packageId),
		m_componentId(componentId),
		m_elementType(elementType)
	{	
	}

	const icomp::IComponentStaticInfo* m_componentStaticInfoPtr;
	int m_elementType;
	std::string m_packageId;
	std::string m_componentId;
};


#endif // !CStaticComponentInfo_included

